IDIR =.
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj
LDIR=lib

LIBS=-lX11

_DEPS = 
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS_))

_OBJ = main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -rf $(ODIR)/*.o *~ $(IDIR)/*~
