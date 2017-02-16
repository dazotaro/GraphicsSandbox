CC = g++

LIB_NAME = 

OPTS = -O2 -g -Wall -fmessage-length=0 -std=c++11

OBJS = test_scene.o GLSceneLighting.o GLSceneNormal.o GLSceneShadow.o GLSceneParticles.o  GLSceneMultipleLights.o GLSceneDeferred.o GLSceneAntTweakBar.o GLSceneTerrain.o #GLSceneSprite.o GLSceneCometTail.o

INC = -iquote../src/ 

LIBDIR = -L../src/

LIBS = -l$(LIB_NAME) -lSOIL -lglut -lGL -lTiming -lrt -lcore -lAntTweakBar -lassimp

TARGET = graphics 

all: $(TARGET)

$(TARGET): $(OBJS) ../src/$(LIBNAME)
	$(CC) -o $(TARGET) $(OBJS) $(OPTS) $(LIBDIR) $(LIBS)
	
%.o : %.cpp 
	$(CC) -c $< $(OPTS) $(INC)

clean:
	rm -f $(OBJS) $(TARGET)
