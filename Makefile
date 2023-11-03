CC= g++
FLAGS= -O2 -Wall -g
INCDIR= -I ./include
#LIBFLAGS= -lSDL2 -lSDL2main
SRC_FILES = ./src/bus.cpp	\
				./src/cpu/cpu.cpp \
				./src/main.cpp \
				./src/cpu/instructions.cpp	\
				./src/component.cpp


OBJ_FILES = ${SRC_FILES:.cpp=.o}

EXEC= chip8_emu

All: $(EXEC)

chip8_emu: $(OBJ_FILES)
	$(CC) $^ $(INCDIR) $(FLAGS) -o $@

bus.o: include/bus.hpp
component.o: include/component.hpp include/bus.hpp
main.o: include/bus.hpp include/cpu.hpp \
 include/component.hpp
cpu.o: include/cpu.hpp include/component.hpp \
 include/bus.hpp
instructions.o: include/cpu.hpp \
 include/component.hpp include/bus.hpp


%.o : %.cpp
	$(CC) $(INCDIR) $(FLAGS) -c $< -o $@

.PHONY : clean
clean:
	rm -rf $(OBJ_FILES)