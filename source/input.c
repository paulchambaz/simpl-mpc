#include "input.h"

void
input_side_up (WINDOW *side_window, WINDOW *main_window, Ui_param *ui_param, Ui_dimension ui_dimension, Music_data music_data)
{
  ui_param->album_selected--;
  ui_param->title_selected = 0;
  if (ui_param->album_selected < 0) {
    ui_param->album_selected = 0;
  }
  if (ui_param->album_selected - ui_param->side_scroll < 0) {
    ui_param->side_scroll--;
  }
}

void
input_side_down (WINDOW *side_window, WINDOW *main_window, Ui_param *ui_param, Ui_dimension ui_dimension, Music_data music_data)
{
  ui_param->album_selected++;
  ui_param->title_selected = 0;
  if (ui_param->album_selected > music_data.size - 1) {
    ui_param->album_selected = music_data.size - 1;
  }
  if (ui_param->album_selected - ui_param->side_scroll > LINES - 2 - 2 - 1) {
    ui_param->side_scroll++;
  }
}

void
input_side_right (WINDOW *side_window, WINDOW *main_window, Ui_param *ui_param, Ui_dimension ui_dimension, Music_data music_data)
{
  ui_param->on_main = true;
}

void
input_start_album (struct mpd_connection *connection, Ui_param ui_param, Music_data music_data) {
  mpd_run_clear(connection);
  int i = ui_param.album_selected;
  for (int j = 0; j < music_data.album_size[i]; j++) {
    mpd_run_add_id(connection, music_data.id[i][j]);
  }
  mpd_run_play(connection);
}

void
input_side (char key, Interface *interface, Music_data music_data, struct mpd_connection *connection)
{
  switch (key) {
    case 'j':
      input_side_down(interface->side_window, interface->main_window, interface->ui_param, *interface->ui_dimension, music_data);
      break;
    case 'k':
      input_side_up(interface->side_window, interface->main_window, interface->ui_param, *interface->ui_dimension, music_data);
      break;
    case 'l':
      input_side_right(interface->side_window, interface->main_window, interface->ui_param, *interface->ui_dimension, music_data);
      break;
    case '\n':
      input_start_album(connection, *interface->ui_param, music_data);
      system(command_input);
      break;
  }
}

void
input_main_down (WINDOW *main_window, Ui_param *ui_param, Ui_dimension ui_dimension, Music_data music_data)
{
  ui_param->title_selected++;
  if (ui_param->title_selected > music_data.album_size[ui_param->album_selected] - 1) {
    ui_param->title_selected = music_data.album_size[ui_param->album_selected] - 1;
  }
  if (ui_param->title_selected - ui_param->main_scroll > LINES - 2 - 2 - 1) {
    ui_param->main_scroll++;
  }
}

void
input_main_up (WINDOW *main_window, Ui_param *ui_param, Ui_dimension ui_dimension, Music_data music_data)
{
  ui_param->title_selected--;
  if (ui_param->title_selected < 0) {
    ui_param->title_selected = 0;
  }
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
input_start_title (struct mpd_connection *connection, Ui_param ui_param, Music_data music_data) {
  mpd_run_clear(connection);
  int i = ui_param.album_selected;
  for (int j = 0; j < music_data.album_size[i]; j++) {
    mpd_run_add_id(connection, music_data.id[i][j]);
  }
  mpd_run_play(connection);
  for (int j = 0; j < ui_param.title_selected; j++) {
     mpd_run_next(connection);
  }
}

void
input_main (char key, Interface *interface, Music_data music_data, struct mpd_connection *connection)
{
  switch (key) {
    case 'j':
      input_main_down(interface->main_window, interface->ui_param, *interface->ui_dimension, music_data);
      break;
    case 'k':
      input_main_up(interface->main_window, interface->ui_param, *interface->ui_dimension, music_data);
      break;
    case 'h':
      input_main_left(interface->side_window, interface->main_window, interface->ui_param, *interface->ui_dimension, music_data);
      break;
    case '\n':
      input_start_title(connection, *interface->ui_param, music_data);
      system(command_input);
  }

}

void
input_toggle_pause (struct mpd_connection *connection, Ui_param ui_param)
{
  if (ui_param.playing) {
    mpd_run_toggle_pause(connection);
  } else {
    mpd_run_play(connection);
  }
}

void
input_next (struct mpd_connection *connection, Ui_param ui_param)
{
  if (ui_param.playing) {
    mpd_run_next(connection);
  }
}

void
input_prev (struct mpd_connection *connection, Ui_param ui_param)
{
  if (ui_param.playing) {
    mpd_run_previous(connection);
  }
}

void
input_volume_up (struct mpd_connection *connection, Ui_param ui_param)
{
  if (ui_param.playing) {
    mpd_run_change_volume(connection, +10);
  }
}

void
input_volume_down (struct mpd_connection *connection, Ui_param ui_param)
{
  if (ui_param.playing) {
    mpd_run_change_volume(connection, -10);
  }
}

void
input_shuffle (struct mpd_connection *connection, Ui_param ui_param)
{
  mpd_run_shuffle(connection);
}

void
input_repeat (struct mpd_connection *connection, Ui_param ui_param)
{
  mpd_run_repeat(connection, ui_param.repeat);
}

void
input_clear (struct mpd_connection *connection)
{
  mpd_run_clear(connection);
}

void
input_seek_forward (struct mpd_connection *connection, Ui_param ui_param)
{
  mpd_run_seek_current(connection, +5, true);
}

void
input_seek_backward (struct mpd_connection *connection, Ui_param ui_param)
{
  mpd_run_seek_current(connection, -5, true);
}

void
input_general (char key, Interface *interface, Music_data music_data, struct mpd_connection *connection)
{
  switch (key) {
    case ' ':
      input_toggle_pause(connection, *interface->ui_param);
      system(command_input);
      break;
    case 'n':
      input_next(connection, *interface->ui_param);
      system(command_input);
      break;
    case 'p':
      input_prev(connection, *interface->ui_param);
      system(command_input);
      break;
    case '=':
      input_volume_up(connection, *interface->ui_param);
      system(command_input);
      break;
    case '-':
      input_volume_down(connection, *interface->ui_param);
      system(command_input);
      break;
    case 's':
      input_shuffle(connection, *interface->ui_param);
      system(command_input);
      break;
    case 'r':
      input_repeat(connection, *interface->ui_param);
      system(command_input);
      break;
    case 'x':
      input_clear(connection);
      system(command_input);
      break;
    case '.':
      input_seek_forward(connection, *interface->ui_param);
      system(command_input);
      break;
    case ',':
      input_seek_backward(connection, *interface->ui_param);
      system(command_input);
      break;
  }
}
