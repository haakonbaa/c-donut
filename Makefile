CC = gcc
CFLAGS = -O3
LIBS = -lm

OBJ = main.o

%.o: %.c
	${CC} -c $^ -o $@ ${CFLAGS} ${LIBS}

main: ${OBJ}
	${CC} $^ -o $@ ${CFLAGS} ${LIBS}

.PHONY: clean

clean:
	rm -f *.o main
