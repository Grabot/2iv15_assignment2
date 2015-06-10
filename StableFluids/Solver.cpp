#include <iostream>
using namespace std;
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

void Solver::velStep(float u[], float v[], float u0[], float v0[])
{
	AddField(u, u0);
	AddField(v, v0);
}

void Solver::densStep(float x[], float x0[], float u[], float v[])
{
	AddField(x, x0);
	//Switch(x0, x);
	float *temp;
	temp = x0;
	x0 = x;
	x = temp;
	Diffuse(0, x, x0 );
}

void Solver::Diffuse(int b, float x[], float x0[])
{
	float a = m_Dt * m_Viscosity * m_NumCells * m_NumCells;
	lin_solve(b, x, x0, a, 1 + 4 * a);
}

void Solver::lin_solve(int b, float x[], float x0[], float a, float c)
{
	for (int k = 0; k < 20; k++)
	{
		for (int i = 1; i <= m_NumCells; i++)
		{
			for (int j = 1; j <= m_NumCells; j++)
			{
				int ij = IX_DIM(i, j);
				int _1ij = IX_DIM(i - 1, j);
				int i1j = IX_DIM(i - 1, j);
				int i_1j = IX_DIM(i, j - 1);
				int ij1 = IX_DIM(i, j + 1);

				x[IX_DIM(i, j)] = (x0[IX_DIM(i, j)] + a * (x[IX_DIM(i - 1, j)] + x[IX_DIM(i + 1, j)] + x[IX_DIM(i, j - 1)] + x[IX_DIM(i, j + 1)])) / c;
			}
		}

		set_bnd(b, x);
	}
}

void Solver::set_bnd(int b, float x[])
{
	int i;

	for (i = 1; i <= m_NumCells; i++)
	{
		x[IX_DIM(0, i)] = b == 1 ? -x[IX_DIM(1, i)] : x[IX_DIM(1, i)];
		x[IX_DIM(m_NumCells + 1, i)] = b == 1 ? -x[IX_DIM(m_NumCells, i)] : x[IX_DIM(m_NumCells, i)];
		x[IX_DIM(i, 0)] = b == 2 ? -x[IX_DIM(i, 1)] : x[IX_DIM(i, 1)];
		x[IX_DIM(i, m_NumCells + 1)] = b == 2 ? -x[IX_DIM(i, m_NumCells)] : x[IX_DIM(i, m_NumCells)];
	}
	x[IX_DIM(0, 0)] = 0.5f * (x[IX_DIM(1, 0)] + x[IX_DIM(0, 1)]);
	x[IX_DIM(0, m_NumCells + 1)] = 0.5f * (x[IX_DIM(1, m_NumCells + 1)] + x[IX_DIM(0, m_NumCells)]);
	x[IX_DIM(m_NumCells + 1, 0)] = 0.5f * (x[IX_DIM(m_NumCells, 0)] + x[IX_DIM(m_NumCells + 1, 1)]);
	x[IX_DIM(m_NumCells + 1, m_NumCells + 1)] = 0.5f * (x[IX_DIM(m_NumCells, m_NumCells + 1)] + x[IX_DIM(m_NumCells + 1, m_NumCells)]);
}

void Solver::Switch(float a1[], float a2[])
{
	float *temp;
	temp = a2;
	a2 = a1;
	a1 = temp;
}

void Solver::AddField(float z[], float s[] )
{
	int size = (m_NumCells + 2) * (m_NumCells + 2);
	for (int i = 0; i < size; i++) 
	{
		z[i] += s[i] * m_Dt;
	}
}