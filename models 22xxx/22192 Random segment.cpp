
/// Project:	FMI 3D (2013)
/// File:		22192 Random segment.cpp
/// Topic:		22 Fractals
/// Slide:		19
///	About:		Random segment based on Koch snowflake
///
/// History:
///		2013.05.19/PB	Initial implementation
///     2013.09.13/PB   Changed button image


#include "fmi3d.h"

//#include <iostream>


#define LEVELS 7
GLuint imageList;

double dir;
double x;
double y;

bool autoMode=false;

Object3D* s1;
Object3D* s2;

void fd( double dist )
{
	x = x + dist*cos( dir );
	y = y + dist*sin( dir );

	glVertex3f( 0,x,y+3 );
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
		float limit = 30+level*(autoMode?2:5);
		float angle = random( limit/2.0,limit )*( rand()%2?1:-1 );
		float len1 = len/random( 3,6 );
		float len2 = ( len-2*len1 )/cos( angle*M_PI/180.0 )/2.0;
		level--;
		drawKochSegment( len1,level );
		lt( angle );
		drawKochSegment( len2,level );
		rt( 2*angle );
		drawKochSegment( len2,level );
		lt( angle );
		drawKochSegment( len1,level );
	}
	else
	{
		fd( len );
	}
}


void drawScene()
{
	s1->drawImage();
	s2->drawImage();
	if( ((int)t)%2 || !autoMode)
		glCallList( imageList );
}


void drawFrames()
{
	drawScene();
}


void newCurve()
{
	double len=30;
	glDeleteLists( imageList,1 );
	glNewList( imageList,GL_COMPILE );
	{
		x = 15;
		y = 0;
		dir = M_PI;

		glLineWidth(2);
		glColor3f( 1,0,0 );
		glDisable( GL_TEXTURE_2D );
		glEnable( GL_LINE_SMOOTH );
		glDepthMask( GL_FALSE );
		glDisable(GL_LIGHTING);
		glBegin( GL_LINE_STRIP );
		{
			fd( 0 );
			drawKochSegment( len,LEVELS );
		}
		glEnd();
		glDepthMask( GL_TRUE );
	}
	glEndList();
}


void toggleAuto()
{
	autoMode = !autoMode;
}


int main()
{
	/// Open window
	if( !openWindow3D( "Random segment" ) ) return 1;
	//noGround();

	/// Add buttons
	addButton( "buttons/button_random.png", 'R', newCurve );
	addButton( "buttons/button_time.png", GLFW_KEY_SPACE, toggleAuto );
	addButton( new exitButton2D );

	imageList = ++UID;
	viewAlpha = 0;
	viewBeta = 0.1;
	viewDistance = 30;
	viewZ = 5;

	s1 = new Sphere3D(0.5);
	s1->setCenter({0,15,3});
	s1->setColor({0.5,0,0});
	s1->setShininess(20);

	s2 = new Sphere3D(0.5);
	s2->setCenter({0,-15,3});
	s2->setColor({0.5,0,0});
	s2->setShininess(20);

	newCurve();

	/// Main loop
	while( isRunning() )
	{
		if( autoMode ) newCurve();
	}

	return 0;
}

