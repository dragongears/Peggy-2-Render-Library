// SpriteTest.pde
//
// Test Peggy2Render library sprite routine

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
int8_t Frame;

uint8_t TestSprite[] = {
	2, 8,
	B00100001,B00000000,
	B10010010,B01000000,
	B10111111,B01000000,
	B11101101,B11000000,
	B11111111,B11000000,
	B01111111,B10000000,
	B00100001,B00000000,
	B01000000,B10000000,

	B00100001,B00000000,
	B00010010,B00000000,
	B00111111,B00000000,
	B01101101,B10000000,
	B11111111,B11000000,
	B11111111,B11000000,
	B10100001,B01000000,
	B10010010,B01000000
	};	

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
	Xpos = 0;
	Ypos = 2;
	Frame = random(2);
	
	while (Ypos < 16)
	{
		while (Xpos < 15)
		{
			Render.Sprite(Xpos, Ypos, &TestSprite[0], (Xpos%2));
			delay(500);
			Render.Sprite(Xpos, Ypos, &TestSprite[0], (Xpos%2));
			Xpos++;
		}
		Ypos += 2;
		while (Xpos > 0)
		{
			Render.Sprite(Xpos, Ypos, &TestSprite[0], (Xpos%2));
			delay(500);
			Render.Sprite(Xpos, Ypos, &TestSprite[0], (Xpos%2));
			Xpos--;
		}
		Ypos += 2;
	}
}

