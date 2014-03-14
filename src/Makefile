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
LIB_FILENAME = 
LIBS_DIR = -L.
INC = 
MACROS =
OPTS = -g -std=gnu++11
OBJS = Mesh.o ShapeHelper.o Object3D.o GLMesh.o Node3D.o GLMeshInstance.o CameraIntrinsic.o CameraFirstPerson.o CameraThirdPerson.o GLScene.o GLSLProgram.o GLSceneLighting.o GLSceneNormal.o GLSceneShadow.o NormalMapHelper.o TextureManager.o GLSceneSprite.o ArcBallController.o CoordinateHelper.o

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
all: $(OBJS)
	ar cvr $(LIB_FILENAME) $(OBJS)

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
