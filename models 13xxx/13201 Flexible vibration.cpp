
/// Project:	FMI 3D (2013)
/// File:		13201 Flexible vibration
/// Topic:		13 Physics
/// Slide:		20
///	About:		Vibrating plates with different flexiblilities
///
/// History:
///		2013.08.22/JB	Initial implementation
///     2013.08.27/JB   Corrections
///		2013.08.28/PB	Finetuning
///     2013.09.13/PB   Simplified spheres

#include "fmi3d.h"


#define N 3
Sphere3D* p[N];
Cube3D* cube[N];


void drawScene()
{
	for( int i=0; i<N; i++ )
	{
        p[i]->drawImage();
	    cube[i]->drawImage();
	}
}

void drawFrames()
{
    for( int i=0; i<N; i++)
    {
        cube[i]->drawFrame();
    }
}

int main()
{
	if( !openWindow3D( "Flexible vibration" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();

	addButton( new exitButton2D() );

	viewZ = 5;

	for( int i=0; i<N; i++ )
	{
		p[i] = new Sphere3D( 1.5, QUALITY_MEDIUM );
		p[i]->setColor( {0.8,0,0} );

        cube[i] = new Cube3D( 4 );
        cube[i]->setColor( {0.5,0.5,1} );
    }


	isRunning();
	double t0=-100;
	double ampl[N]={2,2,2};
	double k[N]={0.4,0.7,0.82};
	double q = -5;
	double oldq = -5;
	while( isRunning() )
	{
		oldq = q;
		q = 1.5*t;

        for(int i=0; i<N; i++)
        {
        	p[i]->setCenter({0,(i-(N-1.0)/2)*5,3+8*fabs(sin(q))});
        	double dt = t-t0;
        	cube[i]->setCenter({0,(i-(N-1.0)/2)*5,-pow(k[i],5*dt)*ampl[i]*sin((20-3*i)*dt)});
        }

       	if( sin(oldq)*sin(q)<0 )
		{
			for(int i=0; i<N; i++) ampl[i]=0.75;
			t0 = t;
		}
	}
	return 0;
}

