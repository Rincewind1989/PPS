CC=g++ -Ofast
CFLAGS=-I. -Wall
DEPS = World.h Functions.h Particle.h
LIBS= -lsfml-graphics -lsfml-window -lsfml-system
OBJ = main.o


%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

PPS: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -f PPS
	rm -f *.o

