CC = g++

LIBNAME = particles

OPTS =	-O2 -g -Wall -fmessage-length=0 -std=c++11

OBJS =	Force.o Particle.o ParticleSystem.o

TESTOBJS = main.o

LIBS = -l$(LIBNAME)

LIBDIR = -L.

INC =

TEST_TARGET = particle_system

all: $(OBJS)
	ar crv lib$(LIBNAME).a $(OBJS)

$(TEST_TARGET): $(OBJS) $(TESTOBJS)
	$(CC) -o $(TEST_TARGET) $(TESTOBJS) $(OPTS) $(OBJS) $(LIBDIR) $(LIBS)
	
%.o : %.cpp 
	$(CC) -c $< $(OPTS) $(INC)

test: $(TEST_TARGET)

clean:
	rm -f $(OBJS) $(TEST_TARGET) lib$(LIBNAME).a
