
/// Project:	FMI 3D (2013)
/// File:		12091 Circular motion - opposite directions.cpp
/// Topic:		12 Trajectories
/// Slide:		9
///	About:		Circulat motion - opposite directions
///
/// History:
///		2013.07.28/MD	Initial implementation
///		2013.07.29/PB	Finetuning


#include "fmi3d.h"

Sphere3D* bigSphere;
Sphere3D* smallSphere;
Circle2D* orbit;

void drawScene()
{
	bigSphere->drawImage();
	smallSphere->drawImage();

	glLineWidth(3);
	glEnable(GL_LINE_SMOOTH);

	orbit->drawImage();
}


void drawFrames()
{
}

void defineObjects( double R, double r, double rO )
{
	bigSphere=new Sphere3D( R, QUALITY_HIGH );
	bigSphere->setColor( {0.75,0,0} );
	bigSphere->setShininess( 20 );

	smallSphere=new Sphere3D( r, QUALITY_HIGH );
	smallSphere->setColor( {0,0,0.5} );
	smallSphere->setShininess( 20 );

	orbit=new Circle2D( rO, QUALITY_HIGH );
	orbit->setColor( {0,0,0} );
	orbit->setCenter( {0,0,3} );
	orbit->setPolygonMode( GL_LINE );
}



int main()
{
	const double R=1.5,r=1.0,rO=7.0;

	/// Open window
	if( !openWindow3D( "Circular motion: opposite directions" )) return 1;
	changeGround();
	changeGround();
	changeGround();

	viewZ = 1;

	addButton( new exitButton2D );

	defineObjects( R,r,rO );

	/// Main loop with automatic animation
	while( isRunning() )
	{
		double angle=t*0.8;
		bigSphere->setCenter( {rO*cos( angle ),rO*sin( angle ),2*R} );
		smallSphere->setCenter( {rO*sin( angle ),rO*cos( angle ),2*R} );
	}

	return 0;
}
