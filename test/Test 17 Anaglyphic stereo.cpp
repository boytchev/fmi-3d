
/// Project:	FMI 3D (2013)
/// File:		Test 17 Anaglyphic stereo.cpp
///	About:		Anaglyphic stereo
///				Testing synthetic OpenGL-based red-cyan anaglyphic stereo.
///
/// History:
///		2013.05.20/PB	Initial implementation


#include "fmi3d.h"

#define MAX_BARS 13
Object3D* bar[MAX_BARS*MAX_BARS];
vect_t personality[MAX_BARS*MAX_BARS];


void drawScene()
{
	for( int i=0; i<MAX_BARS*MAX_BARS; i++ ) bar[i]->drawImage();
}

void drawFrames()
{
	for( int i=0; i<MAX_BARS*MAX_BARS; i++ ) bar[i]->drawFrame();
}

void changeStereo()
{
	if( inMono )
	{
		inMono = false;
		inAnagliphicStereo = true;
	}
	else
	{
		inMono = true;
		inAnagliphicStereo = false;
	}
}


int main()
{
	if( !openWindow3D( "Test 17: Anaglyphic stereo" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();
	changeBrightness();
	changeStereo();

	viewZ = 1;

	addButton( "buttons/button_stereo.png",'A',changeStereo );
	addButton( new exitButton2D );

	for( int i=0; i<MAX_BARS; i++ )
		for( int j=0; j<MAX_BARS; j++ )
		{
			int k=i*MAX_BARS+j;
			bar[k] = new Cube3D( 1 );
			personality[k] = {random( 0,360 ),random( 1,3 )};
			bar[k]->setColor({random(0.5,1),random(0.5,1),random(0.5,1)});
		}

	while( isRunning() )
	{
		for( int i=0; i<MAX_BARS; i++ )
			for( int j=0; j<MAX_BARS; j++ )
			{
				int k=i*MAX_BARS+j;
				bar[k]->identity();
				bar[k]->setCenter( {i-MAX_BARS/2.0,j-MAX_BARS/2.0,0} );
				bar[k]->setScale( {1,1,5+4*sin( t*personality[k].y+personality[k].x )} );
			}
	}
	return 0;
}

