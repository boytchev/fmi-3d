
/// Project:	FMI 3D (2013)
/// File:		26212 Whips.cpp
/// Topic:		26 Deformations
/// Slide:		21
///	About:		Whips
///
/// History:
///		2013.06.06/PB	Initial implementation


#include "fmi3d.h"

#define N 40

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
	k = pow(0.1,1.0/floors);

	setLighting( false );

	glNewList( imageList,GL_COMPILE );
	{
		glBegin( GL_QUAD_STRIP );


		glColor3f( 1,0,0 );
		glNormal3f( 0,0,2 );
		glTexCoord2d( 0,6 );
		glVertex3f( 0,0,0 );

		glNormal3f( 0,0,2 );
		glTexCoord2d( 1,6 );
		glVertex3f( 0,0,0 );


		glColor3f( 1,0.5,0 );
		glNormal3f( 1,1,0 );
		glTexCoord2d( 0,3.5 );
		glVertex3f( 4,4,1 );

		glNormal3f( 1,-1,0 );
		glTexCoord2d( 1,3.5 );
		glVertex3f( 4,-4,1 );

		glColor3f( 1,1,0 );

		glNormal3f( 1,1,0 );
		glTexCoord2d( 0,2 );
		glVertex3f( 4,4,4 );

		glNormal3f( 1,-1,0 );
		glTexCoord2d( 1,2 );
		glVertex3f( 4,-4,4 );

		glColor3f( 1,0,0 );

		glNormal3f( 0,0,2 );
		glTexCoord2d( 0,-1 );
		glVertex3f( 0,0,5 );

		glNormal3f( 0,0,2 );
		glTexCoord2d( 1,-1 );
		glVertex3f( 0,0,5 );

		glEnd();
	}
	glEndList();

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
	glScaled( 0.2,0.2,0.2 );

	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D,gold );

	double rz = 20*sin( t*spd+ofs );
	double ry = 15*cos( t*spd+ofs*ofs );
	for( int i=0; i<floors; i++ )
	{
		for( int j=0; j<4; j++ )
		{
			glCallList( imageList );
			glRotated( 90,0,0,1 );
		}
		glRotated( rz,0,0,1 );
		glRotated( ry,0,1,0 );
		glScaled(k,k,k);
		glTranslated( 0,0,5 );
		rz*=0.9;
		ry*=0.9;
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
	if( !openWindow3D( "Whips" ) ) return 1;
	changeGround();
	changeGround();
	changeBrightness();

	addButton( new exitButton2D );

	gold = loadTexture( "26141 Texture.jpg" );
	viewDistance = 30;

	for( int i=0; i<N; i++ )
	{
		pagoda[i] = new Pagoda3D( irandom( 15,50 ) );
		pagoda[i]->setCenter( {random( -25,25 ),random( -25,25 ),0.2} );
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
