// StableFluids.cpp : Defines the entry point for the console application.
//
#include <iostream>
using namespace std;
#include "Solver.h"
#include "MovingObject.h"
#include "FieldToolbox.h"
#include "imageio.h"
#include "Force.h"
#include "CircularWireConstraint.h"

#include "Particle.h"
#include "MouseForce.h"
#include "Dragforce.h"
#include "SpringForce.h"

#include <algorithm>
#include <list>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include <GL/glu.h>
#include <GL/glut.h>

// static Particle *pList;
static std::vector<Particle*> pVector;

std::vector<MouseForce*> mouses;
std::vector<Force*> forces;
std::vector<Constraint*> constraints;

int particleSelected = -1;

/* macros */

#define IX(i,j) ((i)+(N+2)*(j))

/* global variables */

float *xSpeed;
float *ySpeed;
float *rotation;

static int N;
static float dt, diff, visc;
static float force, source;
float *u;
float *v;
float *u_prev;
float *v_prev;
float *dens;
float *dens_prev;
float *object;
int buttonW = 0;
static int dvel;
static int dump_frames;
static int frame_number;
long long level_elapsed_time = 0;
long long level_start_time = 0;

Vec2f MousePos;
Solver *solver;
std::vector<MovingObject*> movings; 
//std::list<MovingObject*> movings;

static int win_id;
static int win_x, win_y;
static int mouse_down[3];
static int omx, omy, mx, my;

/* external definitions (from solver) */
extern void simulation_step(std::vector<Particle*> pVector, std::vector<Force*> forces, std::vector<Constraint*> constraints, float dt, int solver);

/*
----------------------------------------------------------------------
free/clear/allocate simulation data
----------------------------------------------------------------------
*/


static void free_data ( void )
{
}

static void clear_data ( void )
{

	int i, size = (N + 2)*(N + 2);

	for (int i = 0; i < size; i++)
	{
		u[i] = v[i] = u_prev[i] = v_prev[i] = dens[i] = dens_prev[i] = object[i] = 0.0f;
	}

}

static int allocate_data ( void )
{
	solver = new Solver(N, visc, dt);

	//movings.push_back(new MovingObject(Vec2f(0.4, 0.5), 0.2));
	//movings.push_back(new MovingObject(Vec2f(0.2, 0.3), 0.2));

	xSpeed = new float[movings.size()];
	ySpeed = new float[movings.size()];
	rotation = new float[movings.size()];
	xSpeed[0] = 0.00;
	ySpeed[0] = 0.00;
	rotation[0] = 2;
	xSpeed[1] = 0.00;
	ySpeed[1] = 0.00;
	rotation[1] = -1;

	int particleID = 0;
	pVector.push_back(new Particle(Vec2f(0.4, 0.4), 1.0f, particleID++, 0));
	pVector.push_back(new Particle(Vec2f(0.4, 0.6), 1.0f, particleID++, 0));
	pVector.push_back(new Particle(Vec2f(0.6, 0.6), 1.0f, particleID++, 0));
	pVector.push_back(new Particle(Vec2f(0.6, 0.4), 1.0f, particleID++, 0));
	int i, sizeP = pVector.size();
	for (i = 0; i<sizeP; i++)
	{
		mouses.push_back(new MouseForce(pVector[i], pVector[i]->m_Velocity, 0.5, 0.5));
	}
	for (i = 0; i<sizeP; i++)
	{
		forces.push_back(new DragForce(pVector[i], 0.99));
	}

	forces.push_back(new SpringForce(pVector[0], pVector[1], 0.2, 2, 2, 0));
	forces.push_back(new SpringForce(pVector[1], pVector[2], 0.2, 2, 2, 0));
	forces.push_back(new SpringForce(pVector[2], pVector[3], 0.2, 2, 2, 0));
	forces.push_back(new SpringForce(pVector[3], pVector[0], 0.2, 2, 2, 0));
	forces.push_back(new SpringForce(pVector[0], pVector[2], 0.283, 2, 2, 0));
	forces.push_back(new SpringForce(pVector[1], pVector[3], 0.283, 2, 2, 0));

	int size = (N + 2) * (N + 2);

	u = new float[size];
	v = new float[size];
	u_prev = new float[size];
	v_prev = new float[size];
	dens = new float[size];
	dens_prev = new float[size];
	object = new float[size];

	FieldToolbox::Create();
	return ( 1 );
}


/*
----------------------------------------------------------------------
OpenGL specific drawing routines
----------------------------------------------------------------------
*/

static void pre_display ( void )
{
	glViewport ( 0, 0, win_x, win_y );
	glMatrixMode ( GL_PROJECTION );
	glLoadIdentity ();
	gluOrtho2D ( 0.0, 1.0, 0.0, 1.0 );
	glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear ( GL_COLOR_BUFFER_BIT );
}

static void post_display ( void )
{
	level_elapsed_time = timeGetTime();
	while (!(((level_elapsed_time - level_start_time) % 17) == 0))
	{
		level_elapsed_time = timeGetTime();
		Sleep(1);
	}

	if (((level_elapsed_time - level_start_time) % 1000) <= 16)
	{
		cout << "frames per second: " << frame_number << endl;
		frame_number = 0;
	}
	frame_number++;

	glutSwapBuffers ();
}

static void draw_object(void)
{
	int i, j;
	float x1, x2, y1, y2, h;

	h = 1.0f / N;

	glColor3f(1.0f, 1.0f, 0.0f);
	glLineWidth(1.0f);

	glBegin(GL_QUADS);

	for (i = 1; i <= N; i++)
	{
		x1 = (i - 0.5f) * h;
		x2 = (i + 0.5f) * h;
		for (j = 1; j <= N; j++)
		{
			y1 = (j - 0.5f) * h;
			y2 = (j + 0.5f) * h;

			if (object[IX(i, j)] == 1)
			{
				glColor3f(1.0f, 1.0f, 0.0f);

				glVertex2f(x1 - 1 * h, y1 - 1 * h);
				glVertex2f(x1 + 1 * h, y1 - 1 * h);
				glVertex2f(x1 + 1 * h, y1 + 1 * h);
				glVertex2f(x1 - 1 * h, y1 + 1 * h);
			}
		}
	}
	glEnd();

}
static void draw_velocity ( void )
{
	int i, j;
	float x, y, h;

	h = 1.0f/N;

	glColor3f ( 1.0f, 1.0f, 1.0f );
	glLineWidth ( 1.0f );

	glBegin ( GL_LINES );

	for ( i=1 ; i<=N ; i++ ) {
		x = (i-0.5f)*h;
		for ( j=1 ; j<=N ; j++ ) {
			y = (j-0.5f)*h;

			glVertex2f ( x, y );
			glVertex2f(x + u[IX(i, j)], y + v[IX(i, j)]);
		}
	}

	glEnd ();
}

static void draw_density ( void )
{
	int i, j;
	float x, y, h, d00, d01, d10, d11;

	h = 1.0f/N;

	glBegin ( GL_QUADS );

	for ( i=0 ; i<=N ; i++ ) {
		x = (i-0.5f)*h;
		for ( j=0 ; j<=N ; j++ ) {
			y = (j-0.5f)*h;

			d00 = dens[IX(i, j)];
			d01 = dens[IX(i, j + 1)];
			d10 = dens[IX(i + 1, j)];
			d11 = dens[IX(i + 1, j + 1)];

			glColor3f(d00*0.3, d00*0.8, d00); glVertex2f(x, y);
			glColor3f(d10*0.3, d10*0.8, d10); glVertex2f(x + h, y);
			glColor3f(d11*0.3, d11*0.8, d11); glVertex2f(x + h, y + h);
			glColor3f(d01*0.3, d01*0.8, d01); glVertex2f(x, y + h);
		}
	}

	glEnd ();
}

static void draw_particles(void)
{
	int size = pVector.size();

	for (int i = 0; i< size; i++)
	{
		pVector[i]->draw();
	}
}

static void draw_forces(void)
{
	for_each(forces.begin(), forces.end(), [](Force* f)
	{
		f->draw();
	});

	for_each(constraints.begin(), constraints.end(), [](Constraint* c)
	{
		c->draw();
	});

	for_each(mouses.begin(), mouses.end(), [](MouseForce* m)
	{
		m->draw();
	});
}

/*
----------------------------------------------------------------------
relates mouse movements to forces sources
----------------------------------------------------------------------
*/

static void get_from_UI( float d[], float u[], float v[] )
{
	int i, j, size = (N+2)*(N+2);

	float x = 0;
	float y = 0;

	for ( i=0 ; i<size ; i++ ) {
		u[i] = v[i] = d[i] = 0.0f;
	}


	i = (int)((mx / (float)win_x)*N + 1);
	j = (int)(((win_y - my) / (float)win_y)*N + 1);

	//if (i<1 || i>N || j<1 || j>N) return;

	//if ( !mouse_down[0] && !mouse_down[2] ) return;

	if ( mouse_down[0] ) 
	{
		u[IX(i, j)] = force*(mx - omx);
		v[IX(i, j)] = force*(omy - my);

		x = (float)((float)i / 64);
		y = (float)((float)j / 64);

		int i, size = pVector.size();

		for (i = 0; i<size; i++)
		{

			MousePos[0] = x;
			MousePos[1] = y;

			float xDis = pVector[i]->m_Position[0] - MousePos[0];
			float yDis = pVector[i]->m_Position[1] - MousePos[1];

			float distance = xDis*xDis + yDis*yDis;

			if (distance < 0.01)
			{
				particleSelected = i;
			}

			//particle is selected
			if (particleSelected == i)
			{
				mouses[i]->getMouse(MousePos);
				mouses[i]->setForce(true);
				mouses[i]->apply();
			}
			else
			{
				mouses[i]->getMouse(pVector[i]->m_Position);
				mouses[i]->setForce(false);
			}

		}
	}
	else
	{
		particleSelected = -1;
		int i, size = pVector.size();

		for (i = 0; i < size; i++)
		{
			mouses[i]->getMouse(pVector[i]->m_Position);
			mouses[i]->setForce(false);
		}
	}

	if (mouse_down[2] && buttonW == 1)
	{
		object[IX(i, j)] = 1;
	}

	if ( mouse_down[2] && buttonW == 0 ) {
		d[IX(i, j)] = source;
	}

	omx = mx;
	omy = my;

	return;
}

/*
----------------------------------------------------------------------
GLUT callback routines
----------------------------------------------------------------------
*/

static void key_func ( unsigned char key, int x, int y )
{
	switch ( key )
	{
	case 'w':
	case 'W':
		buttonW = 1;
		break;

	case 'e':
	case 'E':
		buttonW = 0;
		break;

	case 'c':
	case 'C':
		clear_data ();
		break;

	case 'd':
	case 'D':
		dump_frames = !dump_frames;
		break;

	case 'q':
	case 'Q':
		free_data ();
		exit ( 0 );
		break;

	case 'v':
	case 'V':
		dvel = !dvel;
		break;
	}
}

static void mouse_func ( int button, int state, int x, int y )
{
	omx = mx = x;
	omx = my = y;

	if (glutGetModifiers() & GLUT_ACTIVE_SHIFT) {
		button = 2;
	}

	mouse_down[button] = state == GLUT_DOWN;
}

static void motion_func ( int x, int y )
{
	mx = x;
	my = y;
}

static void reshape_func ( int width, int height )
{
	glutSetWindow ( win_id );
	glutReshapeWindow ( width, height );

	win_x = width;
	win_y = height;
}

void MoveObjects()
{
	int size = movings.size();

	for (int i = 0; i < size; i++)
	{
		if ((movings[i]->returnXLeft()) <= 0)
		{
			xSpeed[i] = (xSpeed[i] * -1);
		}

		if ((movings[i]->returnXRight()) >= 1)
		{
			xSpeed[i] = (xSpeed[i] * -1);
		}

		if ((movings[i]->returnYTop()) >= 1)
		{
			ySpeed[i] = (ySpeed[i] * -1);
		}

		if ((movings[i]->returnYBottom()) <= 0)
		{
			ySpeed[i] = (ySpeed[i] * -1);
		}

		xSpeed[i] = xSpeed[i] * 0.99;
		ySpeed[i] = ySpeed[i] * 0.99;

		movings[i]->MoveStep(xSpeed[i], ySpeed[i], rotation[i]);
	}

}

void twoWayCoupling()
{
	int i, j;

	for (i = 1; i <= N; i++) {
		for (j = 1; j <= N; j++) {
			for (int k = 0; k < movings.size(); k++)
			{
				if (movings[k]->pnpoly(4, i, j))
				{
					if ((u[IX(i, j)]) > 0.8)
					{
						xSpeed[k] = -0.01;
					}
					else if ((u[IX(i, j)]) < -0.8)
					{
						xSpeed[k] = 0.01;
					}

					if ((v[IX(i, j)]) > 0.8)
					{
						ySpeed[k] = -0.01;
					}
					else if ((v[IX(i, j)]) < -0.8)
					{
						ySpeed[k] = 0.01;
					}
				}
			}
		}
	}
}

static void idle_func ( void )
{
	get_from_UI( dens_prev, u_prev, v_prev );
	//euler = 1, midpoint = 2 and runge-kutta = 3
	simulation_step(pVector, forces, constraints, dt, 1);
	MoveObjects();
	//twoWayCoupling();
	solver->velStep(u, v, u_prev, v_prev, object, movings);
	solver->densStep(dens, dens_prev, u, v, object, movings);
	
	glutSetWindow ( win_id );
	glutPostRedisplay ();
}

static void display_func ( void )
{
	pre_display ();

	if (dvel)
	{
		draw_velocity();

		int size = movings.size();
		for (int i = 0; i < size; i++)
		{
			movings[i]->draw( false );
		}
	}
	else
	{
		draw_density(); 
		draw_object();

		int size = movings.size();
		for (int i = 0; i < size; i++)
		{
			movings[i]->draw( true );
		}
	}

	draw_forces();
	draw_particles();

	post_display ();
}


/*
----------------------------------------------------------------------
open_glut_window --- open a glut compatible window and set callbacks
----------------------------------------------------------------------
*/

static void open_glut_window ( void )
{
	glutInitDisplayMode ( GLUT_RGBA | GLUT_DOUBLE );

	glutInitWindowPosition ( 0, 0 );
	glutInitWindowSize ( win_x, win_y );
	win_id = glutCreateWindow ( "Stable Fluids!" );

	glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear ( GL_COLOR_BUFFER_BIT );
	glutSwapBuffers ();
	glClear ( GL_COLOR_BUFFER_BIT );
	glutSwapBuffers ();

	pre_display ();

	level_start_time = timeGetTime();

	glutKeyboardFunc ( key_func );
	glutMouseFunc ( mouse_func );
	glutMotionFunc ( motion_func );
	glutReshapeFunc ( reshape_func );
	glutIdleFunc ( idle_func );
	glutDisplayFunc ( display_func );
}


/*
----------------------------------------------------------------------
main --- main routine
----------------------------------------------------------------------
*/

int main ( int argc, char ** argv )
{
	glutInit ( &argc, argv );

	if ( argc != 1 && argc != 6 ) {
		fprintf ( stderr, "usage : %s N dt diff visc force source\n", argv[0] );
		fprintf ( stderr, "where:\n" );\
			fprintf ( stderr, "\t N      : grid resolution\n" );
		fprintf ( stderr, "\t dt     : time step\n" );
		fprintf ( stderr, "\t diff   : diffusion rate of the density\n" );
		fprintf ( stderr, "\t visc   : viscosity of the fluid\n" );
		fprintf ( stderr, "\t force  : scales the mouse movement that generate a force\n" );
		fprintf ( stderr, "\t source : amount of density that will be deposited\n" );
		exit ( 1 );
	}

	if ( argc == 1 ) {
		N = 64;
		dt = 0.1f;
		diff = 0.0f;
		visc = 0.0f;
		force = 5.0f;
		source = 100.0f;
		fprintf ( stderr, "Using defaults : N=%d dt=%g diff=%g visc=%g force = %g source=%g\n",
			N, dt, diff, visc, force, source );
	} else {
		N = atoi(argv[1]);
		dt = atof(argv[2]);
		diff = atof(argv[3]);
		visc = atof(argv[4]);
		force = atof(argv[5]);
		source = atof(argv[6]);
	}

	printf ( "\n\nHow to use this demo:\n\n" );
	printf ( "\t Add densities with the right mouse button\n" );
	printf ( "\t Add velocities with the left mouse button and dragging the mouse\n" );
	printf ( "\t Toggle density/velocity display with the 'v' key\n" );
	printf ( "\t Clear the simulation by pressing the 'c' key\n" );
	printf ( "\t Quit by pressing the 'q' key\n" );

	dvel = 0;

	if ( !allocate_data () ) exit ( 1 );
	clear_data ();

	win_x = 512;
	win_y = 512;
	open_glut_window ();

	glutMainLoop ();

	exit ( 0 );
}


