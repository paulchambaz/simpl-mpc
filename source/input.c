#include "input.h"

  void
input_side (char key, Interface *interface, Music_data music_data, struct mpd_connection *connection)
{
  for (int i = 0; i < sizeof(keys) / sizeof(Key); i++) {
    if (keys[i].context == SIDE) {
      if (key == keys[i].key) {
        switch (keys[i].function) {
          case SIDE_DOWN:
            input_side_down(interface->side_window, interface->main_window, interface->ui_param, *interface->ui_dimension, music_data);
            break;
          case SIDE_UP:
            input_side_up(interface->side_window, interface->main_window, interface->ui_param, *interface->ui_dimension, music_data);
            break;
          case SIDE_MAIN:
            input_side_right(interface->side_window, interface->main_window, interface->ui_param, *interface->ui_dimension, music_data);
            break;
          case SIDE_START:
            input_start_album(connection, *interface->ui_param, music_data);
            system(command_input);
            break;
        }
      }
    }
  }
}

void
input_main (char key, Interface *interface, Music_data music_data, struct mpd_connection *connection)
{
  for (int i = 0; i < sizeof(keys) / sizeof(Key); i++) {
    if (keys[i].context == MAIN) {
      if (key == keys[i].key) {
        switch (keys[i].function) {
          case MAIN_DOWN:
            input_main_down(interface->main_window, interface->ui_param, *interface->ui_dimension, music_data);
            break;
          case MAIN_UP:
            input_main_up(interface->main_window, interface->ui_param, *interface->ui_dimension, music_data);
            break;
          case MAIN_SIDE:
            input_main_left(interface->side_window, interface->main_window, interface->ui_param, *interface->ui_dimension, music_data);
            break;
          case MAIN_START:
            input_start_title(connection, *interface->ui_param, music_data);
            system(command_input);
            break;
        }
      }
    }
  }
}

void
input_general (char key, Interface *interface, Music_data *music_data, struct mpd_connection *connection)
{
  for (int i = 0; i < sizeof(keys) / sizeof(Key); i++) {
    if (keys[i].context == GENERAL) {
      if (key == keys[i].key) {
        switch (keys[i].function) {
          case EXIT:
            input_exit(connection, interface, music_data);
            break;
          case PAUSE:
            input_toggle_pause(connection, *interface->ui_param);
            system(command_input);
            break;
          case NEXT:
            input_next(connection, *interface->ui_param);
            system(command_input);
            break;
          case PREVIOUS:
            input_prev(connection, *interface->ui_param);
            system(command_input);
            break;
          case VOLUME_UP:
            input_volume_up(connection, *interface->ui_param);
            system(command_input);
            break;
          case VOLUME_DOWN:
            input_volume_down(connection, *interface->ui_param);
            system(command_input);
            break;
          case SHUFFLE:
            input_shuffle(connection, *interface->ui_param);
            system(command_input);
            break;
          case REPEAT:
            input_repeat(connection, *interface->ui_param);
            system(command_input);
            break;
          case CLEAR:
            input_clear(connection, *interface->ui_param);
            system(command_input);
            break;
          case SEEK_FORWARD:
            input_seek_forward(connection, *interface->ui_param);
            system(command_input);
            break;
          case SEEK_BACKWARD:
            input_seek_backward(connection, *interface->ui_param);
            system(command_input);
            break;
        }
      }
    }
  }
}

void
input_side_down (WINDOW *side_window, WINDOW *main_window, Ui_param *ui_param, Ui_dimension ui_dimension, Music_data music_data)
{
  ui_param->album_selected++;
  // reset the title selected when leaving the entry
  ui_param->title_selected = 0;
  // prevents going lower than last album
  if (ui_param->album_selected > music_data.size - 1) {
    ui_param->album_selected = music_data.size - 1;
  }
  // moves scroll down
  if (ui_param->album_selected - ui_param->side_scroll > LINES - 2 - 2 - 1) {
    ui_param->side_scroll++;
  }
}

void
input_side_up (WINDOW *side_window, WINDOW *main_window, Ui_param *ui_param, Ui_dimension ui_dimension, Music_data music_data)
{
  ui_param->album_selected--;
  // reset the title selected when leaving the entry
  ui_param->title_selected = 0;
  // prevents going higher than first album
  if (ui_param->album_selected < 0) {
    ui_param->album_selected = 0;
  }
  // moves scroll up
  if (ui_param->album_selected - ui_param->side_scroll < 0) {
    ui_param->side_scroll--;
  }
}

void
input_side_right (WINDOW *side_window, WINDOW *main_window, Ui_param *ui_param, Ui_dimension ui_dimension, Music_data music_data)
{
  ui_param->on_main = true;
}

void
input_start_album (struct mpd_connection *connection, Ui_param ui_param, Music_data music_data)
{
  // first remove all album from the playlist
  mpd_run_clear(connection);
  // then add all the songs from the selected album to the playlist
  int i = ui_param.album_selected;
  for (int j = 0; j < music_data.album_size[i]; j++) {
    mpd_run_add_id(connection, music_data.id[i][j]);
  }
  // finally start playing
  mpd_run_play(connection);
  mpd_response_finish(connection);
}

void
input_main_down (WINDOW *main_window, Ui_param *ui_param, Ui_dimension ui_dimension, Music_data music_data)
{
  ui_param->title_selected++;
  // prevents going lower than last song on the album
  if (ui_param->title_selected > music_data.album_size[ui_param->album_selected] - 1) {
    ui_param->title_selected = music_data.album_size[ui_param->album_selected] - 1;
  }
  // scroll down
  if (ui_param->title_selected - ui_param->main_scroll > LINES - 2 - 2 - 1) {
    ui_param->main_scroll++;
  }
}

void
input_main_up (WINDOW *main_window, Ui_param *ui_param, Ui_dimension ui_dimension, Music_data music_data)
{
  ui_param->title_selected--;
  // prevents going higher than first song on the album
  if (ui_param->title_selected < 0) {
    ui_param->title_selected = 0;
  }
  // scroll up
  if (ui_param->title_selected - ui_param->main_scroll < 0) {
    ui_param->main_scroll--;
  }
}

void
input_main_left (WINDOW *side_window, WINDOW *main_window, Ui_param *ui_param, Ui_dimension ui_dimension, Music_data music_data)
{
  ui_param->on_main = false;
}

void
input_start_title (struct mpd_connection *connection, Ui_param ui_param, Music_data music_data)
{
  // first clear the connection
  mpd_run_clear(connection);
  // then add all the song of the selected album
  int i = ui_param.album_selected;
  for (int j = 0; j < music_data.album_size[i]; j++) {
    mpd_run_add_id(connection, music_data.id[i][j]);
  }
  // then start playing
  mpd_run_play(connection);
  // and immediatly skip to the selected title
  for (int j = 0; j < ui_param.title_selected; j++) {
     mpd_run_next(connection);
  }
  mpd_response_finish(connection);
}

void
input_exit (struct mpd_connection *connection, Interface *interface, Music_data *music_data)
{
  // free all data structures
  quit_interface(interface);
  free_music_data(music_data);
  mpd_instance_free(&connection);
  endwin();
  // and exit the program
  exit(EXIT_SUCCESS);
}

void
input_toggle_pause (struct mpd_connection *connection, Ui_param ui_param)
{
  if (ui_param.playing) {
    // toggle pause if mpd is playing
    mpd_run_toggle_pause(connection);
  } else {
    // if not just start the current playlist
    mpd_run_play(connection);
  }
  mpd_response_finish(connection);
}

void
input_next (struct mpd_connection *connection, Ui_param ui_param)
{
  if (ui_param.playing) {
    mpd_run_next(connection);
    mpd_response_finish(connection);
  }
}

void
input_prev (struct mpd_connection *connection, Ui_param ui_param)
{
  if (ui_param.playing) {
    mpd_run_previous(connection);
    mpd_response_finish(connection);
  }
}

void
input_volume_up (struct mpd_connection *connection, Ui_param ui_param)
{
  if (ui_param.playing) {
    // raise volume by volume_incr defined in config.h
    mpd_run_change_volume(connection, +volume_incr);
    mpd_response_finish(connection);
  }
}

void
input_volume_down (struct mpd_connection *connection, Ui_param ui_param)
{
  if (ui_param.playing) {
    // lower volume by volume_incr defined in config.h
    mpd_run_change_volume(connection, -volume_incr);
    mpd_response_finish(connection);
  }
}

void
input_shuffle (struct mpd_connection *connection, Ui_param ui_param)
{
  mpd_run_random(connection, !ui_param.shuffle);
  mpd_response_finish(connection);
}

void
input_repeat (struct mpd_connection *connection, Ui_param ui_param)
{
  mpd_run_repeat(connection, !ui_param.repeat);
  mpd_response_finish(connection);
}

void
input_clear (struct mpd_connection *connection, Ui_param ui_param)
{
  if (ui_param.playing) {
    mpd_run_clear(connection);
    mpd_response_finish(connection);
  }
}

void
input_seek_forward (struct mpd_connection *connection, Ui_param ui_param)
{
  if (ui_param.playing) {
    // seek forward by seek_incr defined in config.h
    mpd_run_seek_current(connection, +seek_incr, true);
    mpd_response_finish(connection);
  }
}

void
input_seek_backward (struct mpd_connection *connection, Ui_param ui_param)
{
  if (ui_param.playing) {
    // seek forward by seek_incr defined in config.h
    mpd_run_seek_current(connection, -seek_incr, true);
    mpd_response_finish(connection);
  }
}
