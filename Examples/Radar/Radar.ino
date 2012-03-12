// Radar.pde
//
// Test Peggy2Render library line routine

#include <stdlib.h>
#include <math.h>

// Include libraries
#include <Display.h>
#include <Render.h>

// Create library objects
Peggy2Display Display;
Peggy2Render Render;

void setup()
{
	// Construct library objects
	Display = Peggy2Display();
	Display.SetRefreshRate(60.0);
	
	Render = Peggy2Render(Display.framebufferptr);
	Render.RenderFormat(RENDER_XOR);
}

void loop()
{
	for (int8_t x=1; x<25; x++)
	{
		Render.Line(12, 12, x, 0);
		delay(50);
		Render.Line(12, 12, x, 0);
	}

	for (int8_t y=1; y<25; y++)
	{
		Render.Line(12, 12, 24, y);
		delay(50);
		Render.Line(12, 12, 24, y);
	}
	
	for (int8_t x=24; x>0; x--)
	{
		Render.Line(12, 12, x, 24);
		delay(50);
		Render.Line(12, 12, x, 24);
	}
	
	for (int8_t y=24; y>0; y--)
	{
		Render.Line(12, 12, 0, y);
		delay(50);
		Render.Line(12, 12, 0, y);
	}
}

