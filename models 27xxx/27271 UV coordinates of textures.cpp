
/// Project:	FMI 3D (2013)
/// File:		27271 UV coordinates of textures.cpp
/// Topic:		27 Textures
/// Slide:		27
///	About:		27271 UV coordinates of textures
///
/// History:
///		2013.07.24/PB	Initial implementation


#include "fmi3d.h"

Cube3D *c[4];

void drawScene()
{
	for( int i=0; i<4; i++ ) c[i]->drawImage();
}


void drawFrames()
{
	for( int i=0; i<4; i++ ) c[i]->drawFrame();
}



int main()
{
	/// Open window
	if( !openWindow2D( "UV coordinates of textures" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();


	/// Add buttons
	addButton( new exitButton2D );

	GLuint gr = loadTexture("27271 Texture Grid.jpg");
	GLuint ml = loadTexture("27271 Texture Mona Lisa.jpg");

	for(int i=0; i<4; i++)
	{
		c[i] = new Cube3D(5);
		c[i]->setTextureScale({1,-1});
	}

	c[0]->setTexture(gr);
	c[0]->setCenter({-6,0,0});

	c[1]->setTexture(ml);
	c[1]->setCenter({+6,0,0});
	c[1]->setScale({4,5,1});

	c[2]->setTexture(gr);
	c[2]->setCenter({0.25,0,0});

	c[3]->setTexture(ml);
	c[3]->setCenter({0.25,0,0.1});

	/// Main loop
	while( isRunning() )
	{
		c[3]->setColor({1,1,1,0.5+0.5*sin(t)});
		c[2]->setScale({4+sin(t*1.567),5+2*cos(t*1.456),1});
		c[3]->setScale({4+sin(t*1.567),5+2*cos(t*1.456),1});
		c[2]->clearOrientation();
		c[2]->rotateLocal(10*cos(t*0.7),{0,0,1});
		c[3]->clearOrientation();
		c[3]->rotateLocal(10*cos(t*0.7),{0,0,1});
	}
	return 0;
}

