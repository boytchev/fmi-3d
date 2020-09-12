
/// Project:	FMI 3D (2013)
/// File:		13263 Bouncing balls in 3D
/// Topic:		13 Physics
/// Slide:		26
///	About:		Bouncing balls in 3D
///
/// History:
///		2013.08.22/JB	Initial implementation
///		2013.08.23/PB	Finetuning
///     2013.09.11/PB   Fixed animation



#include "fmi3d.h"

#define N 15
Sphere3D* p[N];
vect_t v[N];
double bv[N];
Cube3D* cube;


void drawScene()
{
	for( int i=0; i<N; i++ ) p[i]->drawImage();
	glLineWidth( 2 );
	cube->drawImage();
}

void drawFrames()
{
	cube->drawFrame();
}

int main()
{
	if( !openWindow3D( "Bouncing balls in 3D" ) ) return 1;
//	noGround();
	changeGround();
	changeGround();
	changeGround();

	addButton( new exitButton2D() );

	viewDistance = 15;
	viewZ = 3;

	float size = 7;
	float radius = 0.4;
	float limit = size/2.0-radius;
	float gravity = -12;

	for( int i=0; i<N; i++ )
	{
		p[i] = new Sphere3D( radius, QUALITY_LOW );
		p[i]->setColor( {random( 0,1 ),random( 0,1 ),random( 0,1 )} );
		p[i]->setCenter( {random( -limit,limit ),random( -limit,limit ),4+limit+random( -4,0 )} );
		v[i] = {0,0,0};
		v[i] = {random( -10,10 ),random( -10,10 ),0};
		v[i] = v[i].mul( random( 1,4 )/v[i].distance() );
		bv[i]=-15;
	}

	cube = new Cube3D( size );
	cube->setColor( {0,0,0} );
	cube->setCenter( {0,0,4} );
	cube->setPolygonMode( GL_LINE );

	isRunning();
	double t0=t;
	while( isRunning() )
	{
		double dt=t-t0;
		t0 = t;

		viewAlpha += dt/20;

		for( int i=0; i<N; i++ )
		{
			v[i] = v[i].add( { 0 , 0 , gravity*dt } );
			p[i]->setCenter( p[i]->getCenter().add( v[i].mul( dt ) ) );
			if( p[i]->getCenter().x>=+limit ) p[i]->oxyz.o.x=+limit, v[i].x = -fabs( v[i].x );
			if( p[i]->getCenter().x<=-limit ) p[i]->oxyz.o.x=-limit, v[i].x = +fabs( v[i].x );
			if( p[i]->getCenter().y>=+limit ) p[i]->oxyz.o.y=+limit, v[i].y = -fabs( v[i].y );
			if( p[i]->getCenter().y<=-limit ) p[i]->oxyz.o.y=-limit, v[i].y = +fabs( v[i].y );
			if( p[i]->getCenter().z<=0.5+radius/2 )
			{
				if( bv[i]<-10 ) bv[i]=fabs( v[i].z );
				p[i]->oxyz.o.z = 0.5+radius/2;
				v[i].z = bv[i];
			}
		}
	}
	return 0;
}

