#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>

#include "bus.h"

bool
getProgramFromFile(char* file, uint8_t** program, uint16_t* program_size)
{
    if (!file) {
        fprintf(stderr, "[ERROR PROGRAM] - not a good filename valid\n");
        return false;
    }
    
    //open the file
    int fd = open(file, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "[ERROR PROGRAM] - impossible to open the file %s given\n", file);
        return false;
    }

    //get size and malloc the buffer
    (*program_size) = lseek(fd, 0, SEEK_END);
    (*program) = malloc(sizeof(uint8_t) * (*program_size));
    if (*program == NULL) {
        fprintf(stderr, "[ERROR PROGRAM] - impossible to malloc enough memory\n");
        close(fd);
        return false;
    }

    //read the file and save into the buffer
    lseek(fd, 0, SEEK_SET);
    if (read(fd, (*program), (*program_size)) == -1) {
        free(*program);
        fprintf(stderr, "[ERROR PROGRAM] - impossible to read the file %s given\n", file);
        close(fd);
        return false;
    }

    close(fd);
    return true;
}


int
main(int ac, char** av)
{
    if (ac < 2 || ac > 2) {
        fprintf(stderr, "[ERROR] - program file not given\n");
        return 1;
    }

    uint8_t* program = NULL;
    uint16_t program_size = 0;

    if (getProgramFromFile(av[1], &program, &program_size) == false)
        return 1;

    Bus_t* bus = bus_initialize();
    bus_loadProgram(bus, program, program_size);


    free(program);
    bus_destroy(bus);
    return 0;
}