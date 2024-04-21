CC = gcc
CFLAGS =  -Wvla -Wextra -Werror

obj: 
	mkdir -p obj
bin:
	mkdir -p bin

activator: bin activator.o sharedmemory.o
	${CC} ${CFLAGS} obj/activator.o obj/sharedmemory.o -o bin/activator
activator.o: obj src/activator/activator.c
	${CC} ${CFLAGS} -c src/activator/activator.c -o obj/activator.o

atomo: bin atomo.o sharedmemory.o
	${CC} ${CFLAGS} obj/atomo.o obj/sharedmemory.o -o bin/atomo
atomo.o: obj src/atomo/atomo.c
	${CC} ${CFLAGS} -c src/atomo/atomo.c -o obj/atomo.o

sharedmemory.o: src/lib/sharedmemory/sharedmemory.c
	${CC} ${CFLAGS} -c src/lib/sharedmemory/sharedmemory.c -o obj/sharedmemory.o

master: bin master.o lib_master.o
	${CC} ${CFLAGS} obj/master.o obj/lib_master.o -o bin/master
master.o: obj src/master/master.c
	${CC} ${CFLAGS} -c src/master/master.c -o obj/master.o
	
lib_master.o: src/master/lib_master.c
	${CC} ${CFLAGS} -c src/master/lib_master.c -o obj/lib_master.o
