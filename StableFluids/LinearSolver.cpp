#include "linearSolver.h"
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
// vector helper functions

void vecAddEqual(int n, double r[], double v[])
{
	for (int i = 0; i < n; i++)
		r[i] = r[i] + v[i];
}

void vecDiffEqual(int n, double r[], double v[])
{
	for (int i = 0; i < n; i++)
		r[i] = r[i] - v[i];
}

void vecAssign(int n, double v1[], double v2[])
{
	for (int i = 0; i < n; i++)
		v1[i] = v2[i];
}

void vecTimesScalar(int n, double v[], double s)
{
	for (int i = 0; i < n; i++)
		v[i] *= s;
}

vector<vector<float>> VectorMultiplication(vector<vector<float>> A, vector<vector<float>> B)
{
	if (A[0].size() != B.size())
	{
		//the sizes are wrong, just an error message and no break because we assume correct sizes
		cout << "ERROR!!!!" << endl;
	}

	vector<vector<float>> Result = vector<vector<float>>(A.size(), vector<float>(B[0].size()));
	for (int i = 0; i < A.size(); i++)
	{
		for (int j = 0; j < B[0].size(); j++)
		{
			Result[i][j] = 0;
			for (int k = 0; k < A[0].size(); k++)
			{
				Result[i][j] = Result[i][j] + A[i][k] * B[k][j];
			}
		}
	}
	return Result;
}

vector<float> VectorMultiplication(vector< vector<float>> A, vector<float> B)
{
	if (A[0].size() != B.size())
	{
		//only display error message, it can't multiplicate. We assume it won't come here
		cout << "ERROR!!!!" << endl;
	}

	vector<float>  Result = vector<float>(A.size());
	for (int i = 0; i < A.size(); i++)
	{
		Result[i] = 0;
		for (int k = 0; k < B.size(); k++)
		{
			Result[i] = Result[i] + A[i][k] * B[k];
		}
	}
	return Result;
}

vector<vector<float>> VectorScalarMultiplication(vector<vector<float>> A, float scalar)
{
	vector<vector<float>>  Result = vector<vector<float>>(A.size(), vector<float>(A[0].size()));
	for (int i = 0; i < A.size(); i++)
	{
		for (int j = 0; j < A[0].size(); j++)
		{
			Result[i][j] = A[i][j] * scalar;
		}
	}
	return Result;
}

vector<float> VectorScalarMultiplication(vector<float> A, float scalar)
{
	vector<float>  Result = vector<float>(A.size());
	for (int i = 0; i < A.size(); i++)
	{
		Result[i] = A[i] * scalar;
	}
	return Result;
}

vector<float> VectorSubtraction(vector<float> A, vector<float> B)
{
	if (A.size() != B.size())
	{
		//please don't come here ever.
		cout << "ERROR!!!!" << endl;
	}

	vector<float> Result = vector<float>(A.size());
	for (int i = 0; i < A.size(); i++)
	{
		Result[i] = A[i] - B[i];
	}
	return Result;
}

float vecDotNew(Vec2f first, Vec2f second)
{
	return first[0] * second[0] + first[1] * second[1];
}

double vecDot(int n, double v1[], double v2[])
{
	double dot = 0;
	for (int i = 0; i < n; i++)
		dot += v1[i] * v2[i];
	return dot;
}

double vecSqrLen(int n, double v[])
{
	return vecDot(n, v, v);
}

double ConjGrad(int n, implicitMatrix *A, double x[], double b[],
	double epsilon,	// how low should we go?
	int    *steps)
{
	int		i, iMax;
	double	alpha, beta, rSqrLen, rSqrLenOld, u;

	double *r = (double *)malloc(sizeof(double) * n);
	double *d = (double *)malloc(sizeof(double) * n);
	double *t = (double *)malloc(sizeof(double) * n);
	double *temp = (double *)malloc(sizeof(double) * n);

	vecAssign(n, x, b);

	vecAssign(n, r, b);
	A->matVecMult(x, temp);
	vecDiffEqual(n, r, temp);

	rSqrLen = vecSqrLen(n, r);

	vecAssign(n, d, r);

	i = 0;
	if (*steps)
		iMax = *steps;
	else
		iMax = MAX_STEPS;

	if (rSqrLen > epsilon)
		while (i < iMax) {
		i++;
		A->matVecMult(d, t);
		u = vecDot(n, d, t);

		if (u == 0) {
			//printf("(SolveConjGrad) d'Ad = 0\n");
			break;
		}

		// How far should we go?
		alpha = rSqrLen / u;

		// Take a step along direction d
		vecAssign(n, temp, d);
		vecTimesScalar(n, temp, alpha);
		vecAddEqual(n, x, temp);

		if (i & 0x3F) {
			vecAssign(n, temp, t);
			vecTimesScalar(n, temp, alpha);
			vecDiffEqual(n, r, temp);
		}
		else {
			// For stability, correct r every 64th iteration
			vecAssign(n, r, b);
			A->matVecMult(x, temp);
			vecDiffEqual(n, r, temp);
		}

		rSqrLenOld = rSqrLen;
		rSqrLen = vecSqrLen(n, r);

		// Converged! Let's get out of here
		if (rSqrLen <= epsilon)
			break;

		// Change direction: d = r + beta * d
		beta = rSqrLen / rSqrLenOld;
		vecTimesScalar(n, d, beta);
		vecAddEqual(n, d, r);
		}

	// free memory

	free(r);
	free(d);
	free(t);
	free(temp);

	*steps = i;
	return(rSqrLen);
}


