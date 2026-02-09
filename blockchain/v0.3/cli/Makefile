CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -I. -I.. -I../../../crypto -I../../../llist
LIBS = -L.. -lhblk_blockchain \
       -L../../../crypto -lhblk_crypto \
       -L../../../llist -lllist \
       -Wl,-rpath=$(shell pwd)/.. \
       -Wl,-rpath=$(shell pwd)/../../../crypto \
       -Wl,-rpath=$(shell pwd)/../../../llist \
       -lssl -lcrypto -pthread

SRC = main.c cli_loop.c
OBJ = $(SRC:.c=.o)
NAME = blockchain_cli

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME) $(LIBS)

clean:
	rm -f $(OBJ) $(NAME)
