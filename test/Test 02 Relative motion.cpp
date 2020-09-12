
/// Project:	FMI 3D (2013)
/// File:		Test 02 Relative motion.cpp
///	About:		Object orientation and animation
///				Models relative motion - i.e. motion is dependent on how fast or
///				slow is the machine. Usually, relative animation is done by
///				changing the properties every frame, independent on how much
///				time has elapsed.
///
/// History:
///		2013.04.30/PB	Initial implementation of rotation and panning
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
	if( !openWindow3D( "Test 02: Relative motion" ) ) return 1;

	cube1 = new Cube3D( 3 );
	cube2 = new Cube3D( 2 );

	cube1->setColor( {0,0,1} );
	cube2->setColor( {1,0,0} );
	cube2->setCenter({0,3,1.414});

	while( isRunning() )
	{
		cube1->setCenter({0,0,1.5+fabs(3*sin(frame/70.0))});
		cube1->rotateLocal( 1, {0,0,1} );

		cube2->rotateGlobal( 1, {0,0,1} );
		cube2->rotateLocal( -2, {0,1,0} );
	}
	return 0;
}

