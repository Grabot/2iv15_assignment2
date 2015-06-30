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
	if (m_Vectors.size() == 4)
	{
		m_Corners = m_Vectors;
	}
	else if (m_Vectors.size() == 9)
	{
		m_Corners.push_back(m_Vectors[0]);
		m_Corners.push_back(m_Vectors[2]);
		m_Corners.push_back(m_Vectors[8]);
		m_Corners.push_back(m_Vectors[6]);
	}
	else if (m_Vectors.size() == 16)
	{
		m_Corners.push_back(m_Vectors[0]);
		m_Corners.push_back(m_Vectors[3]);
		m_Corners.push_back(m_Vectors[15]);
		m_Corners.push_back(m_Vectors[12]);
	}
	else
	{
		cout << "Wrong amount of particles" << endl;
	}
}

void Cloth::draw(bool show)
{
	glBegin(GL_POLYGON);
	glColor3f(0.0, 0.0, 1.0);
	if (show)
	{
		//glVertex2f(m_Vectors[0]->m_Position[0], m_Vectors[0]->m_Position[1]);
		//glVertex2f(m_Vectors[1]->m_Position[0], m_Vectors[1]->m_Position[1]);
		//glVertex2f(m_Vectors[2]->m_Position[0], m_Vectors[2]->m_Position[1]);
		//glVertex2f(m_Vectors[3]->m_Position[0], m_Vectors[3]->m_Position[1]);
	}
	glEnd();

}

//ref: http ://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html 
bool Cloth::pnpoly(int nvert, float testx, float testy)
{
	int i, j, c = 0;
	for (i = 0, j = nvert - 1; i < nvert; j = i++) {
		if ((((m_Corners[i]->m_Position[1] * 64)>testy) != ((m_Corners[j]->m_Position[1] * 64)>testy)) &&
			(testx < ((m_Corners[j]->m_Position[0] * 64) - (m_Corners[i]->m_Position[0] * 64)) *
			(testy - (m_Corners[i]->m_Position[1] * 64)) / ((m_Corners[j]->m_Position[1] * 64) -
			(m_Corners[i]->m_Position[1] * 64)) + (m_Corners[i]->m_Position[0] * 64)))
			c = !c;
	}
	return c;
}

//we do make the assumption all particles move at a similar speed
float Cloth::GetVelXRight(int x, int y, float u[])
{
	return -u[IX_DIM(x + 1, y)] + ((m_Corners[0]->m_Velocity[0] * 5) > 0 ? (m_Corners[0]->m_Velocity[0] * 5) : 0);
}

float Cloth::GetVelXLeft(int x, int y, float u[])
{
	return -u[IX_DIM(x - 1, y)] + ((m_Corners[2]->m_Velocity[0] * 5) > 0 ? 0 : (m_Corners[2]->m_Velocity[0] * 5));
}

float Cloth::GetVelYUp(int x, int y, float v[])
{
	return -v[IX_DIM(x, y - 1)] + ((m_Corners[0]->m_Velocity[1] * 5) > 0 ? 0 : (m_Corners[0]->m_Velocity[1] * 5));
}

float Cloth::GetVelYDown(int x, int y, float v[])
{
	return -v[IX_DIM(x, y + 1)] + ((m_Corners[2]->m_Velocity[1] * 5) > 0 ? (m_Corners[2]->m_Velocity[1] * 5) : 0);
}

float Cloth::GetVelocityDensityXRight(int x, int y, float d[])
{
	return d[IX_DIM(x + 1, y)];
}

float Cloth::GetVelocityDensityXLeft(int x, int y, float d[])
{
	return d[IX_DIM(x - 1, y)];
}

float Cloth::GetVelocityDensityYUp(int x, int y, float d[])
{
	return d[IX_DIM(x, y - 1)];
}

float Cloth::GetVelocityDensityYDown(int x, int y, float d[])
{
	return d[IX_DIM(x, y + 1)];
}