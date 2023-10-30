CC= gcc
FLAGS= -O2 -Wall -g
INCDIR= -I ./include
LIBFLAGS= -lSDL2 -lSDL2main
SRC_FILES = ./src/bus.c	\
				./src/cpu/cpu.c \
				./src/main.c \
				./src/cpu/instructions.c	\
				./src/ppu/color.c			\
				./src/ppu/ppu.c

OBJ_FILES = ${SRC_FILES:.c=.o}

EXEC= chip8_emu

All: $(EXEC)

chip8_emu: $(OBJ_FILES)
	$(CC) $^ $(INCDIR) $(LIBFLAGS) $(FLAGS) -o $@

bus.o: include/bus.h
main.o: include/bus.h include/cpu.h include/ppu.h
ppu.o: include/ppu.h include/color.h
cpu.o:	include/cpu.h include/bus.h include/instructions.h
instructions.o: include/cpu.h include/bus.h



%.o : %.c
	$(CC) $(INCDIR) $(FLAGS) $(LIBFLAGS) -c $< -o $@

.PHONY : clean
clean:
	rm -rf $(OBJ_FILES)