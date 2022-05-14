EXE = lab-04_intrusive-list
OBJECTS = obj/main.o obj/clist.o obj/point_list.o
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
