#XRS

RM=del

CC=cl
DEBUG=/Zi
CFLAGS=/std:c11 /FC /Wall $(DEBUG)

SRC=src
OBJ=obj
SRCS=$(wildcard $(SRC)/*.c)
OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.obj, $(SRCS))

BINDIR=bin
EXE=$(BINDIR)/xrs.exe

LIBS=User32.lib Gdi32.lib

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) /Fe: $@ $(LIBS)

$(OBJ)/%.obj: $(SRC)/%.c
	$(CC) $(CFLAGS) /c $< /Fo: $@

run:
	$(EXE)

clean:
	$(RM) /F /Q $(BINDIR)\* $(OBJ)\*

