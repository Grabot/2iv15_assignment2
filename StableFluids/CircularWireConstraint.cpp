#include "CircularWireConstraint.h"
#include <GL/glut.h>
#include <iostream>
using namespace std;
#include "linearSolver.h"

#define PI 3.1415926535897932384626433832795

static void draw_circle(const Vec2f & vect, float radius)
{
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0, 1.0, 0.0);
	for (int i = 0; i<360; i = i + 18)
	{
		float degInRad = i*PI / 180;
		glVertex2f(vect[0] + cos(degInRad)*radius, vect[1] + sin(degInRad)*radius);
	}
	glEnd();
}

CircularWireConstraint::CircularWireConstraint(Particle *p, const Vec2f & center, const float radius) :
m_p(p), m_center(center), m_radius(radius) {}

void CircularWireConstraint::draw()
{
	draw_circle(m_center, m_radius);
}

// return the c: C(x, y) = (x - xc)^2 + (y - yc)^2 - r^2
float CircularWireConstraint::getC(){
	Vec2f posdif = (m_p->m_Position - m_center);
	float C = (posdif[0] * posdif[0] + posdif[1] * posdif[1] - m_radius*m_radius);
	return C;
}

//from the slides C = 1/2(x.x-1) =  (1/2)x.(1/2)x-(1/2)
//from the slides CDot = (1/2)xDot.(1/2)x + (1/2)x.(1/2)xDot
//our C = (x - xc)^2 + (y - yc)^2 - r^2
//return the cDot: (dx/dy)C(x, y) = ((x - xc)^2 + (y - yc)^2 - r^2)dx
//CDot = 2*(x-xc)*vx + 2*(y-yc)*vy - 0
//CDot = 2*((x-xc)*vx + (y-yc)*vy)
float CircularWireConstraint::getCDot(){
	Vec2f posdif = (m_p->m_Position - m_center);
	Vec2f veldif = (m_p->m_Velocity);
	float CDot = vecDotNew(2.0f * posdif, 2.0f * veldif);
	return CDot;
}

vector<Vec2f> CircularWireConstraint::getJacobian(){
	//a vector, only 1 now, might implement multiple particles
	vector<Vec2f> J;
	Vec2f posdif = (m_p->m_Position - m_center);
	Vec2f veldif = (m_p->m_Velocity);
	J.push_back(Vec2f(2 * posdif[0], 2 * posdif[1]));
	return J;
}

//return Jdot, if there are more use same order as particle
vector<Vec2f> CircularWireConstraint::getJacobianDot(){
	vector<Vec2f> JDot;
	Vec2f veldif = (m_p->m_Velocity);
	JDot.push_back(Vec2f(2 * veldif[0], 2 * veldif[1]));
	return JDot;
}

vector<Particle*> CircularWireConstraint::getParticles(){
	//only 1 here, but might implement ability for more
	vector<Particle*> result;
	result.push_back(m_p);
	return result;
}