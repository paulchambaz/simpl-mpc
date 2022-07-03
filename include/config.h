#ifndef CONFIG_H
#define CONFIG_H

#define ctrl(x) ((x) & 0x1f)

static const char *command_input = "";

static const int volume_incr = 10;
static const int seek_incr = 5;

// add new function here
typedef enum {
  EXIT, PAUSE, NEXT, PREVIOUS, VOLUME_UP, VOLUME_DOWN, SHUFFLE, REPEAT, CLEAR, SEEK_FORWARD, SEEK_BACKWARD,
  SIDE_DOWN, SIDE_UP, SIDE_MAIN, SIDE_START,
  MAIN_DOWN, MAIN_UP, MAIN_SIDE, MAIN_START,
} Function;

// add more context here
typedef enum {
  GENERAL, SIDE, MAIN,
} Context;

typedef struct {
 unsigned const char key;
 Context context;
 Function function;
} Key;

// add new shortcuts here
static Key keys[] = {
  // key         context    function
  { 'q',                        GENERAL,    EXIT },
  { 'Q',                        GENERAL,    EXIT },
  { ctrl('q'),                  GENERAL,    EXIT },
  { ' ',                        GENERAL,    PAUSE },
  { 'n',                        GENERAL,    NEXT },
  { 'p',                        GENERAL,    PREVIOUS },
  { '=',                        GENERAL,    VOLUME_UP },
  { '+',                        GENERAL,    VOLUME_UP },
  { '-',                        GENERAL,    VOLUME_DOWN },
  { '_',                        GENERAL,    VOLUME_DOWN },
  { 's',                        GENERAL,    SHUFFLE },
  { 'r',                        GENERAL,    REPEAT },
  { 'x',                        GENERAL,    CLEAR },
  { '.',                        GENERAL,    SEEK_FORWARD },
  { ',',                        GENERAL,    SEEK_BACKWARD },
  { 'j',                        SIDE,       SIDE_DOWN },
  { (unsigned char) KEY_DOWN,   SIDE,       SIDE_DOWN },
  { 'k',                        SIDE,       SIDE_UP },
  { (unsigned char) KEY_UP,     SIDE,       SIDE_UP },
  { 'l',                        SIDE,       SIDE_MAIN },
  { (unsigned char) KEY_RIGHT,  SIDE,       SIDE_MAIN },
  { '\n',                       SIDE,       SIDE_START },
  { 'j',                        MAIN,       MAIN_DOWN },
  { (unsigned char) KEY_DOWN,   MAIN,       MAIN_DOWN },
  { 'k',                        MAIN,       MAIN_UP },
  { (unsigned char) KEY_UP,     MAIN,       MAIN_UP },
  { 'h',                        MAIN,       MAIN_SIDE },
  { (unsigned char) KEY_LEFT,   MAIN,       MAIN_SIDE },
  { '\n',                       MAIN,       MAIN_START },
};

#endif
