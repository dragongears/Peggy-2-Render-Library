// RandomCircles.pde
//
// Test Peggy2Render library circle routine

#include <stdlib.h>
#include <math.h>

// Include libraries
#include <Display.h>
#include <Render.h>

// Create library objects
Peggy2Display Display;
Peggy2Render Render;

int8_t Radius;
int8_t Xpos;
int8_t Ypos;

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
	Radius = random(25);
	Xpos = random(25);
	Ypos = random(25);
	
	for (int8_t c=1; c<Radius; c++)
	{
		Render.Circle(Xpos, Ypos, c);
		delay(50);
		Render.Circle(Xpos, Ypos, c);
	}
}

