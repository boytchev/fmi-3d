
/// Project:	FMI 3D (2013)
/// File:		27351 Texture pixelization and noise.cpp
/// Topic:		27 Textures
/// Slide:		35
///	About:		Texture pixelization and noise
///
/// History:
///		2013.07.28/PB	Initial implementation
///     2013.09.15/PB   Fixed underground artefacts


#include "fmi3d.h"

Cube3D* c;

void drawScene()
{
	glDisable( GL_FOG );
	glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST );
	c->drawImage();
}


void drawFrames()
{
}


int main()
{
	/// Open window
	if( !openWindow3D( "Texture pixelization and noise" ) ) return 1;
	changeBrightness();
	noGround();

	/// Add buttons
	addButton( new exitButton2D );

	viewZ = 0;
	viewBeta = 0.35;

	double s = 1000;
	c = new Cube3D( 2*s );
	c->setScale({2*s,2*s,0.02});
	c->setCenter( {0,0,0} );
	c->setTexture(loadTexture("27351 Texture.jpg"));
	c->setTextureSize(150);
	c->setLighting(false);

	/// Main loop
	double t0 = t;
	while( isRunning() )
	{
		viewAlpha += (t-t0)/50;
		t0 = t;
	}
	return 0;
}

