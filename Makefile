IDIR = src
CC = gcc
CFLAGS = -I$(IDIR) -Wall -Wpedantic

ODIR = obj
LDIR = lib

LIBS = -lX11 -lXext -lpthread

STATIC_LIBS = /home/kamil/repos/libjpeg-turbo/libturbojpeg.a

_DEPS = xapi.h xjpegapi.h socketserver.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o xapi.o xjpegapi.o socketserver.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(IDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(STATIC_LIBS) $(LIBS)

.PHONY: clean

clean:
	rm -rf $(ODIR)/*.o *~ $(IDIR)/*~
