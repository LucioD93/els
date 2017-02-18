all: els clean

els: els.o functions.o functions.h
	gcc -o els els.o functions.o

els.o: els.c functions.h
	gcc -c els.c

functions.o: functions.c functions.h
	gcc -c functions.c

clean:
		rm -f *.o
		rm -f *gch
