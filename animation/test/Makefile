CC = g++

LIB_NAME = 

OPTS = -O2 -g -Wall -fmessage-length=0 -std=c++11

OBJS = 

INC = -iquote../src/

LIBDIR = -L../src/

LIBS = -l$(LIB_NAME)

TARGET = animation

all: $(TARGET)

$(TARGET): $(OBJS) ../src/$(LIBNAME)
	$(CC) -o $(TARGET) $(OBJS) $(OPTS) $(LIBDIR) $(LIBS)
	
%.o : %.cpp 
	$(CC) -c $< $(OPTS) $(INC)

clean:
	rm -f $(OBJS) $(TARGET)
