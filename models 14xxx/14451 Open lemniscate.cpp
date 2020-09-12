
/// Project:	FMI 3D (2013)
/// File:		14451 Open lemniscate.cpp
/// Topic:		14 Orientation
/// Slide:		45
///	About:		Open lemniscate path
///
/// History:
///		2013.08.13/PB	Initial implementation
///     2013.09.13/PB   Fixed speed


#include "fmi3d.h"


#define N (360*4)

CoordSys3D *cs;
PaperPlane3D *p;
vect_t q[N];
int n=0,i=0,fr=0;

void drawScene()
{
	cs->drawImage();
	glFrontFace( GL_CCW );
	p->drawImage();

	glDisable( GL_TEXTURE_2D );
	glDisable( GL_LIGHTING );
	glDepthMask( GL_FALSE );
	glEnable( GL_LINE_SMOOTH );
	glColor3f( 1,0,0 );
	glLineWidth( 7 );
	glBegin( GL_LINE_STRIP );
	for( int j=0; j<n; j++ ) q[(N+fr-j)%N].vertex();
	glEnd();
	glDepthMask( GL_TRUE );
}


void drawFrames()
{
	p->drawFrame();
}


int main()
{
	/// Open window
	if( !openWindow3D( "Open lemniscate" ) ) return 1;
	noGround();

	/// Add buttons
	addButton( new exitButton2D );

	viewDistance = 15;

	p = new PaperPlane3D( 2 );
	p->setScale( {2.5,1.5,1.5} );
	p->setCenter( {0.14,-0.14,4} );
	p->rotateLocal( -48, {0,0,1} );
	p->setColor( {0.5,0.5,1} );
	p->setLighting( false );

	cs = new CoordSys3D( -8,8,-4,4,0,0.8,true,2,0.4,L"XYZ" );
	cs->setCenter( {0,0,0.1} );

	/// Main loop
	isRunning();
	double t0=t, t1=t;
	while( isRunning() )
	{
		viewAlpha += ( t-t0 )/10;
		t0=t;

		viewX = viewX*0.99+p->getCenter().x*0.01;
		viewY = viewY*0.99+p->getCenter().y*0.01;

        if( t-t1>1/100.0 )
		{
		    fr++;
		    t1 = t;
            p->rotateLocal( 2.2*sin( fr*M_PI/180 ), {0,0,1} );
            p->translateLocal( {0.1,0,0} );
            q[fr%N]=p->getCenter().sub( {0,0,0.1} );
            if( n<N ) n++;
        }
	}

	return 0;
}


