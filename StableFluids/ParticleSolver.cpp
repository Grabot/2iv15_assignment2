#include "Particle.h"
#include <vector>
#include <algorithm>
#include "Gravity.h"
#include <iostream>
#include <list>
#include "linearSolver.h"
using namespace std;

//void CalculateForces(std::vector<Force*> forces, std::vector<Constraint*> constraints, std::vector<Particle*> pVector);
//void ComputeConstraint(std::vector<Particle*> pVector, std::vector<Constraint*> constraints);
//void ClearForceAccumulators(std::vector<Particle*> pVector);
//void ApplyForce(std::vector<Force*> forces);
void Euler(std::vector<Force*> forces, std::vector<Constraint*> constraints, std::vector<Particle*> pVector, float dt);
//void MidPoint(std::vector<Force*> forces, std::vector<Constraint*> constraints, std::vector<Particle*> pVector, float dt);
//void RungeKutta(std::vector<Force*> forces, std::vector<Constraint*> constraints, std::vector<Particle*> pVector, float dt);
//void Verlet(std::vector<Force*> forces, std::vector<Constraint*> constraints, std::vector<Particle*> pVector, float dt);

#define DAMP 0.98f
#define RAND (((rand()%2000)/1000.f)-1.f)
void simulation_step(std::vector<Particle*> pVector, std::vector<Force*> forces, std::vector<Constraint*> constraints, float dt, int solver)
{
	int i, size = pVector.size();

	//F= m*a
	//a = F/m
	//V= a*t
	//V= (F*t)/m
	//S= V*t
	if (solver == 1)
	{
		//euler
		Euler(forces, constraints, pVector, dt);
	}
	else if (solver == 2)
	{
		//midpoint
		//MidPoint(forces, constraints, pVector, dt);
	}
	else if (solver == 3)
	{
		//runge-kutta
		//RungeKutta(forces, constraints, pVector, dt);
	}
	else if (solver == 4)
	{
		//verlet?
		//not working yet
		//Verlet(forces, constraints, pVector, dt);
	}
}
