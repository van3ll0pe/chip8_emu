#include "component.hpp"
#include <stdexcept>

Component::Component()
{
    this->bus = nullptr;
}

Component::~Component() { };

bool
Component::connectBus(Bus* bus)
{
    if (bus == nullptr) {
        throw std::runtime_error("[ERROR] - BUS NOT GIVEN TO COMPONENT");
        return false;
    }

    this->bus = bus;
    return true;
}

uint8_t
Component::read(uint16_t address)
{
    if (this->bus == nullptr) {
        throw std::runtime_error("[ERROR] BUS NOT CONNECTED TO READ");
    
    }

    return this->bus->read(address);
}

void
Component::write(uint16_t address, uint8_t data)
{
    if (this->bus == nullptr)
        throw std::runtime_error("[ERROR] BUS NOT CONNECTED TO WRITE");
    
    this->bus->write(address, data);
}