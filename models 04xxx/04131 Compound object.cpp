
/// Project:	FMI 3D (2013)
/// File:		04131 Compound object.cpp
/// Topic:		04 Vectors
/// Slide:		13
///	About:		Showing a pawn as a compound object
///
/// History:
///		2013.08.14/PB	Initial implementation


#include "fmi3d.h"

#define ANIMATION_TIME 2
Pawn3D* p;

void drawScene()
{
	p->drawImage();
}


void drawFrames()
{
	p->drawFrame();
}


double splitT = 0;
double splitS = 0;
double split = 0;
void toggleSplit()
{
	splitT = t;
	splitS = 2-splitS;
}


int main()
{
	/// Open window
	if( !openWindow3D( "Compound object" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();

	/// Add buttons
	addButton( "buttons/button_cut.png",GLFW_KEY_SPACE,toggleSplit );
	addButton( new exitButton2D );

	p = new Pawn3D( 1.5 );
	p->setCenter({0,0,0.5});
	p->setColor({0,0,0});

	/// Main loop
	double t0=t;
	while( isRunning() )
	{
		viewAlpha += ( t-t0 )/10;
		t0 = t;

		if( t-splitT<ANIMATION_TIME )
		{
			double k = cos( ( ( t-splitT )/ANIMATION_TIME )*M_PI/2 );
			split = split*k+splitS*( 1-k );
		}

		p->setOffset(split);
		viewDistance = viewDistance*0.9+(15+5*split)*0.1;
	}
	return 0;
}

