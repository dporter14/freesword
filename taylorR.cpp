#include <iostream>

#include "defs.h"
#include "fonts.h"

int taylor_func(int xres, int yres)
{	
	Rect r;
	r.left   = xres/2;
	r.bot	= yres-100;
	r.center = 1;
	ggprint16(&r, 16, 0x00ffffff, "Taylor Redden");
	return 0;
}



















