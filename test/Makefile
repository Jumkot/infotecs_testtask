CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra
LDFLAGS := -L../build/bin
LDLIBS := -llogger

SRC := logger.test.cpp
OBJ_DIR := ../build/obj
BIN_DIR := ../build/bin
OBJ := $(OBJ_DIR)/logger.test.o
TARGET := $(BIN_DIR)/logger.test

LIB := $(BIN_DIR)/liblogger.so
RESULTS_DIR := results/

all: $(TARGET)

$(TARGET): $(OBJ) $(LIB)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(LDFLAGS) -o $@ $(OBJ) $(LDLIBS)

$(OBJ): $(SRC)
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -I../include -c $< -o $@

$(LIB):
	$(MAKE) -C ../src/lib

test: $(TARGET)
	@mkdir -p $(RESULTS_DIR)
	@cd $(RESULTS_DIR) && LD_LIBRARY_PATH=../../build/bin ../../build/bin/logger.test


clean:
	rm -rf $(OBJ) $(TARGET) $(RESULTS_DIR)

.PHONY: all test clean