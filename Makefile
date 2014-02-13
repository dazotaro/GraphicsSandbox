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
LIBS_DIR =
LIBS_ALL = -lSOIL -lglut -lGL -lTiming -lrt
INC = 
MACROS =
OPTS = -g -std=gnu++11
OBJS = Mesh.o ShapeHelper.o Object3D.o GLMesh.o Node3D.o GLMeshInstance.o CameraIntrinsic.o CameraFirstPerson.o CameraThirdPerson.o GLScene.o GLSLProgram.o GLSceneLighting.o GLSceneNormal.o GLSceneShadow.o NormalMapHelper.o TextureManager.o GLSceneSprite.o ArcBallController.o CoordinateHelper.o
EXEC = shapes

# GLEW SPECIFIC VARIABLE DEFINITIONS 
# ----------------------------------
GLEW = true
LIB_GLEW = -lGLEW
MACROS_GLEW = -DUSING_GLEW

# CONDITIONAL UPDATE OF VARIABLES
# -------------------------------
ifeq ($(GLEW), true)
	MACROS = $(MACROS_GLEW)
	LIBS = $(LIBS_ALL) $(LIB_GLEW)
else
	LIBS = $(LIBS_ALL)
endif

# TARGETS
# -------
all: main_test.hpp  main_glew.hpp main_scene.hpp main.cpp $(OBJS)
	$(CC) -o $(EXEC) main.cpp $(OPTS) $(OBJS) $(LIBS) $(MACROS) $(INC) $(LIBS_DIR)

%.o : %.cpp 
	$(CC) -c $< $(OPTS) $(MACROS) $(INC)

doc : 
	doxygen

clean:
	rm $(OBJS) *~ $(EXEC) 

cleandoc:
	rm -rf html; rm -rf latex

cleanobj:
	rm *.obj

cleanall: cleandoc cleanobj clean 
