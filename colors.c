#include "colors.h"

unsigned char get_red(Uint32 color){
    Uint32 mask = 255 << 0;
    return (color & mask) >> 0;
}
unsigned char get_green(Uint32 color){
    Uint32 mask = 255 << 8;
    return (color & mask) >> 8;
}
unsigned char get_blue(Uint32 color){
    Uint32 mask = 255 << 16;
    return (color & mask) >> 16;
}

Uint32 set_red(Uint32 color, unsigned char red){
    Uint32 mask = 255 << 0;
    color &= ~mask;
    color |= red << 0;
    return color;
}
Uint32 set_green(Uint32 color, unsigned char red){
    Uint32 mask = 255 << 8;
    color &= ~mask;
    color |= red << 8;
    return color;
}
Uint32 set_blue(Uint32 color, unsigned char red){
    Uint32 mask = 255 << 16;
    color &= ~mask;
    color |= red << 16;
    return color;
}
Uint32 set_opacity(Uint32 color, unsigned char opacity){
    Uint32 mask = 255 << 24;

    color = color & (~mask);

    color = color | (opacity << 24);
    return color;
}

Uint32 scale_color(Uint32 color, float multiplier){
    unsigned char red = get_red(color);
    unsigned char green = get_green(color);
    unsigned char blue = get_blue(color);

    red *= multiplier;
    green *= multiplier;
    blue *= multiplier;

    color = set_red(color, red);
    color = set_green(color, green);
    color = set_blue(color, blue);

    return color;
}
