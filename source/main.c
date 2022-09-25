/**
 * @file main.c
 * @author Paul Chambaz
 * @date 16 Sep 2022
 * @brief Main file of the program
 * @license GPLv2
 *
 * simpl-mpc is a simple mpd music client. It is designed with
 * albums use in mind. You can start albums easily, loop them, 
 * start them in the middle and that's about it.
 *
 * simpl-mpc is free software you can dedistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Fondataion; following
 * version 2 or (at your option) any later version.
 *
 * simpl-mpc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the impled warranty of
 * MERCHANTABILITY or FITENESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License version 2 for more details.
 * 
 * You should have recieved a copy of the GNU General Public License
 * version 2 along with simpl-mpc. If not, see 
 * <http://www.gnu.org/licenses/>/
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>

#include <mpd/client.h>
#include <curses.h>

#include "mpd_instance.h"
#include "music_data.h"
#include "interface.h"
#include "input.h"

int
main(int argc, int *argv[])
{
  /* Establishes an mpd_connection using the mpd/client.h project */
  struct mpd_connection *mpd_connection;
  mpd_instance_connect(&mpd_connection);

  /* Recuperates all music files from mpd */
  struct raw_music_data raw_music_data;
  mpd_get_metadata(&mpd_connection, &raw_music_data);

  /* Sort and orders the music data to ready for display */
  struct music_data music_data;
  raw_to_music_data(raw_music_data, &music_data);
  free_raw_music_data(&raw_music_data);
  sort_music_data(&music_data);

  /* Initializes the interface, get information on the mpd state */
  struct interface interface;
  set_default_ui(&interface);
  mpd_sync(&mpd_connection, music_data, interface.ui_param);
  init_interface(&interface);
  print_interface(&interface, music_data);

  int key;
  int lines = LINES, cols = COLS;

  /* Main event loop of the program, updates when pressing a key or on getch timeout */
  while ((key = getch())) {

    /* Automatically resizes the window when detecting a size change */
    if (lines != LINES || cols != COLS) {
      destroy_interface(&interface);
      set_ui_dimensions(&interface);
      lines = LINES;
      cols = COLS;
    }

    /* Input management, for the program and for the individual windows */
    input_general(key, &interface, &music_data, mpd_connection);
    if (!interface.ui_param->on_main) {
      input_side(key, &interface, music_data, mpd_connection);
    } else {
      input_main(key, &interface, music_data, mpd_connection);
    }

    /* Syncs the mpd state */
    mpd_sync(&mpd_connection, music_data, interface.ui_param);

    /* Prints the interface to the screen */
    print_interface(&interface, music_data);
  }
}
