
/// Project:	FMI 3D (2013)
/// File:		23531 Bspline base functions.cpp
/// Topic:		23 Curves
/// Slide:		53
///	About:		B-Spline base functions
///
/// History:
///		2013.05.17/PB	Initial implementation


#include "fmi3d.h"

//#include <iostream>

#define ANIM_TIME 1
#define MAX_POINTS 100
vect_t p[MAX_POINTS+1];
double offset=0;
double t0;
bool split = false;

Object3D* coordSys;

/// Calculate a value of base function for t in [0,1]
vect_t baseFunctions( double t )
{
	double B0 = ( -t*t*t+3*t*t-3*t+1 )/6;
	double B1 = ( 3*t*t*t-6*t*t+4 )/6;
	double B2 = ( -3*t*t*t+3*t*t+3*t+1 )/6;
	double B3 = t*t*t/6;

	return {B0,B1,B2,B3};
}


void drawBaseFunctions()
{
	glPushMatrix();
	glDepthMask( GL_FALSE );
	glLineWidth( 10 );
	glEnable( GL_LINE_SMOOTH );
	glDisable( GL_LINE_STIPPLE );
	glDisable( GL_TEXTURE_2D );

	glTranslated( 2*offset,0,0 );
	glColor3f( 0,0,1 );
	glBegin( GL_LINE_STRIP );
	for( int i=0; i<=MAX_POINTS; i++ )
	{
		glVertex3f( i/( double )MAX_POINTS,p[i].x,1 );
	}
	glEnd();

	glTranslated( -offset,0,0 );
	glColor3f( 0,1,0 );
	glBegin( GL_LINE_STRIP );
	for( int i=0; i<=MAX_POINTS; i++ )
		glVertex3f( i/( double )MAX_POINTS,p[i].y,1 );
	glEnd();

	glTranslated( -offset,0,0 );
	glColor3f( 1,0,0 );
	glBegin( GL_LINE_STRIP );
	for( int i=0; i<=MAX_POINTS; i++ )
		glVertex3f( i/( double )MAX_POINTS,p[i].z,1 );
	glEnd();

	glTranslated( -offset,0,0 );
	glColor3f( 0,0,0 );
	glBegin( GL_LINE_STRIP );
	for( int i=0; i<=MAX_POINTS; i++ )
		glVertex3f( i/( double )MAX_POINTS,p[i].w,1 );
	glEnd();

	glDepthMask( GL_TRUE );
	glPopMatrix();
}


void drawScene()
{
	glPushMatrix();
	glScaled( 10,10,1 );
	drawBaseFunctions();
	coordSys->drawImage();
	glPopMatrix();
}


void drawFrames()
{
}


void splitOrJoin()
{
	t0 = t;
	split = !split;
}


void showOrHide()
{
	coordSys->setIsVisible( !coordSys->getIsVisible() );
}


int main()
{
	/// Open window
	if( !openWindow2D( "Bspline base functions" ) ) return 1;
	noGround();
	viewX = 5;
	viewY = 5.5;
	viewDistance = 25;

	/// Add buttons
	addButton( "buttons/button_toggle.png",GLFW_KEY_SPACE,splitOrJoin );
	addButton( "buttons/button_show.png",'S',showOrHide );
	addButton( new exitButton2D );

	/// Create coordinate system
	coordSys = new CoordSys2D( -1,3,0,1,true,10,0.07 );

	/// Calculate base functions
	for( int i=0; i<=MAX_POINTS; i++ )
	{
		p[i] = baseFunctions( i/( double )MAX_POINTS );
	}

	/// Main loop
	t0 = t-ANIM_TIME;
	while( isRunning() )
	{
		if( t-t0>ANIM_TIME ) t0=t-ANIM_TIME;

		double k = 0.5-0.5*cos( ( t-t0 )/ANIM_TIME * M_PI );
		if( split )
		{
			offset = k;
			viewX = 10*0.1+viewX*0.9;
			viewDistance = 45*0.1+viewDistance*0.9;
		}
		else
		{
			offset = 1-k;
			viewX = 5*0.1+viewX*0.9;
			viewDistance = 25*0.1+viewDistance*0.9;
		}

	}

	return 0;
}

