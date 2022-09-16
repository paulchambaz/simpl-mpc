/**
 * @file mpd_instance.h
 * @author Paul Chambaz
 * @date 16 Sep 2022
 * @brief Header file to connect to the mpd connection and get music data
 * @license GPLv2
 */

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

#define MIN(x, y) (x < y) ? x : y

/* max length for the metadata associated with a song */
#define MAX_STR_LEN 4096
/* max memory given when searching for metadata (here 1Gb) */
#define MAX_MEM_SEARCH 1073741824

void mpd_instance_connect ( struct mpd_connection **mpd_connection );
void qck_strcpy ( char **dest, const char *source );
void mpd_get_metadata ( struct mpd_connection **mpd_connection, struct raw_music_data *raw_music_data );
void mpd_sync ( struct mpd_connection **mpd_connection, struct music_data music_data, struct ui_param *ui_param );

#endif
