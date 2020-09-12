
/// Project:	FMI 3D (2013)
/// File:		Test 14 Object at point.cpp
///	About:		Function objectAtPoint
///				Tests object selection with mouse.
///
/// History:
///		2013.05.10/PB	Initial implementation


#include "fmi3d.h"
//#include <iostream>

#define MAX_PLANE 50
Cube3D* plane[MAX_PLANE];

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
	if( !openWindow3D( "Test 14: Object at point" ) ) return 1;
	GLuint tex = loadTexture("textures/ground_tiles.jpg");
	changeGround();
	changeGround();
	changeGround();
	changeBrightness();

	for(int i=0; i<MAX_PLANE; i++)
	{
		plane[i] = new Cube3D( random(0.25,1.5) );
		plane[i]->setTexture(tex);
		plane[i]->setTextureScale({0.5,0.5,0.5});
		plane[i]->setColor({1,1,1});
		plane[i]->interactive();
	}
	while( isRunning() )
	{
		int x;
		int y;
		glfwGetMousePos( &x,&y );

		Object3D* object = objectAtPoint( x,y );
		if( object )
		{
			object->setColor( {1,0.2,0.2} );
			object->setShininess(10);
		}

		for(int i=0; i<MAX_PLANE; i++)
		{
			plane[i]->identity();
			plane[i]->setCenter({0,7+3*sin(i),3+2*sin(t+2*i)});
			plane[i]->rotateLocal(90,{0,1,0});
			plane[i]->rotateLocal(t*50,{0,0,1});
			plane[i]->rotateGlobal(50*(5+t+2*i+0.01*sin(3*t+i)),{0,0,1});
		}

	}

	return 0;
}

