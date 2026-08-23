SRC_DIR := src
LIB_DIR := $(SRC_DIR)/lib
APP_DIR := $(SRC_DIR)/app

all:
	$(MAKE) -C $(LIB_DIR)
	$(MAKE) -C $(APP_DIR)

run:
	@$(MAKE) --no-print-directory -C $(APP_DIR) run

clean:
	$(MAKE) -C $(LIB_DIR) clean
	$(MAKE) -C $(APP_DIR) clean
	rm -rf build

.PHONY: all clean run