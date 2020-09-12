
/// Project:	FMI 3D (2013)
/// File:		Test 12 Pawn.cpp
///	About:		Pawn class
///				Models several instances of the pawn class.
///
/// History:	2013.05.07/PB	Initial implementation


#include "fmi3d.h"

#define MAX_PLANE 8
Pawn3D* plane[MAX_PLANE];
Button2D* but1;
Button2D* but2;
Button2D* but3;

void drawScene()
{
	for( int i=0; i<MAX_PLANE; i++ ) plane[i]->drawImage();
}

void drawFrames()
{
	for( int i=0; i<MAX_PLANE; i++ ) plane[i]->drawFrame();
}

int main()
{
	if( !openWindow3D( "Test 12: Pawn" ) ) return 1;
	changeGround();

	for( int i=0; i<MAX_PLANE; i++ )
	{
		if( random( 0,1 )>0.6 )
			plane[i] = new WhitePawn3D( random( 0.75,1.5 ) );
		else
			plane[i] = new BlackPawn3D( random( 0.75,1.5 ) );
	}
	plane[0]->setCenter({0,0,3});
	plane[1]->setColor({0,0.5,0});
	plane[3]->setColor({0.5,0,0});

	while( isRunning() )
	{
		plane[0]->setOffset(sin(t));
		for( int i=1; i<MAX_PLANE; i++ )
		{
			plane[i]->identity();
			plane[i]->setCenter( {0,7+3*sin( i ),3+2*sin( t+2*i )} );
			plane[i]->rotateLocal( -90, {0,1,0} );
			plane[i]->rotateGlobal( 50*( 5+t+2*i+0.01*sin( 3*t+i ) ), {0,0,1} );
		}
	}
	return 0;
}

