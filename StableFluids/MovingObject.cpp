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
	: m_Pos(pos), objectPos(pos), m_Size(size)
{
}

void MovingObject::draw()
{
	glBegin(GL_POLYGON);
	glColor3f(0.0, 0.0, 1.0);
	glVertex2f(objectX1, objectY1);
	glVertex2f(objectX2, objectY2);
	glVertex2f(objectX3, objectY3);
	glVertex2f(objectX4, objectY4);
	glEnd();

}

float MovingObject::returnXLeft()
{
	if (objectX1 <= objectX2 && objectX1 <= objectX3 && objectX1 <= objectX4)
	{
		return objectX1;
	}

	if (objectX2 <= objectX1 && objectX2 <= objectX3 && objectX2 <= objectX4)
	{
		return objectX2;
	}

	if (objectX3 <= objectX2 && objectX3 <= objectX1 && objectX3 <= objectX4)
	{
		return objectX3;
	}

	if (objectX4 <= objectX2 && objectX4 <= objectX3 && objectX4 <= objectX1)
	{
		return objectX4;
	}

	return 0.5f;
}

float MovingObject::returnXRight()
{
	if (objectX1 >= objectX2 && objectX1 >= objectX3 && objectX1 >= objectX4)
	{
		return objectX1;
	}

	if (objectX2 >= objectX1 && objectX2 >= objectX3 && objectX2 >= objectX4)
	{
		return objectX2;
	}

	if (objectX3 >= objectX2 && objectX3 >= objectX1 && objectX3 >= objectX4)
	{
		return objectX3;
	}

	if (objectX4 >= objectX2 && objectX4 >= objectX3 && objectX4 >= objectX1)
	{
		return objectX4;
	}

	return 0.5f;
}

float MovingObject::returnYTop()
{
	if (objectY1 >= objectY2 && objectY1 >= objectY3 && objectY1 >= objectY4)
	{
		return objectY1;
	}

	if (objectY2 >= objectY1 && objectY2 >= objectY3 && objectY2 >= objectY4)
	{
		return objectY2;
	}

	if (objectY3 >= objectY2 && objectY3 >= objectY1 && objectY3 >= objectY4)
	{
		return objectY3;
	}

	if (objectY4 >= objectY2 && objectY4 >= objectY3 && objectY4 >= objectY1)
	{
		return objectY4;
	}

	return 0.5f;
}

float MovingObject::returnYBottom()
{
	if (objectY1 <= objectY2 && objectY1 <= objectY3 && objectY1 <= objectY4)
	{
		return objectY1;
	}

	if (objectY2 <= objectY1 && objectY2 <= objectY3 && objectY2 <= objectY4)
	{
		return objectY2;
	}

	if (objectY3 <= objectY2 && objectY3 <= objectY1 && objectY3 <= objectY4)
	{
		return objectY3;
	}

	if (objectY4 <= objectY2 && objectY4 <= objectY3 && objectY4 <= objectY1)
	{
		return objectY4;
	}

	return 0.5f;
}

void MovingObject::MoveStep( float xSpeed, float ySpeed, float rotate )
{
	m_xSpeed = xSpeed;
	m_ySpeed = ySpeed;

	m_Pos[0] = m_Pos[0] + m_xSpeed;
	m_Pos[1] = m_Pos[1] + m_ySpeed;

	rotation += rotate;

	objectX1 = ((m_Pos[0] - (m_Size / 2) - m_Pos[0])*cos(rotation*PI / 180) - (m_Pos[1] - (m_Size / 2) - m_Pos[1])*sin(rotation*PI / 180) + m_Pos[0]);
	objectY1 = ((m_Pos[0] - (m_Size / 2) - m_Pos[0])*sin(rotation*PI / 180) + (m_Pos[1] - (m_Size / 2) - m_Pos[1])*cos(rotation*PI / 180) + m_Pos[1]);
	objectX2 = ((m_Pos[0] - (m_Size / 2) - m_Pos[0])*cos(rotation*PI / 180) - (m_Pos[1] + (m_Size / 2) - m_Pos[1])*sin(rotation*PI / 180) + m_Pos[0]);
	objectY2 = ((m_Pos[0] - (m_Size / 2) - m_Pos[0])*sin(rotation*PI / 180) + (m_Pos[1] + (m_Size / 2) - m_Pos[1])*cos(rotation*PI / 180) + m_Pos[1]);
	objectX3 = ((m_Pos[0] + (m_Size / 2) - m_Pos[0])*cos(rotation*PI / 180) - (m_Pos[1] + (m_Size / 2) - m_Pos[1])*sin(rotation*PI / 180) + m_Pos[0]);
	objectY3 = ((m_Pos[0] + (m_Size / 2) - m_Pos[0])*sin(rotation*PI / 180) + (m_Pos[1] + (m_Size / 2) - m_Pos[1])*cos(rotation*PI / 180) + m_Pos[1]);
	objectX4 = ((m_Pos[0] + (m_Size / 2) - m_Pos[0])*cos(rotation*PI / 180) - (m_Pos[1] - (m_Size / 2) - m_Pos[1])*sin(rotation*PI / 180) + m_Pos[0]);
	objectY4 = ((m_Pos[0] + (m_Size / 2) - m_Pos[0])*sin(rotation*PI / 180) + (m_Pos[1] - (m_Size / 2) - m_Pos[1])*cos(rotation*PI / 180) + m_Pos[1]);

}

//ref: http ://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html 
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