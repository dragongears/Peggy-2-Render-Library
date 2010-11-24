/*
  Render.cpp - Peggy 2.0 display object rendering library for Arduino
  Version 1.0 - 07/26/2008
  Copyright (c) 2008 Arthur J. Dahm III.  All right reserved.
  Email: art@mindlessdiversions.com
  Web: mindlessdiversions.com/peggy2

    This library is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

/******************************************************************************
 * Includes
 ******************************************************************************/

extern "C" {
#include <inttypes.h>
#include <binary.h>
#include <avr/interrupt.h>
#include <avr/io.h>
}

#include "Render.h"


/******************************************************************************
 * Definitions
 ******************************************************************************/



/******************************************************************************
 * Initialize Class Variables
 ******************************************************************************/

uint8_t *Peggy2Render::framebufferptr;
render_format_t Peggy2Render::renderformat;

/******************************************************************************
 * Constructors
 ******************************************************************************/

// Pass in a pointer to a 100 byte frame buffer
Peggy2Render::Peggy2Render(uint8_t *fbp)
{
	framebufferptr = fbp;
	renderformat = RENDER_DRAW;
}

Peggy2Render::Peggy2Render(void)
{
	Peggy2Render(0);
}

/******************************************************************************
 * User API
 ******************************************************************************/

// Change frame buffer used for drawing
void Peggy2Render::DrawBuffer(uint8_t *fbp)
{
	framebufferptr = fbp;
}

// Get frame buffer used for drawing
uint8_t *Peggy2Render::DrawBuffer(void)
{
	return framebufferptr;
}

// Set how objects are rendered
void Peggy2Render::RenderFormat(render_format_t rf)
{
	renderformat = rf;
}

// Get the current render format
render_format_t Peggy2Render::RenderFormat(void)
{
	return renderformat;
}

// Clear frame buffer
void Peggy2Render::Clear(void)
{
	Fill(0);
}

// Fill frame buffer with byte value
void Peggy2Render::Fill(uint8_t b)
{
	uint8_t *fbp = framebufferptr;
	for (uint8_t x = 0; x < 100; x++)
		*fbp++ = b;
}

// Draw a sprite
//  Sprites are an array of bytes
//  The first byte is the width of the sprite (in bytes)
//  The second byte is the height of each frame of the sprite
//  The rest of the array is the actual sprite data
//  Successive frames are stored directly after the preceding frame
void Peggy2Render::Sprite(int8_t xPos, int8_t yPos, uint8_t *sprite, uint8_t frame)
{
	uint8_t shift = 8-xPos%8;
	uint8_t xoffset = xPos>>3;
	uint8_t width = *sprite++;
	uint8_t height = *sprite++;
	
	sprite += (frame*width*height);
	
	for (uint8_t y=0; y<height; y++)
	{
		uint8_t *linestart = &framebufferptr[((yPos+y)<<2)];
		uint8_t *spritestart = linestart+xoffset;

		if ((linestart >= framebufferptr) && (linestart < framebufferptr+100))
		{
			for (int8_t x=0; x<width; x++)
			{
				uint16_t wideseg = uint16_t(*sprite++)<<shift;
			
				if ((spritestart >= linestart) && (spritestart < linestart+4))
				{
					if (renderformat == RENDER_DRAW)
						*spritestart |= wideseg>>8;
					else if (renderformat == RENDER_ERASE)
						*spritestart &= ~(wideseg>>8);
					else if (renderformat == RENDER_XOR)
						*spritestart ^= wideseg>>8;
				}
				spritestart++;

				if ((spritestart >= linestart) && (spritestart < linestart+4))
				{
					if (renderformat == RENDER_DRAW)
						*spritestart |= wideseg;
					else if (renderformat == RENDER_ERASE)
						*spritestart &= ~(wideseg);
					else if (renderformat == RENDER_XOR)
						*spritestart ^= wideseg;
				}

			}
		}
	}
}

// Draw a point
void Peggy2Render::Point(int8_t xPos, int8_t yPos)
{
	if ((xPos >= 0 && xPos <= 24) && (yPos >= 0 && yPos <= 24))
	{
		uint8_t *fb = &framebufferptr[(yPos*4)+(xPos/8)];
		uint8_t b = 0x80 >> xPos%8;
	
		if (renderformat == RENDER_DRAW)
			*fb |= b;
		else if (renderformat == RENDER_ERASE)
			*fb &= (~b);
		else if (renderformat == RENDER_XOR)
			*fb ^= b;
	}
}

// Find out if a point in the frame buffer is on
uint8_t Peggy2Render::TestPoint(int8_t xPos, int8_t yPos)
{
	uint8_t *fb = &framebufferptr[(yPos*4)+(xPos/8)];
	uint8_t b = 0x80 >> xPos%8;
	
	return 	(*fb & (b))?(1):(0);
}

// Extremely Fast Line Algorithm Var E (Addition Fixed Point PreCalc ModeX)
// Copyright 2001-2, By Po-Han Lin
//
// Freely useable in non-commercial applications as long as credits 
// to Po-Han Lin and link to http://www.edepot.com is provided in 
// source code and can been seen in compiled executable.  
// Commercial applications please inquire about licensing the algorithms.
//
// Lastest version at http://www.edepot.com/phl.html
// Note: This version is for small displays like on cell phones.
// with 256x256 max resolution.  For displays up to 65536x65536
// please visit http://www.edepot.com/linee.html
//
// THE EXTREMELY FAST LINE ALGORITHM Variation E (Addition Fixed Point PreCalc Small Display)
// Small Display (256x256) resolution.

void Peggy2Render::Line(int8_t x, int8_t y, int8_t x2, int8_t y2)
{
	bool yLonger = false;
	int16_t shortLen = y2 - y;
	int16_t longLen = x2 - x;

	if (abs(shortLen) > abs(longLen))
	{
		int16_t swap = shortLen;
		shortLen = longLen;
		longLen = swap;				
		yLonger = true;
	}
	int16_t decInc;
	if (longLen==0)
		decInc = 0;
	else
		decInc = (shortLen << 8) / longLen;

	if (yLonger)
	{
		if (longLen > 0)
		{
			longLen += y;
			for (int16_t j=0x80+(x<<8); y<=longLen; ++y)
			{
				Point(j >> 8, y);	
				j += decInc;
			}
			return;
		}
		longLen += y;
		for (int16_t j=0x80+(x<<8); y>=longLen; --y)
		{
			Point(j >> 8, y);	
			j -= decInc;
		}
		return;	
	}

	if (longLen > 0)
	{
		longLen += x;
		for (int16_t j=0x80+(y<<8); x<=longLen; ++x)
		{
			Point(x, j >> 8);
			j += decInc;
		}
		return;
	}
	longLen += x;
	for (int16_t j=0x80+(y<<8); x>=longLen; --x)
	{
		Point(x, j >> 8);
		j -= decInc;
	}
}


// Helper funcion for circle function below
void Peggy2Render::CirclePoints(int8_t cx, int8_t cy, int8_t x, int8_t y)
{
	if (x == 0)
	{
		Point(cx, cy + y);
		Point(cx, cy - y);
		Point(cx + y, cy);
		Point(cx - y, cy);
	}
	else if (x == y)
	{
		Point(cx + x, cy + y);
		Point(cx - x, cy + y);
		Point(cx + x, cy - y);
		Point(cx - x, cy - y);
	}
	else if (x < y)
	{
		Point(cx + x, cy + y);
		Point(cx - x, cy + y);
		Point(cx + x, cy - y);
		Point(cx - x, cy - y);
		Point(cx + y, cy + x);
		Point(cx - y, cy + x);
		Point(cx + y, cy - x);
		Point(cx - y, cy - x);
	}
}

// Draw a circle
void Peggy2Render::Circle(int8_t xCenter, int8_t yCenter, int8_t radius)
{
	int8_t x = 0;
	int8_t y = radius;
	int8_t p = (5 - radius*4)/4;

	CirclePoints(xCenter, yCenter, x, y);
	while (x < y)
	{
		x++;
		if (p < 0)
		{
			p += 2*x+1;
		}
		else
		{
			y--;
			p += 2*(x-y)+1;
		}
		CirclePoints(xCenter, yCenter, x, y);
	}
}

