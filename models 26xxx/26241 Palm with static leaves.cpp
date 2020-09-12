
/// Project:	FMI 3D (2013)
/// File:		26241 Palm with static leaves.cpp
/// Topic:		26 Deformations
/// Slide:		24
///	About:		A palm tree with a set of static leaves
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

	GLdouble cp2[4]= {0,-2,1,0};
	GLdouble cp3[4]= {0,2,1,0};
	glNewList( imageList,GL_COMPILE );
	{
		glPushMatrix();
		glEnable( GL_CLIP_PLANE2 );
		glEnable( GL_CLIP_PLANE3 );
		glClipPlane( GL_CLIP_PLANE2,cp2 );
		glClipPlane( GL_CLIP_PLANE3,cp3 );
		glColor3f( 0,0.5,0 );
		glTranslated( 1,0,0 );
		glCallList( idUnitSphere3D[QUALITY_LOW] );
		glDisable( GL_CLIP_PLANE2 );
		glDisable( GL_CLIP_PLANE3 );
		glPopMatrix();
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
	glScaled(0.5,0.5,0.75);
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
	double ang = -40;
	glScaled( 20,20,15 );
	for( int j=0; j<4; j++ )
	{
		for( int i=0; i<5; i++ )
		{
			glRotated(-ang,0,1,0);
			glCallList( imageList );
			glRotated(ang,0,1,0);
			glRotated( 72,0,0,1 );
		}
		ang +=25;
		glRotated( 36,0,0,1 );
		//glTranslated( 0,0,0.1 );
		glScaled( 0.9,0.9,0.9 );
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
	if( !openWindow3D( "Palm tree with static leaves" ) ) return 1;
	changeGround();

	addButton( new exitButton2D );

	viewZ = 5;
	viewDistance = 40;
	viewBeta = 0.2;
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
