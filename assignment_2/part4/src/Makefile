# $ make
# $ ./flock_sim

CC = g++
CFLAGS = -c
DEBUG = -g
SFML = -lsfml-graphics -lsfml-window -lsfml-system

all: boids

boids: Entity.o Group.o Vectors.o main.o Play.o
	$(CC) $(DEBUG) Entity.o Group.o Vectors.o Play.o main.o -o flock-sim $(SFML)

Entity.o : Entity.cpp
	$(CC) $(CFLAGS) $(DEBUG) Entity.cpp

Vectors.o : Vectors.cpp
	$(CC) $(CFLAGS) $(DEBUG) Vectors.cpp

Group.o : Group.cpp
	$(CC) $(CFLAGS) $(DEBUG) Group.cpp

Play.o : Play.cpp
	$(CC) $(CFLAGS) $(DEBUG) Play.cpp

clean:
	\rm *.o flock-sim

tar:
	tar cfv boids.tar Vectors.h Vectors.cpp Entity.h Entity.cpp \
		Group.h Group.cpp Play.cpp Play.h