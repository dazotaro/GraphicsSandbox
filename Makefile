CC = g++

OPTS =	-O2 -g -Wall -fmessage-length=0 -std=c++11

OBJS =	Force.o main.o Particle.o ParticleSystem.o

LIBS =

INC = -I$(HOME)/include/ -I$(HOME)/include/glm/

TARGET = particle_system

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OPTS) $(OBJS) $(LIBS)
	
%.o : %.cpp 
	$(CC) -c $< $(OPTS) $(INC)
	

all: $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
