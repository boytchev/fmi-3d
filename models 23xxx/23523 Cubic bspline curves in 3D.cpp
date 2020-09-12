
/// Project:	FMI 3D (2013)
/// File:		23523 Cubic bspline curves in 3D.cpp
/// Topic:		23 Curves
/// Slide:		52
///	About:		Cubic bspline curves (n=3) in 3D
///
/// History:
///		2013.05.17/PB	Initial implementation


#include "fmi3d.h"

Object3D* P[4];	/// Control points
Cube3D* cube;
bool showControls = true;

#define MAX_POINTS 1024
vect_t p[MAX_POINTS];
int pNo=0;

/// Calculate a point on a cubic bspline curve, t in [0,1]
vect_t bsplinePoint( double t, Object3D* P0, Object3D* P1, Object3D* P2, Object3D* P3 )
{
	double B0 = ( -t*t*t+3*t*t-3*t+1 )/6;
	double B1 = ( 3*t*t*t-6*t*t+4 )/6;
	double B2 = ( -3*t*t*t+3*t*t+3*t+1 )/6;
	double B3 = t*t*t/6;

	vect_t p = ( P[0]->getCenter() ).mul( B0 );
	p = p.add( ( P[1]->getCenter() ).mul( B1 ) );
	p = p.add( ( P[2]->getCenter() ).mul( B2 ) );
	p = p.add( ( P[3]->getCenter() ).mul( B3 ) );
	return p;
}


void drawBsplineCurve()
{
	glDisable(GL_LIGHTING);
	glDepthMask( GL_FALSE );
	glColor3f( 0,0,1 );
	glLineWidth( 5 );
	glEnable( GL_LINE_SMOOTH );
	glBegin( GL_LINE_STRIP );
	for( int t=0; t<MAX_POINTS; t++ )
	{
		glColor4f(0,0,1,t/(double)MAX_POINTS);
		p[( pNo+t )%MAX_POINTS].vertex();
	}
	glEnd();
	glLineWidth( 1 );
	glDepthMask( GL_TRUE );
}


void drawScene()
{
	cube->drawImage();
	drawBsplineCurve();
}


void drawFrames()
{
	cube->drawFrame();
	drawBsplineCurve();
}


int main()
{
	/// Open window
	if( !openWindow3D( "Cubic bspline curve (n=3) in 3D" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();

	/// Add buttons
	addButton( new exitButton2D );

	cube = new Cube3D(8);
	cube->setCenter({0,0,5});
	cube->setColor({0,0,0});
	cube->setPolygonMode(GL_LINE);

	viewZ = 4;
	viewDistance = 15;

	/// Create control points
	for( int i=0; i<4; i++ )
	{
		P[i] = point3D( 0, 0, 0 );
		P[i]->setSize( 0.15 );
	}

	/// Main loop
	isRunning();
	double t0 = t;
	double q=0;
	while( isRunning() )
	{
		double dq = 3*( t-t0 ); /// Increase 3 to make faster animation

		viewAlpha += dq/20.0;

		/// Is it time to add a new control point?
		if( q+dq>=1 )
		{
			for( int i=0; i<3; i++ ) P[i]->setCenter( P[i+1]->getCenter() );
			P[3]->setCenter( {random(-4,4), random(-4,4), random(1,9)} );
			q-=1;
		}

		for( int i=0; i<5; i++ )
		{
			q = q+dq/5;
			p[pNo] = bsplinePoint( q,P[0],P[1],P[2],P[3] );
			pNo = ( pNo+1 )%MAX_POINTS;
		}
		t0=t;
	}

	return 0;
}

