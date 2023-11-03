#include "bus.hpp"
#include "cpu.hpp"
#include <iostream>

int
main(int ac, char** av)
{
    if (ac != 2) {
        std::cerr << "NO FILE GIVEN\n" << std::endl;
        return 1;
    }
    Bus bus {};
    Cpu cpu{};

    cpu.connectBus(&bus);

    
    return 0;

}