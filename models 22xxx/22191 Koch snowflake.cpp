
/// Project:	FMI 3D (2013)
/// File:		22191 Koch snowflake.cpp
/// Topic:		22 Fractals
/// Slide:		19
///	About:		Koch snowflake at various levels
///
/// History:
///		2013.05.19/PB	Initial implementation
///     2013.09.13/PB   Changed button image


#include "fmi3d.h"

//#include <iostream>


#define LEVELS 6
GLuint imageLists[LEVELS];
int showLevel = 0;
bool showAll = false;

double dir;
double x;
double y;


void fd( double dist )
{
	x = x + dist*cos( dir );
	y = y + dist*sin( dir );
	glVertex3f( x,y,1 );
}

void lt( double angle )
{
	dir = dir + angle*M_PI/180;
}

void rt( double angle )
{
	dir = dir - angle*M_PI/180;
}

void drawKochSegment( double len, int level )
{
	if( level )
	{
		len /= 3.0;
		level--;
		drawKochSegment( len,level );
		lt( 60 );
		drawKochSegment( len,level );
		rt( 120 );
		drawKochSegment( len,level );
		lt( 60 );
		drawKochSegment( len,level );
	}
	else
	{
		fd( len );
	}
}


void drawKochSnowflake( double len, int level )
{
	drawKochSegment( len,level );
	rt( 120 );
	drawKochSegment( len,level );
	rt( 120 );
	drawKochSegment( len,level );
}


void drawScene()
{
	if( showAll )
	{
		glLineWidth( 0.5 );
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(3,0xAAAA);
		for( int i=0; i<showLevel; i++ )
		{
			glCallList( imageLists[i] );
		}
		glDisable(GL_LINE_STIPPLE);
	}

	glLineWidth( 3-showLevel/2.0 );
	glCallList( imageLists[showLevel] );
}


void drawFrames()
{
}


Button2D* nL;
void nextLevel()
{
	showLevel = (showLevel+1)%LEVELS;
	nL->setState(showLevel);
}


void showHide()
{
	showAll = !showAll;
}


int main()
{
	/// Open window
	if( !openWindow2D( "Koch snowflake" ) ) return 1;
	noGround();

	/// Add buttons
	nL = new Button2D( "buttons/button_n.png", GLFW_KEY_SPACE, nextLevel, showLevel, LEVELS);
	addButton( "buttons/button_show.png", 'S', showHide);
	addButton( new exitButton2D );

	/// Generate image lists for Koch snowflakes
	for( int i=0; i<LEVELS; i++ )
	{
		double len=10;
		imageLists[i] = ++UID;
		glNewList( imageLists[i],GL_COMPILE );
		{
			x = -len/2;
			y = len*sqrtf( 3 )/6;
			dir = 0;

			glColor3f( 0,0,0 );
			glDisable( GL_TEXTURE_2D );
			glEnable( GL_LINE_SMOOTH );
			glDepthMask( GL_FALSE );
			glBegin( GL_LINE_STRIP );
			{
				fd( 0 );
				drawKochSnowflake( len,i );
			}
			glEnd();
			glDepthMask( GL_TRUE );
		}
		glEndList();
	}


	/// Main loop
	while( isRunning() )
	{
	}

	return 0;
}

