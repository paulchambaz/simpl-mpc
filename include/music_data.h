#ifndef MUSIC_DATA_H
#define MUSIC_DATA_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#define MAX_ALBUM 65536
#define MAX_TRACK 65536

typedef struct {
  int size;
  char **id;
  char **artist;
  char **album;
  char **title;
  short *date;
  short *track;
  unsigned int *duration;
} Raw_music_data;

typedef struct {
  int size;
  char **artist;
  char **album;
  short *date;
  int *album_size;
  char ***id;
  char ***title;
  short **track;
  unsigned int **duration;
} Music_data;

typedef struct {
  int index;
  int album_index;
} Music_data_index;

// transform raw_music_data into the music_data
void raw_to_music_data ( Raw_music_data raw_music_data, Music_data *music_data );
// function to sort music_data
void sort_music_data ( Music_data *music_data );
// get the id and album_id from music_data
Music_data_index get_index_music_data ( const char *in, Music_data music_data );
// free raw_music_data - careful only frees the lists themselves, not their content
void free_raw_music_data ( Raw_music_data *music_data );
// free music_data
void free_music_data ( Music_data *music_data );

#endif
