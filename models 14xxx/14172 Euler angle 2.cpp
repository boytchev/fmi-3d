
/// Project:	FMI 3D (2013)
/// File:		14172 Euler angle 2.cpp
/// Topic:		14 Orientation
/// Slide:		17
///	About:		Euler angle 2
///
/// History:
///		2013.08.10/PB	Initial implementation


#include "fmi3d.h"

Sphere3D *s;
CoordSys3D *cs,*cse;

void drawScene()
{
	glDisable( GL_TEXTURE_2D );
	cs->drawImage();
	cse->drawImage();

	glEnable( GL_DEPTH_TEST );
	glDepthMask( GL_TRUE );

	s->setPolygonMode( GL_FILL );
	s->setColor( {0.75,0.75,1} );
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	s->drawImage();
	glCullFace(GL_BACK);
	s->drawImage();
	glDisable(GL_CULL_FACE);

	glEnable(GL_LINE_SMOOTH);
	s->setPolygonMode( GL_LINE );
	s->setColor( {0,0,0} );
	s->setScale({4.51,4.51,4.51});
	s->drawImage();
	s->setSize(4.5);
}


void drawFrames()
{
	cs->drawFrame();
	cse->drawFrame();
}


int main()
{
	/// Open window
	if( !openWindow3D( "Euler angle 2" ) ) return 1;

	/// Add buttons
	addButton( new exitButton2D );
	changeGround();
	changeGround();
	changeGround();

	viewAlpha = M_PI/6;
	viewZ = 5;

	s = new Sphere3D( 4.5,QUALITY_LOW );
	s->setShadeModel( GL_FLAT );
	s->setScale({5,5,-5});
	s->setCenter({0,0,5});

	cs = new CoordSys3D( -5,8,-5,8,-5,6.2, false, 0, 0.3, L"XYZ");
	cs->setAxesNamesColor( {0,0,0} );
	cs->setCenter({0,0,5});
	cse = new CoordSys3D( 0.2,6,0.2,6,0.2,6, false, 0, 0.5, L"XYZ");
	cse->setColor({0,0,0.9});
	cse->setAxesNamesColor( {0,0,1} );
	cse->setCenter({0,0,5});


	/// Main loop
	while( isRunning() )
	{
		double a = 32-32*cos(1.5*t);
		cse->clearOrientation();
		cse->rotateLocal(60,{0,0,1});
		cse->rotateLocal(a,{1,0,0});
		s->clearOrientation();
		s->rotateLocal(60,{0,0,1});
		s->rotateLocal(a,{1,0,0});
	}
	return 0;
}

