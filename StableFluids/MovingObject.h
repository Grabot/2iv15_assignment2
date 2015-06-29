#pragma once
#include <gfx/vec2.h>

class MovingObject
{
public:
	MovingObject(Vec2f pos, float size, float xSpeed, float ySpeed, float rotate);
	void draw();
	void MoveStep();
	bool pnpoly(int nvert, float testx, float testy);
	float GetVelocityX(int x, int y, float u[]);
	float GetVelocityY(int x, int y, float v[]);
	float GetVelocityDensity(int x, int y, float d[]);

	Vec2f m_Pos;
	Vec2f objectPos;
	float m_Size;
	float m_rotate;
	float m_x1;
	float m_x2;
	float m_x3;
	float m_x4;
	float m_y1;
	float m_y2;
	float m_y3;
	float m_y4;
	float objectX1;
	float objectX2;
	float objectX3;
	float objectX4;
	float objectY1;
	float objectY2;
	float objectY3;
	float objectY4;
	float m_xSpeed;
	float m_ySpeed;
	float rotation;
};