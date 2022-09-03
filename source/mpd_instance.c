#include "mpd_instance.h"

void
mpd_instance_connect (struct mpd_connection **mpd_connection)
{
  // connection using libmpdclient
   *mpd_connection = mpd_connection_new(NULL, 0, 0);

  // memory error check
  if (!*mpd_connection) {
    fprintf(stderr, "Out of memory\n");
    exit(EXIT_FAILURE);
  }

  // general mpd error check
  if (mpd_connection_get_error(*mpd_connection) != MPD_ERROR_SUCCESS) {
    fprintf(stderr, "%s\n", *mpd_connection_get_error_message(*mpd_connection));
    mpd_connection_free(*mpd_connection);
    exit(EXIT_FAILURE);
  }
}

void
mpd_instance_free (struct mpd_connection **mpd_connection)
{
  mpd_connection_free(*mpd_connection);
}

char *
copy_str (const char *in) 
{
  char *out = (char *) malloc(sizeof(char) * (strlen(in) + 1));
  strncpy(out, in, MIN(strlen(in) + 1, MAX_STR_LEN));
  return out;
}

void
mpd_get_metadata (struct mpd_connection **mpd_connection, Raw_music_data *raw_music_data)
{
  mpd_send_list_all_meta(*mpd_connection, "");

  int alloc_size = 4096;
  raw_music_data->id = (char **) malloc(sizeof(char *) * alloc_size);
  raw_music_data->artist = (char **) malloc(sizeof(char *) * alloc_size);
  raw_music_data->album = (char **) malloc(sizeof(char *) * alloc_size);
  raw_music_data->title = (char **) malloc(sizeof(char *) * alloc_size);
  raw_music_data->date = (short *) malloc(sizeof(short) * alloc_size);
  raw_music_data->track = (short *) malloc(sizeof(short) * alloc_size);
  raw_music_data->duration = (int *) malloc(sizeof(int) * alloc_size);

  int size = 0;

  struct mpd_song *song;
  while ((song = mpd_recv_song(*mpd_connection))) {
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

    raw_music_data->id[size] = copy_str(mpd_song_get_uri(song));
    raw_music_data->artist[size] = copy_str(mpd_song_get_tag(song, MPD_TAG_ARTIST, 0));;
    raw_music_data->album[size] = copy_str(mpd_song_get_tag(song, MPD_TAG_ALBUM, 0));
    raw_music_data->title[size] = copy_str(mpd_song_get_tag(song, MPD_TAG_TITLE, 0));
    raw_music_data->date[size] = atoi(mpd_song_get_tag(song, MPD_TAG_DATE, 0));
    raw_music_data->track[size] = atoi(mpd_song_get_tag(song, MPD_TAG_TRACK, 0));
    raw_music_data->duration[size] = mpd_song_get_duration(song);

    mpd_song_free(song);

    size++;
  }

  // we close the connection with the mpd command
  mpd_response_finish(*mpd_connection);

  raw_music_data->id = (char **) realloc(raw_music_data->id, sizeof(char *) * size);
  raw_music_data->artist = (char **) realloc(raw_music_data->artist, sizeof(char *) * size);
  raw_music_data->album = (char **) realloc(raw_music_data->album, sizeof(char *) * size);
  raw_music_data->title = (char **) realloc(raw_music_data->title, sizeof(char *) * size);
  raw_music_data->date = (short *) realloc(raw_music_data->date, sizeof(short) * size);
  raw_music_data->track = (short *) realloc(raw_music_data->track, sizeof(short) * size);
  raw_music_data->duration = (int *) realloc(raw_music_data->duration, sizeof(int) * size);

  raw_music_data->size = size;
}

void
mpd_sync (struct mpd_connection **mpd_connection, Music_data music_data, Ui_param *ui_param)
{
  // we ask for status
  mpd_send_status(*mpd_connection);

  // we get the status info
	struct mpd_status *status = mpd_recv_status(*mpd_connection);
  ui_param->volume = mpd_status_get_volume(status);
  ui_param->shuffle = mpd_status_get_random(status);
  ui_param->repeat = mpd_status_get_repeat(status);

  // first we make sure that mpd is playing a song
  if (mpd_status_get_state(status) != MPD_STATE_PLAY && mpd_status_get_state(status) != MPD_STATE_PAUSE) {
    mpd_status_free(status);
    // we close the connection with the mpd command
    mpd_response_finish(*mpd_connection);
    ui_param->playing = false;
    return;
  }

  // we ask for current song info
  mpd_send_current_song(*mpd_connection);

  // we recieve the song
  struct mpd_song *song = mpd_recv_song(*mpd_connection);
  if (!song) {
    return;
  }

  // finally we can get the info
  const char *current_uri = mpd_song_get_uri(song);
  Music_data_index music_data_index = get_index_music_data(current_uri, music_data);
  ui_param->playing = true;
  if (ui_param->title_playing != music_data_index.index || ui_param->album_playing != music_data_index.album_index) {
    ui_param->title_playing = music_data_index.index;
    ui_param->album_playing = music_data_index.album_index;
    system(command_input);
  }
  ui_param->current_duration = mpd_status_get_elapsed_time(status);

  mpd_song_free(song);
  mpd_status_free(status);

  // we close the connection with the mpd command
  mpd_response_finish(*mpd_connection);

}

