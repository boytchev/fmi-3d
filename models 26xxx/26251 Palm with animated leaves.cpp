
/// Project:	FMI 3D (2013)
/// File:		26251 Palm with animated leaves.cpp
/// Topic:		26 Deformations
/// Slide:		25
///	About:		A palm tree with a set of animated leaves
///
/// History:
///		2013.06.06/PB	Initial implementation


#include "fmi3d.h"

#define N 20

//#include <iostream>

GLuint gold;

/// Pagoda
class Pagoda3D : public Object3D
{
	public:
		Pagoda3D( int floors );
		void drawImage();
	private:
		int floors;
		double k;
		double ofs;
		double spd;
};

Pagoda3D::Pagoda3D( int floors ) : Object3D()
{
	this->floors = floors;
	ofs = random( 0,2*M_PI );
	spd = random( 0.5,1.5 );
	k = 1.01*pow( 0.1,1.0/floors );
	setColor( {0.5,0.3,0} );
}


void Pagoda3D::drawImage( )
{
	if( !isVisible ) return;
	if( inShadow ) return;

	glEnable( GL_FOG );
	glFogi( GL_FOG_MODE, GL_LINEAR );
	glFogf( GL_FOG_START,viewDistance-10 );
	glFogf( GL_FOG_END,viewDistance+40 );

	prepareImageProps();
	glScaled( 0.5,0.5,0.75 );
	double rz = 4*sin( t*spd+ofs );
	double ry = 3*cos( t*spd+ofs*ofs );
	for( int i=0; i<floors; i++ )
	{
		glCallList( idUnitCylinder3D[QUALITY_LOW] );
		glRotated( rz,0,0,1 );
		glRotated( ry,0,1,0 );
		glTranslated( 0,0,1 );
		glScaled( k,k,k );
		rz*=0.9;
		ry*=0.9;
	}

	/// Palm leaves
	glColor3f( 0,0.5,0 );
	glScaled( 5,5,5 );
	ry = 10+9*cos( t*spd+ofs*ofs );

	for( int j=0; j<5; j++ )
	{
		glRotated( 72,0,0,1 );
		glPushMatrix();
		for( int i=0; i<40; i++ )
		{
			glCallList( idUnitCylinder3D[QUALITY_LOW] );
			glRotated( ry,0,1,0 );
			glTranslated( 0,0,1 );
			glScaled( 0.94,0.94,0.96 );
		}
		glPopMatrix();
	}
	restoreImageProps();
}

Pagoda3D* pagoda[N];

void drawScene()
{
	for( int i=0; i<N; i++ )
		pagoda[i]->drawImage();
}


void drawFrames()
{
//	water->drawFrame();
}

int main()
{
	/// Open window
	if( !openWindow3D( "Palm tree with animated leaves" ) ) return 1;
	changeGround();

	addButton( new exitButton2D );

	viewZ = 7;
	viewDistance = 40;

	for( int i=0; i<N; i++ )
	{
		pagoda[i] = new Pagoda3D( irandom( 25,45 ) );
		pagoda[i]->setCenter( {random( -15,15 ),random( -15,15 ),0.2} );
		pagoda[i]->rotateLocal( random( 0,360 ), {0,0,1} );
	}

	/// Main loop with automatic animation
	double t0=t;
	while( isRunning() )
	{
		viewAlpha += ( t-t0 )/10;
		t0=t;
	}

	return 0;
}
