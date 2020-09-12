
/// Project:	FMI 3D (2013)
/// File:		Test 15 Full interactivity in 3D.cpp
///	About:		Full interavtivity
///				Models full custom user-defined mouse and keyboard interactivity.
///
/// History:
///		2013.05.12/PB	Initial implementation


#include "fmi3d.h"
//#include <iostream>

#define MAX_OBJECTS 10
Sphere3D* obj[MAX_OBJECTS];

void drawScene()
{
	for( int i=0; i<MAX_OBJECTS; i++ ) obj[i]->drawImage();
}

void drawFrames()
{
	for( int i=0; i<MAX_OBJECTS; i++ ) obj[i]->drawFrame();
}


void keys( int key )
{
	if( (key==GLFW_KEY_LEFT)     || (key==GLFW_KEY_KP_4) ) activeObject->iMove(-10,0);
	if( (key==GLFW_KEY_RIGHT)    || (key==GLFW_KEY_KP_6) ) activeObject->iMove(+10,0);
	if( (key==GLFW_KEY_UP)       || (key==GLFW_KEY_KP_8) ) activeObject->iMove(0,-10);
	if( (key==GLFW_KEY_DOWN)     || (key==GLFW_KEY_KP_2) ) activeObject->iMove(0,+10);
	if( (key==GLFW_KEY_PAGEDOWN) || (key==GLFW_KEY_KP_3) ) activeObject->iScale(-10,0.1,10);
	if( (key==GLFW_KEY_PAGEUP)   || (key==GLFW_KEY_KP_9) ) activeObject->iScale(10,0.1,10);
}

void mice( int dX, int dY )
{
	if( glfwGetMouseButton( GLFW_MOUSE_BUTTON_LEFT ) )  activeObject->iMove(dX,dY);
	if( glfwGetMouseButton( GLFW_MOUSE_BUTTON_RIGHT ) ) activeObject->iScale(-dY,0.1,10);
}


int main()
{
	if( !openWindow3D( "Test 15: Full interactivity in 3D" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();
	changeBrightness();

	Button2D* b1 = new lightButton2D;
	Button2D* b2 = new groundButton2D;
	Button2D* b3 = new exitButton2D;

	GLuint tex = loadTexture( "textures/ground_tiles.jpg" );

	for( int i=0; i<MAX_OBJECTS; i++ )
	{
		obj[i] = new Sphere3D( 1 );
		obj[i]->setCenter( {random( -5,5 ),random( -5,5 ),0} );
		obj[i]->setTexture( tex );
		obj[i]->setTextureScale( {2,2,2} );
		obj[i]->setColor( {1,1,1} );
		obj[i]->interactive();
		obj[i]->setMouseMove( mice );
		obj[i]->setKeyPress( keys );
	}

	while( isRunning() )
	{
		for( int i=1; i<MAX_OBJECTS; i++ )
		{
			obj[i]->clearOrientation();
			obj[i]->rotateLocal( 50*( t+i )*( 1+2*sin( i*(i+7) ) ), {0,0,1} );
		}
	}

	return 0;
}

