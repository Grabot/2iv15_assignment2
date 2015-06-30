#include <iostream>
using namespace std;
#include "MovingObject.h"
#include "Particle.h"
#include "Solver.h"
#include <algorithm>
#include <list>
#include <vector>

#define CREATE_DIM1 (new Vec2f[(a_NumCells+2)*(a_NumCells+2)])

void CheckMovingObject(std::vector<MovingObject*> movings, int z, int i, int j, int b, float x[]);
void CheckCloth(Cloth *cloth, int i, int j, int b, float x[]);

Solver::Solver(int a_NumCells, float a_Viscosity, float a_Dt) 
	: m_NumCells(a_NumCells), m_Field(CREATE_DIM1), m_Viscosity(a_Viscosity), m_Dt(a_Dt)
{
	for (int i = 0; i < (m_NumCells + 2)*(m_NumCells + 2); i++)
	{
		m_Field[i][0] = m_Field[i][1] = 0.f;
	}
}

void Solver::velStep(float u[], float v[], float u0[], float v0[], float object[], std::vector<MovingObject*> movings, Cloth *cloth )
{
	AddField(u, u0);
	AddField(v, v0);

	//vorticityConfinement( u, v );

	float *temp;
	temp = u0;
	u0 = u;
	u = temp;

	Diffuse(1, u, u0, object, movings, cloth);

	temp = v0;
	v0 = v;
	v = temp;

	Diffuse(2, v, v0, object, movings, cloth);

	project(u, v, u0, v0, object, movings, cloth);

	temp = u0;
	u0 = u;
	u = temp;

	temp = v0;
	v0 = v;
	v = temp;

	advect(1, u, u0, u0, v0, object, movings, cloth);
	advect(2, v, v0, u0, v0, object, movings, cloth);
	project(u, v, u0, v0, object, movings, cloth);

}

void Solver::densStep(float x[], float x0[], float u[], float v[], float object[], std::vector<MovingObject*> movings, Cloth *cloth )
{
	AddField(x, x0);
	float *temp;
	temp = x0;
	x0 = x;
	x = temp;
	Diffuse(0, x, x0, object, movings, cloth);
	temp = x0;
	x0 = x;
	x = temp;
	advect(0, x, x0, u, v, object, movings, cloth);
}

void Solver::project(float u[], float v[], float p[], float div[], float object[], std::vector<MovingObject*> movings, Cloth *cloth )
{

	for (int i = 1; i <= m_NumCells; i++)
	{
		for (int j = 1; j <= m_NumCells; j++)
		{
			div[IX_DIM(i, j)] = -0.5f * (u[IX_DIM(i + 1, j)] - u[IX_DIM(i - 1, j)] + v[IX_DIM(i, j + 1)] - v[IX_DIM(i, j - 1)]) / m_NumCells;
			p[IX_DIM(i, j)] = 0;
		}
	}
	set_bnd(0, div, object, movings, cloth);
	set_bnd(0, p, object, movings, cloth);

	lin_solve(0, p, div, object, movings, cloth, 1, 4);

	for (int i = 1; i <= m_NumCells; i++)
	{
		for (int j = 1; j <= m_NumCells; j++)
		{
			u[IX_DIM(i, j)] -= 0.5f * m_NumCells * (p[IX_DIM(i + 1, j)] - p[IX_DIM(i - 1, j)]);
			v[IX_DIM(i, j)] -= 0.5f * m_NumCells * (p[IX_DIM(i, j + 1)] - p[IX_DIM(i, j - 1)]);
		}
	}
	set_bnd(1, u, object, movings, cloth);
	set_bnd(2, v, object, movings, cloth);
}

void Solver::advect(int b, float d[], float d0[], float u[], float v[], float object[], std::vector<MovingObject*> movings, Cloth *cloth )
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
	set_bnd(b, d, object, movings, cloth);
}

void Solver::Diffuse(int b, float x[], float x0[], float object[], std::vector<MovingObject*> movings, Cloth *cloth )
{
	float a = m_Dt * m_Viscosity * m_NumCells * m_NumCells;
	lin_solve(b, x, x0, object, movings, cloth, a, 1 + 4 * a);
}

void Solver::lin_solve(int b, float x[], float x0[], float object[], std::vector<MovingObject*> movings, Cloth *cloth, float a, float c)
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

		set_bnd(b, x, object, movings, cloth);
	}
}

void Solver::set_bnd(int b, float x[], float object[], std::vector<MovingObject*> movings, Cloth *cloth )
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

	//moving objects
	for (i = 2; i <= m_NumCells - 1; i++)
	{
		for (j = 2; j <= m_NumCells - 1; j++)
		{
			if (cloth->pnpoly(4, i, j))
			{
				/*
				we will look for the edge blocks of the
				*/
				if (!cloth->pnpoly(4, i + 1, j))
				{
					//edge block on the right side, with possibility of being askew
					if (!cloth->pnpoly(4, i, j + 1))
					{
						x[IX_DIM(i, j)] = b == 1 ? cloth->GetVelXRight(i, j, x) : (b == 2) ? cloth->GetVelYDown(i, j, x) : cloth->GetVelocityDensityXRight(i, j, x);
					}
					else if (!cloth->pnpoly(4, i, j - 1))
					{
						x[IX_DIM(i, j)] = b == 1 ? cloth->GetVelXRight(i, j, x) : (b == 2) ? cloth->GetVelYUp(i, j, x) : cloth->GetVelocityDensityXRight(i, j, x);
					}
					else
					{
						x[IX_DIM(i, j)] = b == 1 ? cloth->GetVelXRight(i, j, x) : (b == 2) ? 0 : cloth->GetVelocityDensityXRight(i, j, x);
					}
				}
				else if (!cloth->pnpoly(4, i - 1, j))
				{
					//edge block on the left side, with possibility being askew
					if (!cloth->pnpoly(4, i, j + 1))
					{
						x[IX_DIM(i, j)] = b == 1 ? cloth->GetVelXLeft(i, j, x) : (b == 2) ? cloth->GetVelYDown(i, j, x) : cloth->GetVelocityDensityXLeft(i, j, x);
					}
					else if (!cloth->pnpoly(4, i, j - 1))
					{
						x[IX_DIM(i, j)] = b == 1 ? cloth->GetVelXLeft(i, j, x) : (b == 2) ? cloth->GetVelYUp(i, j, x) : cloth->GetVelocityDensityXLeft(i, j, x);
					}
					else
					{
						x[IX_DIM(i, j)] = b == 1 ? cloth->GetVelXLeft(i, j, x) : (b == 2) ? 0 : cloth->GetVelocityDensityXLeft(i, j, x);
					}
				}
				else if (!cloth->pnpoly(4, i, j - 1))
				{
					//edge block on the top side, we assume we have all possibilities
					x[IX_DIM(i, j)] = b == 1 ? 0 : (b == 2) ? cloth->GetVelYUp(i, j, x) : cloth->GetVelocityDensityYUp(i, j, x);
				}
				else if (!cloth->pnpoly(4, i, j + 1))
				{
					//edge block on the bottom side, we assume we have all possibilities
					x[IX_DIM(i, j)] = b == 1 ? 0 : (b == 2) ? cloth->GetVelYDown(i, j, x) : cloth->GetVelocityDensityYDown(i, j, x);
				}

				//block is completely inside the block
				if (cloth->pnpoly(4, i - 1, j) && cloth->pnpoly(4, i + 1, j) && cloth->pnpoly(4, i, j + 1) && cloth->pnpoly(4, i, j - 1))
				{
					x[IX_DIM(i, j)] = b == 1 ? 0 : (b == 2) ? 0 : 0;
				}
				//x[IX_DIM(i, j)] = b == 1 ? movings[z]->GetVelocityX(i, j, x) : (b == 2) ? movings[z]->GetVelocityY(i, j, x) : movings[z]->GetVelocityDensity(i, j, x);
			}

			int size = movings.size();
			for (int z = 0; z < size; z++)
			{
				CheckMovingObject(movings, z, i, j, b, x);

			}
		}
	}

	x[IX_DIM(0, 0)] = 0.5f * (x[IX_DIM(1, 0)] + x[IX_DIM(0, 1)]);
	x[IX_DIM(0, m_NumCells + 1)] = 0.5f * (x[IX_DIM(1, m_NumCells + 1)] + x[IX_DIM(0, m_NumCells)]);
	x[IX_DIM(m_NumCells + 1, 0)] = 0.5f * (x[IX_DIM(m_NumCells, 0)] + x[IX_DIM(m_NumCells + 1, 1)]);
	x[IX_DIM(m_NumCells + 1, m_NumCells + 1)] = 0.5f * (x[IX_DIM(m_NumCells, m_NumCells + 1)] + x[IX_DIM(m_NumCells + 1, m_NumCells)]);
}

void CheckMovingObject(std::vector<MovingObject*> movings, int z, int i, int j, int b, float x[])
{
	if (movings[z]->pnpoly(4, i, j))
	{
		/*
		we will look for the edge blocks of the
		*/
		if (!movings[z]->pnpoly(4, i + 1, j))
		{
			//edge block on the right side, with possibility of being askew
			if (!movings[z]->pnpoly(4, i, j + 1))
			{
				x[IX_DIM(i, j)] = b == 1 ? movings[z]->GetVelXRight(i, j, x) : (b == 2) ? movings[z]->GetVelYDown(i, j, x) : movings[z]->GetVelocityDensityXRight(i, j, x);
			}
			else if (!movings[z]->pnpoly(4, i, j - 1))
			{
				x[IX_DIM(i, j)] = b == 1 ? movings[z]->GetVelXRight(i, j, x) : (b == 2) ? movings[z]->GetVelYUp(i, j, x) : movings[z]->GetVelocityDensityXRight(i, j, x);
			}
			else
			{
				x[IX_DIM(i, j)] = b == 1 ? movings[z]->GetVelXRight(i, j, x) : (b == 2) ? 0 : movings[z]->GetVelocityDensityXRight(i, j, x);
			}
		}
		else if (!movings[z]->pnpoly(4, i - 1, j))
		{
			//edge block on the left side, with possibility being askew
			if (!movings[z]->pnpoly(4, i, j + 1))
			{
				x[IX_DIM(i, j)] = b == 1 ? movings[z]->GetVelXLeft(i, j, x) : (b == 2) ? movings[z]->GetVelYDown(i, j, x) : movings[z]->GetVelocityDensityXLeft(i, j, x);
			}
			else if (!movings[z]->pnpoly(4, i, j - 1))
			{
				x[IX_DIM(i, j)] = b == 1 ? movings[z]->GetVelXLeft(i, j, x) : (b == 2) ? movings[z]->GetVelYUp(i, j, x) : movings[z]->GetVelocityDensityXLeft(i, j, x);
			}
			else
			{
				x[IX_DIM(i, j)] = b == 1 ? movings[z]->GetVelXLeft(i, j, x) : (b == 2) ? 0 : movings[z]->GetVelocityDensityXLeft(i, j, x);
			}
		}
		else if (!movings[z]->pnpoly(4, i, j - 1))
		{
			//edge block on the top side, we assume we have all possibilities
			x[IX_DIM(i, j)] = b == 1 ? 0 : (b == 2) ? movings[z]->GetVelYUp(i, j, x) : movings[z]->GetVelocityDensityYUp(i, j, x);
		}
		else if (!movings[z]->pnpoly(4, i, j + 1))
		{
			//edge block on the bottom side, we assume we have all possibilities
			x[IX_DIM(i, j)] = b == 1 ? 0 : (b == 2) ? movings[z]->GetVelYDown(i, j, x) : movings[z]->GetVelocityDensityYDown(i, j, x);
		}

		//block is completely inside the block
		if (movings[z]->pnpoly(4, i - 1, j) && movings[z]->pnpoly(4, i + 1, j) && movings[z]->pnpoly(4, i, j + 1) && movings[z]->pnpoly(4, i, j - 1))
		{
			x[IX_DIM(i, j)] = b == 1 ? 0 : (b == 2) ? 0 : 0;
		}
		//x[IX_DIM(i, j)] = b == 1 ? movings[z]->GetVelocityX(i, j, x) : (b == 2) ? movings[z]->GetVelocityY(i, j, x) : movings[z]->GetVelocityDensity(i, j, x);
	}
}

void CheckCloth(Cloth *cloth, int i, int j, int b, float x[])
{
	if (cloth->pnpoly(4, i, j))
	{
		/*
		we will look for the edge blocks of the
		*/
		if (!cloth->pnpoly(4, i + 1, j))
		{
			//edge block on the right side, with possibility of being askew
			if (!cloth->pnpoly(4, i, j + 1))
			{
				x[IX_DIM(i, j)] = b == 1 ? cloth->GetVelXRight(i, j, x) : (b == 2) ? cloth->GetVelYDown(i, j, x) : cloth->GetVelocityDensityXRight(i, j, x);
			}
			else if (!cloth->pnpoly(4, i, j - 1))
			{
				x[IX_DIM(i, j)] = b == 1 ? cloth->GetVelXRight(i, j, x) : (b == 2) ? cloth->GetVelYUp(i, j, x) : cloth->GetVelocityDensityXRight(i, j, x);
			}
			else
			{
				x[IX_DIM(i, j)] = b == 1 ? cloth->GetVelXRight(i, j, x) : (b == 2) ? 0 : cloth->GetVelocityDensityXRight(i, j, x);
			}
		}
		else if (!cloth->pnpoly(4, i - 1, j))
		{
			//edge block on the left side, with possibility being askew
			if (!cloth->pnpoly(4, i, j + 1))
			{
				x[IX_DIM(i, j)] = b == 1 ? cloth->GetVelXLeft(i, j, x) : (b == 2) ? cloth->GetVelYDown(i, j, x) : cloth->GetVelocityDensityXLeft(i, j, x);
			}
			else if (!cloth->pnpoly(4, i, j - 1))
			{
				x[IX_DIM(i, j)] = b == 1 ? cloth->GetVelXLeft(i, j, x) : (b == 2) ? cloth->GetVelYUp(i, j, x) : cloth->GetVelocityDensityXLeft(i, j, x);
			}
			else
			{
				x[IX_DIM(i, j)] = b == 1 ? cloth->GetVelXLeft(i, j, x) : (b == 2) ? 0 : cloth->GetVelocityDensityXLeft(i, j, x);
			}
		}
		else if (!cloth->pnpoly(4, i, j - 1))
		{
			//edge block on the top side, we assume we have all possibilities
			x[IX_DIM(i, j)] = b == 1 ? 0 : (b == 2) ? cloth->GetVelYUp(i, j, x) : cloth->GetVelocityDensityYUp(i, j, x);
		}
		else if (!cloth->pnpoly(4, i, j + 1))
		{
			//edge block on the bottom side, we assume we have all possibilities
			x[IX_DIM(i, j)] = b == 1 ? 0 : (b == 2) ? cloth->GetVelYDown(i, j, x) : cloth->GetVelocityDensityYDown(i, j, x);
		}

		//block is completely inside the block
		if (cloth->pnpoly(4, i - 1, j) && cloth->pnpoly(4, i + 1, j) && cloth->pnpoly(4, i, j + 1) && cloth->pnpoly(4, i, j - 1))
		{
			x[IX_DIM(i, j)] = b == 1 ? 0 : (b == 2) ? 0 : 0;
		}
		//x[IX_DIM(i, j)] = b == 1 ? movings[z]->GetVelocityX(i, j, x) : (b == 2) ? movings[z]->GetVelocityY(i, j, x) : movings[z]->GetVelocityDensity(i, j, x);
	}
}


void Solver::vorticityConfinement(float p[], float k[]) {

	int N = m_NumCells;
	float *forces = new float[(N + 2) * (N + 2)]();
	int *sign = new int[(N + 2) * (N + 2)]();

	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			float dudy = (p[IX_DIM(i, j + 1)] - p[IX_DIM(i, j - 1)]) / 2;
			float dvdx = (k[IX_DIM(i + 1, j)] - k[IX_DIM(i - 1, j)]) / 2;

			float toStore = dvdx - dudy;
			forces[IX_DIM(i, j)] = abs(toStore);

			sign[IX_DIM(i, j)] = toStore < 0 ? -1 : 1;
			//            std::cout << "Sign " << sign[IX(i,j)] << " toStore" << toStore << std::endl;

		}
	}

	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {

			float dwdx = (forces[IX_DIM(i + 1, j)] - forces[IX_DIM(i - 1, j)]) * 0.5f;
			float dwdy = (forces[IX_DIM(i, j + 1)] - forces[IX_DIM(i, j - 1)]) * 0.5f;

			float length = sqrt(dwdx * dwdx + dwdy * dwdy);

			if (length != 0){
				dwdx /= length;
				dwdy /= length;
			}
			//                dwdx /= 0.0000001f;
			//                dwdy /= 0.0000001f;
			//            }

			float v2 = sign[IX_DIM(i, j)] * forces[IX_DIM(i, j)];

			p[IX_DIM(i, j)] = dwdy * -v2;
			k[IX_DIM(i, j)] = dwdx * v2;
			//            Fvc_x[IX(i, j)] = dwdy * -v;
			//            Fvc_y[IX(i, j)] = dwdx *  v;
		}
	}

	free(forces);
}


void Solver::AddField(float z[], float s[] )
{
	int size = (m_NumCells + 2) * (m_NumCells + 2);
	for (int i = 0; i < size; i++) 
	{
		z[i] += s[i] * m_Dt;
	}
}
