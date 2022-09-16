/**
 * @file config.h
 * @author Paul Chambaz
 * @date 16 Sep 2022
 * @brief Header file to print to configuration options of the program
 * @license GPLv2
 */

#ifndef CONFIG_H
#define CONFIG_H

#define ctrl(x) ((x) & 0x1f)

/* Use this variable to run a command after most action - useful to update status bar */
static const char *command_input = "";

/* Use these variables to change the increment for volume and song seek */
static const int volume_incr = 10;
static const int seek_incr = 5;

/* Use this enum to define new commands the program might run to bind to keys */
enum function {
  EXIT, PAUSE, NEXT, PREVIOUS, VOLUME_UP, VOLUME_DOWN, SHUFFLE, REPEAT, CLEAR, SEEK_FORWARD, SEEK_BACKWARD,
  SIDE_DOWN, SIDE_UP, SIDE_MAIN, SIDE_START, SIDE_TOP, SIDE_BOTTOM,
  MAIN_DOWN, MAIN_UP, MAIN_SIDE, MAIN_START, MAIN_TOP, MAIN_BOTTOM,
};

/* Use this enum to define new contexts in which the program can seperate shorcuts */
enum context {
  GENERAL, SIDE, MAIN,
};

/*
 * @brief Implements a shortcut struct
 */
struct key {
 unsigned const int key;
 const enum context context;
 const enum function function;
};

/* List of all keyboard shortcut - add new shortcuts in this array */
static struct key keys[] = {
  /* key           context     function */
  { 'q',           GENERAL,    EXIT },
  { 'Q',           GENERAL,    EXIT },
  { ctrl('q'),     GENERAL,    EXIT },
  { ' ',           GENERAL,    PAUSE },
  { 'n',           GENERAL,    NEXT },
  { 'p',           GENERAL,    PREVIOUS },
  { '=',           GENERAL,    VOLUME_UP },
  { '+',           GENERAL,    VOLUME_UP },
  { '-',           GENERAL,    VOLUME_DOWN },
  { '_',           GENERAL,    VOLUME_DOWN },
  { 's',           GENERAL,    SHUFFLE },
  { 'r',           GENERAL,    REPEAT },
  { 'x',           GENERAL,    CLEAR },
  { '.',           GENERAL,    SEEK_FORWARD },
  { ',',           GENERAL,    SEEK_BACKWARD },
  { 'j',           SIDE,       SIDE_DOWN },
  {  KEY_DOWN,     SIDE,       SIDE_DOWN },
  { 'k',           SIDE,       SIDE_UP },
  {  KEY_UP,       SIDE,       SIDE_UP },
  { 'l',           SIDE,       SIDE_MAIN },
  {  KEY_RIGHT,    SIDE,       SIDE_MAIN },
  { '\n',          SIDE,       SIDE_START },
  { 'g',           SIDE,       SIDE_TOP },
  { 'G',           SIDE,       SIDE_BOTTOM },
  { 'j',           MAIN,       MAIN_DOWN },
  {  KEY_DOWN,     MAIN,       MAIN_DOWN },
  { 'k',           MAIN,       MAIN_UP },
  {  KEY_UP,       MAIN,       MAIN_UP },
  { 'h',           MAIN,       MAIN_SIDE },
  {  KEY_LEFT,     MAIN,       MAIN_SIDE },
  { '\n',          MAIN,       MAIN_START },
  { 'g',           MAIN,       MAIN_TOP },
  { 'G',           MAIN,       MAIN_BOTTOM },
};

#endif
