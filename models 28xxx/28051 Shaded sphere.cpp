
/// Project:	FMI 3D (2013)
/// File:		28051 Shaded sphere.cpp
/// Topic:		28 Light
/// Slide:		05
///	About:		A shaded and not-shaded spheres
///
/// History:
///		2013.07.06/PB	Initial implementation
///     2013.09.13/PB   Increased sphere smoothness


#include "fmi3d.h"


Sphere3D *s1, *s2;

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
	if( !openWindow3D( "Shaded sphere" ) ) return 1;

	/// Add buttons
	addButton( new exitButton2D );

	viewAlpha = 0;
	viewZ = 5;
	viewDistance = 20;

	s1 = new Sphere3D(5,QUALITY_HIGH);
	s1->setCenter({0,6,6});
	s1->setColor({0.5,0.5,1});

	s2 = new Sphere3D(5);
	s2->setCenter({0,-6,6});
	s2->setColor({0.5,0.5,1});
	s2->setLighting(false);

	/// Main loop
	while( isRunning() )
	{
	}
	return 0;
}

