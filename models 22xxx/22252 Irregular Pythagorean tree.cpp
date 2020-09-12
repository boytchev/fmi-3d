
/// Project:	FMI 3D (2013)
/// File:		22252 Irregular Pythagorean tree.cpp
/// Topic:		22 Fractals
/// Slide:		25
///	About:		A Pythagorean tree with the different angles at every node
///
/// History:
///		2013.05.24/PB	Initial implementation


#include "fmi3d.h"

//#include <iostream>


#define STEPS 20
double dir;
double x;
double y;
double offset[STEPS];
double speed[STEPS];
double limit[STEPS];


void fd( double dist )
{
	x = x + dist*cos( dir );
	y = y + dist*sin( dir );
	glVertex3f( x,y,1 );
}

void lt( double angle )
{
	dir = dir+angle*M_PI/180;
}

void rt( double angle )
{
	dir = dir-angle*M_PI/180;
}

void drawPythaTree( double size, int steps )
{
	glColor3f(0,(size<0.1)?1:0,0);
	fd( size );
	glColor3f(0,0,0);

	if( (steps==STEPS) || (size<0.01) )
	{
		lt( 90 );
		fd( size );
		lt( 90 );
	}
	else
	{
		double angle = 45+limit[steps]*sin( offset[steps]+speed[steps]*t/2+size );
		rt( angle );
		drawPythaTree( size*cos( angle*M_PI/180 ),steps+1 );
		rt( 90 );
		drawPythaTree( size*sin( angle*M_PI/180 ),steps+1 );
		rt( 90-angle );
	}
	glColor3f(0,(size<0.1)?1:0,0);
	fd( size );
	glColor3f(0,0,0);
}

void drawScene()
{
	glLineWidth( 2 );
	glColor3f( 0,0,0 );
	glDisable( GL_TEXTURE_2D );
	glDisable( GL_LIGHTING );
	glEnable( GL_LINE_SMOOTH );
	glDepthMask( GL_FALSE );
	dir=M_PI/2;
	x=1;
	y=-5;
	glBegin( GL_LINE_STRIP );
	fd( 0 );
	drawPythaTree( 1.3,0 );
	glEnd();
}


void drawFrames()
{
}



int main()
{
	/// Open window
	if( !openWindow2D( "Irregular Pythagorean tree" ) ) return 1;
	noGround();

	/// Add buttons
	addButton( new exitButton2D );


	for(int i=0; i<STEPS; i++)
	{
		limit[i]=random(15,35);
		speed[i]=random(0.5,3);
		offset[i]=random(0,4*M_PI);
	}

	/// Main loop
	while( isRunning() )
	{
	}

	return 0;
}

