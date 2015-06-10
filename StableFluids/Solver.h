#pragma once
#include <gfx/vec2.h>

class Solver
{
public:
	Solver(int a_NumCells, float a_Viscosity, float a_Dt);

	void velStep(float u[], float v[], float u0[], float v0[], float object[] );
	void densStep(float x[], float x0[], float u[], float v[], float object[] );
	void AddField(float y[], float s[] );
	void Diffuse(int b, float x[], float x0[], float object[] );
	void lin_solve(int b, float x[], float x0[], float object[], float a, float c);
	void set_bnd(int b, float x[], float object[] );
	void advect(int b, float d[], float d0[], float u[], float v[], float object[] );
	void project(float u[], float v[], float p[], float div[], float object[] );

	int m_NumCells;
	Vec2f *m_Field;
	float m_Viscosity;
	float m_Dt;

};