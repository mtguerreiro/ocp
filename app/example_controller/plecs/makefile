default: all

all: opil_plecs_linux

opil_plecs_linux: opil_plecs_linux.c
	gcc -fPIC opil_plecs_linux.c -c -I../ -I/home/marco/projects/opil
	gcc -L. -Wl,-rpath=. -shared -o libopil_plecs_linux.so opil_plecs_linux.o

clean:
	$(RM) *.o
	$(RM) *.so
