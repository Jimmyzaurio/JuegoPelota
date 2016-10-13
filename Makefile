LIBS = juegoPelota.o gfxModified.o

juegoPelota:  $(LIBS)
	g++ $(LIBS) -o juegoPelota -lX11 -lm

juegoPelota.o: juegoPelota.cpp gfxModified.o
	g++ juegoPelota.cpp -c

gfxModified.o: gfxModified.c gfxModified.h
	gcc gfxModified.c -c

clean:
	rm $(LIBS) juegoPelota