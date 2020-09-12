
/// Project:	FMI 3D (2013)
/// File:		11231 Correct synchronization
/// Topic:		11 Animation
/// Slide:		23
///	About:		Correct synchronization of motions
///
/// History:
///		2013.07.24/JB	Initial implementation
///		2013.07.26/PB	Finetuning
///     2013.09.10/PB   Fixed animation speed


#include "fmi3d.h"

int n = 150;
int w = 25;
Cube3D* cube[2];

void drawScene()
{
	cube[1]->drawImage();
	cube[0]->drawImage();
}

void drawFrames()
{
	cube[1]->drawFrame();
	cube[0]->drawFrame();
}

int main()
{
	if( !openWindow3D( "Correct synchronization" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();

	addButton( new exitButton2D() );

	viewX = 2;
	viewY = 2;
	viewZ = 6;
	viewDistance = 30;

	cube[0] = new Cube3D(10);
    cube[0]->setCenter( {0,0,5} );
	cube[0]->setColor ( {0.5,0.5,1} );

	cube[1] = new Cube3D(6);
	cube[1]->setColor ( {1,1,0} );

    isRunning();
    double t0 = t;
	while( isRunning() )
	{
		int f = int((t-t0)*80)%(2*n+3*w)-w;

		if( f<0 ) cube[1]->setCenter({0,0,13}); else
		if( f<n ) cube[1]->setCenter({0,0+8.0/n*f,13}); else
		if( f<2*n ) cube[1]->setCenter({0,8,13-2*8.0/n*(f-n)});
	}
	return 0;
}

