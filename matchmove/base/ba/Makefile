LIBDIR = ../../lib
INCLUDE = ../../include

CC = gcc
CFLAGS = -g -I$(INCLUDE)

HEADERS = ba.h
OBJECTS = ba.o ba_ext.o ba_ext_coder.o rotations.o
LIBNAME = libba.a

$(LIBNAME): $(HEADERS) $(OBJECTS)
	ar ruv $(LIBNAME) $(OBJECTS)
	ranlib $(LIBNAME)

install: $(LIBNAME)
	mv $(LIBNAME) $(LIBDIR)
	cp $(HEADERS) $(INCLUDE)

clean:
	rm -f *.o *~ *% $(LIBNAME)


