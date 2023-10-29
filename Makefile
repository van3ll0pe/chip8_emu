CC= gcc
FLAGS= -O2 -Wall -g
INCDIR= -I ./src

SRC_FILES = ./src/bus.c	\
				./src/cpu.c \
				./src/main.c \
				./src/instructions.c

OBJ_FILES = ${SRC_FILES:.c=.o}

EXEC= chip8_emu

All: $(EXEC)

chip8_emu: $(OBJ_FILES)
	$(CC) $^ $(INCDIR}) $(FLAGS) -o $@

bus.o: src/bus.h
cpu.o: src/cpu.h src/bus.h src/instructions.h
instructions.o: src/cpu.h src/bus.h
main.o: src/bus.h

%.o : %.c
	$(CC) $(INCDIR) $(FLAGS) -c $< -o $@

.PHONY : clean
clean:
	rm -rf src/*.o