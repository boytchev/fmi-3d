
/// Project:	FMI 3D (2013)
/// File:		09212 OpenGL line strips.cpp
/// Topic:		9 Graphical Primitives
/// Slide:		21
///	About:		OpenGL line strips
///
/// History:
///		2013.06.28/MD	Initial implementation
///		2013.07.02/PB	Minor fixes
///     2013.09.11/PB   Fixed segment smoothness

#include "fmi3d.h"

#define N 8
#define M 3
const double radius=0.2;
Sphere3D* points[3][N];
vect_t cols[M] = { {0,0,0},{1,0,0},{0.2,0.2,1} };

void drawScene()
{
	for( int j=0; j<M; j++ )
	{
		for( int i=0; i<N; i++ )
			points[j][i]->drawImage();
	}

    glEnable(GL_LINE_SMOOTH);
    glDisable(GL_LIGHTING);
    glDepthMask(GL_FALSE);
	glLineWidth( 3 );
	for( int j=0; j<M; j++ )
	{
		glColor3f( cols[j].x, cols[j].y, cols[j].z );
		glBegin( GL_LINE_STRIP );
		for( int i=0; i<N; i++ )
			points[j][i]->getCenter().vertex();
		glEnd();
	}
	glDepthMask(GL_TRUE);
    glDisable(GL_LINE_SMOOTH);
	for( int j=0; j<M; j++ )
	{
		glColor3f( cols[j].x, cols[j].y, cols[j].z );
		glBegin( GL_LINE_STRIP );
		for( int i=0; i<N; i++ )
			points[j][i]->getCenter().vertex();
		glEnd();
	}
}


void drawFrames()
{
}

void randomize()
{
	double r,a[M]= {0,180,-90},p;

	for( int i=0; i<N; i++ )
	{
		r=4.0+random(0,1);
		p=r/2;
		points[0][i]->setCenter( {r*sin(a[0]), r*cos(a[0]), random(-p/2,p/2)+4} );
		points[1][i]->setCenter( {r*sin(a[1]), random(-p/2,p/2), r*cos(a[1])+4} );
		points[2][i]->setCenter( {random(-p/2,p/2), r*sin(a[2]), r*cos(a[2])+4} );
		for(int j=0; j<M; j++) a[j]+=random(0,30*M_PI/180);
	}
}

int main()
{
	/// Open window
	if( !openWindow3D( "OpenGL line strips" ) ) return 1;

	addButton( new Button2D( "buttons/button_random.png",'R',randomize ) );
	addButton( new exitButton2D );

	viewZ = 3;

	for( int j=0; j<M; j++ )
	{
		for( int i=0; i<N; i++ )
		{
			points[j][i] = new Sphere3D( radius );
			points[j][i]->setColor( cols[j] );
			points[j][i]->setShininess( 20 );
		}
	}
	randomize();

	/// Main loop with automatic animation
	double t0=t;
	while( isRunning() )
	{
		viewAlpha += ( t-t0 )/20;
		t0=t;
	}

	return 0;
}
