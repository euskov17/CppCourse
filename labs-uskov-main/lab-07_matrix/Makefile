.PHONY: all clean

CXX=g++
CXXFLAGS=-std=c++17 -Wall -pedantic

all: lab-07_matrix

obj:
	mkdir -p obj

lab-07_matrix: src/main.cpp obj/matrix.o include/*.h obj
	$(CXX) $(CXXFLAGS) -o $@ -Iinclude $< obj/*

obj/%.o: src/%.cpp include/*.h obj
	$(CXX) $(CXXFLAGS) -c -o $@ -Iinclude $<

clean:
	rm -rf obj lab-07_matrix
