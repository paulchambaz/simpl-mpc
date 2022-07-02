#ifndef INPUT_H
#define INPUT_H

#include <ncurses.h>

#include "config.h"

#include "interface.h"
#include "music_data.h"
#include "mpd_instance.h"

// list of input done from anywhere
void input_general ( char key, Interface *interface, Music_data *music_data, struct mpd_connection *connection );
// list of input done from the side window
void input_side ( char key, Interface *interface, Music_data music_data, struct mpd_connection *mpd_connection );
// list of input done from the main window
void input_main ( char key, Interface *interface, Music_data music_data, struct mpd_connection *mpd_connection );

// goes down one entry from the side window
void input_side_up ( WINDOW *side_window, WINDOW *main_window, Ui_param *ui_param, Ui_dimension ui_dimension, Music_data music_data );
// goes up one entry from the side window
void input_side_down ( WINDOW *side_window, WINDOW *main_window, Ui_param *ui_param, Ui_dimension ui_dimension, Music_data music_data );
// goes to the main window from the side window
void input_side_right ( WINDOW *side_window, WINDOW *main_window, Ui_param *ui_param, Ui_dimension ui_dimension, Music_data music_data );
// starts an entire album from the side window
void input_start_album ( struct mpd_connection *connection, Ui_param ui_param, Music_data music_data );

// goes down one entry from the main window
void input_main_down ( WINDOW *main_window, Ui_param *ui_param, Ui_dimension ui_dimension, Music_data music_data );
// goes up one entry from the main window
void input_main_up ( WINDOW *main_window, Ui_param *ui_param, Ui_dimension ui_dimension, Music_data music_data );
// goes to the side window from the main window
void input_main_left ( WINDOW *side_window, WINDOW *main_window, Ui_param *ui_param, Ui_dimension ui_dimension, Music_data music_data );
// starts a album and skip to the title selected from the main widonw
void input_start_title ( struct mpd_connection *connection, Ui_param ui_param, Music_data music_data );

// exits the program
void input_exit (struct mpd_connection *connection, Interface *interface, Music_data *music_data);
// toggle pause mpd
void input_toggle_pause ( struct mpd_connection *connection, Ui_param ui_param );
// skips to the next track
void input_next ( struct mpd_connection *connection, Ui_param ui_param );
// skips to the previous track
void input_prev ( struct mpd_connection *connection, Ui_param ui_param );
// raise volume
void input_volume_up ( struct mpd_connection *connection, Ui_param ui_param );
// lower volume
void input_volume_down ( struct mpd_connection *connection, Ui_param ui_param );
// toggle shuffle mode
void input_shuffle ( struct mpd_connection *connection, Ui_param ui_param );
// toggle repeat mode
void input_repeat ( struct mpd_connection *connection, Ui_param ui_param );
// clear the playlist
void input_clear ( struct mpd_connection *connection, Ui_param ui_param );
// seek forward in the song
void input_seek_forward ( struct mpd_connection *connection, Ui_param ui_param );
// seek backward in the song
void input_seek_backward ( struct mpd_connection *connection, Ui_param ui_param );

#endif

