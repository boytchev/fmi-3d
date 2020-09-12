
/// Project:	FMI 3D (2013)
/// File:		23451 G1 continuity.cpp
/// Topic:		23 Curves
///	Slide:		45
///	About:		G1 continuity of quadric Bezier curves (n=2)
///
/// History:
///		2013.05.16/PB	Initial implementation


#include "fmi3d.h"


Object3D* P[5];	/// Control points
bool showControls = true;


/// Calculate a point on a Bezier curve, t in [0,1]
vect_t bezierPoint( double t, Object3D* P0, Object3D* P1, Object3D* P2 )
{
	double B0 = 1*pow( 1-t,2 )*pow( t,0 );
	double B1 = 2*pow( 1-t,1 )*pow( t,1 );
	double B2 = 1*pow( 1-t,0 )*pow( t,2 );

	vect_t p = (P0->getCenter()).mul( B0 );
	p = p.add( (P1->getCenter()).mul( B1 ) );
	p = p.add( (P2->getCenter()).mul( B2 ) );
	p.z = 0;
	return p;
}


void drawBezierCurve()
{
	double k = 0.5+0.5*sin(3*t); 	// color pulsation
	double p;						// color gradient

	glDepthMask( GL_FALSE );
	glColor3f( 0,0,1 );
	glLineWidth( 10 );
	glBegin( GL_LINE_STRIP );
	for( int t=0; t<=100; t++ )
	{
		p = k*pow(t/100.0,2);
		glColor3f(p,0,1-p);
		bezierPoint( t/100.0,P[0],P[1],P[2] ).vertex();
	}
	for( int t=0; t<=100; t++ )
	{
		p = k*pow(1-t/100.0,2);
		glColor3f(p,0,1-p);
		bezierPoint( t/100.0,P[2],P[3],P[4] ).vertex();
	}
	glEnd();

	glEnable(GL_POINT_SMOOTH);
	glPointSize( 7 );
	glBegin( GL_POINTS );
	glColor3f(k,0,1-k);
	bezierPoint( 1,P[0],P[1],P[2] ).vertex();
	glEnd();
	glDisable(GL_POINT_SMOOTH);

	glDepthMask( GL_TRUE );

	P[2]->setColor({k,0,0});
}


void drawScene()
{
	drawBezierCurve();
	if( showControls )
	{
		drawDottedLine(5,P);
		for( int i=0; i<5; i++ ) P[i]->drawImage();
	}
}


void drawFrames()
{
	if( showControls )
		for( int i=0; i<5; i++ ) P[i]->drawFrame();
}


void randomizeControlPoints()
{
	for( int i=0; i<5; i++ )
		P[i]->setCenter( {random( -5,5 ), random( -5,5 ),1} );
}


void toggleControls()
{
	showControls = !showControls;
}



int main()
{
	/// Open window
	if( !openWindow2D( "G1 continuity of Bezier curves" ) ) return 1;
	noGround();

	/// Add buttons
	addButton( "buttons/button_random.png",'R',randomizeControlPoints );
	addButton( "buttons/button_show.png",'S',toggleControls );
	addButton( new exitButton2D );

	/// Create control points
	for( int i=0; i<5; i++ )
		P[i] = interactivePoint2D( random(-5,5), random(-5,5) );

	/// Modify the internal point
	P[1]->setSize( 0.15 );
	P[3]->setSize( 0.15 );
	P[1]->setColor( {1,0,0} );
	P[3]->setColor( {1,0,0} );

	/// Main loop
	while( isRunning() )
	{
		P[2]->setCenter(closestPointToSegment(P[2],P[1],P[3]));
	}

	return 0;
}

