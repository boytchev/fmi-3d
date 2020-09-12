
/// Project:	FMI 3D (2013)
/// File:		10091 Concrete tetrapod
/// Topic:		10 Composite objects
/// Slide:		09
///	About:		A model of a concrete tetrapod
///
/// History:
///		2013.08.16/PB	Initial implementation


#include "fmi3d.h"


class Tetrapod3D : public Object3D
{
	public:
		Tetrapod3D( );
		void drawImage();
	private:
		Sphere3D *s;
		Cone3D *c;
};

Tetrapod3D::Tetrapod3D( ) : Object3D()
{
	setSize( 2 );

	s = new Sphere3D( 1 );
	c = new Cone3D( 1, 6, false );

	GLdouble cp1[4] = {0,0,-1,2};
	GLdouble cp2[4] = {0,0,1,-0.448};

	/// Create empty display frame list of the pawn
	glNewList( frameList,GL_COMPILE );
	glPushMatrix();
	glEnable( GL_CLIP_PLANE2 );
	glClipPlane( GL_CLIP_PLANE2,cp1 );
	glTranslated( 0,0,0.2 );
	glScaled( 1,1,4 );
	glColor3f( 1,1,1 );
	glCallList( idUnitCone3D[QUALITY_MEDIUM] );
	glDisable( GL_CLIP_PLANE2 );

	//glClipPlane(GL_CLIP_PLANE2,cp2);
	glEnable( GL_CLIP_PLANE2 );
	glClipPlane( GL_CLIP_PLANE2,cp2 );
	glTranslated( 0,0,0.435 );
	glScaled( 0.572,0.572,0.05 );
	glColor3f( 0.75,0.75,0.75 );
	glCallList( idUnitSphere3D[QUALITY_MEDIUM] );
	glPopMatrix();
	glDisable( GL_CLIP_PLANE2 );
	glEndList();

	/// Create display list of the pawn
	glNewList( imageList,GL_COMPILE );
	glCallList( frameList );
	for( int i=0; i<3; i++ )
	{
		glPushMatrix();
		glRotated( 120*i,0,0,1 );
		glRotated( -115,0,1,0 );
		glCallList( frameList );
		glPopMatrix();
	}
	glEndList();
}

void Tetrapod3D::drawImage()
{
	if( !isVisible ) return;
	prepareImageProps();

	glCallList( imageList );

	restoreImageProps();
}



#define N 1
Tetrapod3D *p[N];

void drawScene()
{
	for( int i=0; i<N; i++ )
		p[i]->drawImage();
}


void drawFrames()
{
}


int main()
{
	if( !openWindow3D( "Concrete tetrapod" ) ) return 1;

	addButton( new exitButton2D() );

	GLuint tex = loadTexture( "10091 Concrete.jpg" );

	for( int i=0; i<N; i++ )
	{
		p[i] = new Tetrapod3D();
		p[i]->setCenter( {0,0,3} );
		p[i]->setLighting( !false );
		p[i]->setTexture(tex);
	}

	double t0=t;
	while( isRunning() )
	{
		viewAlpha += ( t-t0 )/20;
		t0 = t;
	}
	return 0;
}

