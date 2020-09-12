
/// Project:	FMI 3D (2013)
/// File:		Test 05 Paper plane.cpp
///	About:		Paper plane object
///				Models several instances of the paper plane class.
///
/// History:
///		2013.05.05/PB	Initial implementation
///		2013.05.09/PB	Updated


#include "fmi3d.h"

#define MAX_PLANE 50
PaperPlane3D* plane[MAX_PLANE];

void drawScene()
{
	for(int i=0; i<MAX_PLANE; i++) plane[i]->drawImage();
}

void drawFrames()
{
	for(int i=0; i<MAX_PLANE; i++) plane[i]->drawFrame();
}

int main()
{
	if( !openWindow3D( "Test 05: Paper plane" ) ) return 1;
	changeGround();
	changeGround();

	for(int i=0; i<MAX_PLANE; i++)
	{
		plane[i] = new PaperPlane3D( random(1,2) );
	}

	while( isRunning() )
	{
		for(int i=0; i<MAX_PLANE; i++)
		{
			plane[i]->identity();
			plane[i]->setCenter({0,7+3*sin(i),3+2*sin(t+2*i)});
			plane[i]->rotateGlobal(-50*(5+t+2*i+0.01*sin(3*t+i)),{0,0,1});
			plane[i]->rotateLocal(-40*sin(t+i/2.0)+20,{1,0,0});
		}
	}
	return 0;
}

