CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -Iinclude
TARGET = srm_rpi_link_test

SRC = \
	src/main.c \
	src/app.c \
	src/serial_port.c \
	src/link.c \
	src/protocol.c

OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)