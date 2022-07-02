include config.mk

SRC = ../source/*.c
OBJ = ${SRC:.c=.o}

all: clean options simpl-mpc

options:
	@echo simpl-mpc build options:
	@echo "CFLAGS  = ${CFLAGS}"
	@echo "LDFLAGS = ${LDFLAGS}"
	@echo "CC      = ${CC}"

../include/config.h:
	cp ../include/config.def.h ../include/config.h

.c.o:
	@echo CC $<
	@${CC} -c ${CFLAGS} $<
	@mv *.o ../source/

${OBJ}: config.mk

simpl-mpc: ${OBJ}
	@echo CC -o $@
	@echo "${CC} -o $@ ${C_FLAGS} ${LDFLAGS} ${SRC}"
	@${CC} -o $@ ${CFLAGS} ${LDFLAGS} ${SRC}
	@rm ../source/*.o

debug:  ${OBJ}
	@echo lissajous-curve-table debug build options:
	@echo "CFLAGS  = ${DEBUG_CFLAGS}"
	@echo "LDFLAGS = ${LDFLAGS}"
	@echo "CC      = ${CC}"
	@echo "${CC} -o debug ${DEBUG_CFLAGS} ${LDFLAGS} ${SRC}"
	@${CC} -o debug ${DEBUG_CFLAGS} ${LDFLAGS} ${SRC}

clean:
	@echo cleaning
	@rm -f simpl-mpc ${OBJ} simpl-mpc-${VERSION}.tar.gz

dist: clean
	@echo creating dist tarball
	@mkdir -p simpl-mpc-${VERSION}
	@cp -R ../LICENSE Makefile ../README.md ../include/config.def.h config.mk ${SRC} ../simpl-mpc.1 simpl-mpc-${VERSION}
	@tar -cf simpl-mpc-${VERSION}.tar simpl-mpc-${VERSION}
	@gzip simpl-mpc-${VERSION}.tar
	@rm -rf simpl-mpc-${VERSION}

install: simpl-mpc
	@echo stripping executable
	@${STRIP} simpl-mpc
	@echo installing executable file to ${DESTDIR}${PREFIX}/bin
	@mkdir -p ${DESTDIR}${PREFIX}/bin
	@cp -f simpl-mpc ${DESTDIR}${PREFIX}/bin
	@chmod 755 ${DESTDIR}${PREFIX}/bin
	@echo installing manual page to ${DESTDIR}${MANPREFIX}/man1
	@mkdir -p ${DESTDIR}${MANPREFIX}/man1
	@sed "s/VERSION/${VERSION}/g" < ../simpl-mpc.1 > ${DESTDIR}${MANPREFIX}/man1/simpl-mpc.1
	@chmod 644 ${DESTDIR}${MANPREFIX}/man1/simpl-mpc.1

uninstall:
	@echo removing executable file from ${DESTDIR}${PREFIX}/bin
	@rm -f ${DESTDIR}${PREFIX}/bin/simpl-mpc
	@echo removing manual page from ${DESTDIR}${MANPREFIX}
	@rm -f ${DESTDIR}${MANPREFIX}/man1/simpl-mpc.1

.PHONY: all options clean dist install uninstall debug
