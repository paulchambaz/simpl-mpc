#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>

#include <mpd/client.h>
#include <curses.h>

#include "help.h"
#include "parser.h"
#include "mpd_instance.h"
#include "music_data.h"
#include "interface.h"
#include "input.h"

int
main(int argc, int *argv[])
{
  // first we need to initialize the mpd instance
  struct mpd_connection *mpd_connection;
  mpd_instance_connect(&mpd_connection);

  Raw_music_data raw_music_data;
  mpd_get_metadata(&mpd_connection, &raw_music_data);

  Music_data music_data;
  raw_to_music_data(raw_music_data, &music_data);
  free_raw_music_data(&raw_music_data);

  sort_music_data(&music_data);

  Interface interface;
  set_default_ui(&interface);
  mpd_sync(&mpd_connection, music_data, interface.ui_param);
  init_interface(&interface);
  print_interface(&interface, music_data);
  refresh_interface(&interface);

  char key;
  int lines = LINES, cols = COLS;

  while ((key = getch())) {

    input_general(key, &interface, &music_data, mpd_connection);

    if (!interface.ui_param->on_main) {
      input_side(key, &interface, music_data, mpd_connection);
    } else {
      input_main(key, &interface, music_data, mpd_connection);
    }

    mpd_sync(&mpd_connection, music_data, interface.ui_param);

    print_interface(&interface, music_data);
    refresh_interface(&interface);
  }
}
