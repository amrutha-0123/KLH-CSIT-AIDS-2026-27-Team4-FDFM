CC = gcc
CFLAGS = -Wall -Wextra -std=c11

TARGET = fileio
SOURCE = src/main.c

all:
	$(CC) $(CFLAGS) $(SOURCE) -o $(TARGET)

clean:
	rm -f $(TARGET)