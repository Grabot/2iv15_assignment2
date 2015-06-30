#pragma once

#include "Particle.h"
#include "Force.h"

class MouseForce : public Force
{
public:
	MouseForce(Particle *p, Vec2f & Mouse, double ks, double kd);

	void getMouse(const Vec2f & Mouse);
	void setForce(bool applyForce);
	void draw();
	void apply();

private:

	Particle * const m_p;   // particle 1
	Vec2f m_mouse;
	float const m_ks, m_kd; // spring strength constants
};
