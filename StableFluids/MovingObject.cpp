#include <iostream>
using namespace std;
#include "MovingObject.h"
#include <GL/glut.h>

MovingObject::MovingObject(Vec2f pos, float size, float xSpeed, float ySpeed) 
	: m_Pos(pos), m_Size(size), m_xSpeed(xSpeed), m_ySpeed(ySpeed), 
	m_x1(pos[0] - (size / 2)), m_x2(pos[0] + (size / 2)), m_y1(pos[1] - (size / 2)), m_y2(pos[1] + (size / 2))
{

}

void MovingObject::draw()
{
	glBegin(GL_POLYGON);
	glColor3f(0.0, 1.0, 0.0);
	glVertex2f(m_x1, m_y1);
	glVertex2f(m_x1, m_y2);
	glVertex2f(m_x2, m_y2);
	glVertex2f(m_x2, m_y1);
	glEnd();
}

void MovingObject::MoveStep()
{
	if (m_x1 < 0 )
	{
		m_xSpeed = m_xSpeed*-1;
	}
	if (m_x2 > 1)
	{
		m_xSpeed = m_xSpeed*-1;
	}
	if (m_y2 > 1)
	{
		m_ySpeed = m_ySpeed*-1;
	}
	if (m_y1 < 0)
	{
		m_ySpeed = m_ySpeed*-1;
	}

	m_x1 += m_xSpeed;
	m_y1 += m_ySpeed;
	m_x2 += m_xSpeed;
	m_y2 += m_ySpeed;
}