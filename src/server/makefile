CXX = g++
INC_DIR = /home/wprzybys/Documents/ZPR/agario_clone/agario_clone/src/server/Beast-master/include
CXXFLAGS = -Wall -std=c++11 -I$(INC_DIR)
PROJECT_LDFLAGS = $(shell pkg-config python-2.7 --libs)
PROJECT_CFLAGS = $(shell pkg-config python-2.7 --cflags)
LINK_FLAGS = -lboost_system -lboost_asio -lpthread -lrt -lm
SOURCE := $(wildcard *.cpp)
OBJS = ${SOURCE:.cpp=.o}
TARGET = program.out
$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS) -lboost_system -lpthread

%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $(PROJECT_LDFLAGS) $(PROJECT_CFLAGS) $< -o $@ $(PROJECT_FLAGS)

.PHONY: clean
clean:
	rm -f *.o

