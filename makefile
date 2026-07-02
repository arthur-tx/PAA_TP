# Variáveis de compilador e flags
CXX = g++
CXXFLAGS = -Wall -std=c++11 -I./src/headers -I./libs

# Diretórios
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Arquivos
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))
EXECUTABLE = $(BIN_DIR)/programa.exe

# --- REGRAS DE CONSTRUÇÃO ---

build: all

all: prepare $(EXECUTABLE)

prepare:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# --- REGRA DE EXECUÇÃO  ---

ARGS = $(filter-out $@,$(MAKECMDGOALS))

run: all
	@./$(EXECUTABLE) $(ARGS)

%:
	@:

clean:
	@rm -rf $(OBJ_DIR)/*.o
	@rm -rf $(BIN_DIR)/*.exe
	@rm -rf $(BIN_DIR)/programa