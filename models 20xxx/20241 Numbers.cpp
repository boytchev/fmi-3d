
/// Project:	FMI 3D (2013)
/// File:		20241 Numbers
/// Topic:		20 Projections
/// Slide:		24
///	About:		Rotating and translating camera to look at a series of 3D numbers.
///
/// History:
///		2013.08.18/KK	Initial implementation
///		2013.08.19/PB	Finetuning


#include "fmi3d.h"
#include <stdio.h>

#define MAX_NUMBERS 50
#define AREA_SIZE 20
#define TRANSITION_TIME 100.0
#define DEG_TO_RAD(Deg) ((Deg * M_PI) / 180.0)

Text3D* numbers[MAX_NUMBERS];
double  rotations[MAX_NUMBERS];
vect_t  positions[MAX_NUMBERS];
vect_t  offset[MAX_NUMBERS];

void drawScene()
{
	for( int i = 0; i < MAX_NUMBERS; i++ )
		numbers[i]->drawImage();
}

void drawFrames()
{}

wstring int2wstring( int i )
{
	// Asserting that it is at most a 3 digit number
	char buff[3];
	sprintf( buff, "%d", i );
	wstring wstr( buff, buff+2 );
	return wstr;
}

int main()
{
	/// Open window
	if( !openWindow3D( "Numbers" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();
	changeBrightness();

	/// Add buttons
	addButton( new exitButton2D );

	for( int i = 0; i < MAX_NUMBERS; i++ )
	{
		numbers[i] = new Text3D( int2wstring( i+1 ), systemFont3D );
		numbers[i]->setColor( {random( 0,0.5 ),random( 0,0.5 ),random( 0,0.5 )} );
		numbers[i]->setSize( 3 );

		double rotation = random( 0, 360 );
		rotations[i] = DEG_TO_RAD( rotation )-M_PI/2;
		numbers[i]->rotateLocal( rotation, {0,0,1} );

		positions[i] = {random( 0, AREA_SIZE ),random( 0, AREA_SIZE ),0.03};
		numbers[i]->setCenter( positions[i] );
		offset[i]= {( i<9 )?0.75:1.5,0.75};
	}

	viewX = viewY = AREA_SIZE/2;
	viewZ = 0;
	viewDistance = 40;

	double alpha = 0;
	viewAlpha = 0;
	viewBeta = 1.3;

	isRunning();
	double t0 = t;
	double q = 0;
	while( isRunning() )
	{
		double dt = ( t-t0 );
		q += dt;
		t0 = t;

		double k = 0.5+0.5*cos( q );
		viewDistance = 3+2*AREA_SIZE*k;
		int chosenNumber = int( q/( 2*M_PI ) )%MAX_NUMBERS;
		double x = positions[chosenNumber].x-offset[chosenNumber].x*sin( rotations[chosenNumber] )-offset[chosenNumber].y*cos( rotations[chosenNumber] );
		double y = positions[chosenNumber].y+offset[chosenNumber].x*cos( rotations[chosenNumber] )-offset[chosenNumber].y*sin( rotations[chosenNumber] );
		viewX = AREA_SIZE/2*k+( 1-k )*x;
		viewY = AREA_SIZE/2*k+( 1-k )*y;
		viewAlpha = rotations[chosenNumber]*( 1-k )+k*alpha;

		alpha += dt/3;
		for( int i = 0; i < MAX_NUMBERS; i++ )
		{
			rotations[i] += dt/3;
			numbers[i]->rotateLocal( dt*180/M_PI/3, {0,0,1} );
			numbers[i]->setColor( {0,0,0} );
			numbers[i]->setScale( {3,3,0} );
		}
		numbers[chosenNumber]->setColor( {1*(1-k),0.5*(1-k),0} );
		numbers[chosenNumber]->setScale( {3,3,-(1-k)} );
	}

	return 0;
}
