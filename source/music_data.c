/**
 * @file music_data.c
 * @author Paul Chambaz
 * @date 16 Sep 2022
 * @brief Definition file to manage the music (meta)data.
 * @license GPLv2
 */

#include "music_data.h"

/**
 * @brief Converts the content of a raw music data struct to a music data struct by reorganizing it and sorting it
 * @param raw_music_data The raw music data struct filled with mpd songs
 * @param music_data Address to the music data struct to store the sorted mpd songs
 */
void
raw_to_music_data ( struct raw_music_data raw_music_data, struct music_data *music_data )
{
  /* alocate memory for all structures */
  music_data->artist = (char **) malloc(sizeof(char *) * MAX_ALBUM);
  music_data->album = (char **) malloc(sizeof(char *) * MAX_ALBUM);
  music_data->date = (short *) malloc(sizeof(short) * MAX_ALBUM);
  music_data->album_size = (int *) malloc(sizeof(int) * MAX_ALBUM);
  music_data->id = (char ***) malloc(sizeof(char **) * MAX_ALBUM);
  music_data->title = (char ***) malloc(sizeof(char **) * MAX_ALBUM);
  music_data->track = (short **) malloc(sizeof(short *) * MAX_ALBUM);
  music_data->duration = (unsigned int **) malloc(sizeof(unsigned int *) * MAX_ALBUM);

  int album_count = -1, track_count = 0;
  for (int i = 0; i < raw_music_data.size; i++) {
    /* if we discover a new album in the list we can assume sort from mpd */
    if (i == 0 || strcmp(raw_music_data.album[i], music_data->album[album_count])) {

      if (album_count >= 0) {
        /* resize for the album that was just processed */
        music_data->album_size[album_count] = track_count;
        music_data->id[album_count] = (char **) realloc(music_data->id[album_count], track_count);
        music_data->title[album_count] = (char **) realloc(music_data->title[album_count], track_count);
      music_data->track[album_count] = (short *) realloc(music_data->track[album_count], sizeof(short) * MAX_TRACK);
      music_data->duration[album_count] = (unsigned int *) realloc(music_data->duration[album_count], sizeof(unsigned int) * MAX_TRACK);
      }

      /* allows for iteration */
      track_count = 0;
      album_count++;

      /* set the value for all album values */
      music_data->album[album_count] = raw_music_data.album[i];
      music_data->artist[album_count] = raw_music_data.artist[i];
      music_data->date[album_count] = raw_music_data.date[i];
      /* and allocate memory for the track values */
      music_data->id[album_count] = (char **) malloc(sizeof(char *) * MAX_TRACK);
      music_data->title[album_count] = (char **) malloc(sizeof(char *) * MAX_TRACK);
      music_data->track[album_count] = (short *) malloc(sizeof(short) * MAX_TRACK);
      music_data->duration[album_count] = (unsigned int *) malloc(sizeof(unsigned int) * MAX_TRACK);
    }   
    /* we set the values of the track values of the current album */
    music_data->title[album_count][track_count] = raw_music_data.title[i];
    music_data->id[album_count][track_count] = raw_music_data.id[i];
    music_data->track[album_count][track_count] = raw_music_data.track[i];
    music_data->duration[album_count][track_count] = raw_music_data.duration[i];
    track_count++;
  }
  if (album_count >= 0) {
    /* resize for the album that was just processed */
    music_data->album_size[album_count] = track_count;
    music_data->id[album_count] = (char **) realloc(music_data->id[album_count], track_count);
    music_data->title[album_count] = (char **) realloc(music_data->title[album_count], track_count);
    music_data->track[album_count] = (short *) realloc(music_data->track[album_count], sizeof(short) * MAX_TRACK);
    music_data->duration[album_count] = (unsigned int *) realloc(music_data->duration[album_count], sizeof(unsigned int) * MAX_TRACK);
  }
  album_count++;

  /* we set the total album count */
  music_data->size = album_count;

  /* finally we reallocate the data to not waste space */
  music_data->artist = (char **) realloc(music_data->artist, sizeof(char *) * music_data->size);
  music_data->album = (char **) realloc(music_data->album, sizeof(char *) * music_data->size);
  music_data->date = (short *) realloc(music_data->date, sizeof(short) * music_data->size);
  music_data->album_size = (int *) realloc(music_data->album_size, sizeof(int) * music_data->size);
  music_data->id = (char ***) realloc(music_data->id, sizeof(char **) * music_data->size);
  music_data->title = (char ***) realloc(music_data->title, sizeof(char **) * music_data->size);
  music_data->track = (short **) realloc(music_data->track, sizeof(short *) * music_data->size);
  music_data->duration = (unsigned int **) realloc(music_data->duration, sizeof(unsigned int *) * music_data->size);
}

/**
 * @brief Compares the value of two elements of a music data struct returning which one is "bigger"
 * @param music_data The music data struct holding mpd data
 * @param i The first element to compare
 * @param j The second element to compare
 */
int
sort_compare ( struct music_data music_data, int i, int j )
{
  /* first we make sure that there is an artist, if one is missing it will go at the start */
  if (music_data.artist[i] == NULL || strlen(music_data.artist[i]) == 0) {
    return -1;
  }
  if (music_data.artist[j] == NULL || strlen(music_data.artist[j]) == 0) {
    return 1;
  }

  /* then we check for leading "The " */
  bool i_the = (strlen(music_data.artist[i]) > 4 && !strncmp(music_data.artist[i], "The ", 4));
  bool j_the = (strlen(music_data.artist[j]) > 4 && !strncmp(music_data.artist[j], "The ", 4));

  /* we calculate the difference between the strings */
  int diff_artist = strcmp((i_the) ? music_data.artist[i] + 4 : music_data.artist[i], (j_the) ? music_data.artist[j] + 4 : music_data.artist[j]);
  
  /* if we have reach a winner we exit */
  if (diff_artist != 0) {
    return diff_artist;
  }

  /* making sure there is a valid date */
  if (music_data.date[i] == 0) {
    return -1;
  } 
  if (music_data.date[j] == 0) {
    return 1;
  }
  
  /* we calculate the difference between the date */
  int diff_date = music_data.date[i] - music_data.date[j];

  /* if we have reached a winner we exit */
  if (diff_date != 0) {
    return diff_date;
  }

  /* first we make sure that there is an album, if one is missing it will go at the start */
  if (music_data.album[i] == NULL || strlen(music_data.album[i]) == 0) {
    return -1;
  }
  if (music_data.album[j] == NULL || strlen(music_data.album[j]) == 0) {
    return 1;
  }

  /* we calculate the difference between the album */
  int diff_album = strcmp(music_data.album[i], music_data.album[j]);

  /* we return that differnce this is the last seperation */
  return diff_album;
}

/**
 * @breif Swaps the value of two element in a music data struct
 * @param i The first element to swap
 * @param j The second element to swap
 */
void
sort_swap ( struct music_data *music_data, int i, int j )
{
  if (i == j)
    return;
  char *artist_tmp = music_data->artist[j]; 
  music_data->artist[j] = music_data->artist[i];
  music_data->artist[i] = artist_tmp;
  char *album_tmp = music_data->album[j];
  music_data->album[j] = music_data->album[i];
  music_data->album[i] = album_tmp;
  short date_tmp = music_data->date[j];
  music_data->date[j] = music_data->date[i];
  music_data->date[i] = date_tmp;
  int album_size_tmp = music_data->album_size[j];
  music_data->album_size[j] = music_data->album_size[i];
  music_data->album_size[i] = album_size_tmp;
  char **id_tmp = music_data->id[j];
  music_data->id[j] = music_data->id[i];
  music_data->id[i] = id_tmp;
  char **title_tmp = music_data->title[j];
  music_data->title[j] = music_data->title[i];
  music_data->title[i] = title_tmp;
  short *track_tmp = music_data->track[j];
  music_data->track[j] = music_data->track[i];
  music_data->track[i] = track_tmp;
  unsigned int *duration_tmp = music_data->duration[j];
  music_data->duration[j] = music_data->duration[i];
  music_data->duration[i] = duration_tmp;
}

/**
 * @brief Implements the quick sort algorithm on the music data struct
 * @param music_data The music data struct holding mpd data
 * @param low Index of the start index to sort from
 * @param high Index of the end index to sort from
 */
void
quick_sort_music_data ( struct music_data *music_data, int low, int high )
{
  if (low < high) {
    int pivot = high;
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
      if (sort_compare(*music_data, j, pivot) <= 0) {
        i++;
        if (i == pivot) {
          pivot = j;
        } else if (j == pivot) {
          pivot = i;
        }
        sort_swap(music_data, i, j);
      }
    }
    sort_swap(music_data, i + 1, high);

    int mid = i + 1;
    quick_sort_music_data(music_data, low, mid - 1);
    quick_sort_music_data(music_data, mid + 1, high);
  }
}

/**
 * @brief Sorts the music data struct
 * @param music data The music data struct holding mpd data
 */
void
sort_music_data ( struct music_data *music_data )
{
  quick_sort_music_data(music_data, 0, music_data->size -1);
}

/**
 * @brief Compares the content of a string and stops after first difference
 * @param a The fist string to compare
 * @param b The second string to compare
 */
bool
qck_strcmp ( const char *a, const char *b )
{
  if (a == NULL || b == NULL) {
    return false;
  }
  if (strlen(a) != strlen(b)) {
    return false;
  }
  for (int i = 0;  i < strlen(a); i++) {
    if (a[i] != b[i]) {
      return false;
    }
  }
  return true;
}

/**
 * @brief Returns the music data index from the name of a song - used for syncing
 * @param in The name of the song to find the index of
 * @param music_data The music data struct that contains the mpd data
 */
struct music_data_index
get_index_music_data ( const char *in, struct music_data music_data )
{
  struct music_data_index out;
  for (int i = 0; i < music_data.size; i++) {
    for (int j = 0; j < music_data.album_size[i]; j++) {
      if (qck_strcmp(in, music_data.id[i][j])) {
        out.index = j;
        out.album_index = i;
        return out;
      }
    }
  }
}

/**
 * @brief Frees the raw music data struct
 * @param raw_music_data Address of the raw_music_data struct to free
 */
void
free_raw_music_data ( struct raw_music_data *raw_music_data )
{
  free(raw_music_data->id);
  free(raw_music_data->artist);
  free(raw_music_data->album);
  free(raw_music_data->title);
  free(raw_music_data->date);
  free(raw_music_data->track);
  free(raw_music_data->duration);
}

/**
 * @brief Fress the music data struct
 * @param music_data Address of the music_data struct to free
 */
void
free_music_data ( struct music_data *music_data )
{
  for (int i = 0; i < music_data->size; i++) {
    for (int j = 0; j < music_data->album_size[i]; j++) {
      free(music_data->id[i][j]);
      free(music_data->title[i][j]);
    }
    free(music_data->artist[i]);
    free(music_data->album[i]);
    free(music_data->id[i]);
    free(music_data->title[i]);
    free(music_data->track[i]);
    free(music_data->duration[i]);
  }
  free(music_data->artist);
  free(music_data->album);
  free(music_data->date);
  free(music_data->album_size);
  free(music_data->id);
  free(music_data->title);
  free(music_data->track);
  free(music_data->duration);
}
