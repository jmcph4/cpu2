PROJ_NAME = cpu2

SRC_DIR = src
BIN_DIR = bin
INCLUDE_DIR = include
LIB_DIR = lib

CC = gcc
CFLAGS = -Wall -Wextra -Wshadow -pedantic-errors -std=c11 -g -I$(INCLUDE_DIR) -L$(LIB_DIR) -ljcrl

$(BIN_DIR)/$(PROJ_NAME): $(SRC_DIR)/*.c
	$(CC) $^ $(CFLAGS) -o $@

.PHONY: clean

clean:
	rm $(BIN_DIR)/*

