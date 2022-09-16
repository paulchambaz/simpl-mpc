/**
 * @file mpd_instance.c
 * @author Paul Chambaz
 * @date 16 Sep 2022
 * @brief Definition file to connect to the mpd connection and get music data
 * @license GPLv2
 */

#include "mpd_instance.h"

/**
 * @brief Connect to mpd using the mpd connection struct
 * @param mpd_connection Address value of the mpd_connection struct
 */
void
mpd_instance_connect ( struct mpd_connection **mpd_connection )
{
  /* connection using libmpdclient */
   *mpd_connection = mpd_connection_new(NULL, 0, 0);

  /* memory error check */
  if (!*mpd_connection) {
    fprintf(stderr, "Out of memory\n");
    exit(EXIT_FAILURE);
  }

  /* general mpd error check */
  if (mpd_connection_get_error(*mpd_connection) != MPD_ERROR_SUCCESS) {
    fprintf(stderr, "%s\n", *mpd_connection_get_error_message(*mpd_connection));
    mpd_connection_free(*mpd_connection);
    exit(EXIT_FAILURE);
  }
}

/**
 * @brief tiny function to copy faster the content of a string to another
 */
void
qck_strcpy ( char **dest, const char *source ) 
{
  *dest = (char *) malloc(sizeof(char) * (strlen(source) + 1));
  strncpy(*dest, source, strlen(source));
  (*dest)[strlen(source)] = '\0';
}

/**
 * @brief Queries the mpd instance to get metadata on every song
 * @param mpd_connection Address of the mpd_conntection struct
 * @param raw_music_data The raw music data struct that holds mpd metadata
 */
void
mpd_get_metadata ( struct mpd_connection **mpd_connection, struct raw_music_data *raw_music_data )
{
  mpd_send_list_all_meta(*mpd_connection, "");

  /* Initializes the raw music data arrays */
  int alloc_size = 1;
  raw_music_data->id = (char **) malloc(sizeof(char *) * alloc_size);
  raw_music_data->artist = (char **) malloc(sizeof(char *) * alloc_size);
  raw_music_data->album = (char **) malloc(sizeof(char *) * alloc_size);
  raw_music_data->title = (char **) malloc(sizeof(char *) * alloc_size);
  raw_music_data->date = (short *) malloc(sizeof(short) * alloc_size);
  raw_music_data->track = (short *) malloc(sizeof(short) * alloc_size);
  raw_music_data->duration = (int *) malloc(sizeof(int) * alloc_size);

  int size = 0;

  /* reads each song from the mpd song database */
  struct mpd_song *song;
  while ((song = mpd_recv_song(*mpd_connection))) {
    /* resizes the array if there is not enough space */
    if (size > alloc_size && size < MAX_MEM_SEARCH) {
      alloc_size *= 2;
      raw_music_data->id = (char **) realloc(raw_music_data->id, sizeof(char *) * alloc_size);
      raw_music_data->artist = (char **) realloc(raw_music_data->artist, sizeof(char *) * alloc_size);
      raw_music_data->album = (char **) realloc(raw_music_data->album, sizeof(char *) * alloc_size);
      raw_music_data->title = (char **) realloc(raw_music_data->title, sizeof(char *) * alloc_size);
      raw_music_data->date = (short *) realloc(raw_music_data->date, sizeof(short) * alloc_size);
      raw_music_data->track = (short *) realloc(raw_music_data->track, sizeof(short) * alloc_size);
      raw_music_data->duration = (int *) realloc(raw_music_data->duration, sizeof(int) * alloc_size);
    }

    /* Copies the content of each song to the array */
    qck_strcpy(&raw_music_data->id[size], mpd_song_get_uri(song));
    qck_strcpy(&raw_music_data->artist[size], mpd_song_get_tag(song, MPD_TAG_ARTIST, 0));
    qck_strcpy(&raw_music_data->album[size], mpd_song_get_tag(song, MPD_TAG_ALBUM, 0));
    qck_strcpy(&raw_music_data->title[size], mpd_song_get_tag(song, MPD_TAG_TITLE, 0));
    raw_music_data->date[size] = atoi(mpd_song_get_tag(song, MPD_TAG_DATE, 0));
    raw_music_data->track[size] = atoi(mpd_song_get_tag(song, MPD_TAG_TRACK, 0));
    raw_music_data->duration[size] = mpd_song_get_duration(song);

    mpd_song_free(song);
    size++;
  }

  /* we close the connection with the mpd command */
  mpd_response_finish(*mpd_connection);

  /* Reallocates to not waste memory space */
  raw_music_data->id = (char **) realloc(raw_music_data->id, sizeof(char *) * size);
  raw_music_data->artist = (char **) realloc(raw_music_data->artist, sizeof(char *) * size);
  raw_music_data->album = (char **) realloc(raw_music_data->album, sizeof(char *) * size);
  raw_music_data->title = (char **) realloc(raw_music_data->title, sizeof(char *) * size);
  raw_music_data->date = (short *) realloc(raw_music_data->date, sizeof(short) * size);
  raw_music_data->track = (short *) realloc(raw_music_data->track, sizeof(short) * size);
  raw_music_data->duration = (int *) realloc(raw_music_data->duration, sizeof(int) * size);

  raw_music_data->size = size;
}

/**
 * @brief Syncs the program mpd state with mpd
 * @param mpd_connection Address of the mpd connection struct
 * @param music_data The music data structure holding mpd information
 * @param ui_param The ui param holding information about the interface
 */
void
mpd_sync ( struct mpd_connection **mpd_connection, struct music_data music_data, struct ui_param *ui_param )
{
  /* we ask for status */
  mpd_send_status(*mpd_connection);

  /* we get the status info */
	struct mpd_status *status = mpd_recv_status(*mpd_connection);
  ui_param->volume = mpd_status_get_volume(status);
  ui_param->shuffle = mpd_status_get_random(status);
  ui_param->repeat = mpd_status_get_repeat(status);

  /* first we make sure that mpd is playing a song */
  if (mpd_status_get_state(status) != MPD_STATE_PLAY && mpd_status_get_state(status) != MPD_STATE_PAUSE) {
    mpd_status_free(status);
    /* we close the connection with the mpd command */
    mpd_response_finish(*mpd_connection);
    ui_param->playing = false;
    return;
  }

  /* we ask for current song info */
  mpd_send_current_song(*mpd_connection);

  /* we recieve the song */
  struct mpd_song *song = mpd_recv_song(*mpd_connection);
  if (!song) {
    return;
  }

  /* finally we can get the info */
  const char *current_uri = mpd_song_get_uri(song);
  struct music_data_index music_data_index = get_index_music_data(current_uri, music_data);
  ui_param->playing = true;
  if (ui_param->title_playing != music_data_index.index || ui_param->album_playing != music_data_index.album_index) {
    ui_param->title_playing = music_data_index.index;
    ui_param->album_playing = music_data_index.album_index;
    system(command_input);
  }
  ui_param->current_duration = mpd_status_get_elapsed_time(status);

  mpd_song_free(song);
  mpd_status_free(status);

  /* we close the connection with the mpd command */
  mpd_response_finish(*mpd_connection);
}
