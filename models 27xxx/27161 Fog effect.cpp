
/// Project:	FMI 3D (2013)
/// File:		27161 Fog effect.cpp
/// Topic:		27 Textures
/// Slide:		07
///	About:		A fog effect
///
/// History:
///		2013.07.24/PB	Initial implementation


#include "fmi3d.h"

#define ANIMATION_TIME 2
#define N 8
Sphere3D *s[N];

double fogD = 10;
void drawScene()
{
	glEnable(GL_FOG);
	glFogf(GL_FOG_START,viewDistance-fogD/2.0-2);
	glFogf(GL_FOG_END,viewDistance+fogD-1);
	for(int i=0; i<N; i++) s[i]->drawImage();
	glDisable(GL_FOG);
}


void drawFrames()
{
}


Button2D *mF;
double fogT;
double fogDD = 10;
void changeFog()
{
	fogT = t;
	fogDD = 12-fogDD;
}


int main()
{
	/// Open window
	if( !openWindow3D( "Fog effect" ) ) return 1;
	noGround();
	viewBeta = 0.2;
	viewZ = 0;
	viewAlpha = 0;


	/// Add buttons
	mF = new Button2D( "buttons/button_toggle.png",GLFW_KEY_SPACE,changeFog );
	addButton( new exitButton2D );

	GLuint earth = loadTexture("27161 Texture Earth.jpg");
	GLuint moon = loadTexture("27161 Texture Moon.jpg");
	for( int i=0; i<N; i++ )
	{
		s[i] = new Sphere3D( (i==0)?3:1 );
		s[i]->setTexture( (i==0)?earth:moon );
		s[i]->setTextureScale( {1,-1} );
	}

	/// Main loop
	double t0 = t;
	double sp = 0;
	while( isRunning() )
	{
		sp += 20*( t-t0 );
		t0 = t;

		if( t-fogT<ANIMATION_TIME )
		{
			double k = cos( ( ( t-fogT )/ANIMATION_TIME )*M_PI/2 );
			fogD = fogD*k+fogDD*( 1-k );
		}

		for( int i=0; i<N; i++ )
		{
			s[i]->identity();
			s[i]->rotateLocal( (i?0:1)*sp, {0,0,1} );
			if( i==0) s[i]->rotateGlobal( -23, {1,0,0} );
			s[i]->translateGlobal( {(i==0)?0:8.0,0,0} );
			if( i ) s[i]->rotateGlobal( -sp+360.0*i/(N-1.0), {0,0,1} );
		}
	}
	return 0;
}

