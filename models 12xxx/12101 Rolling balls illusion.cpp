
/// Project:	FMI 3D (2013)
/// File:		12101 Rolling balls illusion
/// Topic:		12 Trajectories
/// Slide:		10
///	About:		Rolling balls illusion
///
/// History:
///		2013.07.28/MD	Initial implementation
///		2013.07.29/PB	Finetuned


#include "fmi3d.h"

#define N 6
Sphere3D* spheres[N];
bool solution = false;
CoordSys3D *cs;

void drawScene()
{
	for( int i=0; i<N; i++ )
		spheres[i]->drawImage();

	if( solution ) cs->drawImage();
}


void drawFrames()
{
	if( solution ) cs->drawFrame();
}

void showHideSolution()
{
	solution = !solution;
	if( solution )
	{
		spheres[0]->setColor( {0.75,0,0} );
		spheres[3]->setColor( {0.7,0,0} );
		spheres[1]->setColor( {0,0.75,0} );
		spheres[4]->setColor( {0,0.75,0} );
		spheres[2]->setColor( {0,0,0.75} );
		spheres[5]->setColor( {0,0,0.75} );
	}
	else
	{
		for( int i=0; i<N; i++ )
			spheres[i]->setColor( {0.15,0.15,0.15} );
	}
}

int main()
{
	const double R=1.5;
	/// Open window
	if( !openWindow3D( "Rolling balls illusion" ) ) return 1;
	changeGround();

	addButton( new Button2D("buttons/button_show.png",GLFW_KEY_SPACE,showHideSolution));
	addButton( new exitButton2D );

	for( int i=0; i<N; i++ )
	{
		spheres[i]=new Sphere3D( R, QUALITY_HIGH );
		spheres[i]->setColor( {0.15,0.15,0.15} );
		spheres[i]->setShininess( 50 );
	}

	cs = new CoordSys3D(-6,6,-6,6,-4,4,false,5,0.5,L"XYZ");
	cs->setCenter({0,0,4});
	cs->setColor({0,0,0});

	viewZ = 4;
	viewDistance = 15;
	viewBeta = M_PI/2;

	/// Main loop with automatic animation
	double t0=t;
	while( isRunning() )
	{
		viewAlpha += ( t-t0 )/7;
		t0=t;

		double up=4.0;
		double c=1.7*R*cos( 3*t );
		double s=1.7*R*sin( 3*t );
		spheres[0]->setCenter( {c,s,0+up} );
		spheres[1]->setCenter( {0,c,s+up} );
		spheres[2]->setCenter( {s,0,c+up} );
		spheres[3]->setCenter( {-c,-s,0+up} );
		spheres[4]->setCenter( {0,-c,-s+up} );
		spheres[5]->setCenter( {-s,0,-c+up} );
	}

	return 0;
}
