CC = clang
CFLAGS = -Wall -Wextra -std=c99 -g -pthread
LFLAGS = 
LIBS = -lm
SOURCES = sudoku_validator.c
OBJECTS = $(subst .c,.o,$(SOURCES))
BIN = sudoku_validator
.PHONY: clean help

sudoku_validator: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBS) -o $@ 

%.o : %.c
	$(CC) $(CFLAGS) -c $< 

all : $(BIN)

clean:
	rm -f $(OBJECTS) $(BIN) *~

help:
	@echo "Valid targets:"
	@echo "clean: removes .o and binary files"
