
/// Project:	FMI 3D (2013)
/// File:		11211 Fast linear motion
/// Topic:		11 Animation
/// Slide:		21
///	About:		Fast linear motion
///
/// History:
///		2013.07.24/JB	Initial implementation
///		2013.07.26/PB	Finetuning
///		2013.09.09/PB	Fixed speed of animation


#include "fmi3d.h"

Sphere3D* point;
CoordSys3D* cs1;
Cube3D* cube;

double x = 10;
int n = 50;

void drawScene()
{
    cs1->drawImage();
	cube->drawImage();
}

void drawFrames()
{
    cs1->drawFrame();
    cube->drawFrame();
}

int main()
{
	if( !openWindow3D( "Fast linear motion" ) ) return 1;
	noGround();

	addButton( new exitButton2D() );

	viewZ = 0;

    cs1 = new CoordSys3D(-6,6,-6,6,0,3,true,1,0.5,L"XYZ");
	cs1->setAxesNamesColor({0.5,0.5,1});

    cube = new Cube3D(2);
    cube->setColor({0.5, 0.5, 1});

	vect_t p = {5,0,1};
    vect_t v = {-x/n,0,0};

    isRunning();
    double t0 = t;
	while( isRunning() )
	{
	    double dt = (t-t0)*75;
	    t0 = t;

		p = p.add(v.mul(dt));
		if( p.x<-5 ) p.x=5;

        cube->setCenter( p );
	}
	return 0;
}

