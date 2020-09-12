
/// Project:	FMI 3D (2013)
/// File:		10092 Concrete tetrapod pier
/// Topic:		10 Composite objects
/// Slide:		09
///	About:		A model of a pier of concrete tetrapods
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
};

Tetrapod3D::Tetrapod3D( ) : Object3D()
{
	setSize( 1 );

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
	glCallList( idUnitCone3D[QUALITY_LOW] );
	glDisable( GL_CLIP_PLANE2 );

	glEnable( GL_CLIP_PLANE2 );
	glClipPlane( GL_CLIP_PLANE2,cp2 );
	glTranslated( 0,0,0.435 );
	glScaled( 0.572,0.572,0.05 );
	glColor3f( 0.75,0.75,0.75 );
	glCallList( idUnitSphere3D[QUALITY_LOW] );
	glDisable( GL_CLIP_PLANE2 );
	glPopMatrix();
	glEndList();

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



#define N 80
#define W 10
Tetrapod3D *p[N];
Circle2D *water[W];

void drawScene()
{
	if( inReflection ) return;
	if( inShadow ) return;
	for( int i=0; i<N; i++ )
		p[i]->drawImage();

	glDepthMask(GL_FALSE);
	for( int i=0; i<W; i++ )
		water[i]->drawImage();
	glDepthMask(GL_TRUE);
}


void drawFrames()
{
}


int main()
{
	useCustomFogColor = true;
	customFogColor[0] = 0.6;
	customFogColor[1] = 0.65;
	customFogColor[2] = 1;
	customFogColor[3] = 1;

	if( !openWindow3D( "Concrete tetrapod pier" ) ) return 1;
	noGround();

	addButton( new exitButton2D() );

	viewDistance = 30;
	viewBeta = 0.1;

	GLuint tex = loadTexture( "10091 Concrete.jpg" );

	for( int i=0; i<N; i++ )
	{
		p[i] = new Tetrapod3D();
		p[i]->setCenter( {random( -14,14 ),random( -4,4 ),random( -1,3 )} );
		p[i]->setLighting( !false );
		p[i]->setTexture( tex );
		p[i]->rotateLocal( random( 0,360 ), {1,1,0} );
		p[i]->rotateLocal( random( 0,360 ), {1,0,1} );
		p[i]->rotateLocal( random( 0,360 ), {0,1,1} );
	}

	GLuint wat = loadTexture( "10091 Sea.jpg" );
	for( int i=0; i<W; i++ )
	{
		water[i] = new Circle2D( 500 );
		water[i]->setCenter( {0,0,0} );
		water[i]->setLighting( false );
		water[i]->setTexture( wat );
		water[i]->setTextureScale( {12,12,1,1} );
		water[i]->setColor( {1,1,1,0.35} );
	}

	double t0=t;
	while( isRunning() )
	{
		viewAlpha += ( t-t0 )/20;
		t0 = t;

		for( int i=0; i<W; i++ )
			water[i]->setCenter( {7*i+sin( t-i ),2*i+sin( 1.5*t+i ),0.7*cos( 0.72*t-i*i )} );
	}
	return 0;
}

