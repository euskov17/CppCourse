EXE = lab-06_int-vector
OBJECTS = obj/main.o obj/my_vector.o 
CC_FLAGS = -std=c++17 -Wall -Werror -Wextra -MMD -I include
MKDIR = mkdir -p
СС = g++
all : $(EXE)

obj:
	$(MKDIR)  $@
$(EXE) : obj  $(OBJECTS)
	$(СС) -o $@ $(OBJECTS)
$(OBJECTS): obj/%.o: src/%.cpp
	$(CC) $(CC_FLAGS) -c $< -o $@

include $(wildcard obj/*.d)

clean :
	$(RM) $(EXE) -r obj

.PHONY: all clean