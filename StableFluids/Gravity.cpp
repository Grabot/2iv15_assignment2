#include "Gravity.h"
#include <GL/glut.h>

Gravity::Gravity(Particle * p1, const Vec2f & grav) :
m_p1(p1), g(grav){}

void Gravity::draw()
{
	glBegin(GL_LINES);
	glColor3f(0, 0, 1);
	glVertex2f(m_p1->m_Position[0], m_p1->m_Position[1]);
	glColor3f(0, 0, 1);
	glVertex2f(m_p1->m_Position[0] + (g[0] * m_p1->m_mass), m_p1->m_Position[1] + (g[1] * m_p1->m_mass));
	glEnd();
}

void Gravity::apply()
{
	//F = m * a
	//a = F/m
	m_p1->m_Force += g * m_p1->m_mass;
	//m_p1->m_Velocity += g*0.1;
}