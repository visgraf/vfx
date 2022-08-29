ROOT = ../..
LIBDIR = $(ROOT)/lib 
INCLUDE = $(ROOT)/include

OUT = pclcalib
LIBS = -lpcloud -lrelax -lba -lcalib -lgsl -lgslcblas -lfeatureslist -lrandom -lgeom -laux -lm
CC = gcc
CFLAGS = -I$(INCLUDE) -L$(LIBDIR) -o $(OUT) $(LIBS)

install:
	$(CC) main.c $(CFLAGS)
	cp $(OUT) $(ROOT)/bin

clean:
	rm -f *~ *% core $(OUT)



