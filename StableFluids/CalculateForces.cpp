#include "Particle.h"
#include <vector>
#include <algorithm>
#include "Gravity.h"
#include <iostream>
#include <list>
#include "linearSolver.h"
using namespace std;

void ComputeConstraint(std::vector<Particle*> pVector, std::vector<Constraint*> constraints);
void ClearForceAccumulators(std::vector<Particle*> pVector);
void ApplyForce(std::vector<Force*> forces);

void CalculateForces(std::vector<Force*> forces, std::vector<Constraint*> constraints, std::vector<Particle*> pVector)
{
	//DerivEval loop, first clear force accumelators and than apply forces, finnaly calcualte constraints
	ClearForceAccumulators(pVector);
	ApplyForce(forces);
	ComputeConstraint(pVector, constraints);
}

void ClearForceAccumulators(std::vector<Particle*> pVector)
{
	//clear all forces from the accumelator
	for (int i = 0; i < pVector.size(); i++)
	{
		pVector[i]->m_Force[0] = 0;
		pVector[i]->m_Force[1] = 0;
	}
}

void ApplyForce(std::vector<Force*> forces)
{
	//calculate forces
	for (int i = 0; i < forces.size(); i++) {
		forces[i]->apply();
	}
}
