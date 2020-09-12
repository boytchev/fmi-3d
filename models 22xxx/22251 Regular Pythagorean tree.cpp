
/// Project:	FMI 3D (2013)
/// File:		22251 Regular Pythagorean tree.cpp
/// Topic:		22 Fractals
/// Slide:		25
///	About:		A Pythagorean tree with the same angle at every node
///
/// History:
///		2013.05.24/PB	Initial implementation
///     2013.09.13/PB   Changed button image


#include "fmi3d.h"

//#include <iostream>



double dir;
double x;
double y;
double offset = 0;
bool showAll = true;
double alpha = 30;
int steps = 1;


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

void drawAuxLine(double size)
{
	if( showAll )
	{
		glEnd();
		glLineWidth( 0.5 );
		glBegin( GL_LINES );
		lt( 90 );
		fd( 0 );
		fd( size );
		fd( -size );
		rt( 90 );
		glEnd();
		glLineWidth( 3 );
		glBegin( GL_LINE_STRIP );
		fd( 0 );
	}
}

void drawPythaTree( double size, double angle, int steps )
{
	angle = 45+25*sin( steps+t );

	drawAuxLine( size );
	fd( size );
	drawAuxLine( size );

	if( steps==0 )
	{
		lt( 90 );
		fd( size );
		lt( 90 );
	}
	else
	{
		angle = 45+15*sin( t/2 );
		rt( angle );
		drawPythaTree( size*cos( angle*M_PI/180 ),angle,steps-1 );
		rt( 90 );
		drawPythaTree( size*sin( angle*M_PI/180 ),angle,steps-1 );
		rt( 90-angle );
	}
	fd( size );
}

void drawScene()
{
	glLineWidth( 3 );
	glColor3f( 0,0,0 );
	glDisable( GL_TEXTURE_2D );
	glDisable( GL_LIGHTING );
	glEnable( GL_LINE_SMOOTH );
	glDepthMask( GL_FALSE );
	dir=M_PI/2;
	x=1;
	y=-4;
	glBegin( GL_LINE_STRIP );
	fd( 0 );
	drawPythaTree( 2,alpha,steps );
	glEnd();
}


void drawFrames()
{
}

int K=0;
Button2D* nS;
void nextSteps()
{
	steps = int(1.1*steps+1);
	K++;
	if( steps>13 ) {steps=1; K=1;}
	nS->setState(K);
}


void showHide()
{
	showAll = !showAll;
}


int main()
{
	/// Open window
	if( !openWindow2D( "Regular Pythagorean tree" ) ) return 1;
	noGround();

	/// Add buttons
	nS = new Button2D( "buttons/button_n.png", GLFW_KEY_SPACE, nextSteps, K, 11 );
	addButton( "buttons/button_show.png", 'S', showHide );
	addButton( new exitButton2D );


	/// Main loop
	while( isRunning() )
	{
		alpha = 45+25*sin( t );
	}

	return 0;
}

