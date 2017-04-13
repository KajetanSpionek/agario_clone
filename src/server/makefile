CXX = g++
CXXFLAGS = -Wall -std=c++11
PROJECT_LDFLAGS = $(shell pkg-config python-2.7 --libs)
PROJECT_CFLAGS = $(shell pkg-config python-2.7 --cflags)
SOURCE := $(wildcard *.cpp)
OBJS = ${SOURCE:.cpp=.o}
TARGET = program.out
$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS) 

%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $(PROJECT_LDFLAGS) $(PROJECT_CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f *.o

