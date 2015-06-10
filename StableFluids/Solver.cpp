#include "Solver.h"

#define CREATE_DIM1 (new Vec2f[(a_NumCells+2)*(a_NumCells+2)])
#define IX_DIM(i,j) ((i)+(m_NumCells+2)*(j))

Solver::Solver(int a_NumCells, float a_Viscosity, float a_Dt) 
	: m_NumCells(a_NumCells), m_Field(CREATE_DIM1), m_Viscosity(a_Viscosity), m_Dt(a_Dt)
{
	for (int i = 0; i < (m_NumCells + 2)*(m_NumCells + 2); i++)
	{
		m_Field[i][0] = m_Field[i][1] = 0.f;
	}
}

void Solver::AddField(float x[], float s[] )
{
	int size = (m_NumCells + 2) * (m_NumCells + 2);
	for (int i = 0; i < size; i++) {
		x[i] = m_Dt * s[i];
	}
}