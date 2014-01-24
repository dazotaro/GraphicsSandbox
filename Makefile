CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		main.o Particle.o ParticleSystem.o

LIBS =

TARGET =	particle_system

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
