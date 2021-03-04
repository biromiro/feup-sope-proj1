CC = gcc

prog = xmod
SRC := src
OBJ := bin

SOURCES := $(wildcard $(SRC)/*.c)
OBJECTS := $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SOURCES))

flags = -g -Wall
libs =

xmod: $(OBJECTS)
	$(CC) $(flags)  $^ -o $@

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) -I$(SRC) $(flags)  -c $< -o $@

clean:
	rm -f $(OBJ)/*.o $(OBJ)/*.json xmod 
