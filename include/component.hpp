#pragma once
#ifndef __COMPONENT_HPP__
#define __COMPONENT_HPP__

#include "bus.hpp"

class Component
{
    private:
        Bus* bus;
    
    public:
        Component();
        virtual ~Component();
        bool connectBus(Bus* bus);
        uint8_t read(uint16_t address);
        void write(uint16_t address, uint8_t data);
};

#endif //__COMPONENT_HPP__