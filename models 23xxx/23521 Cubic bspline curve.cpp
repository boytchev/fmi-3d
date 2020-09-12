
/// Project:	FMI 3D (2013)
/// File:		23521 Cubic bspline curve.cpp
/// Topic:		23 Curves
/// Slide:		52
///	About:		Cubic bspline curve (n=3)
///
/// History:
///		2013.05.17/PB	Initial implementation


#include "fmi3d.h"


Object3D* P[4];	/// Control points
bool showControls = true;


/// Calculate a point on a cubic bspline curve, t in [0,1]
vect_t bsplinePoint( double t, Object3D* P0, Object3D* P1, Object3D* P2, Object3D* P3 )
{
	double B0 = (-t*t*t+3*t*t-3*t+1)/6;
	double B1 = (3*t*t*t-6*t*t+4)/6;
	double B2 = (-3*t*t*t+3*t*t+3*t+1)/6;
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
	glBegin( GL_LINE_STRIP );
	for( int t=0; t<=100; t++ ) bsplinePoint( t/100.0,P[0],P[1],P[2],P[3] ).vertex();
	glEnd();
	glDepthMask( GL_TRUE );
}


void drawScene()
{
	drawBsplineCurve();
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
	if( !openWindow2D( "Cubic bspline curve (n=3)" ) ) return 1;
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

