/**
 * @file input.c
 * @author Paul Chambaz
 * @date 16 Sep 2022
 * @brief Definition file to functions dealing with the user input
 * @license GPLv2
 */

#include "input.h"

/**
 * @brief Main input function for dealing with all inputs on the side window
 * @param key Value of the key the user pressed
 * @param interface Interface struct for the current interface state
 * @param music_data Music data struct to store the mpd data
 * @param connection The mpd connection
 */
void
input_side ( int key, struct interface *interface, struct music_data music_data, struct mpd_connection *connection )
{
  for (int i = 0; i < sizeof(keys) / sizeof(struct key); i++) {
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
          case SIDE_TOP:
            input_side_top(interface->side_window, interface->main_window, interface->ui_param, *interface->ui_dimension, music_data);
            break;
          case SIDE_BOTTOM:
            input_side_bottom(interface->side_window, interface->main_window, interface->ui_param, *interface->ui_dimension, music_data);
            break;
        }
      }
    }
  }
}

/**
 * @brief Main input function for dealing with all inputs on the main window
 * @param key Value of the key the user pressed
 * @param interface Interface struct for the current interface state
 * @param music_data Music data struct to store the mpd data
 * @param connection The mpd connection
 */
void
input_main ( int key, struct interface *interface, struct music_data music_data, struct mpd_connection *connection )
{
  for (int i = 0; i < sizeof(keys) / sizeof(struct key); i++) {
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
          case MAIN_TOP:
            input_main_top(interface->main_window, interface->ui_param, *interface->ui_dimension, music_data);
            break;
          case MAIN_BOTTOM:
            input_main_bottom(interface->main_window, interface->ui_param, *interface->ui_dimension, music_data);
            break;
        }
      }
    }
  }
}

/**
 * @brief Main input function for dealing with all inputs regarless of the context
 * @param key Value of the key the user pressed
 * @param interface Interface struct for the current interface state
 * @param music_data Music data struct to store the mpd data
 * @param connection The mpd connection
 */
void
input_general ( int key, struct interface *interface, struct music_data *music_data, struct mpd_connection *connection )
{
  for (int i = 0; i < sizeof(keys) / sizeof(struct key); i++) {
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

/**
 * @brief Modifies the interface state to scroll the side window down
 * @param side_window The side window from the interface
 * @param main_window The main window from the interface
 * @param ui_param The ui parameter struct from the interface
 * @param ui_dimension The ui dimension struct from the interface
 * @param music_data The music data struct containing the mpd data
 */
void
input_side_down ( WINDOW *side_window, WINDOW *main_window, struct ui_param *ui_param, struct ui_dimension ui_dimension, struct music_data music_data )
{
  ui_param->album_selected++;
  /* reset the title selected when leaving the entry */
  ui_param->title_selected = 0;
  /* prevents going lower than last album */
  if (ui_param->album_selected > music_data.size - 1) {
    ui_param->album_selected = music_data.size - 1;
  }
  /* moves scroll down */
  if (ui_param->album_selected - ui_param->side_scroll > LINES - 2 - 2 - 1) {
    ui_param->side_scroll++;
  }
}

/**
 * @brief Modifies the interface state to scroll the side window up
 * @param side_window The side window from the interface
 * @param main_window The main window from the interface
 * @param ui_param The ui parameter struct from the interface
 * @param ui_dimension The ui dimension struct from the interface
 * @param music_data The music data struct containing the mpd data
 */
void
input_side_up ( WINDOW *side_window, WINDOW *main_window, struct ui_param *ui_param, struct ui_dimension ui_dimension, struct music_data music_data )
{
  ui_param->album_selected--;
  /* reset the title selected when leaving the entry */
  ui_param->title_selected = 0;
  /* prevents going higher than first album */
  if (ui_param->album_selected < 0) {
    ui_param->album_selected = 0;
  }
  /* moves scroll up */
  if (ui_param->album_selected - ui_param->side_scroll < 0) {
    ui_param->side_scroll--;
  }
}

/**
 * @brief Modifies the interface state to scroll the side window to the bottom
 * @param side_window The side window from the interface
 * @param main_window The main window from the interface
 * @param ui_param The ui parameter struct from the interface
 * @param ui_dimension The ui dimension struct from the interface
 * @param music_data The music data struct containing the mpd data
 */
void
input_side_bottom ( WINDOW *side_window, WINDOW *main_window, struct ui_param *ui_param, struct ui_dimension ui_dimension, struct music_data music_data )
{
  ui_param->album_selected = music_data.size - 1;
  ui_param->side_scroll = MAX(0, music_data.size - 1 - LINES + 5);
}

/**
 * @brief Modifies the interface state to scroll the side window to the top
 * @param side_window The side window from the interface
 * @param main_window The main window from the interface
 * @param ui_param The ui parameter struct from the interface
 * @param ui_dimension The ui dimension struct from the interface
 * @param music_data The music data struct containing the mpd data
 */
void
input_side_top ( WINDOW *side_window, WINDOW *main_window, struct ui_param *ui_param, struct ui_dimension ui_dimension, struct music_data music_data)
{
  ui_param->album_selected = 0;
  ui_param->side_scroll = 0;
}

/**
 * @brief Modifies the interface state to move to the main window
 * @param side_window The side window from the interface
 * @param main_window The main window from the interface
 * @param ui_param The ui parameter struct from the interface
 * @param ui_dimension The ui dimension struct from the interface
 * @param music_data The music data struct containing the mpd data
 */
void
input_side_right ( WINDOW *side_window, WINDOW *main_window, struct ui_param *ui_param, struct ui_dimension ui_dimension, struct music_data music_data )
{
  ui_param->on_main = true;
}

/**
 * @brief Starts an album from the side window - always clears the mpd queue of songs, then add all the song of the album to the mpd queue for loop and then plays the song
 * @param connection The mpd connection
 * @param ui_param The ui param struct from the interface
 * @param music_data The music data struct containing the mpd data
 */
void
input_start_album ( struct mpd_connection *connection, struct ui_param ui_param, struct music_data music_data )
{
  /* first remove all album from the playlist */
  mpd_run_clear(connection);
  /* then add all the songs from the selected album to the playlist */
  int i = ui_param.album_selected;
  for (int j = 0; j < music_data.album_size[i]; j++) {
    mpd_run_add_id(connection, music_data.id[i][j]);
  }
  /* finally start playing */
  mpd_run_play(connection);
  mpd_response_finish(connection);
}

/**
 * @brief Modifies the interface state to scroll the main window down
 * @param main_window The main window from the interface
 * @param ui_param The ui parameter struct from the interface
 * @param ui_dimension The ui dimension struct from the interface
 * @param music_data The music data struct containing the mpd data
 */
void
input_main_down ( WINDOW *main_window, struct ui_param *ui_param, struct ui_dimension ui_dimension, struct music_data music_data )
{
  ui_param->title_selected++;
  /* prevents going lower than last song on the album */
  if (ui_param->title_selected > music_data.album_size[ui_param->album_selected] - 1) {
    ui_param->title_selected = music_data.album_size[ui_param->album_selected] - 1;
  }
  /* scroll down */
  if (ui_param->title_selected - ui_param->main_scroll > LINES - 2 - 2 - 1) {
    ui_param->main_scroll++;
  }
}

/**
 * @brief Modifies the interface state to scroll the main window up
 * @param main_window The main window from the interface
 * @param ui_param The ui parameter struct from the interface
 * @param ui_dimension The ui dimension struct from the interface
 * @param music_data The music data struct containing the mpd data
 */
void
input_main_up ( WINDOW *main_window, struct ui_param *ui_param, struct ui_dimension ui_dimension, struct music_data music_data )
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

/**
 * @brief Modifies the interface state to scroll the main window to the bottom
 * @param main_window The main window from the interface
 * @param ui_param The ui parameter struct from the interface
 * @param ui_dimension The ui dimension struct from the interface
 * @param music_data The music data struct containing the mpd data
 */
void
input_main_bottom (  WINDOW *main_window, struct ui_param *ui_param, struct ui_dimension ui_dimension, struct music_data music_data  )
{
  ui_param->title_selected = music_data.album_size[ui_param->album_selected] - 1;
  ui_param->main_scroll = MAX(0, music_data.album_size[ui_param->album_selected] - 1 - LINES + 5);
}

/**
 * @brief Modifies the interface state to scroll the main window to the top
 * @param main_window The main window from the interface
 * @param ui_param The ui parameter struct from the interface
 * @param ui_dimension The ui dimension struct from the interface
 * @param music_data The music data struct containing the mpd data
 */
void
input_main_top (  WINDOW *main_window, struct ui_param *ui_param, struct ui_dimension ui_dimension, struct music_data music_data )
{
  ui_param->title_selected = 0;
  ui_param->main_scroll = 0;
}

/**
 * @brief Modifies the interface state to move to the side window
 * @param main_window The main window from the interface
 * @param ui_param The ui parameter struct from the interface
 * @param ui_dimension The ui dimension struct from the interface
 * @param music_data The music data struct containing the mpd data
 */
void
input_main_left ( WINDOW *side_window, WINDOW *main_window, struct ui_param *ui_param, struct ui_dimension ui_dimension, struct music_data music_data )
{
  ui_param->on_main = false;
}

/**
 * @brief Starts a song from an album from the main window - always clears the mpd queue of songs, then add all the song of the album to the mpd queue for loop, then skip to that song and then plays the song
 * @param connection The mpd connection
 * @param ui_param The ui param struct from the interface
 * @param music_data The music data struct containing the mpd data
 */
void
input_start_title ( struct mpd_connection *connection, struct ui_param ui_param, struct music_data music_data )
{
  /* first clear the connection */
  mpd_run_clear(connection);
  /* then add all the song of the selected album */
  int i = ui_param.album_selected;
  for (int j = 0; j < music_data.album_size[i]; j++) {
    mpd_run_add_id(connection, music_data.id[i][j]);
  }
  /* then start playing */
  mpd_run_play(connection);
  /* and immediatly skip to the selected title */
  for (int j = 0; j < ui_param.title_selected; j++) {
     mpd_run_next(connection);
  }
  mpd_response_finish(connection);
}

/**
 * @brief Exits the program by free all objects, closing the mpd connection and ending ncurses
 * @param connection The mpd connection
 * @param interface The interface struct containing the state of the interface
 * @param music_data The music data struct containing the music data from mpd
 */
void
input_exit ( struct mpd_connection *connection, struct interface *interface, struct music_data *music_data )
{
  /* free all data structures */
  destroy_interface(interface);
  free_interface(interface);
  free_music_data(music_data);
  mpd_connection_free(connection);
  endwin();
  /* and exit the program */
  exit(EXIT_SUCCESS);
}

/**
 * @brief Toggle pause the mpd music playing
 * @param connection The mpd connection
 * @param ui_param The ui param struct containing state of the interface
 */
void
input_toggle_pause ( struct mpd_connection *connection, struct ui_param ui_param )
{
  if (ui_param.playing) {
    /* toggle pause if mpd is playing */
    mpd_run_toggle_pause(connection);
  } else {
    /* if not just start the current playlist */
    mpd_run_play(connection);
  }
  mpd_response_finish(connection);
}

/**
 * @brief Skips to the next song on the mpd music playing
 * @param connection The mpd connection
 * @param ui_param The ui param struct containing state of the interface
 */
void
input_next ( struct mpd_connection *connection, struct ui_param ui_param )
{
  if (ui_param.playing) {
    mpd_run_next(connection);
    mpd_response_finish(connection);
  }
}

/**
 * @brief Skips to the previous song on the mpd music playing
 * @param connection The mpd connection
 * @param ui_param The ui param struct containing state of the interface
 */
void
input_prev ( struct mpd_connection *connection, struct ui_param ui_param )
{
  if (ui_param.playing) {
    mpd_run_previous(connection);
    mpd_response_finish(connection);
  }
}

/**
 * @brief Highers volume of the mpd music playing
 * @param connection The mpd connection
 * @param ui_param The ui param struct containing state of the interface
 */
void
input_volume_up ( struct mpd_connection *connection, struct ui_param ui_param )
{
  if (ui_param.playing) {
    /* raise volume by volume_incr defined in config.h */
    mpd_run_change_volume(connection, +volume_incr);
    mpd_response_finish(connection);
  }
}

/**
 * @brief Lowers volume of the mpd music playing
 * @param connection The mpd connection
 * @param ui_param The ui param struct containing state of the interface
 */
void
input_volume_down ( struct mpd_connection *connection, struct ui_param ui_param )
{
  if (ui_param.playing) {
    /* lower volume by volume_incr defined in config.h */
    mpd_run_change_volume(connection, -volume_incr);
    mpd_response_finish(connection);
  }
}

/**
 * @brief Toggle shuffle for the mpd music playing
 * @param connection The mpd connection
 * @param ui_param The ui param struct containing state of the interface
 */
void
input_shuffle ( struct mpd_connection *connection, struct ui_param ui_param )
{
  mpd_run_random(connection, !ui_param.shuffle);
  mpd_response_finish(connection);
}

/**
 * @brief Toggle repeat for the mpd music playing
 * @param connection The mpd connection
 * @param ui_param The ui param struct containing state of the interface
 */
void
input_repeat ( struct mpd_connection *connection, struct ui_param ui_param )
{
  mpd_run_repeat(connection, !ui_param.repeat);
  mpd_response_finish(connection);
}

/**
 * @brief Clears the mpd music playing
 * @param connection The mpd connection
 * @param ui_param The ui param struct containing state of the interface
 */
void
input_clear ( struct mpd_connection *connection, struct ui_param ui_param )
{
  if (ui_param.playing) {
    mpd_run_clear(connection);
    mpd_response_finish(connection);
  }
}

/**
 * @brief Goes forward in the mpd music playing
 * @param connection The mpd connection
 * @param ui_param The ui param struct containing state of the interface
 */
void
input_seek_forward ( struct mpd_connection *connection, struct ui_param ui_param )
{
  if (ui_param.playing) {
    /* seek forward by seek_incr defined in config.h */
    mpd_run_seek_current(connection, +seek_incr, true);
    mpd_response_finish(connection);
  }
}

/**
 * @brief Goes backward in the mpd music playing
 * @param connection The mpd connection
 * @param ui_param The ui param struct containing state of the interface
 */
void
input_seek_backward ( struct mpd_connection *connection, struct ui_param ui_param )
{
  if (ui_param.playing) {
    /* seek forward by seek_incr defined in config.h */
    mpd_run_seek_current(connection, -seek_incr, true);
    mpd_response_finish(connection);
  }
}

/**
 * @brief Update the mpd database
 * @param connection The mpd connection
 * @param music_data The music data object holding the mpd data
 */
void
input_update ( struct mpd_connection *connection, struct ui_param ui_parma, struct music_data *music_data )
{
  /* TODO: implement a basic update mechanism - first get the data to a raw_music_data struct, then sort it, then replace the current music_data
   * TODO: think of a way to do all this while the songs are changing and the songs are playing - we want the same song - if it still exists - to be highlighted */
}
