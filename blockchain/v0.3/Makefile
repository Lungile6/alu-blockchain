# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -g3 -I. -Itransaction/ -I../../crypto -I../../llist

# Library name
NAME = libhblk_blockchain.a

# Source files from the current directory
SRC_BLOCK = blockchain_create.c \
            block_create.c \
            block_destroy.c \
            blockchain_destroy.c \
            block_hash.c \
            blockchain_serialize.c \
            blockchain_deserialize.c \
            block_is_valid.c \
            hash_matches_difficulty.c \
            blockchain_difficulty.c \
            block_mine.c

# Source files from the transaction subdirectory
SRC_TX = transaction/tx_out_create.c \
         transaction/unspent_tx_out_create.c \
         transaction/tx_in_create.c \
         transaction/transaction_hash.c \
         transaction/tx_in_sign.c \
         transaction/transaction_create.c \
         transaction/transaction_is_valid.c \
         transaction/coinbase_create.c \
         transaction/coinbase_is_valid.c \
         transaction/transaction_destroy.c \
         transaction/update_unspent.c

# Combine all sources
SRC = $(SRC_BLOCK) $(SRC_TX)

# Convert source file names to object file names
OBJ = $(SRC:.c=.o)

# Default rule
all: $(NAME)

# Archive object files into a static library
$(NAME): $(OBJ)
	ar rcs $(NAME) $(OBJ)

# Clean object files
clean:
	rm -f $(OBJ)

# Clean object files and the library
fclean: clean
	rm -f $(NAME)

# Re-build everything
re: fclean all

.PHONY: all clean fclean re
