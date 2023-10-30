#include <stdint.h>

uint32_t
get_complementary(uint32_t color)
{
    color = ~color;
    return color;
}