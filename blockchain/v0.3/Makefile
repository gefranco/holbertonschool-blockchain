CC = gcc
FLAGS = -Wall -Werror -Wextra -pedantic -I. -Itransaction -Iprovided
TARGET = lib/
TARGET_TX = lib/transaction
TARGET_PROVIDED = lib/provided

SRC = $(wildcard *.c) $(wildcard transaction/*.c) $(wildcard provided/*.c)

OBJ = $(addprefix $(TARGET), $(SRC:%.c=%.o))

NAME = libhblk_blockchain.a

libhblk_blockchain.a: blockchain.h transaction/transaction.h $(TARGET) $(TARGET_TX) $(TARGET_PROVIDED) $(OBJ)
	ar rcs $(NAME) $(OBJ)

$(TARGET)%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@ 

$(TARGET):
	mkdir $(TARGET)

$(TARGET_TX):
	mkdir $(TARGET_TX)

$(TARGET_PROVIDED):
	mkdir $(TARGET_PROVIDED)
