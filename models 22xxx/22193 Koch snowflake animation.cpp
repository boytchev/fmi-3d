
/// Project:	FMI 3D (2013)
/// File:		22191 Koch snowflake.cpp
/// Topic:		22 Fractals
/// Slide:		19
///	About:		Koch snowflake at various levels
///
/// History:
///		2013.05.19/PB	Initial implementation
///     2013.09.13/PB   Changed button key


#include "fmi3d.h"

//#include <iostream>


#define LEVELS 4
GLuint imageList;

double dir;
double x;
double y;

bool bright = false;

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
	double q = t/30;
	glPushMatrix();
	glLineWidth( 0.5 );
	for( int i=0; i<100; i++ )
	{
		if( !bright )
		{
			if( i<30 )
				glColor3f( 0,0,i/30.0 );
			else
				glColor3f( ( i-30 )/30.0,( i-30 )/30.0,1 );
		}
		else
		{
			if( i<30 )
				glColor3f( 1-i/30.0, 1-i/30.0, 1 );
			else
				glColor3f( 0,0,(100-i)/140.0 );
		}
		glTranslatef( sin( 20*q )*i/50.0,cos( 53*q )*i/50.0,0 );
		glRotatef( ( i/3.0+1 )*sin( q*20 )/1,0,0,1 );
		float k = 0.94;
		glScalef( k,k,k );
		glCallList( imageList );
	}
	glPopMatrix();
}


void drawFrames()
{
}



void brightDark()
{
	bright = !bright;
	changeBrightness();
}


int main()
{
	/// Open window
	if( !openWindow2D( "Koch snowflake animation" ) ) return 1;
	changeBrightness();
	noGround();

	/// Add buttons
	addButton( "buttons/button_light.png", GLFW_KEY_SPACE, brightDark );
	addButton( new exitButton2D );

	/// Generate image lists for Koch snowflakes
	double len=20;
	imageList = ++UID;
	glNewList( imageList,GL_COMPILE );
	{
		x = -len/2;
		y = len*sqrtf( 3 )/6;
		dir = 0;

		glDisable( GL_TEXTURE_2D );
		glEnable( GL_LINE_SMOOTH );
		glDepthMask( GL_FALSE );
		glBegin( GL_LINE_STRIP );
		{
			fd( 0 );
			drawKochSnowflake( len,LEVELS );
		}
		glEnd();
		glDepthMask( GL_TRUE );
	}
	glEndList();



	/// Main loop
	while( isRunning() )
	{
	}

	return 0;
}

