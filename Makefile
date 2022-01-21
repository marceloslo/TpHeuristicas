CC = g++
CFLAGS = -Wall

all: main.exe

main.exe: main.o bee_colony/bee_colony.o
	$(CC) -o main.exe main.o bee_colony/bee_colony.o

main.o: main.cpp bee_colony/bee_colony.hpp
	$(CC) -o main.o main.cpp -c $(CFLAGS)

bee_colony/bee_colony.o: bee_colony/bee_colony.cpp bee_colony/bee_colony.hpp bee_colony/Bees.hpp
	$(CC) -o bee_colony/bee_colony.o bee_colony/bee_colony.cpp -c $(CFLAGS)
