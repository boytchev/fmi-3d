
/// Project:	FMI 3D (2013)
/// File:		14521 Centipede algorithm.cpp
/// Topic:		14 Orientation
/// Slide:		52
///	About:		Demonstration of the centipede algorithm
///
/// History:
///		2013.08.14/PB	Initial implementation
///     2013.09.13/PB   Fixed shininess


#include "fmi3d.h"


#define N 500

Sphere3D *p;
vect_t p1[N],p2[N];
int n=0;
double t0,k=0.99;

#define S 10
double s[S],o[S];

void drawScene()
{
	glDepthMask( GL_FALSE );
	glDisable(GL_DEPTH_TEST);
	glDisable( GL_TEXTURE_2D );
	glDisable( GL_LIGHTING );
	glColor3f( 0.5,0.5,1 );
	glEnable( GL_POLYGON_SMOOTH );
	glBegin( GL_QUAD_STRIP );
	for( int i=n; i<n+N; i++ )
	{
		p1[i%N].vertex();
		p2[i%N].vertex();
	}
	glEnd();

	glEnable(GL_LINE_SMOOTH);
	glLineWidth(2);
	glColor3f(0,0,0);
	glBegin( GL_LINE_STRIP );
	for( int i=n; i<n+N; i++ )
	{
		p1[i%N].add(p2[i%N]).mul(0.5).vertex();
	}
	glEnd();
	glBegin( GL_LINES );
	for( int i=n; i<n+N; i+=30 )
	{
		vect_t v = p1[i%N].sub(p2[i%N]);
		p1[i%N].add(v).vertex();
		p2[i%N].sub(v).vertex();
	}
	glEnd();

	glEnable(GL_POINT_SMOOTH);
	glPointSize(8);
	glBegin( GL_POINTS );
	for( int i=n; i<n+N; i+=30 )
	{
		vect_t v = p1[i%N].sub(p2[i%N]);
		p1[i%N].add(v).vertex();
		p2[i%N].sub(v).vertex();
	}
	glEnd();

	glDepthMask( GL_TRUE );
	glEnable(GL_DEPTH_TEST);


	glEnable( GL_CULL_FACE );
	glCullFace( GL_FRONT );
	p->drawImage();
	glDisable( GL_CULL_FACE );
}


void drawFrames()
{
}


void fd( double dist )
{
	p->translateLocal( {dist/10.0,0,0} );
}

void rt( double dist )
{
	p->rotateLocal( dist, {0,0,-1} );
}

void lt( double dist )
{
	p->rotateLocal( dist, {0,0,1} );
}

double f()
{
	double y = 0;
	for(int i=0; i<S; i++)
		y = y+sin(s[i]*t+o[i]);
	return 30*y;
}

int main()
{
	/// Open window
	if( !openWindow3D( "Centipede algorithm" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();

	/// Add buttons
	addButton( new exitButton2D );

	p = new Sphere3D( 0.5, QUALITY_HIGH );
	p->setColor( {0.5,0.75,1,0.75} );
	p->setShininess( 30 );
	p->setCenter( {0,0,0} );
	p->rotateLocal( 90, {0,0,1} );

	for(int i=0; i<S; i++)
	{
		s[i]=random(0.5,5);
		o[i]=random(0,2*M_PI);
	}
	/// Main loop
	isRunning();
	t0=t;
	while( isRunning() )
	{
		double dt = (t-t0);
		viewAlpha += (t-t0)/10;
		t0 = t;
		fd(15*dt);
		lt(f()*dt);

		Object3D o;
		o.oxyz = p->oxyz;
		o.translateLocal({0,0.2,0});
		p1[n%N] = o.getCenter();
		o.translateLocal({0,-0.4,0});
		p2[n%N] = o.getCenter();
		n++;

		viewX = viewX*k+(1-k)*p->getCenter().x;
		viewY = viewY*k+(1-k)*p->getCenter().y;
	}

	return 0;
}

