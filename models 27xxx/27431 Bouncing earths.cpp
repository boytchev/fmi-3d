
/// Project:	FMI 3D (2013)
/// File:		27431 Bouncing earths.cpp
/// Topic:		27 Textures
/// Slide:		43
///	About:		Bouncing earths
///
/// History:
///		2013.07.28/PB	Initial implementation
///     2013.09.15/PB   Fixed reflection of inner sphere


#include "fmi3d.h"

Sphere3D *s1, *s2;

void drawScene()
{
	glPushAttrib(GL_FOG_BIT);
	if( !inReflection )
	{
		glEnable(GL_FOG);
		glFogf(GL_FOG_START,viewDistance-3);
		glFogf(GL_FOG_END,viewDistance+5);
	}

	glEnable(GL_CULL_FACE);
	glCullFace(inReflection?GL_FRONT:GL_BACK);
	s1->drawImage();
	s2->drawImage();
	glCullFace(inReflection?GL_BACK:GL_FRONT);
	s2->drawImage();
	s1->drawImage();
	glDisable(GL_CULL_FACE);

	glDisable(GL_FOG);
	glPopAttrib();
}


void drawFrames()
{
}


int main()
{
	/// Open window
	if( !openWindow3D( "Bouncing earths" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();

	/// Add buttons
	addButton( new exitButton2D );

	viewZ = 5;
	viewDistance = 15;
	viewBeta = 0.1;

	GLuint tex = loadTexture("27431 Texture Earth.png");

	s1 = new Sphere3D( 5 );
	s1->setCenter( {0,0,5} );
	s1->setTexture(tex);
	s1->setTextureScale({1,-1});
	s1->setColor({0,0,1,0.7});
	s1->setLighting(false);

	s2 = new Sphere3D( 3 );
	s2->setCenter( {0,0,5} );
	s2->setTexture(tex);
	s2->setTextureScale({1,-1});
	s2->setColor({1,0,0,0.7});
	s2->setLighting(false);

	/// Main loop
	while( isRunning() )
	{
		s1->clearOrientation();
		s1->rotateLocal(23,{-1,1,0});
		s1->rotateLocal(20*t,{0,0,1});

		s2->clearOrientation();
		s2->setCenter({0,0,3+3.5*fabs(sin(3*t))});
		s2->rotateLocal(30*t,{1,1,0});
		s2->rotateLocal(41*t,{0,1,1});
	}
	return 0;
}

