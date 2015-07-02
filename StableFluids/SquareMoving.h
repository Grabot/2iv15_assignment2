#pragma once
#include <gfx/vec2.h>

class SquareMoving
{
public:
	SquareMoving(Vec2f pos, float size);
	void draw();

	Vec2f m_Pos;
	float m_Size;

};