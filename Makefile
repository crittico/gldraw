LIB = -lGL -lglut
CC = g++ -Wall

all: draw

draw: main.o figures.o 
	$(CC) -o draw main.o figures.o $(LIB)

main.o: main.cpp figures.h
	$(CC) -c main.cpp $(INC)

figures.o: figures.h figures.cpp 
	$(CC) -c figures.cpp $(INC)
	
clean:
	rm *.o draw
