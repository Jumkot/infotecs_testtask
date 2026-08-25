SRC_DIR := src
LIB_DIR := $(SRC_DIR)/lib
APP_DIR := $(SRC_DIR)/app
TEST_DIR := test

FILE ?= demo.log
LEVEL ?= WARNING

all: lib app

lib:
	@$(MAKE) --no-print-directory -C $(LIB_DIR)

app:
	@$(MAKE) --no-print-directory -C $(APP_DIR)

run:
	@$(MAKE) --no-print-directory -C $(APP_DIR) run FILE="$(FILE)" LEVEL="$(LEVEL)"

test:
	@$(MAKE) --no-print-directory -C $(TEST_DIR) test

clean:
	$(MAKE) -C $(LIB_DIR) clean
	$(MAKE) -C $(APP_DIR) clean
	$(MAKE) -C $(TEST_DIR) clean
	rm -rf build

.PHONY: all lib run test clean