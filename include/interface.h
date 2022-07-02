#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdbool.h>
#include <string.h>

#include <locale.h>
#include <curses.h>
#include <mpd/client.h>

#include "music_data.h"

typedef struct {
  int sidebar_width;
  int volume_width;
  int right_bottom_width;
} Ui_dimension;

typedef struct {
  bool on_main;
  int side_scroll;
  int main_scroll;
  int album_selected;
  int title_selected;
  bool playing;
  int album_playing;
  int title_playing;
  int volume;
  bool shuffle;
  bool repeat;
  unsigned int current_duration;
} Ui_param;

typedef struct {
  WINDOW *side_window;
  WINDOW *main_window;
  WINDOW *info_window;
  WINDOW *volume_window;
  WINDOW *bar_window;
  WINDOW *status_window;
  Ui_dimension *ui_dimension;
  Ui_param *ui_param;
} Interface;

// set the default values for the ui
void set_default_ui ( Interface *interface );
// initaliaze the interface and all its components
void init_interface ( Interface *interface );
// prints the side window
void print_side_window ( WINDOW *side_window, Ui_param ui_param, Ui_dimension ui_dimension, Music_data music_data );
// prints the main window
void print_main_window ( WINDOW *main_window, Ui_param ui_param, Ui_dimension ui_dimension, Music_data music_data );
// prints the info window
void print_info_window ( WINDOW *info_window, Ui_param ui_param, Ui_dimension ui_dimensio, Music_data music_data );
// prints the volume window
void print_volume_window ( WINDOW *volume_window, Ui_param ui_param, Ui_dimension ui_dimension );
// prints the bar window
void print_bar_window ( WINDOW *bar_window, Ui_param ui_param, Ui_dimension ui_dimension, Music_data music_data );
// prints the status window
void print_status_window ( WINDOW *status_window, Ui_param ui_param, Ui_dimension ui_dimension );
// prints the entire interface
void print_interface ( Interface *interface, Music_data music_data );
// refreshes all windows
void refresh_interface ( Interface *interface );
// quits the ncurses interface
void quit_interface ( Interface *interface );
// frees the data associated with the interface
void free_interface ( Interface *interface );

#endif
