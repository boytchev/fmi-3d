
/// Project:	FMI 3D (2013)
/// File:		23301 Linear Bezier curve.cpp
/// Topic:		23 Curves
///	Slide:		30
///	About:		Linear Bezier curve (n=1)
///
/// History:
///		2013.05.16/PB	Initial implementation


#include "fmi3d.h"


Object3D* P[2];				/// Control points
bool showControls = true;


/// Calculate a point on a Bezier curve, t in [0,1]
vect_t bezierPoint( double t, Object3D* P0, Object3D* P1 )
{
	double B0 = 1*pow( 1-t,1 )*pow( t,0 );
	double B1 = 1*pow( 1-t,0 )*pow( t,1 );

	vect_t p = ( P0->getCenter() ).mul( B0 );
	p = p.add( ( P1->getCenter() ).mul( B1 ) );
	p.z = 0;
	return p;
}


void drawBezierCurve()
{
	glDepthMask( GL_FALSE );
	glColor3f( 0,0,1 );
	glLineWidth( 10 );
	glBegin( GL_LINE_STRIP );
	for( int t=0; t<=100; t++ ) bezierPoint( t/100.0,P[0],P[1] ).vertex();
	glEnd();
	glDepthMask( GL_TRUE );
}


void drawScene()
{
	drawBezierCurve();
	if( showControls )
	{
		drawDottedLine(2,P);
		for( int i=0; i<2; i++ ) P[i]->drawImage();
	}
}


void drawFrames()
{
	if( showControls )
		for( int i=0; i<2; i++ ) P[i]->drawFrame();
}


void randomizeControlPoints()
{
	for( int i=0; i<2; i++ )
		P[i]->setCenter( {random( -5,5 ), random( -5,5 ),1} );
}


void toggleControls()
{
	showControls = !showControls;
}



int main()
{
	/// Open window
	if( !openWindow2D( "Linear Bezier curve (n=1)" ) ) return 1;
	noGround();

	/// Add buttons
	addButton( "buttons/button_random.png",'R',randomizeControlPoints );
	addButton( "buttons/button_show.png",'S',toggleControls );
	addButton( new exitButton2D );

	/// Create control points
	for( int i=0; i<2; i++ )
		P[i] = interactivePoint2D( random(-5,5), random(-5,5) );

	/// Main loop
	while( isRunning() )
	{
	}

	return 0;
}

