CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -fPIC
LDFLAGS := -shared

SRC := logger.cpp
OBJ_DIR := ../../build/obj
BIN_DIR := ../../build/bin
OBJ := $(OBJ_DIR)/logger.o
TARGET := $(BIN_DIR)/liblogger.so

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(LDFLAGS) -o $@ $^

$(OBJ): $(SRC)
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -I../../include -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean