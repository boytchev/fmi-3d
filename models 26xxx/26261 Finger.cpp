
/// Project:	FMI 3D (2013)
/// File:		26261 Finger.cpp
/// Topic:		26 Deformations
/// Slide:		26
///	About:		A finger
///
/// History:
///		2013.06.16/PB	Initial implementation


#include "fmi3d.h"

//#include <iostream>

double k = 0.8;

/// Finger
class Finger3D : public Object3D
{
	public:
		Finger3D();
		void drawImage();
		double angle;
	private:
};

Finger3D::Finger3D() : Object3D()
{
	this->angle = 0;
	setColor( {1,0.80,0.65} );

	/// imageList = truncated cone
	glNewList( imageList,GL_COMPILE );
	{
#define PREC 64
		double dalpha = 2*M_PI/PREC;
		double dx = cos( dalpha );
		double dy = sin( dalpha );

		double x1 = 1;
		double y1 = 0;

		glBegin( GL_QUAD_STRIP ); /// side
		for( int i=0; i<=PREC; i++ )
		{
			glNormal3f( x1, y1, 0 );
			glVertex3f( x1, y1, 0 );
			glVertex3f( k*x1, k*y1, 10.0/3 );

			double x2 = x1*dx-y1*dy;
			double y2 = y1*dx+x1*dy;
			x1 = x2;
			y1 = y2;
		}
		glEnd();
		glEndList();
	}
}

void Finger3D::drawImage( )
{
	if( !isVisible ) return;
	if( inShadow ) return;

	prepareImageProps();

	for( int i=0; i<3; i++ )
	{
		glCallList( imageList );
		glTranslated( 0,0,10.0/3 );
		glScaled( k,k,k );
		glRotated( angle/2,1,0,0 );
		glCallList( idUnitSphere3D[QUALITY_HIGH] );
		glRotated( angle/2,1,0,0 );
	}

	restoreImageProps();
}

Finger3D* finger;

void drawScene()
{
	finger->drawImage();
}


void drawFrames()
{
}

int main()
{
	/// Open window
	if( !openWindow3D( "A finger" ) ) return 1;
	changeGround();
	changeGround();
	changeBrightness();

	addButton( new exitButton2D );

	viewZ = 4;
	viewDistance = 15;
	finger = new Finger3D();

	/// Main loop
	double t0=t;
	while( isRunning() )
	{
		viewAlpha += ( t-t0 )/10;
		t0=t;

		finger->angle = 50*( 1.1+sin( 2*t ) );
	}

	return 0;
}
