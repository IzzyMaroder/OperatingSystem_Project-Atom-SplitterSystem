CC = gcc
CFLAGS =  -Wvla -Wextra -Werror

m-comp-all:
	${CC} ${CFLAGS} -c sharedmemory.c Master.c Atomo.c
m-mcomp:
	${CC} ${CFLAGS} -c Master.c
m-obj:
	${CC} ${CFLAGS} Master.o sharedmemory.o -o master
m-atom:
	${CC} ${CFLAGS} -c Master.c Atomo.c
m-objatom:
	${CC} ${CFLAGS} Atomo.o sharedmemory.o -o atomo
m-all:
	${CC} ${CFLAGS} Master.c Master.o -o master
