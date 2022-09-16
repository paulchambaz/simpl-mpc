/**
 * @file music_data.h
 * @author Paul Chambaz
 * @date 16 Sep 2022
 * @brief Header file to manage the music (meta)data.
 * @license GPLv2
 */

#ifndef MUSIC_DATA_H
#define MUSIC_DATA_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#define MAX_ALBUM 65536
#define MAX_TRACK 65536

/**
 * @brief Implements list of songs metadata from the mpd server
 */
struct raw_music_data {
  int size;
  char **id;
  char **artist;
  char **album;
  char **title;
  short *date;
  short *track;
  unsigned int *duration;
};

/**
 * @brief Implements organized and sorted list of songs metadata from the mpd server
 */
struct music_data {
  int size;
  char **artist;
  char **album;
  short *date;
  int *album_size;
  char ***id;
  char ***title;
  short **track;
  unsigned int **duration;
};

/**
 * @brief Implements the current song index with its album index and its track number index
 */
struct music_data_index {
  int index;
  int album_index;
};

void raw_to_music_data ( struct raw_music_data raw_music_data, struct music_data *music_data );
int sort_compare ( struct music_data music_data, int i, int j );
void sort_swap ( struct music_data *music_data, int i, int j );
void quick_sort_music_data ( struct music_data *music_data, int low, int high );
void sort_music_data ( struct music_data *music_data );
bool qck_strcmp ( const char *a, const char *b );
struct music_data_index get_index_music_data ( const char *in, struct music_data music_data );
void free_raw_music_data ( struct raw_music_data *raw_music_data );
void free_music_data ( struct music_data *music_data );

#endif
