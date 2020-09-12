
/// Project:	FMI 3D (2013)
/// File:		Test21 Vectors.cpp
///	About:		2D and 3D vectors
///				Showing 2D and 3D vectors.
///
/// History:
///		2013.05.23/PB	Initial implementation


#include "fmi3d.h"

#define N 300
Vector3D* v[N];
Object3D* o;
Object3D* c;

void drawScene()
{
	c->drawImage();
	o->drawImage();
	for( int i=0; i<N; i++ )
		v[i]->drawImage();
}

void drawFrames()
{
	c->drawFrame();
	for( int i=0; i<N; i++ )
		v[i]->drawFrame();
}



int main()
{
	if( !openWindow3D( "Test 21: Vectors" ) ) return 1;

	addButton( new exitButton2D );

	viewDistance = 40;
	viewZ = 7;

	for( int i=0; i<N; i++ )
	{
		v[i] = new Vector3D( {0,0,1},0.5 );
		double x = irandom(0,1)*20-10;
		double y = irandom(0,1)*20-10;
		double z = irandom(0,1)*20-10;
		switch( irandom(0,2) )
		{
			case 0: x=random(-10,10); break;
			case 1: y=random(-10,10); break;
			case 2: z=random(-10,10); break;
		}
		v[i]->setCenter( {x,y,z+10} );

		v[i]->setLength( 3 );
	}

	o = new Sphere3D( 0.4 );
	o->setColor( {1,0,0} );

	c = new Cube3D( 20 );
	c->setCenter({0,0,10.01});
	c->setPolygonMode( GL_LINE );
	c->setColor({1,0,0});

	while( isRunning() )
	{
		viewAlpha = t/10;

		o->setCenter( {9*sin( 1.7345*t ),9*cos( 2.3806*t ),5+4*cos( 3.09835*t )} );
		for( int i=0; i<N; i++ )
		{
			v[i]->setDirection( o );
		}
	}
	return 0;
}

