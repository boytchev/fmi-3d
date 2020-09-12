
/// Project:	FMI 3D (2013)
/// File:		11361 Motion by vector
/// Topic:		11 Animation
/// Slide:		36
///	About:		Motion by vector
///
/// History:
///		2013.07.24/JB	Initial implementation
///		2013.07.26/PB	Finetuning
///		2013.09.09/PB	Fixed warnings


#include "fmi3d.h"

Sphere3D* point[3];
CoordSys3D* cs1;

int n = 100;
int w = 30;

void drawScene()
{

    cs1->drawImage();
    for(int i=0;i<3;i++)
    {
        point[i]->drawImage();
    }

}

void drawFrames()
{
    cs1->drawFrame();
}

int main()
{
	if( !openWindow3D( "Motion by vector" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();

	viewDistance = 20;

	addButton( new exitButton2D() );

    cs1 = new CoordSys3D(-8,8,-8,8,-2,6,false,1,0.5,L"XYZ");
	cs1->setAxesNamesColor({0.2,0.2,0.2});
	cs1->setCenter({0,0,2});

	for(int i=0; i<2;i++)
	{
	    point[i] = new Sphere3D(0.5);
	    point[i]->setColor({0.5, 0.5, 1});
	    point[i]->setShininess(50);
	}

	point[2] = new Sphere3D(0.6);
	point[2]->setColor({1,0,0});
	point[2]->setShininess(50);

	while( isRunning() )
	{
		vect_t v;

		int f = frame%(n+3*w)-w;

		if( f==1-w)
		{
			point[0]->setCenter( {random(-5,5), random(-5,5), random(1,9) } );
			point[1]->setCenter( {random(-5,5), random(-5,5), random(1,9) } );
			v = point[1]->getCenter().sub(point[0]->getCenter()).mul(1.0/n);
		}

		if( f<0 ) point[2]->setCenter( point[0]->getCenter() ); else
		if( f<n ) point[2]->setCenter( point[0]->getCenter().add(v.mul(f)) );

	}
	return 0;
}

