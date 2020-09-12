
/// Project:	FMI 3D (2013)
/// File:		12222 Spherical 2D labyrinth.cpp
/// Topic:		12 Trajectories
/// Slide:		22
///	About:		Random movement in spherical 2D labyrinth
///
/// History:
///		2013.08.23/PB	Initial implementation


#include "fmi3d.h"

#define N 1500
vect_t p[N];
bool q[N];
int n = 0;
int dn = 0;

double dA=3*M_PI/180*2;
double dB=3*M_PI/180*2;
double r=4, a=0, b=0;
double t0;

void drawScene()
{
	glEnable( GL_LINE_SMOOTH );
	glEnable( GL_POINT_SMOOTH );
	glDisable( GL_LIGHTING );
	glDisable( GL_TEXTURE_2D );

	// Draw path
	glDisable( GL_DEPTH_TEST );
	glDepthMask( GL_FALSE );
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
	glDepthMask( GL_TRUE );
	glEnable( GL_DEPTH_TEST );

	glLineWidth( 4 );
	glColor3f( 1,1,0 );
	glBegin( GL_LINE_STRIP );
	for( int i=1+dn; i<N; i++ )
	{
		glColor3f(0.5+0.5*sin(i/5.0-3*t),0.5+0.5*cos(1+i/10.0+2*t),0.5+0.5*sin(2+i/7.0+4*t));
		p[( n+i )%N].vertex();
	}
	glEnd();
}


void drawFrames()
{
}


void moveRAB( double dR, double dA, double dB )
{
	viewAlpha += (t-t0)/7;
	viewBeta = M_PI/6+M_PI/8*sin(t/3);
	t0 = t;

	r += dR;
	a += dA;
	b += dB;
	p[n%N] = {r*cos( a )*sin(b),r*sin( a )*sin(b),5+r*cos(b)};
	q[n%N] = false;
	n++;
	dn=1-dn;
}

int main()
{
	/// Open window
	if( !openWindow3D( "Spherical 2D labyrinth" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();

	viewZ = 5;

	/// Add buttons
	addButton( new exitButton2D );

	for( int i=0; i<N; i++ )
	{
		p[i] = {0,0,5+r};
		q[i] = false;
	}

	/// Main loop
	t0=t;
	while( isRunning() )
	{
		int s;
		double c;

		//horizontal concentric
		c = a; while (fabs(c-a)<0.5) c=a+random( -M_PI,M_PI );
		s = ( c>a )?1:-1;
		while( isRunning() && s*a<s*c ) moveRAB(0,s*dA*2/r,0);
		q[(n-1)%N] = true;

		//vertical concentric
		c = b; while (fabs(c-b)<0.5) c=random( M_PI/10,M_PI*9/10 );
		s = ( c>b )?1:-1;
		while( isRunning() && s*b<s*c ) moveRAB(0,0,s*dB*2/r);
		q[(n-1)%N] = true;
	}
	return 0;
}

