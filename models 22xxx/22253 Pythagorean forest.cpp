
/// Project:	FMI 3D (2013)
/// File:		22253 Pythagorean forest.cpp
/// Topic:		22 Fractals
/// Slide:		25
///	About:		A forest of Pythagorean trees
///
/// History:
///		2013.05.25/PB	Initial implementation


#include "fmi3d.h"

//#include <iostream>


#define STEPS 9

/// PythaTree3D
class PythaTree3D : public Object3D
{
	public:
		PythaTree3D( );
		void regenerate();
		void drawPythaTree( double size, int steps );
		void drawImage();
	private:
		double dir;
		double x;
		double y;
		int n;
		double offset[STEPS];
		double speed[STEPS];
		double limit[STEPS];
		double base[STEPS];
		void fd( double dist );
		void lt( double angle );
		void rt( double angle );
};


PythaTree3D::PythaTree3D( )
{
	for( int i=0; i<STEPS; i++ )
	{
		base[i]=random( 10,80 );
		limit[i]=random( 1,5 );
		speed[i]=random( 0.5,3 );
		offset[i]=random( 0,4*M_PI );
	}
}

void PythaTree3D::fd( double dist )
{
	x = x + dist*cos( dir );
	y = y + dist*sin( dir );
	glVertex3f( x,y,1 );
}

void PythaTree3D::lt( double angle )
{
	dir = dir+angle*M_PI/180;
}

void PythaTree3D::rt( double angle )
{
	dir = dir-angle*M_PI/180;
}

void PythaTree3D::drawPythaTree( double size, int steps )
{
	n++;
	fd( size );

	if( ( steps==STEPS ) )
	{
		lt( 90 );
		fd( size );
		lt( 90 );
	}
	else
	{
		int k = ( steps+int( base[0] )+n )%STEPS;
		double angle = base[k]+limit[k]*sin( offset[k]+speed[k]*t/2+size );
		rt( angle );
		drawPythaTree( size*cos( angle*M_PI/180 ),steps+1 );
		rt( 90 );
		drawPythaTree( size*sin( angle*M_PI/180 ),steps+1 );
		rt( 90-angle );
	}
	fd( size );
}

void PythaTree3D::drawImage()
{
	prepareImageProps();
	glLineWidth( 1 );
	glDisable( GL_TEXTURE_2D );
	glDisable( GL_LIGHTING );
	glEnable( GL_LINE_SMOOTH );
	glDepthMask( GL_FALSE );
	dir=M_PI/2;
	x=1;
	y=0;
	n=0;
	glPushMatrix();
	glRotated( 90,1,0,0 );
	glBegin( GL_LINE_STRIP );
	fd( 0 );
	drawPythaTree( 1.3,0 );
	glEnd();
	glPopMatrix();
	restoreImageProps();
}

#define PYTHAS 10
PythaTree3D* pytha[PYTHAS];

void drawScene()
{
	for( int i=0; i<PYTHAS; i++ )
		pytha[i]->drawImage();
}


void drawFrames()
{
	for( int i=0; i<PYTHAS; i++ )
		pytha[i]->drawImage();
}



int main()
{
	/// Open window
	if( !openWindow3D( "Pythagorean forest" ) ) return 1;

	/// Add buttons
	addButton( new exitButton2D );

	viewZ = 4;
	for( int i=0; i<PYTHAS; i++ )
	{
		pytha[i] = new PythaTree3D();
		pytha[i]->rotateLocal( random(-30-90,30-90), {0,0,1} );
		pytha[i]->setCenter( {6*(i/3-1)+random(-3,3),6*(i%3-1)+random(-3,3),0} );
		pytha[i]->setColor( {random( 0,0.8 ),random( 0,0.8 ),random( 0,0.8 )} );
	}


	/// Main loop
	while( isRunning() )
	{
	}

	return 0;
}

