SRC = $(wildcard *.c)
HDR = $(wildcard *.h)
OBJ = $(SRC:.c=.o)
OUT = ppx

# CC      = gcc
# CFLAGS  = -Wall -I /additional/include/dir
# LDFLAGS = -L /additional/lib/dir
# LDLIBS  = -ldependency1 -ldependency2

$(OUT): $(OBJFILES) $(HDR)

.PHONY: clean
clean:
	rm -f $(OBJFILES) $(OUT)
