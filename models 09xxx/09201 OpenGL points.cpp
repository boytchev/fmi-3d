
/// Project:	FMI 3D (2013)
/// File:		09201 OpenGL points.cpp
/// Topic:		09 Graphical Primitives
/// Slide:		20
///	About:		Points in OpenGL
///
/// History:
///		2013.06.27/MD	Initial implementation
///		2013.06.29/PB	Minor fixes


#include "fmi3d.h"

#define N 50
const double radius=0.2;
Sphere3D* points[N];

void drawScene()
{
	for(int i=0; i<N; i++)
		points[i]->drawImage();
}


void drawFrames()
{
}


void randomize()
{
    double range=7.0;
	for(int i=0; i<N; i++)
	{
		points[i]->setCenter({random(-range,range),
                       random(-range,range),random(0,range)});
    }
}

int main()
{
	/// Open window
	if( !openWindow3D( "OpenGL points" ) ) return 1;

	addButton( new Button2D("buttons/button_random.png",'R',randomize) );
	addButton( new exitButton2D );


	for(int i=0; i<N; i++)
	{
		points[i] = new Sphere3D(radius);
		points[i]->setColor({0,0,0});
		points[i]->setShininess(20);
    }

	randomize();

	/// Main loop with automatic animation
	double t0=t;
	while( isRunning() )
	{
		viewAlpha += (t-t0)/20;
		t0=t;
	}

	return 0;
}
