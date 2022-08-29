LIBDIR = ../../lib
INCLUDE = ../../include

CC = gcc
CFLAGS = -g -I$(INCLUDE)

HEADERS = calib.h
OBJECTS = kmatrix.o fmatrix.o ematrix.o pmatrix.o get3d.o getRT.o getP.o rttransform.o normalize.o normalize3d.o homog.o homog3d.o applyP.o changeref.o camfactor.o singlecam.o reconstruct.o
LIBNAME = libcalib.a

$(LIBNAME): $(HEADERS) $(OBJECTS)
	ar ruv $(LIBNAME) $(OBJECTS)
	ranlib $(LIBNAME)

install: $(LIBNAME)
	mv $(LIBNAME) $(LIBDIR)
	cp $(HEADERS) $(INCLUDE)

clean:
	rm -f *.o *~ *% $(LIBNAME)


