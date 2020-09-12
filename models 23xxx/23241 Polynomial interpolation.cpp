
/// Project:	FMI 3D (2013)
/// File:		23241 Polynomial interpolation.cpp
/// Topic:		23 Curves
/// Slide:		24
///	About:		Polynomial interpolation
///
/// History:
///		2013.05.18/PB	Initial implementation


#include "fmi3d.h"

//#include <iostream>

#define N  7
Object3D* P[N+1];


double X(int i) { return (P[i]->getCenter()).x; }
double Y(int i) { return (P[i]->getCenter()).y; }

/// Calculate a point on the curve, t in [0,1]
vect_t polynomialFunction( double x )
{
	double y = 0;
	for( int i=0; i<=N; i++ )
	{
		double p=1;
		for( int j=0; j<=N; j++ )
			if( j!=i )
				p = p*(x-X(j))/(X(i)-X(j));
		y = y+Y(i)*p;
	}
	return {x,y,0};
}


void drawPolynomialCurve()
{
	glDepthMask( GL_FALSE );
	glColor3f( 0,0,1 );
	glLineWidth( 10 );
	glBegin( GL_LINE_STRIP );
	for( double t=X(0); t<X(N); t+=0.05 ) polynomialFunction( t ).vertex();
	polynomialFunction( X(N) ).vertex();
	glEnd();
	glDepthMask( GL_TRUE );
}


void drawScene()
{
	for( int i=0; i<=N; i++ ) P[i]->drawImage();
	drawPolynomialCurve();
}


void drawFrames()
{
	for( int i=0; i<=N; i++ ) P[i]->drawFrame();
}


void randomizeControlPoints()
{
	for( int i=0; i<=N; i++ )
		P[i]->setCenter( {-N+2.0*i, random( -2,2 ),0} );
}


int main()
{
	/// Open window
	if( !openWindow2D( "Polynomial interpolation" ) ) return 1;
	noGround();

	/// Add buttons
	addButton( "buttons/button_random.png",'R',randomizeControlPoints );
	addButton( new exitButton2D );

	/// Create control points
	for( int i=0; i<=N; i++ )
		P[i] = interactivePoint2D( -N+2*i, random( -2,2 ) );

	/// Main loop
	while( isRunning() )
	{
		/// Test bounds for points
		if( activeObject )
		{
			/// Find which one is active
			for( int i=0; i<=N; i++ )
				if( P[i]==activeObject )
				{
					if( i>0 )
						if( X(i)<X(i-1)+0.3 )
							P[i]->setCenter( {X(i-1)+0.3,Y(i),1} );
					if( i<N )
						if( X(i)>X(i+1)-0.3 )
							P[i]->setCenter( {X(i+1)-0.3,Y(i),1} );
					break;
				}

		}
	}

	return 0;
}

