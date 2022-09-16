/**
 * @file interface.h
 * @author Paul Chambaz
 * @date 16 Sep 2022
 * @brief Header file to print the interface
 * @license GPLv2
 */

#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdbool.h>
#include <string.h>

#include <locale.h>
#include <curses.h>
#include <mpd/client.h>

#include "music_data.h"

/* enum to hold colors used in the program */
enum colors {
  WHITE =  1,
  RED = 2,
  GREEN = 3,
  BG_WHITE = 4,
  BG_BLUE = 5,
  BG_GREEN = 6,
  BG_AQUA = 7,
};

/**
 * @brief Structure for holding information about the interface dimensions
 */
struct ui_dimension {
  int sidebar_width;
  int volume_width;
  int right_bottom_width;
};

/**
 * @brief Structure for holding information about the interface state
 */
struct ui_param {
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
};

/**
 * @brief General structure holding all interface related structures
 */
struct interface {
  WINDOW *side_window;
  WINDOW *main_window;
  WINDOW *info_window;
  WINDOW *volume_window;
  WINDOW *bar_window;
  WINDOW *status_window;
  struct ui_dimension *ui_dimension;
  struct ui_param *ui_param;
};

void set_default_ui ( struct interface *interface );
void set_ui_dimensions ( struct interface *interface ) ;
void init_interface ( struct interface *interface );
void print_side_window ( WINDOW *side_window, struct ui_param ui_param, struct ui_dimension ui_dimension, struct music_data music_data );
void print_main_window ( WINDOW *main_window, struct ui_param ui_param, struct ui_dimension ui_dimension, struct music_data music_data );
void print_info_window ( WINDOW *info_window, struct ui_param ui_param, struct ui_dimension ui_dimensio, struct music_data music_data );
void print_volume_window ( WINDOW *volume_window, struct ui_param ui_param, struct ui_dimension ui_dimension );
void print_bar_window ( WINDOW *bar_window, struct ui_param ui_param, struct ui_dimension ui_dimension, struct music_data music_data );
void print_status_window ( WINDOW *status_window, struct ui_param ui_param, struct ui_dimension ui_dimension );
void print_interface ( struct interface *interface, struct music_data music_data );
void destroy_interface ( struct interface *interface );
void free_interface ( struct interface *interface );

#endif
