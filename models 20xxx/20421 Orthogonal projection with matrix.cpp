
/// Project:	FMI 3D (2013)
/// File:		20421 Orthogonal projection with matrix.cpp
/// Topic:		20 Projections
/// Slide:		42
///	About:		Orthogonal projection with matrix
///
/// History:
///		2013.05.31/PB	Initial implementation
///     2013.09.15/PB   Added fix for 16-bit Z-buffers


#include "fmi3d.h"

#define matrix oxyz_t
#define vector vect_t

#define N  30
Sphere3D* P[N];
Circle2D* Q[N];
double r[N];
double a[N];
double b[N];
double s[N];
CoordSys3D* cs;
double f = 2;
double eps=0.05;


matrix orthoMatrix( )
{
	return
	{
		{ 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 0, f },
		{ 0, 0, 0, 1 },
	};
}

void drawPlane()
{
    glTranslated(0,0,-eps);
	double g = 6;
	glColor3f(0.25,0.25,0.5);
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glBegin(GL_QUADS);
	glVertex3f(-g,-g,f);
	glVertex3f(+g,-g,f);
	glVertex3f(+g,+g,f);
	glVertex3f(-g,+g,f);
	glEnd();

	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glBegin(GL_QUADS);
	glColor4f(0.85,0.85,1,0.8);
	glVertex3f(-g,-g,f);
	glColor4f(0.85,0.85,0.85,0.6);
	glVertex3f(+g,-g,f);
	glColor4f(0.85,0.85,1,0.8);
	glVertex3f(+g,+g,f);
	glColor4f(0.85,0.85,0.85,0.6);
	glVertex3f(-g,+g,f);
	glEnd();
    glTranslated(0,0,eps);
}

void drawScene()
{
	for( int i=0; i<N; i++ )
	{
		P[i]->drawImage();
		Q[i]->drawImage();
	}

	glEnable(GL_LINE_SMOOTH);
	glLineWidth( 1 );
	glDisable(GL_LIGHTING);
	glColor4f( 0,0,0,1 );

	glBegin(GL_LINES);
	for( int i=0; i<N; i++ )
	{
		glVertex3f(P[i]->getCenter().x,P[i]->getCenter().y,f);
		P[i]->getCenter().vertex();
	}
	glEnd();

	cs->drawImage();
	glDisable(GL_LIGHTING);
	drawPlane();
	cs->drawImage();
}


void drawFrames()
{
}


void randomizePoints()
{
	for( int i=0; i<N; i++ )
	{
		r[i] = random(f+5,f+10);
		a[i] = random(0,2*M_PI);
		b[i] = random(30*M_PI/180,90*M_PI/180);
		s[i] = random(0.05,0.5)*(random(0,2)>1?1:-1);
	}
}


int main()
{
	/// Open window
	if( !openWindow3D( "Orthogonal projection with matrix" ) ) return 1;
	noGround();

	/// Add buttons
	addButton( new exitButton2D );

	viewZ = 5;
	viewBeta = 0.1;
	minViewBeta = -M_PI/2+0.01;
	maxViewBeta = +M_PI/2-0.01;

	cs = new CoordSys3D( -3,3, -3,3, -2,11, false, 1, 0.4, L"   " );
	cs->setColor( {0,0,0} );

	for( int i=0; i<N; i++ )
	{
		P[i] = new Sphere3D( 0.3,QUALITY_MEDIUM );
		P[i]->setColor( {0,0,0} );
		P[i]->setShininess( 10 );
		Q[i] = new Circle2D( 0.2,QUALITY_LOW );
		Q[i]->setColor( {1,0,0} );
		Q[i]->setScale({0.2,0.2,0.05});
		Q[i]->setLighting(false);
	}

	randomizePoints();

	/// Main loop
	double dt;
	double t0 = t;
	while( isRunning() )
	{
		dt = t-t0;
		t0 = t;

		f = 3+2*sin(t/2);

		matrix M = orthoMatrix();

		for(int i=0; i<N; i++)
		{
			a[i] += dt*s[i];
			double bb = b[i]+1*sin(t+10*s[i]);
			P[i]->setCenter( {r[i]*cos(a[i])*cos(bb), r[i]*sin(a[i])*cos(bb), r[i]*sin(bb)} );
			Q[i]->setCenter( P[i]->getCenter().mmul(M) );
		}
	}
	return 0;
}

