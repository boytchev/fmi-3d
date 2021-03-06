
/// Project:	FMI 3D (2013)
/// File:		24071 NURBS surface.cpp
/// Topic:		24 Surfaces
/// Slide:		07
///	About:		Example of a NURBS surface
///
/// History:
///		2013.05.29/PB	Initial implementation


#include "fmi3d.h"

//#include <iostream>


#define N 8
GLfloat S[N][N];
GLfloat O[N][N];
Bezier3D* bezier;

void drawScene()
{
	bezier->drawImage();
}


void drawFrames()
{
}


double f( int i, int j )
{
	return sin( t*S[i][j]+O[i][j] );
}


int main()
{
	/// Open window
	if( !openWindow3D( "A NURBS surface" ) ) return 1;
	//noGround();

	/// Add buttons
	addButton( new exitButton2D );

	bezier = new Bezier3D( N );
	bezier->setColor( {0.2,0,1} );
	bezier->setShininess( 2 );
	bezier->setSize( 2 );
	bezier->setTolerance( 50 );

	/// Create control points
	for( int i=0; i<N; i++ )
		for( int j=0; j<N; j++ )
		{
			S[i][j] = random( 3,5 );
			O[i][j] = random( 0,2*M_PI );
		}


	/// Main loop
	double t0=t;
	while( isRunning() )
	{
		viewAlpha += (t-t0)/10;
		t0 = t;

		for( int i=0; i<N; i++ )
			for( int j=0; j<N; j++ )
			{
				double k = ( ( i==0 )||( i==N-1 )||( j==0 )||( j==N-1 ) )?1:3;
				bezier->setControlPoint( i,j, {i-( N-1 )/2.0,j-( N-1 )/2.0,2+k*f( i,j )} );
			}
	}
	return 0;
}

