CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -Iinclude

TARGET = srm_rpi_link_test
BIN_TARGET = srm_rpi_bin_rx_test
TX_TARGET = srm_rpi_bin_tx_test
TX_LOOP_TARGET = srm_rpi_tx_loop_test
MENU_TARGET = srm_rpi_menu_test

SRC = \
	src/main.c \
	src/app.c \
	src/serial_port.c \
	src/link.c \
	src/protocol.c

BIN_SRC = \
	src/bin_rx_test.c \
	src/serial_port.c \
	src/srm_crc.c

TX_SRC = \
	src/tx_test.c \
	src/serial_port.c \
	src/srm_crc.c \
	src/srm_frame_builder.c

TX_LOOP_SRC = \
	src/tx_loop_test.c \
	src/serial_port.c

MENU_SRC = \
	src/menu_test.c \
	src/serial_port.c \
	src/srm_crc.c \
	src/srm_frame_builder.c

OBJ = $(SRC:.c=.o)
BIN_OBJ = $(BIN_SRC:.c=.o)
TX_OBJ = $(TX_SRC:.c=.o)
TX_LOOP_OBJ = $(TX_LOOP_SRC:.c=.o)
MENU_OBJ = $(MENU_SRC:.c=.o)

all: $(TARGET) $(BIN_TARGET) $(TX_TARGET) $(TX_LOOP_TARGET) $(MENU_TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

$(BIN_TARGET): $(BIN_OBJ)
	$(CC) $(BIN_OBJ) -o $(BIN_TARGET)

$(TX_TARGET): $(TX_OBJ)
	$(CC) $(TX_OBJ) -o $(TX_TARGET)

$(TX_LOOP_TARGET): $(TX_LOOP_OBJ)
	$(CC) $(TX_LOOP_OBJ) -o $(TX_LOOP_TARGET)

$(MENU_TARGET): $(MENU_OBJ)
	$(CC) $(MENU_OBJ) -o $(MENU_TARGET)

clean:
	rm -f $(OBJ) $(BIN_OBJ) $(TX_OBJ) $(TX_LOOP_OBJ) $(MENU_OBJ) \
	$(TARGET) $(BIN_TARGET) $(TX_TARGET) $(TX_LOOP_TARGET) $(MENU_TARGET)