LIBS = -lSDL
all: renderer.o vectors.o
	gcc renderer.o vectors.o -o renderer $(LIBS)

renderer.o: renderer.c
	gcc -c renderer.c $(LIBS)

vectors.o: vectors.c
	gcc -c vectors.c $(LIBS)

clean:
	rm *.o renderer
