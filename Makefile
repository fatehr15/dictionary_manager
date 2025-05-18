CC      := gcc
CFLAGS  := -Wall -Wextra -g -Iinclude `pkg-config --cflags gtk+-3.0`
LDFLAGS := `pkg-config --libs gtk+-3.0`

SRC_DIR := src
SRCS    := $(wildcard $(SRC_DIR)/*.c)
OBJS    := $(SRCS:.c=.o)

APP_TARGET := data_manager

.PHONY: all clean

all: $(APP_TARGET)

$(APP_TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	 rm -f $(SRC_DIR)/*.o $(APP_TARGET)
