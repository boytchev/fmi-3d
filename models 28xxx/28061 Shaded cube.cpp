
/// Project:	FMI 3D (2013)
/// File:		28061 Shaded cube.cpp
/// Topic:		28 Light
/// Slide:		06
///	About:		A shaded and not-shaded cubes
///
/// History:
///		2013.07.06/PB	Initial implementation


#include "fmi3d.h"


Cube3D *s1, *s2;

void drawScene()
{
	s1->drawImage();
	s2->drawImage();
}


void drawFrames()
{
}


int main()
{
	/// Open window
	if( !openWindow3D( "Shaded cube" ) ) return 1;

	/// Add buttons
	addButton( new exitButton2D );

	viewAlpha = 0;
	viewZ = 5;
	viewDistance = 20;

	s1 = new Cube3D(7);
	s1->setCenter({0,6,6});
	s1->setColor({0.5,0.5,1});

	s2 = new Cube3D(7);
	s2->setCenter({0,-6,6});
	s2->setColor({0.5,0.5,1});
	s2->setLighting(false);

	/// Main loop
	while( isRunning() )
	{
		s1->clearOrientation();
		s1->rotateLocal(19*t,{0,0,1});
		s1->rotateLocal(13*t,{0,1,0});

		s2->clearOrientation();
		s2->rotateLocal(19*t,{0,0,1});
		s2->rotateLocal(13*t,{0,1,0});
	}
	return 0;
}

