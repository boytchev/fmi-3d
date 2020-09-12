
/// Project:	FMI 3D (2013)
/// File:		29131 Stereo confusion.cpp
/// Topic:		29 Stereo graphics
/// Slide:		13
///	About:		Confusion of stereo perception
///
/// History:
///		2013.07.19/PB	Initial implementation


#include "fmi3d.h"

#define N 11
Pawn3D *p[N];

void drawScene()
{
	for(int i=0; i<N; i++) p[i]->drawImage();
}


void drawFrames()
{
}


int main()
{
	/// Open window
	if( !openWindow2D( "Stereo confusion" ) ) return 1;
	noGround();

	/// Add buttons
	addButton( new exitButton2D );

	for(int i=0; i<N; i++)
	{
		p[i] = new Pawn3D(1);
		p[i]->setLighting(false);
		p[i]->setColor({0,0,0});
		p[i]->setShininess(0);
	}

	minViewBeta = 0;

	/// Main loop
	while( isRunning() )
	{
		viewX = 0;
		viewY = 2;
		viewZ = 0;
		viewDistance = 20;

		for(int i=0; i<N; i++)
		{
			p[i]->identity();
			p[i]->rotateLocal(-90,{1,0,0});
			p[i]->setCenter({0,0,7});
			p[i]->rotateGlobal(double(i)/N*360+10*t,{0,1,0});
		}
	}
	return 0;
}

