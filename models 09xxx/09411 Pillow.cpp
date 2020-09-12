
/// Project:	FMI 3D (2013)
/// File:		09411 Pillow
/// Topic:		9 Graphical Primitives
/// Slide:		41
///	About:		Pillow
///
/// History:
///		2013.07.20/MD	Initial implementation
///		2013.07.26/PB	Finetunning
///     2013.07.13/PB   Reduced the number of capsules

#include "fmi3d.h"

#define N 550

Cylinder3D* capsules[N];

void drawScene()
{
	for( int i=0; i<N; i++ )
		capsules[i]->drawImage();
}


void drawFrames()
{
}

void randomize()
{
	double x,y,z;
	double r=8.0;
	for( int i=0; i<N; i++ )
	{
		x=random( -r,r );
		y=random( -0.75*r,0.75*r );
		z=random( -0.25*r,0.25*r );
		capsules[i]->setCenter( {x,y,z+4} );
		capsules[i]->setColor( {random(0.2,0.5),random(0.3,0.7),1} );
		capsules[i]->rotateLocal(random(0,360),{1,0,0});
		capsules[i]->rotateLocal(random(0,360),{0,1,0});
		capsules[i]->rotateLocal(random(0,360),{0,0,1});
	}

}
int main()
{
	/// Open window
	if( !openWindow3D( "Pillow" ) ) return 1;
	changeGround();
	changeGround();

	addButton( new Button2D( "buttons/button_random.png",'R',randomize ) );
	addButton( new exitButton2D );

    viewDistance = 25;

	for( int i=0; i<N; i++ )
	{
		capsules[i] = new Cylinder3D( 1,1,false,QUALITY_LOW );
		capsules[i]->setScale({0.4,0.5,1});
		capsules[i]->setShininess( 20 );
	}
	randomize();

	/// Main loop with automatic animation
	double t0=t;
	while( isRunning() )
	{
		viewAlpha += ( t-t0 )/20;
		t0=t;
	}

	return 0;
}
