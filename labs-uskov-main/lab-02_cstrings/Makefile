EXE = lab-02_str
OBJECTS = obj/str.o obj/test_str.o obj/test.o
CC_FLAGS = -Wall -Werror -Wextra -MMD -I include
MKDIR = mkdir -p
all : $(EXE)

obj:
	$(MKDIR)  $@
$(EXE) : obj  $(OBJECTS) 
	$(CC) -o $@ $(OBJECTS)
$(OBJECTS): obj/%.o: src/%.c
	$(CC) $(CC_FLAGS) -c $< -o $@

include $(wildcard obj/*.d)

clean :
	$(RM) $(EXE) -r obj

.PHONY: all clean