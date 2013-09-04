LIBS = -lSDL -lm
all: renderer.o vectors.o objects.o scene.o loadmodel.o
	gcc renderer.o vectors.o objects.o scene.o loadmodel.o -o renderer $(LIBS)

renderer.o: renderer.c
	gcc -c renderer.c $(LIBS)

vectors.o: vectors.c
	gcc -c vectors.c $(LIBS)

objects.o: objects.c
	gcc -c objects.c $(LIBS)
scene.o: scene.c
	gcc -c scene.c $(LIBS)

loadmodel.o: loadmodel.c
	gcc -c loadmodel.c $(LIBS)
clean:
	rm *.o renderer
