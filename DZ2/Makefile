CC=g++
LD=g++
NAME=optimizer

OBJ_DIR=obj
SRC_DIR=src

SRC=$(shell find $(SRC_DIR) -type f -regex ".*\.cpp")
OBJ=$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))

CXX_FLAGS = -c -O3
LD_FLAGS =

all: $(NAME)

$(NAME): $(OBJ)
	@echo [LD]
	$(LD) -o $@ $^ $(LD_FLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo [CC]
	$(CC) -o $@ $^ $(CXX_FLAGS)

clean:
	rm $(NAME) $(OBJ_DIR)/*.o 


