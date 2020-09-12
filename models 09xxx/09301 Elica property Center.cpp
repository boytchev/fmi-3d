
/// Project:	FMI 3D (2013)
/// File:		09301 Elica property Center
/// Topic:		9 Graphical Primitives
/// Slide:		30
///	About:		Sphere of cubes made by using the Center property
///
/// History:
///		2013.07.20/MD	Initial implementation
///		2013.07.27/PB	Finetuning


#include "fmi3d.h"

#define N 350
Cube3D* cubes[N];

void drawScene()
{
	for( int i=0; i<N; i++ )
		cubes[i]->drawImage();
}


void drawFrames()
{
	for( int i=0; i<N; i++ )
		cubes[i]->drawFrame();
}

void randomize()
{
	double a,b;
	double r=6.0;
	for( int i=0; i<N; i++ )
	{
		a=random( 0,2*M_PI ); b=random( 0,2*M_PI );
		cubes[i]->setCenter( {r*cos( a )*cos( b ),r*sin( a )*cos( b ),r*sin( b )+7} );
		cubes[i]->setSize( random( 1,2.0 ) );
		cubes[i]->setColor( {random(0.4,1),random(0.4,1),random(0.4,1)} );
	}

}
int main()
{
	/// Open window
	if( !openWindow3D( "Elica property: center" ) ) return 1;

	addButton( new Button2D( "buttons/button_random.png",'R',randomize ) );
	addButton( new exitButton2D );

	viewZ = 6.5;
	viewDistance = 25;

	for( int i=0; i<N; i++ )
	{
		cubes[i] = new Cube3D( 1 );
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
