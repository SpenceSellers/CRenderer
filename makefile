LIBS = -lSDL
all: renderer.o vectors.o objects.o
	gcc renderer.o vectors.o objects.o -o renderer $(LIBS)

renderer.o: renderer.c
	gcc -c renderer.c $(LIBS)

vectors.o: vectors.c
	gcc -c vectors.c $(LIBS)

objects.o: objects.c
	gcc -c objects.c $(LIBS)
clean:
	rm *.o renderer
