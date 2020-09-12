
/// Project:	FMI 3D (2013)
/// File:		11452 Tennis
///	About:		Tennis
///
/// History:
///		2013.07.26/JB	Initial implementation
///		2013.07.29/PB	Finetuning


#include "fmi3d.h"

Sphere3D* point;
Cylinder3D* cyl[2];

double k=0.5;
double v=2;

void drawScene()
{
	point->drawImage();

	for( int i=0; i<2; i++ )
	{
		cyl[i]->drawImage();
	}
}

void drawFrames()
{
	for( int i=0; i<2; i++ )
	{
		cyl[i]->drawFrame();
	}
}

int main()
{
	if( !openWindow3D( "Tennis" ) ) return 1;
	changeBrightness();
	changeGround();
	changeGround();
	changeGround();

	addButton( new exitButton2D() );

	viewZ = 3;

	for( int i=0; i<2; i++ )
	{
		cyl[i] = new Cylinder3D( 2,0.5,true,QUALITY_HIGH );
	}
	cyl[0]->setColor( { 0.75, 0, 0 } );
	cyl[1]->setColor( { 0.5, 0.5, 1 } );

	point = new Sphere3D( 0.3 );
	point->setColor( {0.8,0.8,0.8} );
	point->setShininess( 10 );

	double t0 = t;
	while( isRunning() )
	{
		k+=v*( t-t0 );
		if( k>1 ) k=0.99;
		if( k<0 ) k=0.01;
		t0 = t;


		///като си смени позицията едната хилка променяме фокуса и на двете хилки
		cyl[0]->setCenter( { 6*pow( sin( 3.7*t ),3 ), -6, 5+3*cos( 2.1*t ) } );
		cyl[1]->setCenter( { 6*pow( cos( 2.3*t ),3 ),  6, 5+3*sin( 3.1*t ) } );
		point->setCenter( { cyl[0]->getCenter().x * ( 1-k ) + cyl[1]->getCenter().x * k ,
							cyl[0]->getCenter().y * ( 1-k ) + cyl[1]->getCenter().y * k ,
							cyl[0]->getCenter().z * ( 1-k ) + cyl[1]->getCenter().z * k
						  } );

		for( int i=0; i<2; i++ )
		{
			vect_t d = point->getCenter().sub( cyl[i]->getCenter() );
			cyl[i]->clearOrientation();

			cyl[i]->rotateLocal( -atan2( d.x,d.y )*180/M_PI, {0,0,1} );
			cyl[i]->rotateLocal( 90, {1,0,0} );
			cyl[i]->rotateLocal( 90+( i==0?-1:+1 )*atan2( d.y,d.z )*180/M_PI, {1,0,0} );
		}

		if( k>0.98 ) v=-fabs( v );
		if( k<0.02 ) v=fabs( v );
	}
	return 0;
}

