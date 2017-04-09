CXX = g++
CXXFLAGS = -Wall -std=c++11
SOURCE := $(wildcard *.cpp)
OBJS = ${SOURCE:.cpp=.o}
TARGET = program.out
$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS) 

%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

