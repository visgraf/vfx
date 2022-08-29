ROOT = ../..
LIBDIR = $(ROOT)/lib
INCLUDE = $(ROOT)/include

OUT = track
LIBS = 
CPPC = g++
CFLAGS = -I$(INCLUDE) -L$(LIBDIR) -o $(OUT) $(LIBS) `pkg-config --cflags --libs opencv4`

install:
	$(CPPC) main.cxx $(CFLAGS)
	cp $(OUT) $(ROOT)/bin

clean:
	rm -f *~ *% core $(OUT)
