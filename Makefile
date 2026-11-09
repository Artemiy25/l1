CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude -O0 --coverage

TARGET = bin/lab
OBJECTS = build/l1_UserInterface.o build/l2_ApplicationLayer.o build/l3_DomainLayer.o build/l4_InfrastructureLayer.o
HEADERS = include/hw/l2_ApplicationLayer.h include/hw/l3_DomainLayer.h include/hw/l4_InfrastructureLayer.h

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

build/%.o: src/%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf bin/* build/*
