
/// Project:	FMI 3D (2013)
/// File:		14372 Turtle graphics - Houses.cpp
/// Topic:		14 Orientation
/// Slide:		37
///	About:		Drawing three house via Turtle graphics
///
/// History:
///		2013.08.13/PB	Initial implementation
///     2013.09.13/PB   Fixed shininess


#include "fmi3d.h"


#define N 500

Sphere3D *p;
Cylinder3D *c1,*c2;
Cube3D *c;
vect_t q[N];
vect_t s[N];
int n=0,m=0;
double a1=0,a2=0;
bool store = true;
double kk = 0.995, viewY2, viewDistance2;

void drawScene()
{
	c->drawImage();

	glDepthMask( GL_FALSE );
	glDisable( GL_TEXTURE_2D );
	glDisable( GL_LIGHTING );
	glColor3f( 1,1,1 );
	glEnable( GL_LINE_SMOOTH );
	glLineWidth( 7 );
	glBegin( GL_LINE_STRIP );
	for( int i=0; i<n; i++ ) q[i].vertex();
	glEnd();
	glEnable( GL_POINT_SMOOTH );
	glPointSize( 5 );
	glBegin( GL_POINTS );
	for( int i=0; i<m; i++ ) s[i].vertex();
	glEnd();
	glDepthMask( GL_TRUE );


	c1->oxyz = p->oxyz;
	c1->translateLocal( {0,1.1,0.5} );
	c1->rotateLocal( 90, {1,0,0} );
	c1->rotateLocal( a1, {0,0,1} );
	c1->drawImage();

	c2->oxyz = p->oxyz;
	c2->translateLocal( {0,-0.6,0.5} );
	c2->rotateLocal( 90, {1,0,0} );
	c2->rotateLocal( a2, {0,0,1} );
	c2->drawImage();

	glEnable( GL_CULL_FACE );
	glCullFace( GL_FRONT );
	p->drawImage();
	glDisable( GL_CULL_FACE );

	viewY = viewY*kk+viewY2*( 1-kk );
	viewDistance = viewDistance*kk+viewDistance2*( 1-kk );
}


void drawFrames()
{
}


void push()
{
	if( store && n<N )
	{
		q[n]=p->getCenter();
		n++;
	}
	else store = false;
}

void pushs()
{
	if( store && m<N )
	{
		s[m]=p->getCenter();
		m++;
	}
}

void fd( double dist )
{
	while( isRunning() && dist>0 )
	{
		p->translateLocal( {( dist>=2 )?0.2:dist/5.0,0,0} );
		dist -= 2;
		a1 -= 12;
		a2 -= 12;
		push();
	}
}

void rt( double dist )
{
	pushs();
	while( isRunning() && dist>0 )
	{
		p->rotateLocal( ( dist>=8 )?8:dist, {0,0,-1} );
		a1 -= 8;
		a2 += 8;
		dist -= 8;
	}
}

void lt( double dist )
{
	pushs();
	while( isRunning() && dist>0 )
	{
		p->rotateLocal( ( dist>=8 )?8:dist, {0,0,1} );
		a1 += 8;
		a2 -= 8;
		dist -= 8;
	}
}

void house()
{
	fd( 50 );
	rt( 90 );
	fd( 15 );
	lt( 135 );
	fd( 80/sqrt( 2 ) );
	lt( 90 );
	fd( 80/sqrt( 2 ) );
	lt( 135 );
	fd( 15 );

	viewY2 = 0;
	viewDistance2 = 34;

	rt( 90 );
	fd( 50 );
	lt( 90 );
	fd( 50 );
}


int main()
{
	/// Open window
	if( !openWindow2D( "Turtle graphics: Houses" ) ) return 1;
	noGround();
	changeBrightness();

	viewX = -2.5;
	viewY = 4.3;
	viewDistance = 24;
	viewY2 = 4.3;
	viewDistance2 = 24;

	/// Add buttons
	addButton( new exitButton2D );

	p = new Sphere3D( 1.5, QUALITY_HIGH );
	p->setColor( {0.5,0.75,1,0.75} );
	p->setShininess( 30 );
	p->setCenter( {0,0,0} );
	p->rotateLocal( 90, {0,0,1} );

	GLuint tex = loadTexture( "Metal.jpg" );

	c = new Cube3D( 1 );
	c->setCenter( {0,0,-1} );
	c->setScale( {100,100,1} );
	c->setLighting( false );
	c->setTexture( loadTexture( "textures/Material_Scratch_Round.jpg" ) );
	c->setTextureSize( 15 );

	c1 = new Cylinder3D( 0.5,0.5,true,QUALITY_LOW );
	c1->setTexture( tex );
	c1->setTextureSize( 7 );

	c2 = new Cylinder3D( 0.5,0.5,true,QUALITY_LOW );
	c2->setTexture( tex );
	c2->setTextureSize( 7 );

	/// Main loop
	fd( 0.01 );

	while( isRunning() )
	{
		for( int i=0; i<3; i++ )
		{
			house();
			rt( 360/3 );
			fd( 50 );
			lt( 90 );
		}
		p->identity();
		p->setCenter( {0,0,0} );
		p->rotateLocal( 90, {0,0,1} );
	}

	return 0;
}

