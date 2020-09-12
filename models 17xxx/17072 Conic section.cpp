
/// Project:	FMI 3D (2013)
/// File:		17072 Conic section.cpp
/// Topic:		17 Cutting
/// Slide:		07
///	About:		Conic section with plane
///
/// History:
///		2013.08.08/PB	Initial implementation


#include "fmi3d.h"

Cone3D *c[3];
GLdouble cp[3][4]= {{1,0,0,0},{0,0,-1,0},{0,0,0,2}};

void drawScene()
{
	for( int i=0; i<3; i++ )
	{
		glEnable( GL_CLIP_PLANE2 );
		glClipPlane( GL_CLIP_PLANE2,cp[i] );
		c[i]->drawImage();
		glDisable( GL_CLIP_PLANE2 );
	}
}


void drawFrames()
{
}


int main()
{
	/// Open window
	if( !openWindow3D( "Conic section" ) ) return 1;

	/// Add buttons
	addButton( new exitButton2D );

	viewAlpha = -M_PI;

	for(int i=0; i<3; i++)
	{
		c[i] = new Cone3D( 3,7,false,QUALITY_HIGH );
		c[i]->setCenter( {0,7.0*(i-1),0} );
		c[i]->setColor({0.5,0.5,1});
	}

	/// Main loop
	while( isRunning() )
	{
		cp[0][3] = 2*sin( t*0.8 );
		cp[1][3] = 3+2*cos( t );
		double q = -0.75+0.25*sin(t);
		cp[2][0] = cos( q );
		cp[2][2] = sin( q );
		cp[2][3] = 2.5+0.5*cos( 1.5*q );
	}
	return 0;
}

