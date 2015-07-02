#include "SquareMoving.h"
#include <GL/glut.h>
#include "Solver.h"

SquareMoving::SquareMoving(Vec2f pos, float size)
	: m_Pos(pos), m_Size(size)
{
}


void SquareMoving::draw()
{
	glBegin(GL_POLYGON);
	glColor3f(0.0, 0.0, 1.0);
	glVertex2f(m_Pos[0] - (m_Size / 2), m_Pos[1] - (m_Size / 2));
	glVertex2f(m_Pos[0] - (m_Size / 2), m_Pos[1] + (m_Size / 2));
	glVertex2f(m_Pos[0] + (m_Size / 2), m_Pos[1] + (m_Size / 2));
	glVertex2f(m_Pos[0] + (m_Size / 2), m_Pos[1] - (m_Size / 2));
	glEnd();

}