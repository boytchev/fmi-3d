
/// Project:	FMI 3D (2013)
/// File:		11451 Motion by linear combination
/// Topic:		11 Animation
/// Slide:		36
///	About:		Motion by linear combination
///
/// History:
///		2013.07.24/JB	Initial implementation
///		2013.07.28/PB	Finetuning



#include "fmi3d.h"

Sphere3D* point[3];
CoordSys3D* cs1;

void drawScene()
{
    cs1->drawImage();
    for(int i=0; i<3; i++)
    {
        point[i]->drawImage();
    }
}

void drawFrames()
{
    cs1->drawFrame();
}

void randomize()
{
	point[0]->setCenter( {random(-5,5), random(-5,5), random(1,9) } );
	point[1]->setCenter( {random(-5,5), random(-5,5), random(1,9) } );
}

int main()
{
	if( !openWindow3D( "Motion by linear combination" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();

	viewZ = 3;
	viewDistance = 20;

	addButton( new Button2D("buttons/button_random.png",'R',randomize));
	addButton( new exitButton2D() );

    cs1 = new CoordSys3D(-8,8,-8,8,-2,6,false,1,0.5,L"XYZ");
	cs1->setAxesNamesColor({0.2,0.2,0.2});
	cs1->setCenter({0,0,2});

	for(int i=0; i<2;i++)
	{
	    point[i] = new Sphere3D(0.5,QUALITY_HIGH);
	    point[i]->setColor({0.5, 0.5, 1});
	    point[i]->setShininess(50);
	}

	point[2] = new Sphere3D(0.6,QUALITY_HIGH);
	point[2]->setColor({1,0,0});
	point[2]->setShininess(50);

	randomize();

	while( isRunning() )
	{
		double k = 0.5+0.5*sin(2*t);
        point[2]->setCenter( point[0]->getCenter().mul(1-k).add(point[1]->getCenter().mul(k)) );
	}
	return 0;
}

