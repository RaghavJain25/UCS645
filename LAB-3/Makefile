CXX = g++
CXXFLAGS = -O3 -march=native -std=c++17 -fopenmp

TARGET = correlation
SRC = main.cpp correlate.cpp
OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp correlate.h
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
