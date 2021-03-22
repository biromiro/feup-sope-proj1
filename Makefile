CC = gcc

prog = xmod
SRC := src
OBJ := bin

SOURCES := $(wildcard $(SRC)/*.c)
OBJECTS := $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SOURCES))

flags = -g -Wall
libs =

xmod: bin xmod_inner

xmod_inner: $(OBJECTS)
	$(CC) $(flags)  $^ -o xmod

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) -I$(SRC) $(flags)  -c $< -o $@

bin: 
	mkdir bin

clean:
	rm -f $(OBJ)/*.o xmod 
