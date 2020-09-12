
/// Project:	FMI 3D (2013)
/// File:		21401 Texture antialiasing.cpp
/// Topic:		21 Dithering and antialiasing
/// Slide:		40
///	About:		Antialiasing texture
///
/// History:
///		2013.07.30/PB	Initial implementation


#include "fmi3d.h"

GLuint tex;
bool antialiased = false;
Cube3D* c;

void drawScene()
{
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D,tex );
	if( antialiased )
	{
		glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR );
		glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR );
	}
	else
	{
		glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST );
	}

	c->drawImage();
}


void drawFrames()
{
}


void toggle()
{
	antialiased = !antialiased;
}

int main()
{
	useCustomFogColor = true;
	customFogColor[0] = 1;
	customFogColor[1] = 1;
	customFogColor[2] = 0.8;
	customFogColor[3] = 1;

	/// Open window
	if( !openWindow3D( "Texture antialiasing" ) ) return 1;
	noGround();

	/// Add buttons
	addButton( new Button2D( "buttons/button_toggle.png",GLFW_KEY_SPACE,toggle ) );
	addButton( new exitButton2D );

	viewZ = 0;
	viewBeta = minViewBeta = 0.1;

	tex = loadTexture( "textures/Ground_Tiles.jpg" );
	double s = 1000;
	c = new Cube3D( 2*s );
	c->setCenter( {0,0,-s+0.5} );
	c->setTexture( tex );
	c->setTextureSize( 150 );
	c->setLighting( false );
	c->setColor({1,1,1});

	/// Main loop
	double t0 = t;
	while( isRunning() )
	{
		viewAlpha += ( t-t0 )/50;
		t0 = t;
	}
	return 0;
}

