LIBDIR = ../../lib
INCLUDE = ../../include

CC = gcc
CFLAGS = -g -I$(INCLUDE)

HEADERS = defs.h laux.h list.h
OBJECTS = error.o mem.o io.o list.o
LIBNAME = libaux.a

$(LIBNAME): $(HEADERS) $(OBJECTS)
	ar ruv $(LIBNAME) $(OBJECTS)
	ranlib $(LIBNAME)

install: $(LIBNAME)
	mv $(LIBNAME) $(LIBDIR)
	cp $(HEADERS) $(INCLUDE)

clean:
	rm -f *.o *~ $(LIBNAME)


