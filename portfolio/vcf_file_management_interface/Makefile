CFLAGS = -Wall -std=c99 -Iinclude -fPIC

parser: bin/libvcparser.so

test: bin/libvcparser.so test.c
	gcc $(CFLAGS) -o test test.c -Lbin -lvcparser

bin/libvcparser.so: VCParser.o VCHelper.o VCWrapper.o LinkedListAPI.o
	gcc -shared -o bin/libvcparser.so VCParser.o VCHelper.o VCWrapper.o LinkedListAPI.o

VCParser.o: src/VCParser.c include/VCParser.h include/VCHelper.h include/LinkedListAPI.h
	gcc $(CFLAGS) -c src/VCParser.c

VCHelper.o: src/VCHelper.c include/VCHelper.h include/LinkedListAPI.h
	gcc $(CFLAGS) -c src/VCHelper.c

VCWrapper.o: src/VCWrapper.c include/VCWrapper.h include/VCParser.h
	gcc $(CFLAGS) -c src/VCWrapper.c

LinkedListAPI.o: src/LinkedListAPI.c include/LinkedListAPI.h
	gcc $(CFLAGS) -c src/LinkedListAPI.c

clean:
	rm -f *.o *.so