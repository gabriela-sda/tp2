CC = g++
CFLAGS = -Wall -I include/

BIN_DIR = ./bin
OBJ_DIR = ./obj
SRC_DIR = ./src

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
EXECUTABLE = $(BIN_DIR)/tp2.out

# Default
INPUT_FILE = input.txt

all: directories $(EXECUTABLE)

directories:
	mkdir -p $(BIN_DIR)
	mkdir -p $(OBJ_DIR)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	./$(EXECUTABLE) $(INPUT_FILE)

clean:
	rm -rf $(OBJ_DIR)/*.o $(EXECUTABLE)
