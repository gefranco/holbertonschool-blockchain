CC = gcc
FLAGS = -Wall -Werror -Wextra -pedantic
TARGET = lib/

SRC = sha256.c ec_create.c ec_to_pub.c ec_from_pub.c ec_save.c ec_load.c ec_sign.c ec_verify.c
OBJ = $(addprefix $(TARGET), $(SRC:%.c=%.o))
NAME = libhblk_crypto.a

libhblk_crypto.a: hblk_crypto.h $(TARGET) $(OBJ)
	ar rcs $(NAME) $(OBJ)

$(TARGET)%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

$(TARGET):
	mkdir $(TARGET) 
