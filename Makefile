IDIR = src
CC = gcc
CFLAGS = -I$(IDIR) -Wall -Wpedantic

ODIR = obj
LDIR = lib

LIBS = -lX11

_DEPS = xapi.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o xapi.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(IDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -rf $(ODIR)/*.o *~ $(IDIR)/*~
