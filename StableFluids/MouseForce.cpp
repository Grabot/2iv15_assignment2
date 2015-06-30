#include "MouseForce.h"
#include <GL/glut.h>
using namespace std;

bool m_setForce = false;

MouseForce::MouseForce(Particle *p, Vec2f & Mouse, double ks, double kd) :
m_p(p), m_mouse(Mouse), m_ks(ks), m_kd(kd) {}

void MouseForce::draw()
{

	glBegin(GL_LINES);
	glColor3f(1, 1, 0);
	glVertex2f(m_p->m_Position[0], m_p->m_Position[1]);
	glColor3f(1, 1, 0);
	glVertex2f(m_mouse[0], m_mouse[1]);
	glEnd();

}

void MouseForce::setForce(bool applyForce)
{
	m_setForce = applyForce;
}

void MouseForce::getMouse(const Vec2f & Mouse)
{
	m_mouse = Mouse;
}

void MouseForce::apply()
{
	if (m_setForce)
	{
		Vec2f posdif = (m_p->m_Position - m_mouse);
		Vec2f speeddif = (m_p->m_Velocity);
		float posLength = (sqrt(posdif[0] * posdif[0] + posdif[1] * posdif[1]));
		float dotProduct = (speeddif[0] * posdif[0] + speeddif[1] * posdif[1]);

		Vec2f force_p1 = ((posdif / posLength) * ((m_ks * (posLength)) + (m_kd * (dotProduct / posLength))));
		Vec2f force_p2 = -force_p1;

		m_p->m_Velocity -= force_p1;
	}
}