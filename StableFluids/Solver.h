#pragma once
#include <gfx/vec2.h>
#include "MovingObject.h"

class Solver
{
public:
	Solver(int a_NumCells, float a_Viscosity, float a_Dt);

	void velStep(float u[], float v[], float u0[], float v0[], float object[], MovingObject *movingObject );
	void densStep(float x[], float x0[], float u[], float v[], float object[], MovingObject *movingObject );
	void AddField(float y[], float s[] );
	void Diffuse(int b, float x[], float x0[], float object[], MovingObject *movingObject );
	void lin_solve(int b, float x[], float x0[], float object[], MovingObject *movingObject, float a, float c);
	void set_bnd(int b, float x[], float object[], MovingObject *movingObject );
	void advect(int b, float d[], float d0[], float u[], float v[], float object[], MovingObject *movingObject);
	void project(float u[], float v[], float p[], float div[], float object[], MovingObject *movingObject );

	int m_NumCells;
	Vec2f *m_Field;
	float m_Viscosity;
	float m_Dt;

};