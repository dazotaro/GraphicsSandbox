LIB_DIR = /usr/local/lib/
INC_DIR = /usr/local/include/JU/
LIB_NAME = core
LIB_FILENAME = lib$(LIB_NAME).a

# Subdirectories
COMMON_DIR = ./common/
XML_DIR = ./resource/xml/

.PHONY: test

all :
	cd $(XML_DIR)src; make 

install : archive
	cp $(SRC_DIR)$(LIB_FILENAME) $(LIB_DIR); cp --parents $(COMMON_DIR)*.hpp $(INC_DIR); cp --parents $(XML_DIR)src/*.hpp $(INC_DIR)

archive :
	ar cvr $(LIB_FILENAME) $(XML_DIR)src/*.o

test :
	cd $(TEST_DIR); make LIB_NAME=$(LIB_NAME)

clean :
	cd $(XML_DIR)src; make clean

