LIBDIR = ../../lib
INCLUDE = ../../include

CC = gcc
CFLAGS = -g -I$(INCLUDE)

HEADERS = geom.h ray.h
OBJECTS = vector.o matrix.o ray.o inode.o
LIBNAME = libgeom.a

$(LIBNAME): $(HEADERS) $(OBJECTS)
	ar ruv $(LIBNAME) $(OBJECTS)
	ranlib $(LIBNAME)

install: $(LIBNAME)
	mv $(LIBNAME) $(LIBDIR)
	cp $(HEADERS) $(INCLUDE)

clean:
	rm -f *.o *~ $(LIBNAME)


