CC = gcc
CFLAGS =  -Wvla -Wextra -Werror

default: activator atomo alimentator master inibitore

obj: 
	mkdir -p obj
bin:
	mkdir -p bin

activator: bin activator.o general.o
	${CC} ${CFLAGS} obj/activator.o obj/general.o -o bin/activator
activator.o: obj src/activator/activator.c
	${CC} ${CFLAGS} -c src/activator/activator.c -o obj/activator.o

alimentator: bin alimentator.o general.o
	${CC} ${CFLAGS} obj/alimentator.o obj/general.o -o bin/alimentator
alimentator.o:
	${CC} ${CFLAGS} -c src/alimentator/alimentator.c -o obj/alimentator.o

atomo: bin atomo.o general.o
	${CC} ${CFLAGS} obj/atomo.o obj/general.o -o bin/atomo
atomo.o: obj src/atomo/atomo.c
	${CC} ${CFLAGS} -c src/atomo/atomo.c -o obj/atomo.o

inibitore: bin inhibitor.o general.o
	${CC} ${CFLAGS} obj/inhibitor.o obj/general.o -o bin/inhibitor
inhibitor.o: obj src/inhibitor/inhibitor.c
	${CC} ${CFLAGS} -c src/inhibitor/inhibitor.c -o obj/inhibitor.o


general.o: src/lib/general_lib/general.c
	${CC} ${CFLAGS} -c src/lib/general_lib/general.c -o obj/general.o

master: bin master.o lib_master.o general.o
	${CC} ${CFLAGS} obj/master.o obj/lib_master.o obj/general.o -o bin/master
master.o: obj src/master/master.c
	${CC} ${CFLAGS} -c src/master/master.c -o obj/master.o
	
lib_master.o: src/master/lib_master.c
	${CC} ${CFLAGS} -c src/master/lib_master.c -o obj/lib_master.o
