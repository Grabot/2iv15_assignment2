#include <iostream>
using namespace std;
#include "Cloth.h"
#include <GL/glut.h>
#include "Particle.h"
#include <math.h>       /* floor */
#include "Solver.h"

Cloth::Cloth(vector<Particle*> pVector)
	: m_Vectors(pVector)
{
}

void Cloth::draw(bool show)
{
	glBegin(GL_POLYGON);
	glColor3f(0.0, 0.0, 1.0);
	if (show)
	{
		glVertex2f(m_Vectors[0]->m_Position[0], m_Vectors[0]->m_Position[1]);
		glVertex2f(m_Vectors[1]->m_Position[0], m_Vectors[1]->m_Position[1]);
		glVertex2f(m_Vectors[2]->m_Position[0], m_Vectors[2]->m_Position[1]);
		glVertex2f(m_Vectors[3]->m_Position[0], m_Vectors[3]->m_Position[1]);
	}
	glEnd();

}