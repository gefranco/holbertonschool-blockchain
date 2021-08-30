CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic
RM = rm -f
LIBS = -L../blockchain/v0.3 -lhblk_blockchain -L../crypto -lhblk_crypto -lssl -lcrypto -lllist -pthread

SRC = $(wildcard *.c)
OBJ = $(patsubst %.c, %.o, $(SRC))


all: cli

cli: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

clean: obj_clean
	$(RM) cli

obj_clean:
	$(RM) $(OBJ)
