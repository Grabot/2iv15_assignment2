#include <iostream>
using namespace std;
#include "MovingObject.h"
#include <GL/glut.h>

MovingObject::MovingObject(Vec2f pos, float size) : m_Pos(pos), m_Size(size)
{

}

void MovingObject::draw()
{
	glBegin(GL_POLYGON);
	glColor3f(0.0, 1.0, 0.0);
	glVertex2f((m_Pos[0] - (m_Size / 2)), (m_Pos[1] - (m_Size / 2)));
	glVertex2f((m_Pos[0] - (m_Size / 2)), (m_Pos[1] + (m_Size / 2)));
	glVertex2f((m_Pos[0] + (m_Size / 2)), (m_Pos[1] + (m_Size / 2)));
	glVertex2f((m_Pos[0] + (m_Size / 2)), (m_Pos[1] - (m_Size / 2)));
	glEnd();
}

void MovingObject::MoveStep()
{

}