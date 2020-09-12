
/// Project:	FMI 3D (2013)
/// File:		19201 Matrix translation.cpp
/// Topic:		19 Matrices
/// Slide:		20
///	About:		Translation with matrix
///
/// History:
///		2013.05.26/PB	Initial implementation


#include "fmi3d.h"

//#include <iostream>

#define matrix oxyz_t
#define vector vect_t

#define N  20
#define MAX_TRACES 10240
Object3D* P[N];
vect_t traces[N][MAX_TRACES];
int trace;
CoordSys3D* cs;
Vector3D* v;
matrix T;

matrix translationMatrix( vector v )
{
	return
	{
		{ 1, 0, 0, v.x },
		{ 0, 1, 0, v.y },
		{ 0, 0, 1, v.z },
		{ 0, 0, 0,  1  },
	};
}

void drawScene()
{
	cs->drawImage();
	v->drawImage();

	glEnable(GL_LINE_SMOOTH);
	glLineWidth( 2 );

	for( int i=0; i<N; i++ )
	{
		P[i]->drawImage();

		glDisable(GL_LIGHTING);
		glColor4f( 0,0,0,0.5 );
		drawLine(trace,traces[i]);
	}
}


void drawFrames()
{
}


void randomizePoints()
{
	/// Random translation vector
	v->setDirection( {random( -1,1 ),random( -1,1 ),random( -1,1 )} );
	v->setLength( 2 );

	/// Random points
	trace=0;
	for( int i=0; i<N; i++ )
	{
		traces[i][0] = {random( -5,5 ),random( -5,5 ),random( -5,5 )};
		P[i]->setCenter( traces[i][0] );
	}

	/// Build translation matrix (scaled by 2/fps)
	T = translationMatrix( v->oxyz.oz.mul( 2/fps ) );
}


int main()
{
	/// Open window
	if( !openWindow3D( "Matrix translation" ) ) return 1;
	noGround();

	/// Add buttons
	addButton( "buttons/button_random.png",'R',randomizePoints );
	addButton( new exitButton2D );

	viewZ = 0;
	viewBeta = 0.1;
	minViewBeta = -M_PI/2+0.01;
	maxViewBeta = +M_PI/2-0.01;

	cs = new CoordSys3D( -9,9, -9,9, -7,7, true, 2, 0.4, L"XYZ" );
	cs->setColor( {0.5,0.5,1} );

	v = new Vector3D( {2,-1,-6},0.5 );
	v->setColor( {0.8,0,0} );

	/// Create control points
	for( int i=0; i<N; i++ )
	{
		P[i] = new Sphere3D( 0.3,QUALITY_LOW );
		P[i]->setColor( {0,0,0} );
		P[i]->setShininess( 10 );
	}

	randomizePoints();

	/// Main loop
	double t0=t;

	while( isRunning() )
	{
		viewAlpha += ( t-t0 )/10;
		t0 = t;

		trace++;
		if( trace<MAX_TRACES )
		{
			for( int i=0; i<N; i++ )
			{
				P[i]->setCenter( P[i]->getCenter().mmul( T ) );
				traces[i][trace] = P[i]->getCenter();
			}
		} else trace--;
	}
	return 0;
}

