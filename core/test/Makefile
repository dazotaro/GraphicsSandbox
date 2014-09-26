CC = g++

LIB_NAME = 

OPTS = -O2 -g -Wall -fmessage-length=0 -std=c++11

OBJS = test.o TestClass.o

INC = -iquote../src/

LIBDIR = -L../src/

LIBS = -l$(LIB_NAME) 

TARGET = core 

all: $(TARGET)

$(TARGET): $(OBJS) ../src/$(LIBNAME)
	$(CC) -o $(TARGET) $(OBJS) $(OPTS) $(LIBDIR) $(LIBS)
	
%.o : %.cpp 
	$(CC) -c $< $(OPTS) $(INC)

clean:
	rm -f $(OBJS) $(TARGET) *.xml *.json
