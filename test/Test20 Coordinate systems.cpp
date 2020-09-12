
/// Project:	FMI 3D (2013)
/// File:		Test20 Coordinate systems.cpp
///	About:		2D and 3D coordinate systems
///				Showing and manipulating 2D and 3D coordinate systems.
///
/// History:
///		2013.05.22/PB	Initial implementation


#include "fmi3d.h"


CoordSys2D* cs1;
CoordSys2D* cs2;
CoordSys3D* cs3;

void drawScene()
{
	cs1->drawImage();
	cs2->drawImage();
	cs3->drawImage();
}

void drawFrames()
{
	cs1->drawFrame();
	cs2->drawFrame();
	cs3->drawFrame();
}



int main()
{
	if( !openWindow3D( "Test 20: Coordinate systems" ) ) return 1;

	addButton( new exitButton2D );

	cs1 = new CoordSys2D(-1,5,-1,2,true,3,0.5,L"UV");
	cs1->setCenter({0,6,3});
	cs1->rotateLocal(90,{1,0,0});
	cs1->rotateLocal(90,{0,1,0});

	cs2 = new CoordSys2D(-2,2,0,2,false,10,0.5,L"AB");
	cs2->setCenter({5,-5,3});
	cs2->rotateLocal(90,{1,0,0});
	cs2->rotateLocal(90,{0,1,0});
	cs2->setColor({0,0.5,0});
	cs2->setAxesNamesColor({0,1,0});

	cs3 = new CoordSys3D(-4,4,-4,4,-2,2,true,2,0.5,L"XYZ");
	cs3->setCenter({-3,-3,3});
	cs3->setAxesNamesColor({1,0,0});

	while( isRunning() )
	{
		cs1->clearOrientation();
		cs1->rotateLocal(90,{1,0,0});
		cs1->rotateLocal(20*t,{0,1,0});

		cs2->clearOrientation();
		cs2->rotateLocal(90,{1,0,0});
		cs2->rotateLocal(50*t,{0,0,1});

		cs3->clearOrientation();
		cs3->rotateLocal(180*sin(t/2),{0,0,1});
		cs3->rotateLocal(4*sin(5*t),{1,0,0});
		cs3->rotateLocal(4*cos(7*t),{0,1,0});
	}
	return 0;
}

