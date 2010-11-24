/*
  Render.h - Peggy 2.0 display object rendering library for Arduino
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

#include <Display.h>

#ifndef Render_h
#define Render_h

#include <inttypes.h>
#include <wiring.h>

enum render_format_t {RENDER_DRAW, RENDER_ERASE, RENDER_XOR};

class Peggy2Render
{
	public:
		// Constructor
		Peggy2Render(void);
		Peggy2Render(uint8_t *fbp);

		void DrawBuffer(uint8_t *fbp);
		uint8_t *DrawBuffer(void);

		void RenderFormat(render_format_t rf);
		render_format_t RenderFormat(void);

		void Clear(void);
		void Fill(uint8_t b);
		
		void Sprite(int8_t xPos, int8_t yPos, uint8_t *sprite, uint8_t frame);
		void Point(int8_t x, int8_t y);
		uint8_t TestPoint(int8_t xPos, int8_t yPos);
		void Line(int8_t x, int8_t y, int8_t x2, int8_t y2);
		void Circle(int8_t xCenter, int8_t yCenter, int8_t radius);

	private:
		static uint8_t *framebufferptr;				// Pointer to draw frame buffer
		static render_format_t renderformat;
		void CirclePoints(int8_t cx, int8_t cy, int8_t x, int8_t y);
};

#endif
