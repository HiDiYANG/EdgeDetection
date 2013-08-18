CC = g++
CFLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`

SRC = src/main.cpp src/VisWatershed.cpp
OBJ = edges
program: 
	$(CC) -o $(OBJ) $(SRC) $(CFLAGS) $(LIBS)