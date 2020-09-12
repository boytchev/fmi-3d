
/// Project:	FMI 3D (2013)
/// File:		20201 Viewpoint h-rotation
/// Topic:		20 Projections
/// Slide:		20
///	About:		Horizontal rotation of the viewpoint.
///
/// History:
///		2013.08.17/KK	Initial implementation
///		2013.08.19/PB	Finetuning
///     2013.09.13/PB   Changed button image


#include "fmi3d.h"

#define ANIMATION_TIME 4
#define MAX_SPHERES 40
#define MAX_CUBES 60

CoordSys3D* cs;
Sphere3D*   spheres[MAX_SPHERES];
Cube3D*     cubes[MAX_CUBES];

double speedT = 0;
double speedMax = 0;
double speed = 0;

void toggleHorizontalRotation()
{
	speedT = t;
	speedMax = 3-speedMax;
}


void drawScene()
{
	cs->drawImage();

	for(int i = 0; i < MAX_SPHERES; i++)
        spheres[i]->drawImage();

    for(int i = 0; i < MAX_CUBES; i++)
        cubes[i]->drawImage();
}

void drawFrames()
{
	cs->drawFrame();

    for(int i = 0; i < MAX_CUBES; i++)
        cubes[i]->drawFrame();
}



int main()
{
	if( !openWindow3D( "Viewpoint h-rotation" ) ) return 1;
	noGround();

    addButton( new Button2D( "buttons/button_time.png", 'H', toggleHorizontalRotation) );
	addButton( new exitButton2D() );

	cs = new CoordSys3D(-0.5, 8, -0.5, 8, -0.5, 6, false, 1, 0.4, L"XYZ");

    for(int i = 0; i < MAX_SPHERES; i++)
    {
        spheres[i] = new Sphere3D(random(0.3,1.2));

        spheres[i]->setCenter({random(-4,4), random(-4,4), random(-4,4)});
        spheres[i]->setColor({random(0,1), random(0,1), random(0,1)});
    }

    for(int i = 0; i < MAX_CUBES; i++)
    {
        cubes[i] = new Cube3D(random(0.3,1.8));

        cubes[i]->rotateLocal(random(0,360), {random(-10,10), random(-10,10), random(-10,10)});
        cubes[i]->setCenter({random(-4,4), random(-4,4), random(-4,4)});
        cubes[i]->setColor({random(0,1), random(0,1), random(0,1)});
    }

    viewZ = 0.5;
    viewBeta = 0.2;
    minViewBeta = -maxViewBeta;

	isRunning();
	double t0=t;
	while( isRunning() )
    {
    	double dt = t-t0;
    	t0 = t;
		viewAlpha += speed*dt;

		if( t-speedT<ANIMATION_TIME )
		{
			double k = 0.5+0.5*cos( ( ( t-speedT )/ANIMATION_TIME )*M_PI );
			speed = speed*k+speedMax*( 1-k );
		}
    }

	return 0;
}
