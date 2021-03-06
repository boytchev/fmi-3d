
/// Project:	FMI 3D (2013)
/// File:		11311 Syncronized multiobject motion
/// Topic:		11 Animation
/// Slide:		31
///	About:		Correct synchronization of multiobject motion
///
/// History:
///		2013.07.28/JB	Initial implementation
///		2013.07.29/PB	Finetuned
///     2013.09.10/PB   Fixed animation speed


#include "fmi3d.h"

int n1 = 70;
int n2 = 15;
int w = 40;
Cube3D* cube[3];

void drawScene()
{
	cube[2]->drawImage();
	cube[1]->drawImage();
	cube[0]->drawImage();
}

void drawFrames()
{
	cube[2]->drawFrame();
	cube[1]->drawFrame();
	cube[0]->drawFrame();
}

int main()
{
	if( !openWindow3D( "Syncronized multiobject motion" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();

	addButton( new exitButton2D() );

	viewX = 2;
	viewY = 2;
	viewZ = 8.5;
	viewDistance = 30;

	cube[0] = new Cube3D( 10 );
	cube[0]->setCenter( {0,0,5} );
	cube[0]->setColor( {0.5,0.5,1} );

	cube[1] = new Cube3D( 6 );
	cube[1]->setColor( {1,1,0} );

	cube[2] = new Cube3D( 2 );
	cube[2]->setCenter( {0,0,17} );
	cube[2]->setColor( {1,0,0} );

    isRunning();
    double t0 = t;
	while( isRunning() )
	{
		int f = int((t-t0)*80)%( n1+n2+3*w )-w;

		if( f<0 )
		{
			cube[1]->setCenter( {0,0,13} );
			cube[2]->setCenter( {0,0,17.0} );
		}
		else if( f<n1 )cube[1]->setCenter( {0,0+8.0/n1*f,13} ); else if( f<n1+n2+1 )
		{
			cube[1]->setCenter( {0,8,13-10.0/n2*( f-n1 )} );
		}
		if( f >= n1/2 && f < n1/2+n2+1 ) cube[2]->setCenter( {0,0,17.0-6.0/n2*( f-n1/2 )} );;

	}
	return 0;
}

