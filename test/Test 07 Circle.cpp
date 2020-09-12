
/// Project:	FMI 3D (2013)
/// File:		Test 07 Circle.cpp
///	About:		Circle object
///				Models several instances of the cylinder class.
///
/// History:
///		2013.05.06/PB	Initial implementation
///		2013.05.09/PB	Updated


#include "fmi3d.h"

Circle2D* c1;
Circle2D* c2;
Circle2D* c3;

void drawScene()
{
	c1->drawImage();
	c2->drawImage();
	c3->drawImage();
}

void drawFrames()
{
	c1->drawFrame();
	c2->drawFrame();
	c3->drawFrame();
}

int main()
{
	if( !openWindow3D( "Test 07: Circle" ) ) return 1;

	GLuint tex = loadTexture("textures/material_wood.jpg");

	c1 = new Circle2D( 4 );
	c2 = new Circle2D( 3 );
	c3 = new Circle2D( 3 );

	c1->setTexture(tex);
	c2->setTexture(tex);
	c3->setTexture(tex);

	c1->setCenter({0,0,4});

	while( isRunning() )
	{
		c1->clearOrientation();
		//c1->rotateLocal(20*t,{0,1,0});
		c1->rotateLocal(200*t,{0,0,1});

		c2->identity();
		c2->setCenter({0,-7,4});
		c2->rotateLocal(-90,{1,0,0});
		c2->rotateGlobal(100*t,{0,0,1});

		c3->identity();
		c3->setCenter({0,-7.5,4});
		c3->rotateLocal(90,{1,0,0});
		c3->rotateGlobal(100*t,{0,0,1});
	}
	return 0;
}

