CXX = g++
CXXFLAGS = -O2 -Wall -Werror -Wextra -std=c++17 -Iinclude
LDFLAGS = 

TESTDIR = tests
TEST = test
EXE = hw-02_huffman
SRCDIR = src
OBJDIR = obj

OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(wildcard $(SRCDIR)/*.cpp))

all: $(EXE)

$(EXE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(EXE) $(LDFLAGS)
	
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c -MMD -o $@ $<

include $(wildcard $(OBJDIR)/*.d)

$(OBJDIR): 
	mkdir -p $(OBJDIR)

$(TEST): test.cpp
	g++ tests/test.cpp src/huffman.cpp $(CXXFLAGS) -o $(TEST) 

test.cpp:
	

clean:
	rm -rf $(OBJDIR) $(EXE) $(TEST)

.PHONY: clean all