CC = g++

LIB_FILENAME =

OPTS = -O2 -g -Wall -fmessage-length=0 -std=c++11

OBJS = Force.o Particle.o ParticleSystem.o

INC =

TARGET = $(LIB_FILENAME)

all: $(OBJS)
	ar crv $(TARGET) $(OBJS)

%.o : %.cpp 
	$(CC) -c $< $(OPTS) $(INC)

clean:
	rm -v $(OBJS) $(TARGET)
