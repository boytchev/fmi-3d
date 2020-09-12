
/// Project:	FMI 3D (2013)
/// File:		03131 Polar coordinate system
///	About:		Sphere moving within polar coordinates
///
/// History:
///		2013.06.16/JB	Initial implementation
///		2013.06.17/PB	Fine tuning


#include "fmi3d.h"

#define MAX_CIRCLES 5
Sphere3D* point;
Text3D* text1;
Text3D* text2;
CoordSys2D* cs1;
Circle2D* c[MAX_CIRCLES];
double a;

void drawScene()
{
	point->drawImage();

	text1->drawImage();
	text2->drawImage();
	cs1->drawImage();

	/// Draw line to (0,0)
	glDisable( GL_LIGHTING );
	glLineWidth( 2.5 );
	glBegin( GL_LINES );
	glColor3f( 1 , 0 , 0 );
	glVertex3f(0,0,3);
	point->getCenter().vertex();
	glEnd();
	glLineWidth( 1 );

	/// Draw circles
	c[MAX_CIRCLES-1]->drawImage();
	glEnable( GL_LINE_STIPPLE );
	glLineStipple( 1, 0b1010101010101010 );
	for( int i=0; i<MAX_CIRCLES-1; i++ ) c[i]->drawImage();

	int n=16;
	double alpha;

	/// Draw radial lines
	glBegin( GL_LINES );
	for( int i=0; i<n; i++ )
	{
		glVertex3f( 0,0,2 );
		alpha=2*M_PI*i/n;
		glVertex3f( 5*cos( alpha ), 5*sin( alpha ), 2 );
	}
	glEnd();
	glDisable( GL_LINE_STIPPLE );


	/// Draw filled angle
	glColor4f( 1,0,0,0.2 );
	glPolygonMode( GL_FRONT_AND_BACK,GL_FILL );

	double b=a;
	if( b>M_PI ) b=M_PI;
	glBegin( GL_POLYGON );
	n=32;
	for( int i=0; i<=n; i++ )
	{
		alpha=b*i/n;
		glVertex3f( cos( alpha ), sin( alpha ), 3 );
	}
	glVertex3f( 0,0,3 );
	glEnd();

	if( a>b )
	{
		glBegin( GL_POLYGON );
		glVertex3f( -1,0,3 );
		for( int i=0; i<=n; i++ )
		{
			alpha=b+(a-b)*i/n;
			glVertex3f( cos( alpha ),sin( alpha ),3 );
		}
		glVertex3f( 0,0,3 );
		glEnd();
	}

	/// Draw angle border
	glColor3f( 1,0,0 );
	glLineWidth( 2.5 );
	glDepthMask( GL_FALSE );
	glBegin( GL_LINE_STRIP );
	n=64;
	for( int i=0; i<=n; i++ )
	{
		 alpha=i*a/n;
		glVertex3f( cos(alpha), sin(alpha), 3 );
		 }
	glEnd();
	glDepthMask( GL_TRUE );
}

void drawFrames()
{
	cs1->drawFrame();
}

int main()
{
	if( !openWindow2D( "Polar Coordinate System" ) ) return 1;
	noGround();

	addButton( new exitButton2D() );

	point = new Sphere3D( 0.25 );
	point->setColor( {0.5,0.5,1} );
	point->setShininess( 20.0 );

	for( int i=0; i<MAX_CIRCLES; i++ )
	{
		c[i] = new Circle2D( i+1,QUALITY_HIGH );
		c[i]->setColor( {0,0,0} );
		c[i]->setPolygonMode( GL_LINE );
	}

	cs1 = new CoordSys2D( -5,5,-5,5,false,1,0.5,L"XYZ" );
	cs1->setCenter( {0,0,2} );
	cs1->setAxesNamesColor( {0.2,0.2,0.2} );

	text1 = new Text3D( L"α",systemFont3D );
	text1->setColor( {0,0,0} );
	text1->setSize(0.8);

	text2 = new Text3D( L"R",systemFont3D );
	text2->setColor( {0,0,0} );
	text2->setSize(0.8);

	while( isRunning() )
	{
		a=10*sin( t*0.23/4 )-8*cos( t*0.17/4 )+20*2*M_PI;
		a=a-2*M_PI*int(a/2/M_PI );

		double r=3.5+1.5*sin( 2*t );
		double x = r*cos( a );
		double y = r*sin( a );
		point->setCenter( { x , y ,3 } );
		text1->setCenter({ 1.1*cos(a/2+0.2),1.1*sin(a/2+0.2),4 } );
		text1->clearOrientation();
		text1->rotateLocal(a*180/M_PI/2-90,{0,0,1});
		text2->setCenter({ 0.5*r*cos(a)-0.1*sin(a),0.5*r*sin(a)+0.1*cos(a),4} );
		text2->clearOrientation();
		text2->rotateLocal(a*180/M_PI,{0,0,1});
	}
	return 0;
}

