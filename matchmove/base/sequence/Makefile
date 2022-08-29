LIBDIR = ../../lib
INCLUDE = ../../include

CC = gcc
CFLAGS = -g -I$(INCLUDE)

HEADERS = sequence.h
OBJECTS = dlt.o ba.o subseq_alloc.o subseq.o seq_alloc.o seq.o inliers.o scale.o recalibrate.o
LIBNAME = libsequence.a

$(LIBNAME): $(HEADERS) $(OBJECTS)
	ar ruv $(LIBNAME) $(OBJECTS)
	ranlib $(LIBNAME)

install: $(LIBNAME)
	mv $(LIBNAME) $(LIBDIR)
	cp $(HEADERS) $(INCLUDE)

clean:
	rm -f *.o *~ *% $(LIBNAME)


