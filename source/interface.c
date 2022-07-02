#include "interface.h"

void
set_default_ui (Interface *interface)
{
  interface->ui_dimension = (Ui_dimension *) malloc(sizeof(Ui_dimension));
  interface->ui_dimension->sidebar_width = 40;
  interface->ui_dimension->volume_width = 32;
  interface->ui_dimension->right_bottom_width = 9;
  interface->ui_param = (Ui_param *) malloc(sizeof(Ui_param));
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

void
set_ui_dimensions (Interface *interface) 
{
  Ui_dimension ui_dimension = *(interface->ui_dimension);
  interface->side_window = newwin(LINES - 2, ui_dimension.sidebar_width, 0, 0);
  interface->main_window = newwin(LINES - 2, COLS - ui_dimension.sidebar_width, 0, ui_dimension.sidebar_width);
  interface->info_window = newwin(1, COLS - ui_dimension.right_bottom_width, LINES - 2, 0);
  interface->volume_window = newwin(1, ui_dimension.volume_width, LINES - 1, 0);
  interface->bar_window = newwin(1, COLS - ui_dimension.volume_width - ui_dimension.right_bottom_width, LINES - 1, ui_dimension.volume_width);
  interface->status_window = newwin(2, ui_dimension.right_bottom_width, LINES - 2, COLS - ui_dimension.right_bottom_width);
}

void
init_interface (Interface *interface)
{
  setlocale(LC_CTYPE, "");

  initscr();

  raw();
  keypad(stdscr, TRUE);
  timeout(1000);
  refresh();
  start_color();
  init_pair(0, 8, 0);
  init_pair(1, 9, 0);
  init_pair(2, 0, 12);
  init_pair(3, 0, 2);
  init_pair(4, 0, 6);
  init_pair(5, 2, 0);
  init_pair(6, 0, 8);
  noecho();
  curs_set(0);

  set_ui_dimensions(interface);

}

void
print_side_window (WINDOW *side_window, Ui_param ui_param, Ui_dimension ui_dimension, Music_data music_data)
{
  wclear(side_window);
  for (int j = 0; j < music_data.size; j++) {
    mvwprintw(side_window, j + 1 - ui_param.side_scroll, 1, "%.*s", ui_dimension.sidebar_width - 2, music_data.album[j]);
  }
  mvwchgat(side_window, ui_param.album_selected + 1 - ui_param.side_scroll, 1, -1, 0, 2, NULL);
  if (ui_param.playing) {
    mvwchgat(side_window, ui_param.album_playing + 1 - ui_param.side_scroll, 1, -1, 0, 3, NULL);
    if (ui_param.album_selected == ui_param.album_playing) {
      mvwchgat(side_window, ui_param.album_playing + 1 - ui_param.side_scroll, 1, -1, 0, 4, NULL);
    }
  }
  if (!ui_param.on_main) {
    wattron(side_window, COLOR_PAIR(1));
    wattron(side_window, A_BOLD);
  }
  box(side_window, 0, 0);
  char *album_str = " Albums ";
  mvwprintw(side_window, 0, (ui_dimension.sidebar_width - strlen(album_str)) / 2 - 1, "%s", album_str);
  if (!ui_param.on_main) {
    wattroff(side_window, A_BOLD);
    wattroff(side_window, COLOR_PAIR(1));
  }
}

void
print_main_window (WINDOW *main_window, Ui_param ui_param, Ui_dimension ui_dimension, Music_data music_data)
{
  wclear(main_window);

  int i = ui_param.album_selected;
  for (int j = 0; j < music_data.album_size[i]; j++) {
    int pos_dur = COLS - ui_dimension.sidebar_width - 7 - strlen(music_data.artist[i]) - 1;
    mvwprintw(main_window, j + 1 - ui_param.main_scroll, 2, "%2d - %.*s", music_data.track[i][j], pos_dur - 4 + 1, music_data.title[i][j]);
    mvwprintw(main_window, j + 1 - ui_param.main_scroll, pos_dur, "%02u:%02u %s", music_data.duration[i][j] / 60, music_data.duration[i][j] % 60, music_data.artist[i]);
  }
  if (ui_param.on_main) {
    mvwchgat(main_window, ui_param.title_selected + 1 - ui_param.main_scroll, 1, -1, 0, 2, NULL);
  }

  if (ui_param.playing && ui_param.album_selected == ui_param.album_playing) {
    mvwchgat(main_window, ui_param.title_playing + 1 - ui_param.main_scroll, 1, -1, 0, 3, NULL);
    if (ui_param.title_selected == ui_param.title_playing) {
      mvwchgat(main_window, ui_param.title_playing + 1 - ui_param.main_scroll, 1, -1, 0, 4, NULL);
    }
  }

  if (ui_param.on_main) {
    wattron(main_window, COLOR_PAIR(1));
    wattron(main_window, A_BOLD);
  }
  box(main_window, 0, 0);
  char *title_str = " Titles ";
  mvwprintw(main_window, 0, 8, "%s", title_str);
  if (ui_param.on_main) {
    wattroff(main_window, A_BOLD);
    wattroff(main_window, COLOR_PAIR(1));
  }
}

void
print_info_window (WINDOW *info_window, Ui_param ui_param, Ui_dimension ui_dimension, Music_data music_data)
{
  wclear(info_window);
  if (ui_param.playing) {
    int i = ui_param.album_playing, j = ui_param.title_playing;
    mvwprintw(info_window, 0, 1, "Playing ");

    wattron(info_window, COLOR_PAIR(5));
    wprintw(info_window, "%s ", music_data.title[i][j]);
    wattroff(info_window, COLOR_PAIR(5));
    
    wprintw(info_window, "by ");

    wattron(info_window, COLOR_PAIR(5));
    wprintw(info_window, "%s ", music_data.artist[i]);
    wattroff(info_window, COLOR_PAIR(5));

    wprintw(info_window, "from ");

    wattron(info_window, COLOR_PAIR(5));
    wprintw(info_window, "%s ", music_data.album[i]);
    wattroff(info_window, COLOR_PAIR(5));

  } else {
    mvwprintw(info_window, 0, 1, "Nothing is playing");
  }
}

void
print_volume_window (WINDOW *volume_window, Ui_param ui_param, Ui_dimension ui_dimension)
{
  wclear(volume_window);
  mvwprintw(volume_window, 0, 1, "Vol ");
  if (ui_param.volume == -1) {
    mvwhline(volume_window, 0, 5, 0, ui_dimension.volume_width - 10 - 2);
    mvwprintw(volume_window, 0, 27, "n/a");
  } else {
    mvwhline(volume_window, 0, 5, 0, ui_dimension.volume_width - 10 - 2);
    wattron(volume_window, COLOR_PAIR(6));
    mvwaddch(volume_window, 0, 5 + (ui_dimension.volume_width - 10 - 2) * ui_param.volume / 100, ' ');
    wattroff(volume_window, COLOR_PAIR(6));
    mvwprintw(volume_window, 0, 27, "%d%%", ui_param.volume);
  }
}

void
print_bar_window (WINDOW *bar_window, Ui_param ui_param, Ui_dimension ui_dimension, Music_data music_data)
{
  wclear(bar_window);
  int current_duration = (ui_param.playing) ? ui_param.current_duration : 0;
  int duration = (ui_param.playing) ? music_data.duration[ui_param.album_playing][ui_param.title_playing] : 0;

  wclear(bar_window);

  mvwprintw(bar_window, 0, 0, "%02u:%02u ", ui_param.current_duration / 60, ui_param.current_duration % 60);

  whline(bar_window, 0, COLS - ui_dimension.volume_width - ui_dimension.right_bottom_width - 2 * 6);

  int pos = (ui_param.playing) ? 6 + (COLS - ui_dimension.volume_width - ui_dimension.right_bottom_width - 12 - 1) * current_duration / duration : 6;
  wattron(bar_window, COLOR_PAIR(6));
  mvwaddch(bar_window, 0, pos, ' ');
  wattroff(bar_window, COLOR_PAIR(6));

  mvwprintw(bar_window, 0, COLS - ui_dimension.volume_width - ui_dimension.right_bottom_width -  6, " %02u:%02u", duration / 60, duration % 60);
}

void
print_status_window (WINDOW *status_window, Ui_param ui_param, Ui_dimension ui_dimension)
{
  wclear(status_window);
  char *shuffle_str = "shuffle";
  char *repeat_str = "repeat";
  mvwprintw(status_window, 0, ui_dimension.right_bottom_width - strlen(shuffle_str) - 1, shuffle_str);
  mvwchgat(status_window, 0, 0, -1, (ui_param.shuffle) ? A_BOLD : 0, (ui_param.shuffle) ? 5 : 0, NULL);
  mvwprintw(status_window, 1, ui_dimension.right_bottom_width - strlen(repeat_str) - 1, repeat_str);
  mvwchgat(status_window, 1, 0, -1, (ui_param.repeat) ? A_BOLD : 0, (ui_param.repeat) ? 5 : 0, NULL);

}

void
print_interface (Interface *interface, Music_data music_data)
{
  print_side_window(interface->side_window, *interface->ui_param, *interface->ui_dimension, music_data);
  print_main_window(interface->main_window, *interface->ui_param, *interface->ui_dimension, music_data);
  print_info_window(interface->info_window, *interface->ui_param, *interface->ui_dimension, music_data);
  print_volume_window(interface->volume_window, *interface->ui_param, *interface->ui_dimension);
  print_bar_window(interface->bar_window, *interface->ui_param, *interface->ui_dimension, music_data);
  print_status_window(interface->status_window, *interface->ui_param, *interface->ui_dimension);
}

void
refresh_interface (Interface *interface)
{
  wrefresh(interface->side_window);
  wrefresh(interface->main_window);
  wrefresh(interface->info_window);
  wrefresh(interface->volume_window);
  wrefresh(interface->bar_window);
  wrefresh(interface->status_window);
}

void
quit_interface (Interface *interface)
{
  endwin();
}

void
free_interface (Interface *interface)
{
  free(interface->ui_dimension);
  free(interface->ui_param);
}
