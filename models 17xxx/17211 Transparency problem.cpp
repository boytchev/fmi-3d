
/// Project:	FMI 3D (2013)
/// File:		17211 Transparency problem.cpp
/// Topic:		17 Cutting
/// Slide:		21
///	About:		Transparency problem
///
/// History:
///		2013.08.08/PB	Initial implementation


#include "fmi3d.h"

Sphere3D *s;

void drawScene()
{
	s->setPolygonMode( GL_LINE );
	s->setColor( {0,0,0} );
	s->drawImage();

	s->setPolygonMode( GL_FILL );
	s->setColor( {0.5,0.5,1,0.5} );
	s->drawImage();
}


void drawFrames()
{
}


int main()
{
	/// Open window
	if( !openWindow3D( "Transparency problem" ) ) return 1;

	/// Add buttons
	addButton( new exitButton2D );
	changeGround();
	changeGround();
	changeGround();

	viewAlpha = -M_PI;
	viewZ = 4;

	s = new Sphere3D( 5,QUALITY_LOW );
	s->setCenter( {0,0,5} );
	s->setColor( {0.5,0.5,1,0.5} );

	/// Main loop
	while( isRunning() )
	{
		s->clearOrientation();
		s->rotateLocal(30*t,{0,0,1});
		s->rotateLocal(23*t,{1,0,0});
	}
	return 0;
}

