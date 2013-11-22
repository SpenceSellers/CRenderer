#ifndef COLORS_H
#define COLORS_H
#include <SDL/SDL.h>
unsigned char get_red(Uint32 color);
unsigned char get_green(Uint32 color);
unsigned char get_blue(Uint32 color);
Uint32 set_red(Uint32 color, unsigned char red);
Uint32 set_green(Uint32 color, unsigned char green);
Uint32 set_blue(Uint32 color, unsigned char blue);
Uint32 set_opacity(Uint32 color, unsigned char opacity);
Uint32 scale_color(Uint32 color, float multiplier);

#endif
