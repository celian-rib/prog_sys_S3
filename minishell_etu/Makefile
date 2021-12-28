SRC_C=shell.c StringVector.c actions.c
OBJ=${SRC_C:.c=.o}
CFLAGS= -g -std=c11 -Wall -Wextra -pedantic -D_XOPEN_SOURCE=700

all: main

main: $(OBJ) main.o
	gcc $(CFLAGS) $(OBJ) main.o -o main

%.o: %.c
	gcc -c $(CFLAGS) $<

clean:
	@rm -f $(OBJ) main.o main *~

