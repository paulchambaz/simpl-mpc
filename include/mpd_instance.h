#ifndef MPD_INSTANCE_H
#define MPD_INSTANCE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>

#include <mpd/client.h>

#include "music_data.h"
#include "interface.h"
#include "config.h"

#define MIN(x,y) (x < y) ? x : y

// max length for the metadata associated with a song
#define MAX_STR_LEN 4096
// max memory given when searching for metadata (here 1Gb)
#define MAX_MEM_SEARCH 1073741824

// primary structure that hold information about the mpd instance
typedef struct {
  struct mpd_connection *connection;
} Mpd_instance;

// finds and connect to the mpd instance running
void mpd_instance_connect( struct mpd_connection **mpd_connection );
// free the mpd instance
void mpd_instance_free( struct mpd_connection **mpd_connection );
// get a list of files metadata from mpd
void mpd_get_metadata( struct mpd_connection **mpd_connection, Raw_music_data *raw_music_data );
// sync the current parameter with mpd
void mpd_sync ( struct mpd_connection **mpd_connection, Music_data music_data, Ui_param *ui_param );
#endif
