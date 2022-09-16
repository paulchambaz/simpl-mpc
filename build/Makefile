include config.mk

SRC = ../source/*.c

simpl-mpc: clean
	@echo "Building" $@
	@echo "Flags  :" ${CFLAGS}
	@echo "Libs   :" ${LDFLAGS}
	@echo "Source :" ${SRC}
	@${CC} -o $@ ${CFLAGS} ${LDFLAGS} ${SRC}

debug:  clean
	@echo "Building debug"
	@echo "Flags  :" ${DEBUG_CFLAGS}
	@echo "Libs   :" ${LDFLAGS}
	@echo "Source :" ${SRC}
	@${CC} -o debug ${DEBUG_CFLAGS} ${LDFLAGS} ${SRC}

install: clean
	@echo "Installing simpl-mpc"
	@echo "Flags  :" ${INSTALL_CFLAGS}
	@echo "Libs   :" ${LDFLAGS}
	@echo "Source :" ${SRC}
	@${CC} -o simpl-mpc ${INSTALL_CFLAGS} ${LDFLAGS} ${SRC}
	@${STRIP} simpl-mpc
	@echo "Installing executable file to ${BIN_DIR}"
	@cp -f simpl-mpc ${BIN_DIR}/simpl-mpc
	@chmod 755 ${BIN_DIR}/simpl-mpc
	@echo "Installing manual page to ${MAN_DIR}/man1"
	@mkdir -p ${MAN_DIR}/man1
	@sed "s/VERSION/${VERSION}/g" < ../simpl-mpc.1 > ${MAN_DIR}/man1/simpl-mpc.1
	@chmod 644 ${MAN_DIR}/man1/simpl-mpc.1
	@rm -f simpl-mpc

uninstall:
	@echo removing executable file from ${DESTDIR}${PREFIX}/bin
	@rm -f ${DESTDIR}${PREFIX}/bin/simpl-mpc
	@echo removing manual page from ${DESTDIR}${MANPREFIX}
	@rm -f ${DESTDIR}${MANPREFIX}/man1/simpl-mpc.1

clean:
	@echo cleaning
	@rm -f simpl-mpc debug
