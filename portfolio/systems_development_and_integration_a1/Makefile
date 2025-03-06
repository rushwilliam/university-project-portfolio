CFLAGS = -Wall -std=c99 -Iinclude -fPIC

parser: bin/libvcparser.so

bin/libvcparser.so: VCParser.o VCHelper.o LinkedListAPI.o
	gcc -shared -o bin/libvcparser.so VCParser.o VCHelper.o LinkedListAPI.o

VCParser.o: src/VCParser.c include/VCParser.h include/VCHelper.h include/LinkedListAPI.h
	gcc $(CFLAGS) -c src/VCParser.c

VCHelper.o: src/VCHelper.c include/VCHelper.h include/LinkedListAPI.h
	gcc $(CFLAGS) -c src/VCHelper.c

LinkedListAPI.o: src/LinkedListAPI.c include/LinkedListAPI.h
	gcc $(CFLAGS) -c src/LinkedListAPI.c

clean:
	rm -f *.o *.so