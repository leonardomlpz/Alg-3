# Makefile para Prova 4 de Algoritmos 3

TARGET = prova4_20242343

# Compilador e Flags
CC = gcc

CFLAGS = -Wall -Wextra -g -std=c99

LDFLAGS = -lm


SOURCES = main.c kdtree.c

OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

%.o: %.c kdtree.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)