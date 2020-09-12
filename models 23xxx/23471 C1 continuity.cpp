
/// Project:	FMI 3D (2013)
/// File:		23471 C1 continuity.cpp
/// Topic:		23 Curves
///	Slide:		47
///	About:		C1 continuity of cubic Bezier curves (n=3)
///
/// History:
///		2013.05.16/PB	Initial implementation


#include "fmi3d.h"


Object3D* P[7];	/// Control points
bool showControls = true;


/// Calculate a point on a Bezier curve, t in [0,1]
vect_t bezierPoint( double t, Object3D* P0, Object3D* P1, Object3D* P2, Object3D* P3 )
{
	double B0 = 1*pow( 1-t,3 )*pow( t,0 );
	double B1 = 3*pow( 1-t,2 )*pow( t,1 );
	double B2 = 3*pow( 1-t,1 )*pow( t,2 );
	double B3 = 1*pow( 1-t,0 )*pow( t,3 );

	vect_t p = (P0->getCenter()).mul( B0 );
	p = p.add( (P1->getCenter()).mul( B1 ) );
	p = p.add( (P2->getCenter()).mul( B2 ) );
	p = p.add( (P3->getCenter()).mul( B3 ) );
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
		bezierPoint( t/100.0,P[0],P[1],P[2],P[3] ).vertex();
	}
	for( int t=0; t<=100; t++ )
	{
		p = k*pow(1-t/100.0,2);
		glColor3f(p,0,1-p);
		bezierPoint( t/100.0,P[3],P[4],P[5],P[6] ).vertex();
	}
	glEnd();

	glEnable(GL_POINT_SMOOTH);
	glPointSize( 7 );
	glBegin( GL_POINTS );
	glColor3f(k,0,1-k);
	bezierPoint( 1,P[0],P[1],P[2],P[3] ).vertex();
	glEnd();
	glDisable(GL_POINT_SMOOTH);

	glDepthMask( GL_TRUE );

	P[3]->setColor({k,0,0});
}


void drawScene()
{
	drawBezierCurve();
	if( showControls )
	{
		drawDottedLine(7,P);
		for( int i=0; i<7; i++ ) P[i]->drawImage();
	}
}


void drawFrames()
{
	if( showControls )
		for( int i=0; i<7; i++ ) P[i]->drawFrame();
}


void randomizeControlPoints()
{
	for( int i=0; i<7; i++ )
		P[i]->setCenter( {random( -5,5 ), random( -5,5 ),1} );
}


void toggleControls()
{
	showControls = !showControls;
}



int main()
{
	/// Open window
	if( !openWindow2D( "C1 continuity of Bezier cubic curves" ) ) return 1;
	noGround();

	/// Add buttons
	addButton( "buttons/button_random.png",'R',randomizeControlPoints );
	addButton( "buttons/button_show.png",'S',toggleControls );
	addButton( new exitButton2D );

	/// Create control points
	for( int i=0; i<7; i++ )
		P[i] = interactivePoint2D( random(-5,5), random(-5,5) );

	/// Modify the internal point
	P[1]->setSize( 0.15 );
	P[2]->setSize( 0.15 );
	P[4]->setSize( 0.15 );
	P[5]->setSize( 0.15 );
	P[1]->setColor( {1,0,0} );
	P[2]->setColor( {1,0,0} );
	P[4]->setColor( {1,0,0} );
	P[5]->setColor( {1,0,0} );

	/// Main loop
	while( isRunning() )
	{
		P[3]->setCenter((P[2]->getCenter()).add((P[4]->getCenter())).mul(0.5));
	}

	return 0;
}

