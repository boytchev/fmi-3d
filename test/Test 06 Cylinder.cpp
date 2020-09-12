
/// Project:	FMI 3D (2013)
/// File:		Test 06 Cylinder.cpp
///	About:		Cylinder object
///				Models several instances of the cylinder class.
///
/// History:
///		2013.05.05/PB	Initial implementation
///		2013.05.09/PB	Updated


#include "fmi3d.h"

#define MAX_PLANE 25
Cylinder3D* plane[MAX_PLANE];
double rot[MAX_PLANE];

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
	if( !openWindow3D( "Test 06: Cylinder" ) ) return 1;
	changeGround();
	changeGround();

	GLuint tex = loadTexture("textures/material_paper.jpg");
	GLuint tix = loadTexture("textures/material_scratch.jpg");

	for(int i=0; i<MAX_PLANE; i++)
	{
		plane[i] = new Cylinder3D( random(0.25,2), random(1,4) );
		plane[i]->setTexture(i==0?tix:tex);
		plane[i]->setTextureScale({0.5,0.5,3,1});
		rot[i] = random(0,360);
	}

	while( isRunning() )
	{
		for(int i=0; i<MAX_PLANE; i++)
		{
			plane[i]->identity();
			plane[i]->setCenter({0,7+3*sin(i),3+2*sin(t+2*i)});
			plane[i]->rotateGlobal(-50*(5+t+2*i+0.01*sin(3*t+i)),{0,0,1});
			plane[i]->rotateLocal(20*t+rot[i],{0,1,0});
			plane[i]->rotateLocal(270*sin(t+rot[i]),{0,0,1});
		}
	}
	return 0;
}

