
/// Project:	FMI 3D (2013)
/// File:		Test 13 Drag and drop.cpp
///	About:		Drag and drop
///				Models custom user-defined drag and drop.
///
/// History:
///		2013.05.09/PB	Initial implementation


#include "fmi3d.h"
//#include <iostream>

#define MAX_PLANE 10
Sphere3D* plane[MAX_PLANE];
Button2D* but;

void drawScene()
{
	for(int i=0; i<MAX_PLANE; i++) plane[i]->drawImage();
}

void drawFrames()
{
	for(int i=0; i<MAX_PLANE; i++) plane[i]->drawFrame();
}


void dragObject( int dX, int dY)
{
	vect_t p = activeObject->getCenter();
	p.x += dX*viewDistance/1000;
	p.y -= dY*viewDistance/1000;
	activeObject->setCenter(p);
}

int main()
{
	if( !openWindow2D( "Test 13: Drag and drop" ) ) return 1;
	GLuint tex = loadTexture("textures/ground_tiles.jpg");
	changeGround();
	changeGround();
	changeGround();
	changeBrightness();

	but = new Button2D("buttons/button_exit.png",GLFW_KEY_ESC,glfwCloseWindow);

	for(int i=0; i<MAX_PLANE; i++)
	{
		plane[i] = new Sphere3D( 1 );
		plane[i]->setCenter({random(-5,5),random(-5,5),0});
		plane[i]->setTexture(tex);
		plane[i]->setTextureScale({2,2,2});
		plane[i]->setColor({1,1,1});
		plane[i]->interactive();
		plane[i]->setMouseMove(dragObject);
	}

	plane[0]->setColor({1,0,0});
	plane[0]->setMouseMove(NULL);

	while( isRunning() )
	{
		for(int i=1; i<MAX_PLANE; i++)
		{
			plane[i]->clearOrientation();
			plane[i]->rotateLocal(50*(t+i)*(1+2*sin(i*i)),{0,0,1});
		}

	}

	return 0;
}

