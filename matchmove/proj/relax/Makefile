ROOT = ../..
LIBDIR = $(ROOT)/lib 
INCLUDE = $(ROOT)/include
LIBX11 = /usr/X11R6/lib

OUT = relax
LIBS = -lrelax -lsequence -lransac -lransac -lba -lcalib -lgsl -lgslcblas -lfeatureslist -lrandom -lgeom -laux -lm
CC = gcc
CFLAGS = -I$(INCLUDE) -L$(LIBDIR) -L$(LIBX11) -o $(OUT) $(LIBS)

install:
	$(CC) main.c $(CFLAGS)
	cp $(OUT) $(ROOT)/bin

clean:
	rm -f *~ *% core $(OUT)



