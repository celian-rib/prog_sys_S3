SRC_C=waitingList.c
OBJ=${SRC_C:.c=.o}
CFLAGS= -g -std=c11 -Wall -Wextra -pedantic -pthread -D_XOPEN_SOURCE=700 -O2

all: syracuse_thread

syracuse_thread: $(OBJ) main.o
	gcc $(CFLAGS) $(OBJ) main.o -o syracuse_thread

%.o: %.c
	gcc -c $(CFLAGS) $<

clean:
	@rm -f $(OBJ) main.o syracuse_thread *~

