all: els clean

els: els.o functions.o header.h
	gcc -o els els.o functions.o

els.o: els.c header.h
	gcc -c els.c

functions.o: functions.c header.h
	gcc -c functions.c

clean:
		rm -f *.o
		rm -f *gch
