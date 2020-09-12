
/// Project:	FMI 3D (2013)
/// File:		09222 OpenGL triangle fans
/// Topic:		9 Graphical Primitives
/// Slide:		22
///	About:		OpenGL triangle fans
///
/// History:
///		2013.07.01/MD	Initial implementation
///		2013.07.02/PB	Minor fixes

#include "fmi3d.h"
#define N 12
#define M 3
const double radius=0.15;
Sphere3D* points[M][N];
vect_t col[M];

void drawScene()
{
	for( int j=0; j<M; j++ )
		for( int i=0; i<N; i++ )
			points[j][i]->drawImage();

	glDisable( GL_LIGHTING );

	glColor4f( 0.5,0.5,1,0.5 );
	glPolygonMode( GL_FRONT_AND_BACK,GL_FILL );
	glDepthMask( GL_FALSE );
	for( int j=0; j<M; j++ )
	{
		glColor4f( col[j].x,col[j].y,col[j].z,0.5 );
		glBegin( GL_TRIANGLE_FAN );
		for( int i=0; i<N; i++ ) points[j][i]->getCenter().vertex();
		glEnd();
	}
	glDepthMask( GL_TRUE );
	drawFrames();
}


void drawFrames()
{
	glColor3f( 0,0,0 );
	glDisable( GL_LIGHTING );

	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glDisable( GL_POLYGON_OFFSET_FILL );
	glEnable( GL_LINE_SMOOTH );
	double lw=3.0-viewDistance/10.0;
	glLineWidth( lw>0.5?lw:0.5 );

	for( int j=0; j<M; j++ )
	{
		glBegin( GL_TRIANGLE_FAN );
		for( int i=0; i<N; i++ ) points[j][i]->getCenter().vertex();
		glEnd();
	}
}

void randomize()
{
	double r,a,y;

	for( int j=0; j<M; j++ )
		col[j] = {random( 0,1 ),random( 0,1 ),random( 0,1 )};

	for( int j=0; j<M; j++ )
	{
		a=-1.7;
		y=7*( j-M/2.0+0.5 );
		points[j][0]->setCenter( { random( -1,1 ),random( -2,2 )+y,random( 2,4 )} );
		points[j][0]->setColor(col[j]);
		for( int i=1; i<N; i++ )
		{
			r=5.0+random( -1,1 );
			double z = 2+1.25*r*cos( a );
			if( z<0.1 ) z = 0.1;
			y=y+random( -1,1 );
			points[j][i]->setCenter( {r*sin( a ),y,z} );
			points[j][i]->setColor(col[j]);
			a=a+random( 10,30 )*M_PI/180;
		}
	}
}
int main()
{
	/// Open window
	if( !openWindow3D( "OpenGL triangle fans" ) ) return 1;

	addButton( new Button2D( "buttons/button_random.png",'R',randomize ) );
	addButton( new exitButton2D );

	viewZ = 3;

	for( int j=0; j<M; j++ )
	{
		col[j] = {random( 0,1 ),random( 0,1 ),random( 0,1 )};
		for( int i=0; i<N; i++ )
		{
			points[j][i] = new Sphere3D( radius );
			points[j][i]->setColor( col[j] );
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
