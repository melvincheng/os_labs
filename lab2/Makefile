CC = clang
CFLAGS = -Wall -Wextra -std=c99 -g
LFLAGS = 
LIBS = -lm
SOURCES = myshell.c utility.c
OBJECTS = $(subst .c,.o,$(SOURCES))
BIN = myshell
.PHONY: clean help

myshell: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBS) -o $@ 

%.o : %.c
	$(CC) $(CFLAGS) -c $< 

all : $(BIN)

clean:
	rm -f $(OBJECTS) $(BIN) *~

help:
	@echo "Valid targets:"
	@echo "  clean:  removes .o and binary files"
