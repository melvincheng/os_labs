CC = clang
CFLAGS = -Wall -Wextra -std=c99 -g -pthread
LFLAGS = 
LIBS = -lm
SOURCES = sudoku_solver.c
OBJECTS = $(subst .c,.o,$(SOURCES))
BIN = myshell
.PHONY: clean help

sudoku_solver: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBS) -o $@ 

%.o : %.c
	$(CC) $(CFLAGS) -c $< 

all : $(BIN)

clean:
	rm -f $(OBJECTS) $(BIN) *~

help:
	@echo "Valid targets:"
	@echo "clean:  removes .o and binary files"
