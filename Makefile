LIB = -lGL -lglut
CC = g++ -Wall

all: glDraw

glDraw: main.o figures.o 
	$(CC) -o glDraw main.o figures.o $(LIB)

main.o: main.cpp figures.h
	$(CC) -c main.cpp $(INC)

figures.o: figures.h figures.cpp 
	$(CC) -c figures.cpp $(INC)
	
clean:
	rm *.o glDraw
