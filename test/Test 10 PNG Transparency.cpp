
/// Project:	FMI 3D (2013)
/// File:		Test 10 PNG Transparency.cpp
///	About:		PNG Transparency
///				Models several instances of the sphere class.
///
/// History:	2013.05.07/PB	Initial implementation


#include "fmi3d.h"

#define MAX_PLANE 50
Sphere3D* plane[MAX_PLANE];
Button2D* but1;
Button2D* but2;
Button2D* but3;

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
	if( !openWindow3D( "Test 10: PNG Transparency" ) ) return 1;

	GLuint tex = loadTexture("textures/ground_marble.jpg");

	but1 = new Button2D("buttons/button_light.png",'L',changeBrightness);
	but2 = new Button2D("buttons/button_ground.png",'G',changeGround);
	but3 = new Button2D("buttons/button_exit.png",GLFW_KEY_ESC,glfwCloseWindow);

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
			plane[i]->rotateLocal(-(20.0+5*sin(5*i))*t,{0,0,1});
			plane[i]->rotateGlobal(50*(5+t+2*i+0.01*sin(3*t+i)),{0,0,1});
		}
	}
	return 0;
}

