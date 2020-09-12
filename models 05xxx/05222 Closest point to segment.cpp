/// Project:	FMI 3D (2013)
/// File:		05222 Closest point to segment.cpp
/// Topic:		05 Lines and polygons
/// Slide:		22
///	About:		Closest point from a segment to a point
///
/// History:
///		2013.07.27/VH	Initial implementation
///		2013.07.28/PB	Finetuning



#include "fmi3d.h"

vect_t a, b;
Object3D* center;
Circle2D* ans;
Circle2D *left_end , *right_end;
CoordSys2D* cs;

void drawScene()
{
	glDisable(GL_LIGHTING);

	cs->drawImage();
	center->drawImage();
	ans->drawImage();
	left_end->drawImage();
	right_end->drawImage();

	glTranslated(0,0,-1);

	glLineWidth( 5 );
	glEnable(GL_LINE_SMOOTH);
	glBegin( GL_LINES );
	glColor3f( 0,0,0 );
	a.vertex();
	b.vertex();
	glEnd();

	glPushAttrib( GL_ENABLE_BIT );
	glLineWidth( 2 );
	glEnable( GL_LINE_STIPPLE );
	glLineStipple( 3, 0xCCCC );
	glColor3f( 1,0,0 );
	glBegin( GL_LINES );
	center->getCenter().vertex();
	ans->getCenter().vertex();
	glEnd();
	glPopAttrib();

	glTranslated(0,0,1);

}

void drawFrames()
{
	center->drawFrame();
	ans->drawFrame();
	left_end->drawFrame();
	right_end->drawFrame();
}


int main()
{
	/// Open window
	if( !openWindow2D( "Closest point to a segment" ) ) return 1;
	noGround();

	viewDistance = 30;

	/// Add buttons
	addButton( new exitButton2D() );

	cs = new CoordSys2D(-12,12,-8,8,false,1,0.5,L"XY");
	ans = new Circle2D(0.3);
	ans->setColor({1,0,0});
	center = interactivePoint2D( -random( 1,5 ), random( 1,5 ) );

	left_end = new Circle2D(0.2);
	right_end = new Circle2D(0.2);
	left_end->setColor({0,0,0});
    right_end->setColor({0,0,0});

	while( isRunning() )
	{
		viewX = viewY = 0;

		a = {4*cos(t/2),7*sin(t/2)};
		b = {7*cos(t/2+M_PI),4*sin(t/2+M_PI/3)};

        left_end->setCenter(a);
		right_end->setCenter(b);

		double len2 = pow(a.distance(b),2);
		double q = ((center->getCenter().x-a.x)*(b.x-a.x)+(center->getCenter().y-a.y)*(b.y-a.y))/len2;
		if( q<0 ) q=0;
		if( q>1 ) q=1;
		ans->setCenter(a.add(b.sub(a).mul(q)));
	}

	return 0;
}

