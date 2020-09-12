
/// Project:	FMI 3D (2013)
/// File:		15391 Gift-wrapping algorithm.cpp
/// Topic:		15 Convex hulls
/// Slide:		39
///	About:		Constructing convex hull using the gift-wrapping algorithm
///
/// History:
///		2013.08.20/PB	Initial implementation


#include "fmi3d.h"

#define N 40

Object3D *p[N];
Object3D *q;
double alpha = 0;
int current = 0, first;
bool done;
int wrap[N];
int w=0;
int n=0;

double area( Object3D *a, Object3D *b, Object3D *c )
{
	vect_t A = a->getCenter();
	vect_t B = b->getCenter();
	vect_t C = c->getCenter();

	return ( A.x*( B.y-C.y )+B.x*( C.y-A.y )+C.x*( A.y-B.y ) )/2;
}

void drawScene()
{
	// Draw polygon vertices
	for( int i=0; i<n; i++ )
	{
		p[i]->drawImage();
	}

	// Draw wrappinf line
	if( !done )
	{
		glEnable( GL_LINE_SMOOTH );
		glDisable( GL_LIGHTING );
		glDisable( GL_TEXTURE_2D );
		glLineWidth( 5 );
		glColor3f( 1,0,0 );
		glBegin( GL_LINES );
		q->getCenter().sub( {0,0,0.5} ).vertex();
		p[current]->getCenter().sub( {0,0,0.5} ).vertex();
		glEnd();
	}

	// Draw polygon edges
	glLineWidth( 5 );
	glColor3f( 0,0,0 );
	glBegin( GL_LINE_STRIP );
	for( int i=0; i<w; i++ )
		p[wrap[i]]->getCenter().sub( {0,0,0.5} ).vertex();
	glEnd();


	// Draw polygon face
	glPolygonMode( GL_FRONT_AND_BACK,GL_FILL );
	glColor4f( 0.5,0.5,1,0.5 );
	glBegin( GL_POLYGON );
	for( int i=0; i<w; i++ )
		p[wrap[i]]->getCenter().sub( {0,0,0.5} ).vertex();
	glEnd();
}


void drawFrames()
{
	for( int i=0; i<n; i++ ) p[i]->drawFrame();
}


void randomize()
{
	n = irandom(N/10,N);
	double min = 1000;
	for( int i=0; i<n; )
	{
		double a = random( 0,2*M_PI );
		double r = sqrt( random( 0,1 ) );
		double x = r*6*cos( a );
		double y = r*4*sin( a );
		p[i]->setCenter( {x,y,1} );
		bool ok=true;
		for( int j=0; j<i; j++ )
			if( p[i]->getCenter().distance( p[j]->getCenter() )<0.75 )
				ok=false;

		if( ok )
		{
			p[i]->setColor( {0,0,0} );
			if( x<min )
			{
				min=x;
				current=i;
			}
			i++;
		}
	}

	alpha = 0;
	first = current;
	w=0;
	wrap[w] = current;
	w++;
	p[current]->setColor( {1,0,0} );
	done=false;
	q->setCenter( p[current]->getCenter().add( {100*sin( alpha ),100*cos( alpha ),0} ) );
}


int main()
{
	/// Open window
	if( !openWindow2D( "Gift-wrapping algorithm" ) ) return 1;
	noGround();

	/// Add buttons
	addButton( new Button2D( "buttons/button_random.png",GLFW_KEY_SPACE,randomize ) );
	addButton( new exitButton2D );

	q = new Object3D();
	for( int i=0; i<N; i++ ) p[i] = point2D( 0,0 );
	randomize();

	/// Main loop
	isRunning();
	double t0=t;
	while( isRunning() )
	{
		double dt = t-t0;
		t0=t;
		if( !done && alpha<2*M_PI )
		{
			alpha += dt/2;
			q->setCenter( p[current]->getCenter().add( {100*sin( alpha ),100*cos( alpha ),0} ) );

			for( int i=0; i<n; i++ ) if( i!=current )
				{
					double a = area( p[current],p[i],q );
					if( a<0 )
					{
						p[current]->setColor( {0.5,0.5,1} );
						wrap[w] = i;
						w++;
						current = i;
						done = ( current==first );
						if( !done ) p[i]->setColor( {1,0,0} );
					}
				}
		}
	}
	return 0;
}

