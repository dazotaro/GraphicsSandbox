SRC_DIR = ./src/
TEST_DIR = ./test/
LIB_DIR = /usr/local/lib/
INC_DIR = /usr/local/include/JU/core/
LIB_NAME = core
LIB_FILENAME = lib$(LIB_NAME).a

.PHONY: test

all :
	cd $(SRC_DIR); make LIB_FILENAME=$(LIB_FILENAME)

install:
	cp $(SRC_DIR)$(LIB_FILENAME) $(LIB_DIR); mkdir -p $(INC_DIR); cp $(SRC_DIR)*.hpp $(INC_DIR)

test:
	cd $(TEST_DIR); make LIB_NAME=$(LIB_NAME)

clean:
	cd $(SRC_DIR); make LIB_FILENAME=$(LIB_FILENAME) clean; cd ../; cd $(TEST_DIR); make clean

