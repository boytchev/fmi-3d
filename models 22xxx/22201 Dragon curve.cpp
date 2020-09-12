
/// Project:	FMI 3D (2013)
/// File:		22201 Dragon curve.cpp
/// Topic:		22 Fractals
/// Slide:		20
///	About:		A 2D dragon curve
///
/// History:
///		2013.05.19/PB	Initial implementation
///     2013.09.13/PB   Changed button image


#include "fmi3d.h"

//#include <iostream>


#define ANIM_TIME 2
#define LEVELS 16
int showLevel = 0;
GLuint imageLists[LEVELS];
bool showAll = false;

double dir;
double x;
double y;
double offset = 0;
double t0 = -ANIM_TIME-1;

void fd( double dist )
{
	x = x + dist*cos( dir );
	y = y + dist*sin( dir );
	glVertex3f( x,y,1 );
}

void cell( int swap )
{
	fd( 4 );
	dir+=M_PI/2*swap;
}

void nogard( int level, int swap );
void dragon( int level, int swap )
{
	if( level )
	{
		dragon( level-1,+swap );
		cell( swap );
		nogard( level-1,-swap );
	}
}

void nogard( int level, int swap )
{
	if( level )
	{
		dragon( level-1,-swap );
		cell( swap );
		nogard( level-1,swap );
	}
}


void drawScene()
{
	double lw=5.0-viewDistance/5.0;
	glLineWidth( lw>2?lw:2 );

	if( showAll || (offset>0.1) )
	{
		glRotated( offset,0,0,1 );
		glColor3f( 0,0,1 );
		glCallList( imageLists[showLevel] );
		glRotated( offset,0,0,1 );
		glColor3f( 1,0,0 );
		glCallList( imageLists[showLevel] );
		glRotated( offset,0,0,1 );
		glColor3f( 0,0.7,0 );
		glCallList( imageLists[showLevel] );
		glRotated( offset,0,0,1 );
	}
	glColor3f( 0,0,0 );
	glCallList( imageLists[showLevel] );
}


void drawFrames()
{
}

Button2D *nL;
void nextLevel()
{
	showLevel = ( showLevel+1 )%LEVELS;
	nL->setState(showLevel);
}


void showHide()
{
	if( t-t0>ANIM_TIME )
	{
		showAll = !showAll;
		t0 = t;
	}
}


int main()
{
	/// Open window
	if( !openWindow2D( "Dragon curve" ) ) return 1;
	noGround();

	/// Add buttons
	nL = new Button2D( "buttons/button_n.png", GLFW_KEY_SPACE, nextLevel, showLevel, LEVELS );
	addButton( "buttons/button_show.png", 'S', showHide );
	addButton( new exitButton2D );

	minViewDistance = 1;
	maxViewDistance = 200;

	/// Generate image lists for Koch snowflakes
	for( int i=0; i<LEVELS; i++ )
	{
		imageLists[i] = ++UID;
		glNewList( imageLists[i],GL_COMPILE );
		{
			x = 0;
			y = 0;
			dir = 0;

			glPushMatrix();
			glScaled( 0.05,0.05,1 );
			glDisable( GL_TEXTURE_2D );
			glEnable( GL_LINE_SMOOTH );
			glDepthMask( GL_FALSE );
			glBegin( GL_LINE_STRIP );
			{
				fd(0);
				dragon( i+1,1 );
			}
			glEnd();
			glDepthMask( GL_TRUE );
			glPopMatrix();
		}
		glEndList();
	}


	/// Main loop
	while( isRunning() )
	{
		viewDistance = viewDistance*0.92+0.1*pow( 1.4,showLevel );
		if( t-t0<=ANIM_TIME )
		{
			if( showAll )
				offset = 90*(0.5+0.5*cos( (t-t0)/ANIM_TIME * M_PI-M_PI));
			else
				offset = 90*(0.5-0.5*cos( (t-t0)/ANIM_TIME * M_PI-M_PI));
		}
		else
		{
			if( showAll )
				offset = 90;
			else
				offset = 0;
		}
	}

	return 0;
}

