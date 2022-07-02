#ifndef INPUT_H
#define INPUT_H

#include <ncurses.h>

#include "config.h"

#include "interface.h"
#include "music_data.h"
#include "mpd_instance.h"

// goes down one entry from the side window
void input_side_up ( WINDOW *side_window, WINDOW *main_window, Ui_param *ui_param, Ui_dimension ui_dimension, Music_data music_data );
// goes up one entry from the side window
void input_side_down ( WINDOW *side_window, WINDOW *main_window, Ui_param *ui_param, Ui_dimension ui_dimension, Music_data music_data );
// goes to the main window from the side window
void input_side_right ( WINDOW *side_window, WINDOW *main_window, Ui_param *ui_param, Ui_dimension ui_dimension, Music_data music_data );
// goes down one entry from the side window
void input_main_down ( WINDOW *main_window, Ui_param *ui_param, Ui_dimension ui_dimension, Music_data music_data );
// goes up one entry from the side window
void input_main_up ( WINDOW *main_window, Ui_param *ui_param, Ui_dimension ui_dimension, Music_data music_data );
// goes to the side window from the main window
void input_main_left ( WINDOW *side_window, WINDOW *main_window, Ui_param *ui_param, Ui_dimension ui_dimension, Music_data music_data );
// list of input done from the side window
void input_side ( char key, Interface *interface, Music_data music_data, struct mpd_connection *mpd_connection );
// list of input done from the main window
void input_main ( char key, Interface *interface, Music_data music_data, struct mpd_connection *mpd_connection );
// list of input done from anywhere
void input_general ( char key, Interface *interface, Music_data music_data, struct mpd_connection *connection );

#endif

