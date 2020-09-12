
/// Project:	FMI 3D (2013)
/// File:		12221 Circular labyrinth.cpp
/// Topic:		12 Trajectories
/// Slide:		22
///	About:		Random movement in circular labyrinth
///
/// History:
///		2013.08.23/PB	Initial implementation


#include "fmi3d.h"

#define N 500
vect_t p[N];
bool q[N];
int n = 0;
int dn = 0;
double dR=0.1*1.5;
double dA=3*M_PI/180*1.5;
double r=4, a=0;

void drawScene()
{
	glEnable( GL_LINE_SMOOTH );
	glEnable( GL_POINT_SMOOTH );
	glDisable( GL_LIGHTING );
	glDisable( GL_TEXTURE_2D );

	// Draw path
	glDisable( GL_DEPTH_TEST );
	glColor3f( 0,0,0 );
	glPointSize( 6 );
	glBegin( GL_POINTS );
	for( int i=1; i<N-1; i++ ) if( q[(n+N-i)%N] ) p[( n+N-i )%N].vertex();
	glEnd();
	glLineWidth( 6 );
	glBegin( GL_LINE_STRIP );
	for( int i=1; i<N-1; i++ ) p[( n+N-i )%N].vertex();
	glEnd();
	glLineWidth( 4 );
	glColor3f( 1,1,0 );
	glBegin( GL_LINE_STRIP );
	for( int i=1+dn; i<N; i++ )
	{
		glColor3f(0.5+0.5*sin(i/5.0-3*t),0.5+0.5*cos(1+i/10.0+2*t),0.5+0.5*sin(2+i/7.0+4*t));
		p[( n+i )%N].vertex();
	}
	glEnd();
	glEnable( GL_DEPTH_TEST );
}


void drawFrames()
{
}


void move( double dR, double dA )
{
	r += dR;
	a += dA;
	p[n%N] = {r*cos( a ),r*sin( a ),3};
	q[n%N] = false;
	n++;
	dn=1-dn;
}

int main()
{
	/// Open window
	if( !openWindow3D( "Circular labyrinth" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();

	viewBeta = 1.2;

	/// Add buttons
	addButton( new exitButton2D );

	for( int i=0; i<N; i++ )
	{
		p[i] = {r,0,3};
		q[i] = false;
	}

	/// Main loop
	while( isRunning() )
	{
		double c;

		//radial
		c = r; while (fabs(c-r)<0.5) c = random( 1,7 );
		int s = ( c>r )?1:-1;
		while( isRunning() && s*r<s*c ) move(s*dR,0);
		q[(n-1)%N] = true;

		//concentric
		c = a; while (fabs(c-a)<0.5) c=a+random( -2*M_PI,2*M_PI );
		s = ( c>a )?1:-1;
		while( isRunning() && s*a<s*c ) move(0,s*dA*2/r);
		q[(n-1)%N] = true;
	}
	return 0;
}

