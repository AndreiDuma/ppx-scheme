SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
OUT = ppx

# CC      = gcc
# CFLAGS  = -Wall -I /additional/include/dir
# LDFLAGS = -L /additional/lib/dir
# LDLIBS  = -ldependency1 -ldependency2

$(OUT): $(OBJFILES)

.PHONY: clean
clean:
	rm -f $(OBJFILES) $(OUT)
