CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -pthread
LDFLAGS := -L../../build/bin
LDLIBS := -llogger -pthread

SRC := app.cpp main.cpp
OBJ_DIR := ../../build/obj
BIN_DIR := ../../build/bin
OBJ := $(OBJ_DIR)/app.o $(OBJ_DIR)/main.o
TARGET := $(BIN_DIR)/app

LIB := $(BIN_DIR)/liblogger.so

all: $(TARGET)

$(TARGET): $(OBJ) $(LIB)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(LDFLAGS) -o $@ $(OBJ) $(LDLIBS)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -I../../include -c $< -o $@

$(LIB):
	$(MAKE) -C ../lib

run: $(TARGET)
	@LD_LIBRARY_PATH=$(BIN_DIR) ./$(TARGET) $(FILE) $(LEVEL)

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean run