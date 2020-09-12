
/// Project:	FMI 3D (2013)
/// File:		03181 Spherical coordinate system
/// Topic:		03 Coordinate systems
/// Slide:		18
///	About:		Sphere moving within spherical coordinates
///
/// History:
///		2013.07.30/PB	Initial implementation


#include "fmi3d.h"

#define MAX_CIRCLES 8
Sphere3D* point;
Text3D *text1, *text2, *text3;
CoordSys3D* cs1;
Circle2D* c[MAX_CIRCLES];
double a,b;

double f = 3;

void drawScene()
{
	point->drawImage();

	text1->drawImage();
	text2->drawImage();
	text3->drawImage();
	cs1->drawImage();

	/// Draw line to (0,0)
	glDisable( GL_LIGHTING );
	glLineWidth( 2.5 );
	glBegin( GL_LINE_STRIP );
	glColor3f( 1 , 0 , 0 );
	point->getCenter().vertex();
	glVertex3f( 0,0,0 );
	glVertex3f( f*cos(a),f*sin(a),0 );
	glEnd();
	glLineWidth( 1 );

	/// Draw circles
	c[MAX_CIRCLES-1]->drawImage();
	glEnable( GL_LINE_STIPPLE );
	glLineStipple( 1, 0b1010101010101010 );
	for( int i=0; i<MAX_CIRCLES-1; i++ ) c[i]->drawImage();

	int n=16;
	double alpha,beta;

	/// Draw radial lines
	glBegin( GL_LINES );
	for( int i=0; i<n; i++ )
	{
		glVertex3f( 0,0,0 );
		alpha=2*M_PI*i/n;
		glVertex3f( MAX_CIRCLES*cos( alpha ), MAX_CIRCLES*sin( alpha ), 0 );
	}
	glEnd();
	glDisable( GL_LINE_STIPPLE );


	/// Draw filled angle
	glColor4f( 1,0,0,0.2 );
	glPolygonMode( GL_FRONT_AND_BACK,GL_FILL );

	double aa=a;
	if( aa>M_PI ) aa=M_PI;
	glBegin( GL_POLYGON );
	n=32;
	for( int i=0; i<=n; i++ )
	{
		alpha=aa*i/n;
		glVertex3f( f*cos( alpha ), f*sin( alpha ), 0 );
	}
	glVertex3f( 0,0,0 );
	glEnd();

	if( a>aa )
	{
		glBegin( GL_POLYGON );
		glVertex3f( -1,0,0 );
		for( int i=0; i<=n; i++ )
		{
			alpha=aa+( a-aa )*i/n;
			glVertex3f( f*cos( alpha ),f*sin( alpha ),0 );
		}
		glVertex3f( 0,0,0 );
		glEnd();
	}

	/// Draw filled vertical angle
	glColor4f( 1,0,0,0.2 );
	glPolygonMode( GL_FRONT_AND_BACK,GL_FILL );

	glBegin( GL_POLYGON );
	n=32;
	for( int i=0; i<=n; i++ )
	{
		beta=b*i/n;
		glVertex3f( f*cos(a)*cos(beta), f*sin(a)*cos(beta), f*sin(beta) );
	}
	glVertex3f( 0,0,0 );
	glEnd();

	if( a>aa )
	{
		glBegin( GL_POLYGON );
		glVertex3f( -1,0,0 );
		for( int i=0; i<=n; i++ )
		{
			alpha=aa+( a-aa )*i/n;
			glVertex3f( f*cos( alpha ),f*sin( alpha ),0 );
		}
		glVertex3f( 0,0,0 );
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
		glVertex3f( f*cos( alpha ), f*sin( alpha ), 0 );
	}
	glEnd();
	glDepthMask( GL_TRUE );

	/// Draw vertical angle border
	glColor3f( 1,0,0 );
	glLineWidth( 2.5 );
	glDepthMask( GL_FALSE );
	glBegin( GL_LINE_STRIP );
	n=32;
	for( int i=0; i<=n; i++ )
	{
		beta=i*b/n;
		glVertex3f( f*cos(a)*cos(beta), f*sin(a)*cos(beta), f*sin(beta) );
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
	if( !openWindow3D( "Spherical Coordinate System" ) ) return 1;
	noGround();

	addButton( new exitButton2D() );

	viewDistance = 25;
	viewZ = 1;
	viewBeta = 0.4;

	point = new Sphere3D( 0.3 );
	point->setColor( {0.5,0.5,1} );
	point->setShininess( 20.0 );

	for( int i=0; i<MAX_CIRCLES; i++ )
	{
		c[i] = new Circle2D( i+1,QUALITY_HIGH );
		c[i]->setColor( {0,0,0} );
		c[i]->setPolygonMode( GL_LINE );
	}

	cs1 = new CoordSys3D( -10,10,-10,10,-5,7,false,1,0.8,L"XYZ" );
	cs1->setCenter( {0,0,0} );
	cs1->setAxesNamesColor( {0.5,0.5,1} );

	text1 = new Text3D( L"α",systemFont3D );
	text1->setColor( {0,0,0} );
	text1->setScale( {1.2,1.2,0.2} );

	text2 = new Text3D( L"R",systemFont3D );
	text2->setColor( {0,0,0} );
	text2->setScale( {1.2,1.2,0.2} );

	text3 = new Text3D( L"β",systemFont3D );
	text3->setColor( {0,0,0} );
	text3->setScale( {1.2,1.2,0.2} );

	while( isRunning() )
	{
		a = t - 2*M_PI*int(t/2/M_PI);
		b = 0.8+0.4*sin(t*0.843);

		double r=7+2.5*sin( 1.2*t );
		double x = r*cos(a)*cos(b);
		double y = r*sin(a)*cos(b);
		double z = r*sin(b);
		point->setCenter( {x,y,z} );

		// alpha
		text1->identity();
		text1->rotateGlobal( 90, {0,0,1} );
		text1->setCenter( {f+0.7,0,0} );
		text1->rotateGlobal( a*180/M_PI-15, {0,0,1} );

		// beta
		text3->identity();
		text3->rotateGlobal( 90, {1,0,0} );
		text3->setCenter( {f+0.1,0,0} );
		text3->rotateGlobal( 20-b*180/M_PI, {0,1,0} );
		text3->rotateGlobal( a*180/M_PI, {0,0,1} );

		// R
		text2->identity();
		text2->rotateGlobal( 90, {1,0,0} );
		text2->setCenter( {r-1,0,0.1} );
		text2->rotateGlobal( -b*180/M_PI, {0,1,0} );
		text2->rotateGlobal( a*180/M_PI, {0,0,1} );
	}
	return 0;
}

