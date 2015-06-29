#include <iostream>
using namespace std;
#include "MovingObject.h"
#include <GL/glut.h>
#include <math.h>       /* floor */
#include "Solver.h"

#define PI 3.14159265
std::vector<float> vertx;
std::vector<float> verty;

MovingObject::MovingObject(Vec2f pos, float size, float xSpeed, float ySpeed, float rotate)
	: m_Pos(pos), objectPos(pos), m_Size(size), m_xSpeed(xSpeed), m_ySpeed(ySpeed), m_rotate(rotate), rotation(30),
	m_x1(pos[0] - (size / 2)), m_x2(pos[0] - (size / 2)), m_x3(pos[0] + (size / 2)), m_x4(pos[0] + (size / 2)),
	m_y1(pos[1] - (size / 2)), m_y2(pos[1] + (size / 2)), m_y3(pos[1] + (size / 2)), m_y4(pos[1] - (size / 2)),
	objectX1(pos[0] - (size / 2)), objectX2(pos[0] - (size / 2)), objectX3(pos[0] + (size / 2)), objectX4(pos[0] + (size / 2)),
	objectY1(pos[1] - (size / 2)), objectY2(pos[1] + (size / 2)), objectY3(pos[1] + (size / 2)), objectY4(pos[1] - (size / 2))
{
}

void MovingObject::draw()
{
	glTranslatef(m_Pos[0], m_Pos[1], 0.0);
	glRotatef(rotation, 0, 0, 1);
	glTranslatef(-m_Pos[0], -m_Pos[1], 0.0);

	glBegin(GL_POLYGON);
	glColor3f(0.0, 0.0, 1.0);
	glVertex2f(m_x1, m_y1);
	glVertex2f(m_x2, m_y2);
	glVertex2f(m_x3, m_y3);
	glVertex2f(m_x4, m_y4);
	glEnd();

	rotation += m_rotate;
}

void MovingObject::MoveStep()
{
	if (m_x1 < 0)
	{
		m_xSpeed = m_xSpeed*-1;
	}
	if (m_x3 > 1)
	{
		m_xSpeed = m_xSpeed*-1;
	}
	if (m_y3 > 1)
	{
		m_ySpeed = m_ySpeed*-1;
	}
	if (m_y1 < 0)
	{
		m_ySpeed = m_ySpeed*-1;
	}

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

float MovingObject::GetVelocityX(int x, int y, float u[])
{
	float realObjectX1 = m_x1 * 64;
	float realObjectX2 = m_x3 * 64;

	if (x == floor(realObjectX2))
	{
		return -u[IX_DIM(x + 1, y)] + ((m_xSpeed * 64) > 0 ? (m_xSpeed * 64) : 0);
	}

	if (x == ceil(realObjectX1))
	{
		return -u[IX_DIM(x - 1, y)] + ((m_xSpeed * 64) > 0 ? 0 : (m_xSpeed * 64));
	}

	return 0;
}

float MovingObject::GetVelocityY(int x, int y, float v[])
{
	float realObjectY1 = m_y1 * 64;
	float realObjectY2 = m_y2 * 64;

	if (y == floor(realObjectY2))
	{
		return -v[IX_DIM(x, y + 1)] + ((m_ySpeed * 64) > 0 ? (m_ySpeed * 64) : 0);
	}

	if (y == ceil(realObjectY1))
	{
		return -v[IX_DIM(x, y - 1)] + ((m_ySpeed * 64) > 0 ? 0 : (m_ySpeed * 64));
	}

	return 0;
}

float MovingObject::GetVelocityDensity(int x, int y, float d[])
{
	float realObjectX1 = m_x1 * 64;
	float realObjectX2 = m_x3 * 64;
	float realObjectY1 = m_y1 * 64;
	float realObjectY2 = m_y2 * 64;

	if (x == floor(realObjectX2))
	{
		return d[IX_DIM(x + 1, y)];
	}

	if (x == ceil(realObjectX1))
	{
		return d[IX_DIM(x - 1, y)];
	}

	if (y == floor(realObjectY2))
	{
		return d[IX_DIM(x, y + 1)];
	}

	if (y == ceil(realObjectY1))
	{
		return d[IX_DIM(x, y - 1)];
	}

	return 0;
}