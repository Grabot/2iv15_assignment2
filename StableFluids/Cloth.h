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
	bool pnpoly(int nvert, float testx, float testy);
	float GetVelXRight(int x, int y, float u[]);
	float GetVelXLeft(int x, int y, float u[]);
	float GetVelYUp(int x, int y, float v[]);
	float GetVelYDown(int x, int y, float v[]);
	float GetVelocityDensityXRight(int x, int y, float d[]);
	float GetVelocityDensityXLeft(int x, int y, float d[]);
	float GetVelocityDensityYUp(int x, int y, float d[]);
	float GetVelocityDensityYDown(int x, int y, float d[]);

	vector<Particle*> m_Vectors;
	vector<Particle*> m_Corners;
};