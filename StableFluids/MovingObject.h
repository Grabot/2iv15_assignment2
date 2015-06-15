#pragma once
#include <gfx/vec2.h>

class MovingObject
{
public:
	MovingObject(Vec2f pos, float size);
	void draw();
	void MoveStep();
	
	Vec2f m_Pos;
	float m_Size;
};