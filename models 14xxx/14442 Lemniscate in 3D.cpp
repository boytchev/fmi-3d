
/// Project:	FMI 3D (2013)
/// File:		14442 Lemniscate in 3D.cpp
/// Topic:		14 Orientation
/// Slide:		44
///	About:		Lemniscate of Bernoulli in 3D
///
/// History:
///		2013.08.13/PB	Initial implementation
///     2013.09.13/PB   Fixed speed


#include "fmi3d.h"


#define N 361

CoordSys3D *cs;
PaperPlane3D *p;
vect_t q[N];
int n=0;

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
	for( int i=0; i<n; i++ ) q[i].vertex();
	glEnd();
	glDepthMask( GL_TRUE );
}


void drawFrames()
{
	p->drawFrame();
//	cs->drawFrame();
}


int main()
{
	/// Open window
	if( !openWindow3D( "Lemniscate in 3D" ) ) return 1;
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
	double t0=t, t1=t, fr=0;
	while( isRunning() )
	{
		viewAlpha += ( t-t0 )/10;
		t0=t;

        if( t-t1>1/100.0 )
		{
		    fr++;
		    t1 = t;
            p->rotateLocal( 2.405*sin( fr*M_PI/180 ), {0,0,1} );
            p->translateLocal( {0.1,0,0} );
            if( n<N )
            {
                q[n]=p->getCenter().sub( {0,0,0.1} );
                n++;
            }
		}
	}

	return 0;
}

