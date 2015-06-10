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

void Solver::velStep(float u[], float v[], float u0[], float v0[], float object[])
{
	AddField(u, u0);
	AddField(v, v0);

	float *temp;
	temp = u0;
	u0 = u;
	u = temp;

	Diffuse(1, u, u0, object);

	temp = v0;
	v0 = v;
	v = temp;

	Diffuse(2, v, v0, object);

	project(u, v, u0, v0, object);

	temp = u0;
	u0 = u;
	u = temp;

	temp = v0;
	v0 = v;
	v = temp;

	advect(1, u, u0, u0, v0, object);
	advect(2, v, v0, u0, v0, object);
	project(u, v, u0, v0, object);

}

void Solver::densStep(float x[], float x0[], float u[], float v[], float object[])
{
	AddField(x, x0);
	//Switch(x0, x);
	float *temp;
	temp = x0;
	x0 = x;
	x = temp;
	Diffuse(0, x, x0, object);
	temp = x0;
	x0 = x;
	x = temp;
	advect(0, x, x0, u, v, object);
}

void Solver::project(float u[], float v[], float p[], float div[], float object[])
{

	for (int i = 1; i <= m_NumCells; i++)
	{
		for (int j = 1; j <= m_NumCells; j++)
		{
			div[IX_DIM(i, j)] = -0.5f * (u[IX_DIM(i + 1, j)] - u[IX_DIM(i - 1, j)] + v[IX_DIM(i, j + 1)] - v[IX_DIM(i, j - 1)]) / m_NumCells;
			p[IX_DIM(i, j)] = 0;
		}
	}
	set_bnd(0, div, object); 
	set_bnd(0, p, object);

	lin_solve(0, p, div, object, 1, 4);

	for (int i = 1; i <= m_NumCells; i++)
	{
		for (int j = 1; j <= m_NumCells; j++)
		{
			u[IX_DIM(i, j)] -= 0.5f * m_NumCells * (p[IX_DIM(i + 1, j)] - p[IX_DIM(i - 1, j)]);
			v[IX_DIM(i, j)] -= 0.5f * m_NumCells * (p[IX_DIM(i, j + 1)] - p[IX_DIM(i, j - 1)]);
		}
	}
	set_bnd(1, u, object); 
	set_bnd(2, v, object);
}

void Solver::advect(int b, float d[], float d0[], float u[], float v[], float object[])
{
	int i0, j0, i1, j1;
	float x, y, s0, t0, s1, t1, dt0;

	dt0 = m_Dt * m_NumCells;
	for (int i = 1; i <= m_NumCells; i++)
	{
		for (int j = 1; j <= m_NumCells; j++)
		{
			x = i - dt0 * u[IX_DIM(i, j)];
			y = j - dt0 * v[IX_DIM(i, j)];
			if (x < 0.5f) x = 0.5f;
			if (x > m_NumCells + 0.5f) x = m_NumCells + 0.5f;
			i0 = (int)x;
			i1 = i0 + 1;
			if (y < 0.5f) y = 0.5f;
			if (y > m_NumCells + 0.5f) y = m_NumCells + 0.5f;
			j0 = (int)y;
			j1 = j0 + 1;
			s1 = x - i0;
			s0 = 1 - s1;
			t1 = y - j0;
			t0 = 1 - t1;
			d[IX_DIM(i, j)] = s0 * (t0 * d0[IX_DIM(i0, j0)] + t1 * d0[IX_DIM(i0, j1)]) +
				s1 * (t0 * d0[IX_DIM(i1, j0)] + t1 * d0[IX_DIM(i1, j1)]);
		}
	}
	set_bnd(b, d, object);
}

void Solver::Diffuse(int b, float x[], float x0[], float object[])
{
	float a = m_Dt * m_Viscosity * m_NumCells * m_NumCells;
	lin_solve(b, x, x0, object, a, 1 + 4 * a);
}

void Solver::lin_solve(int b, float x[], float x0[], float object[], float a, float c)
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

		set_bnd(b, x, object);
	}
}

void Solver::set_bnd(int b, float x[], float object[])
{
	int i, j;

	for (i = 1; i <= m_NumCells; i++)
	{
		x[IX_DIM(0, i)] = b == 1 ? -x[IX_DIM(1, i)] : x[IX_DIM(1, i)];
		x[IX_DIM(m_NumCells + 1, i)] = b == 1 ? -x[IX_DIM(m_NumCells, i)] : x[IX_DIM(m_NumCells, i)];
		x[IX_DIM(i, 0)] = b == 2 ? -x[IX_DIM(i, 1)] : x[IX_DIM(i, 1)];
		x[IX_DIM(i, m_NumCells + 1)] = b == 2 ? -x[IX_DIM(i, m_NumCells)] : x[IX_DIM(i, m_NumCells)];
	}

	for (i = 1; i <= m_NumCells; i++)
	{
		for (j = 1; j <= m_NumCells; j++)
		{
			if (object[IX_DIM(i, j)] == 1)
			{
				//Center
				x[IX_DIM(i, j)] = 0;
				//Left, invert x
				x[IX_DIM(i - 1, j)] = b == 1 ? -x[IX_DIM(i - 2, j)] : x[IX_DIM(i - 2, j)];
				//Right, invert x
				x[IX_DIM(i + 1, j)] = b == 1 ? -x[IX_DIM(i + 2, j)] : x[IX_DIM(i + 2, j)];
				//Bottom, invert y
				x[IX_DIM(i, j - 1)] = b == 2 ? -x[IX_DIM(i, j - 2)] : x[IX_DIM(i, j - 2)];
				//Top, invert y
				x[IX_DIM(i, j + 1)] = b == 2 ? -x[IX_DIM(i, j + 2)] : x[IX_DIM(i, j + 2)];
			}
		}
	}

	x[IX_DIM(0, 0)] = 0.5f * (x[IX_DIM(1, 0)] + x[IX_DIM(0, 1)]);
	x[IX_DIM(0, m_NumCells + 1)] = 0.5f * (x[IX_DIM(1, m_NumCells + 1)] + x[IX_DIM(0, m_NumCells)]);
	x[IX_DIM(m_NumCells + 1, 0)] = 0.5f * (x[IX_DIM(m_NumCells, 0)] + x[IX_DIM(m_NumCells + 1, 1)]);
	x[IX_DIM(m_NumCells + 1, m_NumCells + 1)] = 0.5f * (x[IX_DIM(m_NumCells, m_NumCells + 1)] + x[IX_DIM(m_NumCells + 1, m_NumCells)]);
}

void Solver::AddField(float z[], float s[] )
{
	int size = (m_NumCells + 2) * (m_NumCells + 2);
	for (int i = 0; i < size; i++) 
	{
		z[i] += s[i] * m_Dt;
	}
}