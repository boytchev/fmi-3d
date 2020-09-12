
/// Project:	FMI 3D (2013)
/// File:		22202 Dragon surface.cpp
/// Topic:		22 Fractals
/// Slide:		20
///	About:		A dragon curve on a 3D surface
///
/// History:
///		2013.05.19/PB	Initial implementation


#include "fmi3d.h"

//#include <iostream>


#define LEVELS 12
#define SCALE 0.1
int showLevel = 0;
GLuint imageList;
GLuint frameList;

double dir;
double x;
double y;
bool wall = false;
int i;

double x1,y1,x2,y2;
void fd( double dist )
{
	i++;
	x2=x1; x1=x; y2=y1; y1=y;
	x = x + dist*cos( dir );
	y = y + dist*sin( dir );
	double d = sqrt(x*x+y*y+1000*cos(i/10.0));
	double z = 3+3*cos(0.03*d-1.2+i/1000.0);

	if( wall )
	{
        glColor3f(0,0,0);
		glVertex3d( x,y,0 );
        glColor3f(0.5+0.4*cos(i/60.0),0.5+0.4*cos(i/50.0),0.5+0.4*cos(i/70.0));
	}
	glVertex3f( x,y,1+z+(wall?0:0.01) );
}

void cell( int swap )
{
	fd( 1 );
	dir+=M_PI/4*swap;
	fd( 4 );
	dir+=M_PI/4*swap;
	fd( 1 );
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
    glEnable( GL_FOG );
    glFogf(GL_FOG_DENSITY, 0.015);
    glFogf(GL_FOG_START, 40);
    glFogf(GL_FOG_END, 120);
    glFogi(GL_FOG_MODE,GL_LINEAR);

	for( int i=0; i<4; i++ )
	{
		glRotated( 90,0,0,1 );
		glCallList( imageList );
	}
}


void drawFrames()
{
	for( int i=0; i<4; i++ )
	{
		glRotated( 90,0,0,1 );
		glCallList( frameList );
	}
}



int main()
{
	/// Open window
	if( !openWindow3D( "Dragon surface" ) ) return 1;
	noGround();
	changeBrightness();

	/// Add buttons
	addButton( new exitButton2D );

	/// Generate image lists for the dragon curve border
	frameList = ++UID;
	glNewList( frameList,GL_COMPILE );
	{
		i = 0;
		x = 4;
		y = 0;
		dir = 0;
		wall = false;

		glPushMatrix();
		glScaled( SCALE,SCALE,1 );
		glColor4f(1,1,1,0.5);
		glLineWidth(1.5);
		glDisable( GL_TEXTURE_2D );
		glEnable( GL_LINE_SMOOTH );
		glDisable(GL_LIGHTING);
		glDepthMask( GL_FALSE );
		glBegin( GL_LINE_STRIP );
		{
			fd( 0 );
			dragon( LEVELS+1,1 );
		}
		glEnd();
		glDepthMask( GL_TRUE );
		glPopMatrix();
	}
	glEndList();

	/// Generate image lists for the dragon curve wall
	imageList = ++UID;
	glNewList( imageList,GL_COMPILE );
	{
		i = 0;
		x = 4;
		y = 0;
		dir = 0;
		wall = true;

		glPushMatrix();
		glScaled( SCALE,SCALE,1 );
		glDisable( GL_TEXTURE_2D );
		glBegin( GL_QUAD_STRIP );
		{
			fd( 0 );
			dragon( LEVELS+1,1 );
		}
		glEnd();
		glPopMatrix();
	}
	glEndList();


	/// Main loop with automatic animation
	while( isRunning() )
	{
		viewX = 10*sin(t/3);
		viewY = 10*cos(t/1.5);
		viewAlpha = t/5;
		viewBeta = 0.5+0.2*sin(t/2);
		viewDistance = 40-20*sin(0.2*t);
	}

	return 0;
}

