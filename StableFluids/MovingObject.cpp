#include <iostream>
using namespace std;
#include "MovingObject.h"
#include <GL/glut.h>
#include <math.h>       /* floor */
#include "Solver.h"

#define PI 3.14159265
std::vector<float> vertx;
std::vector<float> verty;

MovingObject::MovingObject(Vec2f pos, float size)
	: m_Pos(pos), objectPos(pos), m_Size(size), m_xSpeed(0), m_ySpeed(0), m_rotate(0), rotation(0),
	m_x1(pos[0] - (size / 2)), m_x2(pos[0] - (size / 2)), m_x3(pos[0] + (size / 2)), m_x4(pos[0] + (size / 2)),
	m_y1(pos[1] - (size / 2)), m_y2(pos[1] + (size / 2)), m_y3(pos[1] + (size / 2)), m_y4(pos[1] - (size / 2)),
	objectX1(pos[0] - (size / 2)), objectX2(pos[0] - (size / 2)), objectX3(pos[0] + (size / 2)), objectX4(pos[0] + (size / 2)),
	objectY1(pos[1] - (size / 2)), objectY2(pos[1] + (size / 2)), objectY3(pos[1] + (size / 2)), objectY4(pos[1] - (size / 2))
{
}

void MovingObject::draw( bool show )
{
	glBegin(GL_POLYGON);
	glColor3f(0.0, 0.0, 1.0);
	if (show)
	{
		glVertex2f(objectX1, objectY1);
		glVertex2f(objectX2, objectY2);
		glVertex2f(objectX3, objectY3);
		glVertex2f(objectX4, objectY4);
	}
	glEnd();

}

float MovingObject::returnXLeft()
{
	if (m_x1 <= m_x2 && m_x1 <= m_x3 && m_x1 <= m_x4)
	{
		return m_x1;
	}

	if (m_x2 <= m_x1 && m_x2 <= m_x3 && m_x2 <= m_x4)
	{
		return m_x2;
	}

	if (m_x3 <= m_x2 && m_x3 <= m_x1 && m_x3 <= m_x4)
	{
		return m_x3;
	}

	if (m_x4 <= m_x2 && m_x4 <= m_x3 && m_x4 <= m_x1)
	{
		return m_x4;
	}

	return 0.5f;
}

float MovingObject::returnXRight()
{
	if (m_x1 >= m_x2 && m_x1 >= m_x3 && m_x1 >= m_x4)
	{
		return m_x1;
	}

	if (m_x2 >= m_x1 && m_x2 >= m_x3 && m_x2 >= m_x4)
	{
		return m_x2;
	}

	if (m_x3 >= m_x2 && m_x3 >= m_x1 && m_x3 >= m_x4)
	{
		return m_x3;
	}

	if (m_x4 >= m_x2 && m_x4 >= m_x3 && m_x4 >= m_x1)
	{
		return m_x4;
	}

	return 0.5f;
}

float MovingObject::returnYTop()
{
	if (m_y1 >= m_y2 && m_y1 >= m_y3 && m_y1 >= m_y4)
	{
		return m_y1;
	}

	if (m_y2 >= m_y1 && m_y2 >= m_y3 && m_y2 >= m_y4)
	{
		return m_y2;
	}

	if (m_y3 >= m_y2 && m_y3 >= m_y1 && m_y3 >= m_y4)
	{
		return m_y3;
	}

	if (m_y4 >= m_y2 && m_y4 >= m_y3 && m_y4 >= m_y1)
	{
		return m_y4;
	}

	return 0.5f;
}

float MovingObject::returnYBottom()
{
	if (m_y1 <= m_y2 && m_y1 <= m_y3 && m_y1 <= m_y4)
	{
		return m_y1;
	}

	if (m_y2 <= m_y1 && m_y2 <= m_y3 && m_y2 <= m_y4)
	{
		return m_y2;
	}

	if (m_y3 <= m_y2 && m_y3 <= m_y1 && m_y3 <= m_y4)
	{
		return m_y3;
	}

	if (m_y4 <= m_y2 && m_y4 <= m_y3 && m_y4 <= m_y1)
	{
		return m_y4;
	}

	return 0.5f;
}

void MovingObject::MoveStep( float xSpeed, float ySpeed, float rotate )
{
	m_xSpeed = xSpeed;
	m_ySpeed = ySpeed;

	m_Pos[0] = m_Pos[0] + m_xSpeed;
	m_Pos[1] = m_Pos[1] + m_ySpeed;

	m_x1 += m_xSpeed;
	m_x2 += m_xSpeed;
	m_x3 += m_xSpeed;
	m_x4 += m_xSpeed;

	m_y1 += m_ySpeed;
	m_y2 += m_ySpeed;
	m_y3 += m_ySpeed;
	m_y4 += m_ySpeed;

	rotation += rotate;

	objectX1 = ((m_x1 - m_Pos[0])*cos(rotation*PI / 180) - (m_y1 - m_Pos[1])*sin(rotation*PI / 180) + m_Pos[0]);
	objectY1 = ((m_x1 - m_Pos[0])*sin(rotation*PI / 180) + (m_y1 - m_Pos[1])*cos(rotation*PI / 180) + m_Pos[1]);
	objectX2 = ((m_x2 - m_Pos[0])*cos(rotation*PI / 180) - (m_y2 - m_Pos[1])*sin(rotation*PI / 180) + m_Pos[0]);
	objectY2 = ((m_x2 - m_Pos[0])*sin(rotation*PI / 180) + (m_y2 - m_Pos[1])*cos(rotation*PI / 180) + m_Pos[1]);
	objectX3 = ((m_x3 - m_Pos[0])*cos(rotation*PI / 180) - (m_y3 - m_Pos[1])*sin(rotation*PI / 180) + m_Pos[0]);
	objectY3 = ((m_x3 - m_Pos[0])*sin(rotation*PI / 180) + (m_y3 - m_Pos[1])*cos(rotation*PI / 180) + m_Pos[1]);
	objectX4 = ((m_x4 - m_Pos[0])*cos(rotation*PI / 180) - (m_y4 - m_Pos[1])*sin(rotation*PI / 180) + m_Pos[0]);
	objectY4 = ((m_x4 - m_Pos[0])*sin(rotation*PI / 180) + (m_y4 - m_Pos[1])*cos(rotation*PI / 180) + m_Pos[1]);

}

bool MovingObject::pnpoly(int nvert, float testx, float testy)
{
	vertx.clear();
	verty.clear();

	vertx.push_back(objectX1 * 64);
	vertx.push_back(objectX2 * 64);
	vertx.push_back(objectX3 * 64);
	vertx.push_back(objectX4 * 64);

	verty.push_back(objectY1 * 64);
	verty.push_back(objectY2 * 64);
	verty.push_back(objectY3 * 64);
	verty.push_back(objectY4 * 64);

	int i, j, c = 0;
	for (i = 0, j = nvert - 1; i < nvert; j = i++) {
		if (((verty[i]>testy) != (verty[j]>testy)) &&
			(testx < (vertx[j] - vertx[i]) * (testy - verty[i]) / (verty[j] - verty[i]) + vertx[i]))
			c = !c;
	}
	return c;
}

float MovingObject::GetVelXRight(int x, int y, float u[])
{
	return -u[IX_DIM(x + 1, y)] + ((m_xSpeed * 64) > 0 ? (m_xSpeed * 64) : 0);
}

float MovingObject::GetVelXLeft(int x, int y, float u[])
{
	return -u[IX_DIM(x - 1, y)] + ((m_xSpeed * 64) > 0 ? 0 : (m_xSpeed * 64));
}

float MovingObject::GetVelYUp(int x, int y, float v[])
{
	return -v[IX_DIM(x, y - 1)] + ((m_ySpeed * 64) > 0 ? 0 : (m_ySpeed * 64));
}

float MovingObject::GetVelYDown(int x, int y, float v[])
{
	return -v[IX_DIM(x, y + 1)] + ((m_ySpeed * 64) > 0 ? (m_ySpeed * 64) : 0);
}

float MovingObject::GetVelocityDensityXRight(int x, int y, float d[])
{
	return d[IX_DIM(x + 1, y)];
}

float MovingObject::GetVelocityDensityXLeft(int x, int y, float d[])
{
	return d[IX_DIM(x - 1, y)];
}

float MovingObject::GetVelocityDensityYUp(int x, int y, float d[])
{
	return d[IX_DIM(x, y - 1)];
}

float MovingObject::GetVelocityDensityYDown(int x, int y, float d[])
{
	return d[IX_DIM(x, y + 1)];
}