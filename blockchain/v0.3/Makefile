CC = gcc
FLAGS = -Wall -Werror -Wextra -pedantic
TARGET = lib/
TARGET_TX = lib/transaction

SRC = blockchain_create.c block_create.c block_destroy.c blockchain_destroy.c block_hash.c blockchain_serialize.c blockchain_deserialize.c block_is_valid.c hash_matches_difficulty.c blockchain_difficulty.c block_mine.c transaction/tx_out_create.c transaction/unspent_tx_out_create.c transaction/tx_in_create.c transaction/transaction_hash.c transaction/tx_in_sign.c transaction/transaction_create.c transaction/transaction_is_valid.c transaction/coinbase_create.c transaction/coinbase_is_valid.c transaction/transaction_destroy.c transaction/update_unspent.c

OBJ = $(addprefix $(TARGET), $(SRC:%.c=%.o))

NAME = libhblk_blockchain.a

libhblk_blockchain.a: blockchain.h transaction/transaction.h $(TARGET) $(TARGET_TX) $(OBJ)
	ar rcs $(NAME) $(OBJ)

$(TARGET)%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@ 

$(TARGET):
	mkdir $(TARGET)

$(TARGET_TX):
	mkdir $(TARGET_TX)
