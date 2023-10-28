CC= gcc
FLAGS= -O2 -Wall -g
INCDIR= -I ./src

SRC_FILES = ./src/bus.c	\
				./src/cpu.c \
				./src/main.c

OBJ_FILES = ${SRC_FILES:.c=.o}

EXEC= chip8_emu

All: $(EXEC)

chip8_emu: $(OBJ_FILES)
	$(CC) $^ $(INCDIR}) $(FLAGS) -o $@

%.o : %.c
	$(CC) $(INCDIR) $(FLAGS) -c $< -o $@

.PHONY : clean
clean:
	rm -rf src/*.o