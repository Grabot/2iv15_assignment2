#pragma once
#include <gfx/vec2.h>

class MovingObject
{
public:
	MovingObject(Vec2f pos, float size, float rotate);
	void draw( bool show );
	void MoveStep( float xSpeed, float ySpeed );
	bool pnpoly(int nvert, float testx, float testy);
	float GetVelXRight(int x, int y, float u[]);
	float GetVelXLeft(int x, int y, float u[]);
	float GetVelYUp(int x, int y, float v[]);
	float GetVelYDown(int x, int y, float v[]);
	float GetVelocityDensityXRight(int x, int y, float d[]);
	float GetVelocityDensityXLeft(int x, int y, float d[]);
	float GetVelocityDensityYUp(int x, int y, float d[]);
	float GetVelocityDensityYDown(int x, int y, float d[]);
	float returnXLeft();
	float returnXRight();
	float returnYTop();
	float returnYBottom();

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