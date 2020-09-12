
/// Project:	FMI 3D (2013)
/// File:		Test 03 Textured cube.cpp
///	About:		Textured cube
///				Models texture mapped onto a cube.
///
/// History:
///		2013.05.01/PB	Initial implementation of rotation and panning
///		2013.05.09/PB	Updated


#include "fmi3d.h"

Cube3D* cube;

void drawScene()
{
	cube->drawImage();
}

void drawFrames()
{
	cube->drawFrame();
}

int main()
{
	if( !openWindow3D( "Test 03 Textured cube" ) ) return 1;

	cube = new Cube3D( 5 );
	cube->setCenter({0,0,5});

	GLuint texId = loadTexture("textures/Metal.jpg");
	cube->setTexture(texId);

	while( isRunning() )
	{
		cube->clearOrientation();
		cube->rotateLocal( 43*t, {0,0,1} );
		cube->rotateLocal( 32*t, {0,1,0} );
		cube->rotateLocal( 21*t, {1,0,0} );
	}
	return 0;
}

