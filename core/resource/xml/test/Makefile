CC = g++

LIB_NAME = 

OPTS = -O2 -g -Wall -fmessage-length=0 -std=c++11

OBJS = XMLHandler.o test.o

INC = -iquote../src/

LIBDIR = -L../src/

VPATH = ../src:./

TARGET = test 


all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(OPTS) $(LIBDIR) $(LIBS)
	
%.o: %.cpp 
	$(CC) -c $< $(OPTS) $(INC)

clean:
	rm -f $(OBJS) $(TARGET)
