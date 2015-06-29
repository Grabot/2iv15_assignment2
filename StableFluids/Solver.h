#pragma once
#include <gfx/vec2.h>
#include "MovingObject.h"
#include <algorithm>
#include <list>
#include <vector>

class Solver
{
public:
	Solver(int a_NumCells, float a_Viscosity, float a_Dt);

	void velStep(float u[], float v[], float u0[], float v0[], float object[], std::vector<MovingObject*> movings );
	void densStep(float x[], float x0[], float u[], float v[], float object[], std::vector<MovingObject*> movings );
	void AddField(float y[], float s[] );
	void Diffuse(int b, float x[], float x0[], float object[], std::vector<MovingObject*> movings );
	void lin_solve(int b, float x[], float x0[], float object[], std::vector<MovingObject*> movings, float a, float c);
	void set_bnd(int b, float x[], float object[], std::vector<MovingObject*> movings );
	void advect(int b, float d[], float d0[], float u[], float v[], float object[], std::vector<MovingObject*> movings );
	void project(float u[], float v[], float p[], float div[], float object[], std::vector<MovingObject*> movings );
	void vorticityConfinement(float u[], float v[]);

	int m_NumCells;
	Vec2f *m_Field;
	float m_Viscosity;
	float m_Dt;
	#define IX_DIM(i,j) ((i)+(64+2)*(j))

};