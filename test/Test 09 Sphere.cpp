
/// Project:	FMI 3D (2013)
/// File:		Test 09 Sphere.cpp
///	About:		Sphere object
///				Models several instances of the sphere class.
///
/// History:
///		2013.05.06/PB	Initial implementation
///		2013.05.09/PB	Updated


#include "fmi3d.h"

#define MAX_PLANE 50
Sphere3D* plane[MAX_PLANE];

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
	if( !openWindow3D( "Test 09: Sphere" ) ) return 1;

	GLuint tex = loadTexture("textures/ground_tiles.jpg");
	changeGround();
	changeGround();
	changeGround();
	changeBrightness();

	for(int i=0; i<MAX_PLANE; i++)
	{
		plane[i] = new Sphere3D( random(0.25,1.5) );
		plane[i]->setTexture(tex);
		plane[i]->setTextureScale({2,1});
		plane[i]->setColor({random(0,1),random(0,1),random(0,1)});
	}

	while( isRunning() )
	{
		for(int i=0; i<MAX_PLANE; i++)
		{
			plane[i]->identity();
			plane[i]->setCenter({0,7+3*sin(i),3+2*sin(t+2*i)});
			plane[i]->rotateLocal(90,{0,1,0});
			double s = 0.6+0.1*sin(14*t+i);
			plane[i]->scaleLocal({s,s,1.3});
			plane[i]->rotateLocal(-(20.0+5*sin(5*i))*t,{0,0,1});
			plane[i]->rotateGlobal(50*(5+t+2*i+0.01*sin(3*t+i)),{0,0,1});
		}
	}
	return 0;
}

