
/// Project:	FMI 3D (2013)
/// File:		12141 Nested circular motion
/// Topic:		12 Trajectories
/// Slide:		14
///	About:		Nested circular motion
///
/// History:
///		2013.07.28/MD	Initial implementation
///		2013.07.29/PB	Finetuned
///     2013.09.13/PB   Simplified some objects


#include "fmi3d.h"

#define N 30
Sphere3D* bigSphere;
Sphere3D* smallSphere;
Sphere3D* trace[N];

const double R=1.5,r=0.7,rO=10.0,ro=5.0;

void drawScene()
{
	bigSphere->drawImage();
	smallSphere->drawImage();
	for( int i=0; i<N; i++ )
	{
		int j = (frame+i)%N;
		trace[j]->setSize(0.5*r*double(i)/N);
		trace[j]->setColor({0.5,0.5,1,0.5*double(i)/N});
		trace[j]->drawImage();
	}
}


void drawFrames()
{
}

void defineObjects( double R, double r, double rO, double ro )
{
	for( int i=0; i<N; i++ )
	{
		trace[i]=new Sphere3D( 0.3, QUALITY_LOWER );
		trace[i]->setColor( {0,0,0} );
		trace[i]->setCenter( {0,0,-10} );
	}

	bigSphere=new Sphere3D( R, QUALITY_HIGH );
	bigSphere->setColor( {1,0.8,0} );
	bigSphere->setShininess( 10 );

	smallSphere=new Sphere3D( r, QUALITY_HIGH );
	smallSphere->setColor( {0.5,0.5,1} );
}



int main()
{

	/// Open window
	if( !openWindow3D( "Nested circular motion" ) ) return 1;
	changeBrightness();
	changeGround();
	changeGround();

	addButton( new exitButton2D );

	defineObjects( R,r,rO,ro );

	/// Main loop with automatic animation
	while( isRunning() )
	{
		double r=5;
		double angle= 0.7*t;
		bigSphere->setCenter( {r*cos( angle ),r*sin( angle ),2*R} );
		smallSphere->setCenter( {r*cos( angle )+r*0.8*sin( 3*angle ),r*sin( angle )+r*0.8*cos( 3*angle ),2*R} );
		trace[frame%N]->setCenter(smallSphere->getCenter());
	}

	return 0;
}
