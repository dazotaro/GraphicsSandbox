###################################################
# with GLEW:
#	make GLEW='true'
#
# without GLEW (i.e. with glext.h)
#	make
#
# to CLEAN:
#	make clean
##################################################

# VARIABLE DEFINITIONS
# --------------------
CC = g++
LIBS_DIR = -L.
INC =
MACROS =
OPTS = -g
OBJS = XMLHandler.o

# TARGETS
# -------
all: $(OBJS)

%.o : %.cpp 
	$(CC) -c $< $(OPTS) $(MACROS) $(INC)

doc : 
	doxygen

clean:
	rm $(OBJS) *~ $(LIB_FILENAME) 

cleandoc:
	rm -rf html; rm -rf latex

cleanobj:
	rm *.obj

cleanall: cleandoc cleanobj clean 
