
/// Project:	FMI 3D (2013)
/// File:		20202 Round2
/// Topic:		20 Projections
/// Slide:		20
///	About:		Rotation of the viewpoint horizontally and vertically, and zooming.
///
/// History:
///		2013.08.17/KK	Initial implementation
///		2013.08.19/PB	Finetuning
///     2013.09.13/PB   Change buttons images


#include "fmi3d.h"

#define ANIMATION_TIME 4
#define MAX_SPHERES 40
#define MAX_CUBES 60

CoordSys3D* cs;
Sphere3D*   spheres[MAX_SPHERES];
Cube3D*     cubes[MAX_CUBES];


double hT = -10;
double hMax = 0;
double h = 0;
void toggleHorizontalRotation()
{
	hT = t;
	hMax = 2-hMax;
}

double vT = -10;
double vMax = 0;
double v = 0;
void toggleVerticalRotation()
{
	vT = t;
	vMax = 1.234-vMax;
}

double zT = -10;
double zMax = 0;
double z = 0;
void toggleZoom()
{
	zT = t;
	zMax = 1.876-zMax;
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
	if( !openWindow3D( "Viewpoint hv-rotation" ) ) return 1;
	noGround();

    addButton( new Button2D( "buttons/button_time.png", 'H', toggleHorizontalRotation) );
    addButton( new Button2D( "buttons/button_time.png", 'V', toggleVerticalRotation) );
    addButton( new Button2D( "buttons/button_time.png", 'Z', toggleZoom) );
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
    viewBeta = 0;
    minViewBeta = -maxViewBeta;
    viewDistance = 15;

	isRunning();
	double t0=t;
	double vb = 0;
	double vd = 0;
	while( isRunning() )
    {
    	double dt = t-t0;
    	t0 = t;

		viewAlpha += h*dt;
		vb += v*dt;
		viewBeta = 0.7*maxViewBeta*sin(vb);
		vd += z*dt;
		viewDistance = 30-15*cos(vd);

		if( t-hT<ANIMATION_TIME )
		{
			double k = 0.5+0.5*cos( ( ( t-hT )/ANIMATION_TIME )*M_PI );
			h = h*k+hMax*( 1-k );
		}

		if( t-vT<ANIMATION_TIME )
		{
			double k = 0.5+0.5*cos( ( ( t-vT )/ANIMATION_TIME )*M_PI );
			v = v*k+vMax*( 1-k );
		}

		if( t-zT<ANIMATION_TIME )
		{
			double k = 0.5+0.5*cos( ( ( t-zT )/ANIMATION_TIME )*M_PI );
			z = z*k+zMax*( 1-k );
		}
    }

	return 0;
}
