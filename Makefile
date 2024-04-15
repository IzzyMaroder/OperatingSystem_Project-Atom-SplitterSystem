CC = gcc
CFLAGS =  -Wvla -Wextra -Werror

m-comp-all:
	${CC} ${CFLAGS} -c Master.c Atomo.c
m-mcomp:
	${CC} ${CFLAGS} -c Master.c
m-obj:
	${CC} ${CFLAGS} Master.o -o master
m-atom:
	${CC} ${CFLAGS} -c Master.c Atomo.c
m-objatom:
	${CC} ${CFLAGS} Atomo.o -o atomo
m-all:
	${CC} ${CFLAGS} Master.c Master.o -o master
