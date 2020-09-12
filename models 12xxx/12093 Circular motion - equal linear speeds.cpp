
/// Project:	FMI 3D (2013)
/// File:		12093 Circular motion - equal linear speeds
/// Topic:		12 Trajectories
/// Slide:		9
///	About:		Circular motion - equal linear speeds, different angular speeds
///
/// History:
///		2013.07.28/MD	Initial implementation
///		2013.07.29/PB	Finetuned
///     2013.09.13/PB   Orbits made prettier


#include "fmi3d.h"

Sphere3D* bigSphere;
Sphere3D* smallSphere;
Circle2D* orbitBig;
Circle2D* orbitSmall;


void drawScene()
{
	bigSphere->drawImage();
	smallSphere->drawImage();

	glLineWidth(3);
	glEnable(GL_LINE_SMOOTH);

	orbitBig->drawImage();
	orbitSmall->drawImage();
}


void drawFrames()
{
}

void defineObjects( double R, double r, double rO, double ro )
{
	bigSphere=new Sphere3D( R, QUALITY_HIGH );
	bigSphere->setColor( {0.75,0,0} );
	bigSphere->setShininess( 20 );

	smallSphere=new Sphere3D( r, QUALITY_HIGH );
	smallSphere->setColor( {0,0,1} );
	smallSphere->setShininess( 20 );

	orbitBig=new Circle2D( rO, QUALITY_HIGHER );
	orbitBig->setColor( {0,0,0} );
	orbitBig->setCenter( {0,0,3} );
	orbitBig->setPolygonMode( GL_LINE );

	orbitSmall=new Circle2D( ro, QUALITY_HIGHER );
	orbitSmall->setColor( {0,0,0} );
	orbitSmall->setCenter( {0,0,3} );
	orbitSmall->setPolygonMode( GL_LINE );
}



int main()
{
	const double R=1.5,r=1.0,rO=8.0,ro=5.0;

	/// Open window
	if( !openWindow3D( "Circular motion: equal linear speeds" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();

	addButton( new exitButton2D );

	defineObjects( R,r,rO,ro );

	/// Main loop with automatic animation
	while( isRunning() )
	{
		double angle = t;
		bigSphere->setCenter( {rO*cos( angle/3.0 ),rO*sin( angle/3.0 ),2*R} );
		smallSphere->setCenter( {ro*cos( angle ),ro*sin( angle ),2*R} );
	}

	return 0;
}
