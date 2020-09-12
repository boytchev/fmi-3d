
/// Project:	FMI 3D (2013)
/// File:		23522 Cubic bspline curves.cpp
/// Topic:		23 Curves
/// Slide:		52
///	About:		Cubic bspline curves (n=3)
///
/// History:
///		2013.05.17/PB	Initial implementation


#include "fmi3d.h"

Object3D* P[4];	/// Control points
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
	p.z = 0;
	return p;
}


void drawBsplineCurve()
{
	glDepthMask( GL_FALSE );
	glColor3f( 0,0,1 );
	glLineWidth( 10 );
	glEnable( GL_LINE_SMOOTH );
	glBegin( GL_LINE_STRIP );
	for( int t=0; t<MAX_POINTS; t++ )
	{
		double c = 1-t/( double )MAX_POINTS;
		glColor4f( 0,0,1,c );
		p[( pNo-t-1+MAX_POINTS )%MAX_POINTS].vertex();
	}
	glEnd();
	glDepthMask( GL_TRUE );
}


void drawScene()
{
	drawBsplineCurve();
	if( showControls )
	{
		drawDottedLine( 4,P );
		for( int i=0; i<4; i++ ) P[i]->drawImage();
	}
}


void drawFrames()
{
	if( showControls )
		for( int i=0; i<4; i++ ) P[i]->drawFrame();
}


void toggleControls()
{
	showControls = !showControls;
}



int main()
{
	/// Open window
	if( !openWindow2D( "Cubic bspline curve (n=3)" ) ) return 1;
	noGround();

	/// Add buttons
	addButton( "buttons/button_show.png",'S',toggleControls );
	addButton( new exitButton2D );

	/// Create control points
	for( int i=0; i<4; i++ )
	{
		P[i] = point2D( 0, 0 );
		P[i]->setSize( 0.15 );
	}

	/// Main loop
    isRunning();
	double t0 = t;
	double q=0;
	double a=0;
	while( isRunning() )
	{
		double dq = 3*( t-t0 ); /// Increase 3 to make faster animation

		/// Is it time to add a new control point?
		if( q+dq>=1 )
		{
			for( int i=0; i<3; i++ ) P[i]->setCenter( P[i+1]->getCenter() );
			double r = random( 3,6 );
			a = a + random( 1,4 );
			P[3]->setCenter( {r*cos( a ),r*sin( a ),1} );
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

