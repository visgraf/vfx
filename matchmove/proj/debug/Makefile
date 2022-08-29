ROOT = ../..
LIBDIR = $(ROOT)/lib 
INCLUDE = $(ROOT)/include

OUT = debug
LIBS = -lpcloud -lrelax -lba -lcalib -lgsl -lgslcblas -lfeatureslist -lrandom -lgeom -laux -lm
CC = g++
CFLAGS = -I$(INCLUDE) -L$(LIBDIR) -o $(OUT) $(LIBS) `pkg-config --cflags --libs opencv4`

install:
	$(CC) main.cxx $(CFLAGS)
	cp $(OUT) $(ROOT)/bin

clean:
	rm -f *~ *% core $(OUT)



