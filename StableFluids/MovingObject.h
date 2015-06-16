#pragma once
#include <gfx/vec2.h>

class MovingObject
{
public:
	MovingObject(Vec2f pos, float size, float xSpeed, float ySpeed);
	void draw();
	void MoveStep();
	bool collisionObject(int x, int y);
	
	Vec2f m_Pos;
	float m_Size;
	float m_x1;
	float m_x2;
	float m_y1;
	float m_y2;
	float m_xSpeed;
	float m_ySpeed;
};