ROOT = ../..
LIBDIR = $(ROOT)/lib 
INCLUDE = $(ROOT)/include
LIBX11 = /usr/X11R6/lib

OUT = singlecam
LIBS = -lsinglecam -lba -lcalib -lpthread -lgsl -lgslcblas -lm
CC = gcc
CFLAGS = -I$(INCLUDE) -L$(LIBDIR) -L$(LIBX11) -o $(OUT) $(LIBS)

install:
	$(CC) main.c $(CFLAGS)
	cp $(OUT) $(ROOT)/bin

clean:
	rm -f *~ *% core $(OUT)



