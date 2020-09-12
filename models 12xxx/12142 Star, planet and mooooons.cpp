
/// Project:	FMI 3D (2013)
/// File:		12142 Star, planet and mooooons
/// Topic:		12 Trajectories
/// Slide:		14
///	About:		Star, planet and mooooons
///
/// History:
///		2013.07.28/MD	Initial implementation
///		2013.07.29/PB	Finetuning
///     2013.09.13/PB   Simplified moons


#include "fmi3d.h"

#define N 20
Sphere3D* bigSphere;
Sphere3D* smallSphere;
Sphere3D* bodies[N];
double s[N];
double o[N];
double d[N];

void drawScene()
{
	bigSphere->drawImage();
	smallSphere->drawImage();
	for( int i=0; i<N; i++ )
		bodies[i]->drawImage();

}


void drawFrames()
{
}

void defineObjects( double R, double r, double rO, double ro )
{
	for( int i=0; i<N; i++ )
	{
		bodies[i]=new Sphere3D( 0.2, QUALITY_LOWER );
		bodies[i]->setColor( {1,1,1} );
		s[i] = random(4,10);
		if( random(0,2)>1 ) s[i]=-s[i];
		o[i] = random(0,2*M_PI);
		d[i] = random(1,3);
	}

	bigSphere=new Sphere3D( R, QUALITY_HIGH );
	bigSphere->setColor( {1,0.8,0} );
	bigSphere->setShininess( 10 );

	smallSphere=new Sphere3D( r, QUALITY_HIGH );
	smallSphere->setColor( {0.5,0.5,1} );
}



int main()
{
	const double R=1.5,r=0.7,rO=10.0,ro=6.0;

	/// Open window
	if( !openWindow3D( "Star, planet and mooooons" ) ) return 1;
	changeBrightness();
	changeGround();
	changeGround();

	addButton( new exitButton2D );

	defineObjects( R,r,rO,ro );

	/// Main loop with automatic animation
	while( isRunning() )
	{
		double r=5;
		double angle=t/4;
		bigSphere->setCenter( {r*cos(angle),r*sin(angle),2*R} );
		smallSphere->setCenter( bigSphere->getCenter().add({r*sin(-3*angle),r*cos(-3*angle),0}) );
		for( int i=0; i<N; i++ )
		{
			bodies[i]->setCenter( smallSphere->getCenter().add({d[i]*sin(angle*s[i]+o[i]),d[i]*cos(angle*s[i]+o[i]),sin(i+2*t)}) );
		}
	}

	return 0;
}
