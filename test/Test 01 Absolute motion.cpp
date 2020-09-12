
/// Project:	FMI 3D (2013)
/// File:		Test 01 Absolute motion.cpp
///	About:		Module for 3D camera navigation
///
/// History:
///		2013.04.28/PB	Initial implementation of rotation and panning
///		2013.05.09/PB	Updated


#include "fmi3d.h"

Cube3D* cube1;
Cube3D* cube2;

void drawScene()
{
	cube1->drawImage();
	cube2->drawImage();
}

void drawFrames()
{
	cube1->drawFrame();
	cube2->drawFrame();
}

int main()
{
	if( !openWindow3D( "Test 01: Absolute motion" ) ) return 1;

	cube1 = new Cube3D( 3 );
	cube2 = new Cube3D( 2 );

	cube1->setColor( {0,0,1} );
	cube2->setColor( {1,0,0} );

	while( isRunning() )
	{
		cube1->setCenter({0,0,1.5+fabs(3*sin(3*t))});
		cube1->clearOrientation();
		cube1->rotateLocal( 40*t, {0,0,1} );

		cube2->identity();
		cube2->setCenter({0,3,1.414});
		cube2->rotateGlobal( 40*t, {0,0,1} );
		cube2->rotateLocal( -100*t, {0,1,0} );
	}
	return 0;
}

