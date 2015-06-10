#pragma once
#include <gfx/vec2.h>

class Solver
{
public:
	Solver(int a_NumCells, float a_Viscosity, float a_Dt);

	void velStep(float u[], float v[], float u0[], float v0[] );
	void densStep(float x[], float x0[], float u[], float v[]);
	void AddField(float y[], float s[] );

	int m_NumCells;
	Vec2f *m_Field;
	float m_Viscosity;
	float m_Dt;

private:
	void Switch(float a1[], float a2[]);

};