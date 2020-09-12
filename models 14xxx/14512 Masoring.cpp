
/// Project:	FMI 3D (2013)
/// File:		14512 Masoring.cpp
/// Topic:		14 Orientation
/// Slide:		51
///	About:		A ring of pawns pointing inward
///
/// History:
///		2013.08.14/PB	Initial implementation
///     2013.09.13/PB   Increased speed


#include "fmi3d.h"


#define N 18

Object3D *p;
Pawn3D *pawn[N];
Cylinder3D *c;
double t0;

void drawScene()
{
	c->drawImage();
	if( !inShadow ) for(int i=0; i<N; i++) pawn[i]->drawImage();
}


void drawFrames()
{
	c->drawFrame();
}


void fd( double dist )
{
	p->translateLocal( {dist,0,0} );
}

void dn( double dist )
{
	p->rotateLocal( dist, {0,1,0} );
}

void lr( double dist )
{
	p->rotateLocal( dist, {1,0,0} );
}


int main()
{
	/// Open window
	if( !openWindow3D( "Masoring" ) ) return 1;
	changeGround();

	viewZ = 5;
	viewDistance = 20;

	/// Add buttons
	addButton( new exitButton2D );

	p = new Object3D( );
	p->setCenter({0,0,10});
	fd(-0.7);

	for(int i=0; i<N; i++)
	{
		fd(0.7);
		pawn[i] = new Pawn3D(0.5);
		pawn[i]->setColor({0,0,0});
		lr(180);
		pawn[i]->oxyz = p->oxyz;
		lr(180);
		fd(0.7);
		dn(360.0/N);
	}

	c = new Cylinder3D(3.9,1.5,false,QUALITY_HIGH);
	c->setColor({0.5,0.5,1});
	c->rotateLocal(90,{1,0,0});
	c->setCenter({0,0.75,6.03});

	double z = c->getCenter().z;

	/// Main loop
	isRunning();
	t0=t;
	while( isRunning() )
	{
		double a = 20*(t-t0);
		viewAlpha += (t-t0)/10;
		t0 = t;

		for(int i=0; i<N; i++)
		{
			pawn[i]->translateGlobal({0,0,-z});
			pawn[i]->rotateGlobal(a,{0,1,0});
			pawn[i]->translateGlobal({0,0,+z});
		}
	}

	return 0;
}

