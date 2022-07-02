VERSION = 0.1
PREFIX ?= /usr/local
MANPREFIX = ${PREFIX}/share/man
TERMINFO := ${DESTDIR}${PREFIX}/share/terminfo
INCS = -I ../include
LIBS = -lncurses -lmpdclient
CFLAGS += -std=c99 ${INCS} -DVERSION=\"${VERSION}\" -DNDBUG
LDFLAGS += ${LIBS}
DEBUG_CFLAGS = ${CFLAGS} -UNDEBUG -O0 -g -ggdb -Wall -Wextra -Wno-unused-parameter
CC ?= cc
STRIP ?= strip