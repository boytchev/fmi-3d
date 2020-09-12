
/// Project:	FMI 3D (2013)
/// File:		15321 Point-by-point algorithm.cpp
/// Topic:		15 Convex hulls
/// Slide:		32
///	About:		Constructing convex hull using the point-by-point algorithm
///
/// History:
///		2013.08.20/PB	Initial implementation


#include "fmi3d.h"

#define N 6

Object3D *p[N],*q;
int inner[N];

double area( Object3D *a, Object3D *b, Object3D *c )
{
	vect_t A = a->getCenter();
	vect_t B = b->getCenter();
	vect_t C = c->getCenter();

	return ( A.x*( B.y-C.y )+B.x*( C.y-A.y )+C.x*( A.y-B.y ) )/2;
}

void drawScene()
{
	// Draw interactive point
	q->drawImage();

	// Draw polygon vertices
	for( int i=0; i<N; i++ )
	{
		if( inner[i]<2 )
		{
			p[i]->setSize( 0.3 );
			p[i]->setColor( {0,0,0} );
		}
		else
		{
			p[i]->setSize( 0.1 );
			p[i]->setColor( {1,1,1} );
		}
		p[i]->drawImage();
	}

	// Draw polygon edges
	glEnable( GL_LINE_SMOOTH );
	glDisable( GL_LIGHTING );
	glDisable( GL_TEXTURE_2D );
	glPolygonMode( GL_FRONT_AND_BACK,GL_LINE );
	for( int i=0; i<N; i++ )
	{
		if( inner[i]+inner[( i+1 )%N]>1 )
		{
			glEnable( GL_LINE_STIPPLE );
			glLineStipple( 3,0b1010101010101010 );
			glLineWidth( 1 );
			glColor3f( 0,0,0 );
		}
		else
		{
			glLineWidth( 5 );
			glColor3f( 0,0,0 );
		}
		glBegin( GL_LINES );
		p[i]->getCenter().sub( {0,0,0.5} ).vertex();
		p[( i+1 )%N]->getCenter().sub( {0,0,0.5} ).vertex();
		glEnd();
		glDisable( GL_LINE_STIPPLE );
	}

	// Draw polygon face
	glPolygonMode( GL_FRONT_AND_BACK,GL_FILL );
	glColor4f( 0.5,0.5,1,0.5 );
	glBegin( GL_POLYGON );
	for( int i=0; i<N; i++ ) p[i]->getCenter().vertex();
	glEnd();

	// Draw new faces
	glPolygonMode( GL_FRONT_AND_BACK,GL_FILL );
	glBegin( GL_TRIANGLES );
	for( int i=0; i<N; i++ ) if( inner[i]>0 && inner[(i+1)%N]>0 )
		{
			glColor4f( 1,0.5,1,1 );
			q->getCenter().sub( {0,0,0.5} ).vertex();
			glColor4f( 0.5,0.5,1,0.25 );
			p[i]->getCenter().sub( {0,0,0.5} ).vertex();
			p[(i+1)%N]->getCenter().sub( {0,0,0.5} ).vertex();
		}
	glEnd();

	// Draw new edges
	glPolygonMode( GL_FRONT_AND_BACK,GL_LINE );
	glLineWidth( 5 );
	glBegin( GL_LINES );
	for( int i=0; i<N; i++ ) if( inner[i]==1 )
		{
			glColor3f( 0.5,0.5,1 );
			q->getCenter().sub( {0,0,0.5} ).vertex();
			glColor3f( 0,0,0 );
			p[i]->getCenter().sub( {0,0,0.5} ).vertex();
		}
	glEnd();

}


void drawFrames()
{
	q->drawFrame();
	for( int i=0; i<N; i++ ) p[i]->drawFrame();
}


void randomize()
{
	for( int i=0; i<N; i++ )
	{
		double a=double( i )/N*2*M_PI;
		double r=random( 2,4 );
		p[i]->setCenter( {r*cos( a ), r*sin( a ), 1} );
	}
}


int main()
{
	/// Open window
	if( !openWindow2D( "Point-by-point algorithm" ) ) return 1;
	noGround();

	/// Add buttons
	addButton( new Button2D( "buttons/button_random.png",GLFW_KEY_SPACE,randomize ) );
	addButton( new exitButton2D );

	for( int i=0; i<N; i++ ) p[i] = point2D( 0,0 );
	randomize();

	q = interactivePoint2D( 0,0 );
	q->setColor( {0.5,0.5,1} );

	/// Main loop
	while( isRunning() )
	{
		for( int i=0; i<N; i++ ) inner[i]=0;
		for( int i=0; i<N; i++ )
		{
			double a = area( q,p[i],p[( i+1 )%N] );
			if( a<=0 )
			{
				inner[i]++;
				inner[( i+1 )%N]++;
			}
		}
	}
	return 0;
}

