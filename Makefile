CC= gcc
FLAGS= -Wall -g
INCDIR= -I ./include
LIBFLAGS= -lSDL2 -lSDL2main
SRC_FILES =     ./src/bus.c\
				./src/cpu.c\
				./src/main.c \
				./src/ppu.c


OBJ_FILES = ${SRC_FILES:.c=.o}

EXEC= chip8_emu

All: $(EXEC)

chip8_emu: $(OBJ_FILES)
	$(CC) $(OBJ_FILES)  $(INCDIR) $(LIBFLAGS) $(FLAGS) -o $@

bus.o: include/bus.h
cpu.o: include/cpu.h include/bus.h include/ppu.h \
 include/opcode.h include/input.h
main.o: include/bus.h include/ppu.h include/cpu.h
ppu.o: include/ppu.h include/bus.h

%.o : %.c
	$(CC) $(INCDIR) $(LIBFLAGS) $(FLAGS) -c $< -o $@

.PHONY : clean
clean:
	rm -rf $(OBJ_FILES)