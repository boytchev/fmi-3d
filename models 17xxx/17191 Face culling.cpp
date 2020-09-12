
/// Project:	FMI 3D (2013)
/// File:		17191 Face culling.cpp
/// Topic:		17 Cutting
/// Slide:		19
///	About:		Front and back face culling
///
/// History:
///		2013.08.08/PB	Initial implementation
///     2013.09.13/PB   Minor tweaks

#include "fmi3d.h"

Sphere3D *s1,*s2;

void drawScene()
{
	GLdouble cp[4] = {0,0,1,0};

	glEnable( GL_CLIP_PLANE2 );
	glClipPlane( GL_CLIP_PLANE2,cp );
	glDisable( GL_DEPTH_TEST );
    glDisable( GL_TEXTURE_2D );

	s1->setPolygonMode( GL_LINE );
	s2->setPolygonMode( GL_LINE );
	s1->setColor( {0,0,0} );
	s2->setColor( {0,0,0} );
	glEnable( GL_CULL_FACE );
	glCullFace( GL_FRONT );
	glCullFace( (inReflection)?GL_BACK:GL_FRONT );
	s1->drawImage();
	glCullFace( (!inReflection)?GL_BACK:GL_FRONT );
	s2->drawImage();
	glDisable( GL_CULL_FACE );

	s1->setPolygonMode( GL_FILL );
	s2->setPolygonMode( GL_FILL );
	s1->setColor( {0,0,1,0.5} );
	s2->setColor( {0,0,1,0.5} );
	glEnable( GL_CULL_FACE );
	glCullFace( (inReflection)?GL_BACK:GL_FRONT );
	s1->drawImage();
	glCullFace( (!inReflection)?GL_BACK:GL_FRONT );
	s2->drawImage();
	glDisable( GL_CULL_FACE );

	glEnable( GL_DEPTH_TEST );
	glDisable( GL_CLIP_PLANE2 );
}


void drawFrames()
{
}


int main()
{
	/// Open window
	if( !openWindow3D( "Face culling" ) ) return 1;
//	changeGround();

	/// Add buttons
	addButton( new exitButton2D );

	viewAlpha = -M_PI;

	s1 = new Sphere3D( 4,QUALITY_LOW );
	s1->setCenter( {0,6,2} );
	s1->setColor( {0,0,1,0.5} );
	s1->setShadeModel( GL_FLAT );
	s1->setShininess( 11 );

	s2 = new Sphere3D( 4,QUALITY_LOW );
	s2->setCenter( {0,-6,2} );
	s2->setColor( {0,0,1,0.5} );
	s2->setShadeModel( GL_FLAT );
	s2->setShininess( 11 );

	/// Main loop
	while( isRunning() )
	{
		s1->clearOrientation();
		s1->rotateLocal(30*t,{0,0,1});
		s2->clearOrientation();
		s2->rotateLocal(-30*t,{0,0,1});
	}
	return 0;
}

