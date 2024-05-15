RM=del /Q /F

CC=clang
CFLAGS=-g -Wall $(shell pkgconf --cflags raylib)
LDFLAGS=$(shell pkgconf --libs raylib) -lgdi32 -lwinmm

SRC=src
OBJ=obj
SRCS=$(wildcard $(SRC)/*.c)
OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))
HDRS=$(wildcard $(SRC)/*.h)
BIN_DIR=bin
BIN=xrs.exe

# Build all.
all: $(BIN_DIR)/$(BIN)

# Build the binary.
$(BIN_DIR)/$(BIN): $(OBJS) $(OBJ) $(BIN_DIR)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)

# Build the object files.
$(OBJ)/%.o: $(SRC)/%.c $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

# Make the bin directory.
$(BIN_DIR):
	mkdir $@

# Make the obj directory.
$(OBJ):
	mkdir $@

# Maybe copy the raylib.dll using pkgconf?

# Run the binary.
run: $(BIN_DIR)/$(BIN)
	$(BIN_DIR)/$(BIN)

# Clean up.
clean:
	$(RM) $(OBJ)
	$(RM) $(BIN_DIR)
