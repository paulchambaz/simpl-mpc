/**
 * @file interface.c
 * @author Paul Chambaz
 * @date 16 Sep 2022
 * @brief Definition file to print the interface
 * @license GPLv2
 */

#include "interface.h"

/**
 * @breif Initializes the interface values
 * @param interface The interface struct holding data on the interface
 */
void
set_default_ui ( struct interface *interface )
{
  /* Initialize ui_dimension - the static component of the ui */
  interface->ui_dimension = (struct ui_dimension *) malloc(sizeof(struct ui_dimension));
  interface->ui_dimension->sidebar_width = 40;
  interface->ui_dimension->volume_width = 32;
  interface->ui_dimension->right_bottom_width = 9;
  /* Initialize ui_param - the dynamic component of the ui */
  interface->ui_param = (struct ui_param *) malloc(sizeof(struct ui_param));
  interface->ui_param->on_main = false;
  interface->ui_param->side_scroll = 0;
  interface->ui_param->main_scroll = 0;
  interface->ui_param->album_selected = 0;
  interface->ui_param->title_selected = 0;
  interface->ui_param->playing = false;
  interface->ui_param->album_playing = 0;
  interface->ui_param->title_playing = 0;
  interface->ui_param->volume = 0;
  interface->ui_param->shuffle = false;
  interface->ui_param->repeat = true;
  interface->ui_param->current_duration = 0;
}

/**
 * @brief Intitializes the interface windows using the interface dimensions
 * @param interface The interface struct holding data on the interface
 */
void
set_ui_dimensions ( struct interface *interface ) 
{
  /* set the dimension of the ncurses windows */
  struct ui_dimension ui_dimension = *interface->ui_dimension;
  interface->side_window = newwin(LINES - 2, ui_dimension.sidebar_width, 0, 0);
  interface->main_window = newwin(LINES - 2, COLS - ui_dimension.sidebar_width, 0, ui_dimension.sidebar_width);
  interface->info_window = newwin(1, COLS - ui_dimension.right_bottom_width, LINES - 2, 0);
  interface->volume_window = newwin(1, ui_dimension.volume_width, LINES - 1, 0);
  interface->bar_window = newwin(1, COLS - ui_dimension.volume_width - ui_dimension.right_bottom_width, LINES - 1, ui_dimension.volume_width);
  interface->status_window = newwin(2, ui_dimension.right_bottom_width, LINES - 2, COLS - ui_dimension.right_bottom_width);
}

/**
 * @brief Initializes the ncurses interface and the interface struct
 * @param interface The interface struct holding data on the interface
 */
void
init_interface ( struct interface *interface )
{
  /* sets the locale for proper character printing */
  setlocale(LC_CTYPE, "");

  /* intializes the ncurses screen with default parameter */
  initscr();
  raw();
  keypad(stdscr, TRUE);
  timeout(100);
  refresh();
  noecho();
  curs_set(0);

  /* defines the color pairs for all colors */
  start_color();
  init_pair(WHITE, 8, 0);
  init_pair(RED, 9, 0);
  init_pair(GREEN, 2, 0);
  init_pair(BG_WHITE, 0, 8);
  init_pair(BG_BLUE, 0, 12);
  init_pair(BG_GREEN, 0, 2);
  init_pair(BG_AQUA, 0, 6);

  /* initializes the ncurses windows */
  set_ui_dimensions(interface);
}

/**
 * @brief Displays the ncurses side window
 * @param side_window The ncurses side window
 * @param ui_param The ui param struct used for interface changing values
 * @param ui_dimension The ui dimension struct used for interface pseudo static values
 * @param music_data The music data holding the mpd music data
 */
void
print_side_window ( WINDOW *side_window, struct ui_param ui_param, struct ui_dimension ui_dimension, struct music_data music_data )
{
  /* first clear the window to start from a blank canvas */
  wclear(side_window);
  /* prints all the albums name */
  wattron(side_window, COLOR_PAIR(WHITE));
  for (int j = 0; j < music_data.size; j++) {
    mvwprintw(side_window, j + 1 - ui_param.side_scroll, 1, "%.*s", ui_dimension.sidebar_width - 2, music_data.album[j]);
  }
  wattroff(side_window, COLOR_PAIR(WHITE));
  /* prints the cursors - current, playing and current/playing */
  mvwchgat(side_window, ui_param.album_selected + 1 - ui_param.side_scroll, 1, -1, 0, BG_BLUE, NULL);
  if (ui_param.playing) {
    mvwchgat(side_window, ui_param.album_playing + 1 - ui_param.side_scroll, 1, -1, 0, BG_GREEN, NULL);
    if (ui_param.album_selected == ui_param.album_playing) {
      mvwchgat(side_window, ui_param.album_playing + 1 - ui_param.side_scroll, 1, -1, 0, BG_AQUA, NULL);
    }
  }
  /* prints the ui box window */
  wattron(side_window, (!ui_param.on_main) ? COLOR_PAIR(RED) | A_BOLD : COLOR_PAIR(WHITE));
  box(side_window, 0, 0);
  char *album_str = " Albums ";
  mvwprintw(side_window, 0, (ui_dimension.sidebar_width - strlen(album_str)) / 2 - 1, "%s", album_str);
  wattroff(side_window, (!ui_param.on_main) ? COLOR_PAIR(RED) | A_BOLD : COLOR_PAIR(WHITE));
  /* refreshes the window */
  wrefresh(side_window);
}

/**
 * @brief Displays the ncurses main window
 * @param side_window The ncurses main window
 * @param ui_param The ui param struct used for interface changing values
 * @param ui_dimension The ui dimension struct used for interface pseudo static values
 * @param music_data The music data holding the mpd music data
 */
void
print_main_window ( WINDOW *main_window, struct ui_param ui_param, struct ui_dimension ui_dimension, struct music_data music_data )
{
  /* first clear the window to start from a blank canvas */
  wclear(main_window);
  /* prints the title list with track number - title - duration - artist */
  wattron(main_window, COLOR_PAIR(WHITE));
  int i = ui_param.album_selected;
  for (int j = 0; j < music_data.album_size[i]; j++) {
    int pos_dur = COLS - ui_dimension.sidebar_width - 7 - strlen(music_data.artist[i]) - 1;
    mvwprintw(main_window, j + 1 - ui_param.main_scroll, 2, "%2d - %.*s", music_data.track[i][j], pos_dur - 4 + 1, music_data.title[i][j]);
    mvwprintw(main_window, j + 1 - ui_param.main_scroll, pos_dur, "%02u:%02u %s", music_data.duration[i][j] / 60, music_data.duration[i][j] % 60, music_data.artist[i]);
  }
  wattroff(main_window, COLOR_PAIR(WHITE));
  /* prints the cursors - current, playing and current/playing */
  if (ui_param.on_main) {
    mvwchgat(main_window, ui_param.title_selected + 1 - ui_param.main_scroll, 1, -1, 0, BG_BLUE, NULL);
  }
  if (ui_param.playing && ui_param.album_selected == ui_param.album_playing) {
    mvwchgat(main_window, ui_param.title_playing + 1 - ui_param.main_scroll, 1, -1, 0, BG_GREEN, NULL);
    if (ui_param.title_selected == ui_param.title_playing) {
      mvwchgat(main_window, ui_param.title_playing + 1 - ui_param.main_scroll, 1, -1, 0, BG_AQUA, NULL);
    }
  }
  /* prints the ui box window */
  wattron(main_window, (ui_param.on_main) ? COLOR_PAIR(RED) | A_BOLD : COLOR_PAIR(WHITE));
  box(main_window, 0, 0);
  char *title_str = " Titles ";
  mvwprintw(main_window, 0, 8, "%s", title_str);
  wattroff(main_window, (ui_param.on_main) ? COLOR_PAIR(RED) | A_BOLD : COLOR_PAIR(WHITE));
  /* refreshes the window */
  wrefresh(main_window);
}

/**
 * @brief Displays the ncurses info window
 * @param side_window The ncurses info window
 * @param ui_param The ui param struct used for interface changing values
 * @param ui_dimension The ui dimension struct used for interface pseudo static values
 * @param music_data The music data holding the mpd music data
 */
void
print_info_window ( WINDOW *info_window, struct ui_param ui_param, struct ui_dimension ui_dimension, struct music_data music_data )
{
  /* first clear the window to start from a blank canvas */
  wclear(info_window);
  if (ui_param.playing) {
    /* prints the Sentence Playing title by artist from album with appropriate colors */
    int i = ui_param.album_playing, j = ui_param.title_playing;
    wattron(info_window, COLOR_PAIR(WHITE));
    mvwprintw(info_window, 0, 1, "Playing ");
    wattron(info_window, COLOR_PAIR(WHITE));
    wattron(info_window, COLOR_PAIR(GREEN));
    wprintw(info_window, "%s ", music_data.title[i][j]);
    wattroff(info_window, COLOR_PAIR(GREEN));
    wattron(info_window, COLOR_PAIR(WHITE));
    wprintw(info_window, "by ");
    wattroff(info_window, COLOR_PAIR(WHITE));
    wattron(info_window, COLOR_PAIR(GREEN));
    wprintw(info_window, "%s ", music_data.artist[i]);
    wattroff(info_window, COLOR_PAIR(GREEN));
    wattron(info_window, COLOR_PAIR(WHITE));
    wprintw(info_window, "from ");
    wattroff(info_window, COLOR_PAIR(WHITE));
    wattron(info_window, COLOR_PAIR(GREEN));
    wprintw(info_window, "%s", music_data.album[i]);
    wattroff(info_window, COLOR_PAIR(GREEN));
  } else {
    /* if not playing only prints Nothing is playing */
    wattron(info_window, COLOR_PAIR(WHITE));
    mvwprintw(info_window, 0, 1, "Nothing is playing");
    wattroff(info_window, COLOR_PAIR(WHITE));
  }
  /* refreshes the window */
  wrefresh(info_window);
}

/**
 * @brief Displays the ncurses volume window
 * @param side_window The ncurses volume window
 * @param ui_param The ui param struct used for interface changing values
 * @param ui_dimension The ui dimension struct used for interface pseudo static values
 */
void
print_volume_window ( WINDOW *volume_window, struct ui_param ui_param, struct ui_dimension ui_dimension )
{
  /* first clear the window to start from a blank canvas */
  wclear(volume_window);
  /* prints Vol and a horizontal line */
  wattron(volume_window, COLOR_PAIR(WHITE));
  mvwprintw(volume_window, 0, 1, "Vol ");
  mvwhline(volume_window, 0, 5, 0, ui_dimension.volume_width - 10 - 2);
  wattroff(volume_window, COLOR_PAIR(WHITE));
  if (ui_param.volume == -1) {
    /* if there is no volume played ie mpd is not playing then print n/a for the volume value */
    wattron(volume_window, COLOR_PAIR(WHITE));
    mvwprintw(volume_window, 0, 27, "n/a");
    wattroff(volume_window, COLOR_PAIR(WHITE));
  } else {
    /* else print the volume in % and a block cursor for the volume */
    wattron(volume_window, COLOR_PAIR(BG_WHITE));
    mvwaddch(volume_window, 0, 5 + (ui_dimension.volume_width - 10 - 2) * ui_param.volume / 100, ' ');
    wattroff(volume_window, COLOR_PAIR(BG_WHITE));
    wattron(volume_window, COLOR_PAIR(WHITE));
    mvwprintw(volume_window, 0, 27, "%d%%", ui_param.volume);
    wattroff(volume_window, COLOR_PAIR(WHITE));
  }
  /* refreshes the window */
  wrefresh(volume_window);
}

/**
 * @brief Displays the ncurses bar window
 * @param side_window The ncurses bar window
 * @param ui_param The ui param struct used for interface changing values
 * @param ui_dimension The ui dimension struct used for interface pseudo static values
 * @param music_data The music data holding the mpd music data
 */
void
print_bar_window ( WINDOW *bar_window, struct ui_param ui_param, struct ui_dimension ui_dimension, struct music_data music_data )
{
  /* first clear the window to start from a blank canvas */
  wclear(bar_window);
  /* prints a horizontal line, the song current position and the song duration */
  int current_duration = (ui_param.playing) ? ui_param.current_duration : 0;
  int duration = (ui_param.playing) ? music_data.duration[ui_param.album_playing][ui_param.title_playing] : 0;
  wattron(bar_window, COLOR_PAIR(WHITE));
  mvwprintw(bar_window, 0, 0, "%02u:%02u ", current_duration / 60, current_duration % 60);
  whline(bar_window, 0, COLS - ui_dimension.volume_width - ui_dimension.right_bottom_width - 2 * 6);
  mvwprintw(bar_window, 0, COLS - ui_dimension.volume_width - ui_dimension.right_bottom_width -  6, " %02u:%02u", duration / 60, duration % 60);
  wattroff(bar_window, COLOR_PAIR(WHITE));
  /* prints the block cursor */
  int pos = (ui_param.playing) ? 6 + (COLS - ui_dimension.volume_width - ui_dimension.right_bottom_width - 12 - 1) * current_duration / duration : 6;
  wattron(bar_window, COLOR_PAIR(BG_WHITE));
  mvwaddch(bar_window, 0, pos, ' ');
  wattroff(bar_window, COLOR_PAIR(BG_WHITE));
  /* refreshes the window */
  wrefresh(bar_window);
}

/**
 * @brief Displays the ncurses status window
 * @param side_window The ncurses status window
 * @param ui_param The ui param struct used for interface changing values
 * @param ui_dimension The ui dimension struct used for interface pseudo static values
 */
void
print_status_window ( WINDOW *status_window, struct ui_param ui_param, struct ui_dimension ui_dimension )
{
  /* first clear the window to start from a blank canvas */
  wclear(status_window);
  const char *shuffle_str = "shuffle";
  const char *repeat_str = "repeat";
  /* prints the suffle and repeat button and colors them if they are true */
  mvwprintw(status_window, 0, ui_dimension.right_bottom_width - strlen(shuffle_str) - 1, shuffle_str);
  mvwchgat(status_window, 0, 0, -1, (ui_param.shuffle) ? A_BOLD : 0, (ui_param.shuffle) ? GREEN : WHITE, NULL);
  mvwprintw(status_window, 1, ui_dimension.right_bottom_width - strlen(repeat_str) - 1, repeat_str);
  mvwchgat(status_window, 1, 0, -1, (ui_param.repeat) ? A_BOLD : 0, (ui_param.repeat) ? GREEN : WHITE, NULL);
  /* refreshes the window */
  wrefresh(status_window);
}

/**
 * @brief Displays the ncurses interface
 * @param interface The interface struct holding interface data
 * @param music_data The music data holding the mpd music data
 */
void
print_interface ( struct interface *interface, struct music_data music_data )
{
  /* pritnts all the windows */
  print_side_window(interface->side_window, *interface->ui_param, *interface->ui_dimension, music_data);
  print_main_window(interface->main_window, *interface->ui_param, *interface->ui_dimension, music_data);
  print_info_window(interface->info_window, *interface->ui_param, *interface->ui_dimension, music_data);
  print_volume_window(interface->volume_window, *interface->ui_param, *interface->ui_dimension);
  print_bar_window(interface->bar_window, *interface->ui_param, *interface->ui_dimension, music_data);
  print_status_window(interface->status_window, *interface->ui_param, *interface->ui_dimension);
}

/**
 * @brief Destroys the ncurses interface - to reset their size
 * @param interface The interface struct holding interface data
 */
void
destroy_interface ( struct interface *interface )
{
  /* destroy all the windows */
  delwin(interface->side_window);
  delwin(interface->main_window);
  delwin(interface->info_window);
  delwin(interface->volume_window);
  delwin(interface->bar_window);
  delwin(interface->status_window);
}

/**
 * @brief Frees the data of the interface struct
 * @param interface The interface struct holding interface data
 */
void
free_interface ( struct interface *interface )
{
  /* frees the interface structs */
  free(interface->ui_dimension);
  free(interface->ui_param);
}
