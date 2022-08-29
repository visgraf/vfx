DIRS = base bin doc include lib proj
TARFILE = 3dp_vfx.tar

all:
	cd base; make install; cd ..;
	cd proj; make install; cd ..;
 

tar:
	make clean;
	tar cvf $(TARFILE) $(DIRS) Makefile README
	gzip $(TARFILE)

back:
	make clean;
	tar cvf $(TARFILE) $(DIRS) Makefile
	gzip $(TARFILE)
 
clean:
	cd proj; make clean; cd ..;
	cd base; make clean; cd ..;
	cd bin; rm -f *; cd ..;
	cd lib; rm -f *; cd ..;
	cd include; rm -f *; cd ..;









