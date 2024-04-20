CC = gcc
CFLAGS =  -Wvla -Wextra -Werror

obj: 
	mkdir -p obj
bin:
	mkdir -p bin

activator: bin activator.o
	${CC} ${CFLAGS} obj/activator.o -o bin/activator
activator.o: obj src/activator/activator.c
	${CC} ${CFLAGS} -c src/activator/activator.c -o obj/activator.o

atomo: bin atomo.o
	${CC} ${CFLAGS} obj/atomo.o -o bin/atomo
atomo.o: obj src/atomo/atomo.c
	${CC} ${CFLAGS} -c src/atomo/atomo.c src/sharedmemory/sharedmemory.c -o obj/atomo.o

lib_master: bin lib_master.o
	${CC} ${CFLAGS} obj/lib_master.o -o bin/lib_master
lib_master.o: obj src/master/lib_master.c
	${CC} ${CFLAGS} -c src/master/lib_master.c src/sharedmemory/sharedmemory.c -o obj/lib_master.o
