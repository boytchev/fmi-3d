
/// Project:	FMI 3D (2013)
/// File:		23361 Cubic Bezier curve.cpp
/// Topic:		23 Curves
/// Slide:		36
///	About:		Cubic Bezier curve (n=3)
///
/// History:
///		2013.05.16/PB	Initial implementation


#include "fmi3d.h"


Object3D* P[4];	/// Control points
bool showControls = true;


/// Calculate a point on a Bezier curve, t in [0,1]
vect_t bezierPoint( double t, Object3D* P0, Object3D* P1, Object3D* P2, Object3D* P3 )
{
	double B0 = 1*pow( 1-t,3 )*pow( t,0 );
	double B1 = 3*pow( 1-t,2 )*pow( t,1 );
	double B2 = 3*pow( 1-t,1 )*pow( t,2 );
	double B3 = 1*pow( 1-t,0 )*pow( t,3 );

	vect_t p = ( P[0]->getCenter() ).mul( B0 );
	p = p.add( ( P[1]->getCenter() ).mul( B1 ) );
	p = p.add( ( P[2]->getCenter() ).mul( B2 ) );
	p = p.add( ( P[3]->getCenter() ).mul( B3 ) );
	p.z = 0;
	return p;
}


void drawBezierCurve()
{
	glDepthMask( GL_FALSE );
	glColor3f( 0,0,1 );
	glLineWidth( 10 );
	glBegin( GL_LINE_STRIP );
	for( int t=0; t<=100; t++ ) bezierPoint( t/100.0,P[0],P[1],P[2],P[3] ).vertex();
	glEnd();
	glDepthMask( GL_TRUE );
}


void drawScene()
{
	drawBezierCurve();
	if( showControls )
	{
		drawDottedLine(4,P);
		for( int i=0; i<4; i++ ) P[i]->drawImage();
	}
}


void drawFrames()
{
	if( showControls )
		for( int i=0; i<4; i++ ) P[i]->drawFrame();
}


void randomizeControlPoints()
{
	for( int i=0; i<4; i++ )
		P[i]->setCenter( {random( -5,5 ), random( -5,5 ),1} );
}


void toggleControls()
{
	showControls = !showControls;
}



int main()
{
	/// Open window
	if( !openWindow2D( "Cubic Bezier curve (n=3)" ) ) return 1;
	noGround();

	/// Add buttons
	addButton( "buttons/button_random.png",'R',randomizeControlPoints );
	addButton( "buttons/button_show.png",'S',toggleControls );
	addButton( new exitButton2D );

	/// Create control points
	for( int i=0; i<4; i++ )
		P[i] = interactivePoint2D( random(-5,5), random(-5,5) );

	/// Modify the internal points
	P[1]->setSize( 0.15 );
	P[2]->setSize( 0.15 );
	P[1]->setColor( {1,0,0} );
	P[2]->setColor( {1,0,0} );

	/// Main loop
	while( isRunning() )
	{
	}

	return 0;
}

