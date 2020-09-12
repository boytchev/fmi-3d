
/// Project:	FMI 3D (2013)
/// File:		19451 Matrix rotation in XY.cpp
/// Topic:		19 Matrices
/// Slide:		30
///	About:		Scaling with matrix
///
/// History:
///		2013.05.27/PB	Initial implementation


#include "fmi3d.h"

//#include <iostream>

#define matrix oxyz_t
#define vector vect_t

#define N  30
#define MAX_TRACES 10240
Object3D* Q;
vect_t V;
Object3D* P[N];
vect_t traces[N][MAX_TRACES];
int trace;
CoordSys3D* cs;
matrix M;


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
	Q->drawImage();

	glEnable(GL_LINE_SMOOTH);
	glColor3f(1,0,0);
	glLineWidth( 1 );
	glDisable(GL_LIGHTING);
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(5,0b1010101010101010);
	glBegin(GL_LINES);
	Q->getCenter().add(V.mul(+12)).vertex();
	Q->getCenter().add(V.mul(-12)).vertex();
	glEnd();
	glDisable(GL_LINE_STIPPLE);

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
	/// Random line
	double a=random(0,2*M_PI);
	double b=random(0,2*M_PI);
	V={cos(a)*cos(b),sin(a)*cos(b),sin(b)};
	Q->setCenter({random( -5,5 ),random( -5,5 ),random( -5,5 )});

	/// Random points
	trace=0;
	for( int i=0; i<N; i++ )
	{
		traces[i][0] = {random( -6,6 ),random( -6,6 ),random( -6,6 )};
		P[i]->setCenter( traces[i][0] );
	}

	/// Build translation matrices
	matrix M1 = translationMatrix( Q->oxyz.o.mul(-1) );
	matrix M7 = translationMatrix( Q->oxyz.o.mul(+1) );

	/// Build rotation matrices
	double d2 = sqrt(V.x*V.x+V.y*V.y);
	double co = V.x/d2;
	double si = V.y/d2;
	matrix M2 = { {co,+si,0,0}, {-si,co,0,0}, {0,0,1,0}, {0,0,0,1} };
	matrix M6 = { {co,-si,0,0}, {+si,co,0,0}, {0,0,1,0}, {0,0,0,1} };

	/// Build rotation matrices
	double d3 = sqrt(V.x*V.x+V.y*V.y+V.z*V.z);
	co = V.z/d3;
	si = d2/d3;
	matrix M3 = { {co,0,-si,0}, {0,1,0,0}, {+si,0,co,0}, {0,0,0,1} };
	matrix M5 = { {co,0,+si,0}, {0,1,0,0}, {-si,0,co,0}, {0,0,0,1} };

	/// Build rotation matrice
	double s = 1/fps;
	co = cos(s);
	si = sin(s);
	matrix M4 = { {co,-si,0,0}, {+si,co,0,0}, {0,0,1,0}, {0,0,0,1} };

	/// Multiply all matrices in one matrix
	M = M7.multiply(M6).multiply(M5).multiply(M4).multiply(M3).multiply(M2).multiply(M1);
}


int main()
{
	/// Open window
	if( !openWindow3D( "Matrix rotation around line" ) ) return 1;
	noGround();

	/// Add buttons
	addButton( "buttons/button_random.png",'R',randomizePoints );
	addButton( new exitButton2D );

	viewZ = 0;
	viewBeta = 0.1;
	minViewBeta = -M_PI/2+0.01;
	maxViewBeta = +M_PI/2-0.01;

	cs = new CoordSys3D( -9,9, -9,9, -2,2, true, 2, 0.4, L"XYZ" );
	cs->setColor( {0.5,0.5,1} );

	/// Create control points
	Q = new Sphere3D(0.5);
	Q->setColor({0.5,0.5,1});
	Q->setShininess( 10 );

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
				P[i]->setCenter( P[i]->getCenter().mmul( M ) );
				traces[i][trace] = P[i]->getCenter();
			}
		} else trace--;
	}
	return 0;
}

