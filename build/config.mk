VERSION = 0.2
PREFIX ?= /usr
BIN_DIR = ${PREFIX}/bin
MAN_DIR = ${PREFIX}/share/man
INCS = -I ../include
LIBS = -lncurses -lmpdclient
CFLAGS += -std=c99 ${INCS} -DVERSION=\"${VERSION}\" -DNDBUG
INSTALL_CFLAGS += -std=c99 ${INCS} -DVERSION=\"${VERSION}\" -DNDBUG -Ofast
DEBUG_CFLAGS = ${CFLAGS} -UNDEBUG -O0 -g -ggdb -Wall -Wextra -Wno-unused-parameter
LDFLAGS += ${LIBS}
CC ?= cc
STRIP ?= strip
