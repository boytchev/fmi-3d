
/// Project:	FMI 3D (2013)
/// File:		09231 OpenGL quads
/// Topic:		9 Graphical Primitives
/// Slide:		23
///	About:		OpenGL quads
///
/// History:
///		2013.08.30/PB	Initial implementation


#include "fmi3d.h"
#define N 36
const double radius=0.1;
Sphere3D* points[N];


void drawScene()
{
	for( int i=0; i<N; i++ )
		points[i]->drawImage();

	glDisable( GL_LIGHTING );

	glColor4f( 0.5,0.5,1,0.5 );
	glPolygonMode( GL_FRONT_AND_BACK,GL_FILL );
	glDepthMask( GL_FALSE );
	glBegin( GL_QUADS );
	for( int i=0; i<N; i++ ) points[i]->getCenter().vertex();
	glEnd();
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

	glBegin( GL_QUADS );
	for( int i=0; i<N; i++ ) points[i]->getCenter().vertex();
	glEnd();
}

void randomize()
{
	double p=12.0;
	for( int i=0; i<N; i+=4 )
	{
		Object3D o;
		o.setCenter( { random( -p/2,p/2 ),random( -p/2,p/2 ), random( 3,p ) } );
		o.rotateLocal( random( 0,360 ), {random( -1,1 ),random( -1,1 ),random( -1,1 )} );

		points[i+0]->setCenter( o.oxyz.o.add( o.oxyz.ox.mul( random( 2,4 ) ) ) );
		points[i+1]->setCenter( o.oxyz.o.add( o.oxyz.oy.mul( random( 2,4 ) ) ) );
		points[i+2]->setCenter( o.oxyz.o.sub( o.oxyz.ox.mul( random( 2,4 ) ) ) );
		points[i+3]->setCenter( o.oxyz.o.sub( o.oxyz.oy.mul( random( 2,4 ) ) ) );
	}
}
int main()
{
	/// Open window
	if( !openWindow3D( "OpenGL quads" ) ) return 1;

	addButton( new Button2D( "buttons/button_random.png",'R',randomize ) );
	addButton( new exitButton2D );

	viewZ = 6;

	for( int i=0; i<N; i++ )
	{
		points[i] = new Sphere3D( radius );
		points[i]->setColor( {0,0,0} );
		points[i]->setShininess( 20 );
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
