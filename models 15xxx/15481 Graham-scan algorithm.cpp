
/// Project:	FMI 3D (2013)
/// File:		15391 15481 Graham-scan algorithm.cpp
/// Topic:		15 Convex hulls
/// Slide:		48
///	About:		Constructing convex hull using the Graham's-scan algorithm
///
/// History:
///		2013.08.20/PB	Initial implementation
///     2013.09.13/PB   Change button image

#include "fmi3d.h"

#define N 40

Object3D *p[N];
bool done;
int wrap[N];
int w=0;
int n=0,m;
vect_t col;
double maxAnimT = 3;

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


	// Draw polygon face
	glPolygonMode( GL_FRONT_AND_BACK,GL_FILL );
	if( !done )
	{
		glBegin( GL_POLYGON );
		glColor4f( col.x,col.y,col.z,0 );
		p[wrap[w-3]]->getCenter().sub( {0,0,0.5} ).vertex();
		glColor4f( col.x,col.y,col.z,1 );
		p[wrap[w-2]]->getCenter().sub( {0,0,0.5} ).vertex();
		glColor4f( col.x,col.y,col.z,0 );
		p[wrap[w-1]]->getCenter().sub( {0,0,0.5} ).vertex();
		glEnd();
	}
	else
	{
		glColor4f( 0.5,0.5,1,0.5 );
		glBegin( GL_POLYGON );
		for( int i=0; i<w; i++ )
			p[wrap[i]]->getCenter().sub( {0,0,0.5} ).vertex();
		glEnd();
	}

	// Draw polygon edges
	glLineWidth( 5 );
	glColor3f( 0,0,0 );
	glBegin( GL_LINE_STRIP );
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
	n = 1;
	vect_t q = {random( -6,-3 ),random( -1,1 ),1};
	p[0]->setCenter( q );

	double a = random( -1.5,-0.8 );
	for( int i=1; i<N && a<1.5; i++ )
	{
		double r = sqrt( random( 0,1 ) );
		double x = r*12*cos( a );
		double y = r*5*sin( a );
		p[i]->setCenter( q.add( {x,y,1} ) );
		p[i]->setColor( {0,0,0} );
		a += random( 0.1,0.2 );
		n++;
	}
	p[n-1]->setCenter( p[0]->getCenter() );
	wrap[0]=0;
	wrap[1]=1;
	wrap[2]=2;
	w=3; m=3;
	p[wrap[w-3]]->setColor( {1,0,0} );
	p[wrap[w-2]]->setColor( {1,0,0} );
	p[wrap[w-1]]->setColor( {1,0,0} );
	done = false;
}


Button2D *aB;
void changeAnim()
{
	if( maxAnimT>2 )
	{
		maxAnimT = 0.5;
		aB->setState(1);
	}
	else
	if( maxAnimT>0.2 )
	{
		maxAnimT = 0;
		aB->setState(2);
	}
	else
	{
		maxAnimT = 3;
		aB->setState(0);
	}

}

int main()
{
	/// Open window
	if( !openWindow2D( "Graham's-scan algorithm" ) ) return 1;
	noGround();

	/// Add buttons
	aB = new Button2D( "buttons/button_time.png",'S',changeAnim,0,3 );
	addButton( new Button2D( "buttons/button_random.png",GLFW_KEY_SPACE,randomize ) );
	addButton( new exitButton2D );

	for( int i=0; i<N; i++ ) p[i] = point2D( 0,0 );
	randomize();

	/// Main loop
	isRunning();
	double t0=t;
	double animT=0;
	while( isRunning() )
	{
		double dt = t-t0;
		t0=t;
		animT += dt;

		if( !done )
		{
			if( m<n )
			{
				if( animT<maxAnimT )
				{
					double a = area( p[wrap[w-3]],p[wrap[w-2]],p[wrap[w-1]] );
					if( a>0 )
						col = {0,0,1};
					else
						col = {1,0,0};
				}
				else
				{
					double a = area( p[wrap[w-3]],p[wrap[w-2]],p[wrap[w-1]] );
					if( a>0 )
					{
						p[wrap[w-3]]->setColor( {0.5,0.5,1} );
						wrap[w] = m;
						w++; m++;
						p[wrap[w-2]]->setColor( {1,0,0} );
						p[wrap[w-1]]->setColor( {1,0,0} );
					}
					else
					{
						p[wrap[w-2]]->setColor( {1,1,1} );
						wrap[w-2] = wrap[w-1];
						w--;
						p[wrap[w-3]]->setColor( {1,0,0} );
						p[wrap[w-2]]->setColor( {1,0,0} );
						p[wrap[w-1]]->setColor( {1,0,0} );
					}
					animT = 0;
				}
			}
			else
			{
				done = true;
				p[wrap[w-3]]->setColor( {0.5,0.5,1} );
				p[wrap[w-2]]->setColor( {0.5,0.5,1} );
				p[wrap[w-1]]->setColor( {0.5,0.5,1} );
			}
		}
	}
	return 0;
}

