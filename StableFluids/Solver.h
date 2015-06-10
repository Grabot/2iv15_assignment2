#pragma once
#include <gfx/vec2.h>

class Solver
{
public:
	Solver(int a_NumCells, float a_Viscosity, float a_Dt);

	void velStep();
	void AddField(float x[], float s[] );

	int m_NumCells;
	Vec2f *m_Field;
	float m_Viscosity;
	float m_Dt;
};