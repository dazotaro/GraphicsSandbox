SRC_DIR = ./src/
TEST_DIR = ./test/
LIB_DIR = /usr/local/lib/
INC_DIR = /usr/local/include/JU/
LIB_NAME = Particles
LIB_FILENAME = lib$(LIB_NAME).a

all :
	cd $(SRC_DIR); make LIB_NAME=$(LIB_NAME)

install:
	cp $(SRC_DIR)$(LIB_FILENAME) $(LIB_DIR); cp $(SRC_DIR)*.hpp $(INC_DIR)

test:
	cd $(TEST_DIR); make LIB_NAME=$(LIB_NAME)

clean:
	cd $(SRC_DIR); make LIB_NAME=$(LIB_NAME) clean; cd $(TEST_DIR); make clean

