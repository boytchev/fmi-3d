
/// Project:	FMI 3D (2013)
/// File:		09221 OpenGL triangles
/// Topic:		9 Graphical Primitives
/// Slide:		22
///	About:		OpenGL triangles
///
/// History:
///		2013.07.01/MD	Initial implementation
///		2013.07.02/PB	Minor fixes


#include "fmi3d.h"
#define N 18
const double radius=0.1;
Sphere3D* points[N];


void drawScene()
{
	for( int i=0; i<N; i++ )
		points[i]->drawImage();

	glDisable(GL_LIGHTING);

	glColor4f(0.5,0.5,1,0.5);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glDepthMask(GL_FALSE);
    glBegin( GL_TRIANGLES);
	for(int i=0; i<N; i++) points[i]->getCenter().vertex();
	glEnd();
    glDepthMask(GL_TRUE);
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

    glBegin( GL_TRIANGLES);
	for(int i=0; i<N; i++) points[i]->getCenter().vertex();
	glEnd();
}

void randomize()
{
	double p=12.0;
	for( int i=0; i<N; i++ )
	{
		points[i]->setCenter( { random( -p/2,p/2 ),
								random( -p/2,p/2 ), random( 0,0.75*p )
							  } );
	}
}
int main()
{
	/// Open window
	if( !openWindow3D( "OpenGL triangles" ) ) return 1;

	addButton( new Button2D( "buttons/button_random.png",'R',randomize ) );
	addButton( new exitButton2D );

	viewZ = 3;

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
