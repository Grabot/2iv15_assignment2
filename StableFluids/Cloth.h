#pragma once
#include <gfx/vec2.h>
#include "Particle.h"
#include <list>
#include <vector>

class Cloth
{
public:
	Cloth(vector<Particle*> pVector);
	void draw(bool show);

	vector<Particle*> m_Vectors;
};