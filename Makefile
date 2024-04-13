CC = gcc
CFLAGS =  -Wvla -Wextra -Werror

m-comp:
	${CC} ${CFLAGS} -c Master.c
m-obj:
	${CC} ${CFLAGS} Master.o -o master
m-all:
	${CC} ${CFLAGS} Master.c Master.o -o master
