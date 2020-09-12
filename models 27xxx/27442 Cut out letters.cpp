
/// Project:	FMI 3D (2013)
/// File:		27442 Cut out letters.cpp
/// Topic:		27 Textures
/// Slide:		44
///	About:		Using textures to cut out
///
/// History:
///		2013.07.28/PB	Initial implementation


#include "fmi3d.h"

Cylinder3D *s1;
Sphere3D *s2;

void drawScene()
{
	glPushAttrib(GL_FOG_BIT);
	glFogf(GL_FOG_START,viewDistance-3);
	glFogf(GL_FOG_END,viewDistance+8);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_FOG);
	s1->drawImage();
	glDisable(GL_FOG);

	glDepthMask(GL_FALSE);
	s2->drawImage();
	glCullFace(GL_FRONT);
	s2->drawImage();
	glDepthMask(GL_TRUE);


	glEnable(GL_FOG);
	s1->drawImage();
	glDisable(GL_FOG);
	glDisable(GL_CULL_FACE);

	glPopAttrib();
}


void drawFrames()
{
}


int sign(double x)
{
	if( x>0 ) return 1;
	return 0;
}

int main()
{
	/// Open window
	if( !openWindow3D( "Cut out letters" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();

	/// Add buttons
	addButton( new exitButton2D );

	viewZ = 4;
	viewDistance = 17;
	viewBeta = 0.1;

	GLuint tex2 = loadTexture("27431 Texture Earth.png");
	GLuint tex1 = loadTexture("27442 Texture.png");

	s1 = new Cylinder3D( 4,8,false,QUALITY_HIGH );
	s1->setCenter( {0,0,-0.05} );
	s1->setTexture(tex1);
	s1->setTextureScale({1,-1,4,-2});
	s1->setColor({0,0,1,1});
	s1->setLighting(false);

	s2 = new Sphere3D( 3 );
	s2->setCenter( {0,0,5} );
	s2->setTexture(tex2);
	s2->setColor({1,0,0,0.7});
	s2->setLighting(false);

	/// Main loop
	double k=0;
	double tk = 0;
	double t0 = t+0.1;
	while( isRunning() )
	{
		if( sign(sin(3*t))!=sign(sin(3*t0)) )
		{
			k = 0.5;
			tk = 0;
		}
		tk = tk+20*(t-t0);
		t0 = t;

		k = k*0.9;
		viewZ = 4+k*sin(tk);

		s1->clearOrientation();
		s1->rotateLocal(20*t,{0,0,1});

		s2->clearOrientation();
		s2->setCenter({0,0,3+5*fabs(sin(3*t))});
		s2->rotateLocal(30*t,{1,1,0});
		s2->rotateLocal(41*t,{0,1,1});
	}
	return 0;
}

