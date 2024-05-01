CC = gcc
CFLAGS =  -Wvla -Wextra -Werror

default: activator atomo alimentator master

obj: 
	mkdir -p obj
bin:
	mkdir -p bin

activator: bin activator.o sharedmemory.o general.o
	${CC} ${CFLAGS} obj/activator.o obj/sharedmemory.o obj/general.o -o bin/activator
activator.o: obj src/activator/activator.c
	${CC} ${CFLAGS} -c src/activator/activator.c -o obj/activator.o

alimentator: bin alimentator.o sharedmemory.o general.o
	${CC} ${CFLAGS} obj/alimentator.o obj/sharedmemory.o obj/general.o -o bin/alimentator
alimentator.o:
	${CC} ${CFLAGS} -c src/alimentator/alimentator.c -o obj/alimentator.o

atomo: bin atomo.o sharedmemory.o general.o
	${CC} ${CFLAGS} obj/atomo.o obj/sharedmemory.o obj/general.o -o bin/atomo
atomo.o: obj src/atomo/atomo.c
	${CC} ${CFLAGS} -c src/atomo/atomo.c -o obj/atomo.o

sharedmemory.o: src/lib/sharedmemory/sharedmemory.c
	${CC} ${CFLAGS} -c src/lib/sharedmemory/sharedmemory.c -o obj/sharedmemory.o

general.o: src/lib/general_lib/general.c
	${CC} ${CFLAGS} -c src/lib/general_lib/general.c -o obj/general.o

master: bin master.o lib_master.o general.o
	${CC} ${CFLAGS} obj/master.o obj/lib_master.o obj/general.o -o bin/master
master.o: obj src/master/master.c
	${CC} ${CFLAGS} -c src/master/master.c -o obj/master.o
	
lib_master.o: src/master/lib_master.c
	${CC} ${CFLAGS} -c src/master/lib_master.c -o obj/lib_master.o
