/**
 * @file input.h
 * @author Paul Chambaz
 * @date 16 Sep 2022
 * @brief Header file to functions dealing with the user input
 * @license GPLv2
 */

#ifndef INPUT_H
#define INPUT_H

#include <ncurses.h>

#include "config.h"

#include "interface.h"
#include "music_data.h"
#include "mpd_instance.h"

#define MAX(x, y) x > y ? x : y

void input_general ( int key, struct interface *interface, struct music_data *music_data, struct mpd_connection *connection );
void input_side ( int key, struct interface *interface, struct music_data music_data, struct mpd_connection *mpd_connection );
void input_main ( int key, struct interface *interface, struct music_data music_data, struct mpd_connection *mpd_connection );

void input_side_up ( WINDOW *side_window, WINDOW *main_window, struct ui_param *ui_param, struct ui_dimension ui_dimension, struct music_data music_data );
void input_side_down ( WINDOW *side_window, WINDOW *main_window, struct ui_param *ui_param, struct ui_dimension ui_dimension, struct music_data music_data );
void input_side_right ( WINDOW *side_window, WINDOW *main_window, struct ui_param *ui_param, struct ui_dimension ui_dimension, struct music_data music_data );
void input_start_album ( struct mpd_connection *connection, struct ui_param ui_param, struct music_data music_data );
void input_side_bottom ( WINDOW *side_window, WINDOW *main_window, struct ui_param *ui_param, struct ui_dimension ui_dimension, struct music_data music_data );
void input_side_top ( WINDOW *side_window, WINDOW *main_window, struct ui_param *ui_param, struct ui_dimension ui_dimension, struct music_data music_data);

void input_main_down ( WINDOW *main_window, struct ui_param *ui_param, struct ui_dimension ui_dimension, struct music_data music_data );
void input_main_up ( WINDOW *main_window, struct ui_param *ui_param, struct ui_dimension ui_dimension, struct music_data music_data );
void input_main_left ( WINDOW *side_window, WINDOW *main_window, struct ui_param *ui_param, struct ui_dimension ui_dimension, struct music_data music_data );
void input_start_title ( struct mpd_connection *connection, struct ui_param ui_param, struct music_data music_data );
void input_main_bottom ( WINDOW *main_window, struct ui_param *ui_param, struct ui_dimension ui_dimension, struct music_data music_data );
void input_main_top ( WINDOW *main_window, struct ui_param *ui_param, struct ui_dimension ui_dimension, struct music_data music_data);

void input_exit (struct mpd_connection *connection, struct interface *interface, struct music_data *music_data);
void input_toggle_pause ( struct mpd_connection *connection, struct ui_param ui_param );
void input_next ( struct mpd_connection *connection, struct ui_param ui_param );
void input_prev ( struct mpd_connection *connection, struct ui_param ui_param );
void input_volume_up ( struct mpd_connection *connection, struct ui_param ui_param );
void input_volume_down ( struct mpd_connection *connection, struct ui_param ui_param );
void input_shuffle ( struct mpd_connection *connection, struct ui_param ui_param );
void input_repeat ( struct mpd_connection *connection, struct ui_param ui_param );
void input_clear ( struct mpd_connection *connection, struct ui_param ui_param );
void input_seek_forward ( struct mpd_connection *connection, struct ui_param ui_param );
void input_seek_backward ( struct mpd_connection *connection, struct ui_param ui_param );

#endif

