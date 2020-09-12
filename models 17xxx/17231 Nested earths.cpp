
/// Project:	FMI 3D (2013)
/// File:		17231 Nested earths.cpp
/// Topic:		17 Cutting
/// Slide:		23
///	About:		Nested earths
///
/// History:
///		2013.08.08/PB	Initial implementation
///     2013.09.13/PB   Simplified objects complexity


#include "fmi3d.h"

#define N 10
Sphere3D *s[N];

void drawScene()
{
	glPushAttrib( GL_FOG_BIT );
	if( !inReflection )
	{
		glEnable( GL_FOG );
		glFogf( GL_FOG_START,viewDistance-3 );
		glFogf( GL_FOG_END,viewDistance+5 );
	}

	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );
	for( int i=0; i<N; i++ ) s[i]->drawImage();
	glCullFace( GL_FRONT );
	for( int i=N-1; i>=0; i-- ) s[i]->drawImage();
	glDisable( GL_CULL_FACE );

	glDisable( GL_FOG );
	glPopAttrib();
}


void drawFrames()
{
}


int main()
{
	/// Open window
	if( !openWindow3D( "Nested earths" ) ) return 1;
	changeGround();
	changeBrightness();

	/// Add buttons
	addButton( new exitButton2D );

	viewZ = 5;
	viewDistance = 15;
	viewBeta = 0.1;

	GLuint tex = loadTexture( "17231 Texture Earth.png" );

	double r=5;
	for( int i=0; i<N; i++,r=r*0.8 )
	{
		s[i] = new Sphere3D( r, QUALITY_LOW );
		s[i]->setCenter( {0,0,5} );
		s[i]->setTexture( tex );
		s[i]->setTextureScale( {1,-1} );
		s[i]->setColor( {0.5-0.5*cos(1.5*i),0.5-0.5*cos(3.5*i),0.5+0.5*sin(3*i),1-r/10} );
		s[i]->setLighting( false );
	}

	/// Main loop
	while( isRunning() )
	{
		for( int i=0; i<N; i++ )
		{
			s[i]->clearOrientation();
			s[i]->rotateLocal( 30*t*( 1+i/1.2 )*(i%2-0.5), {0,0,1} );
		}
	}
	return 0;
}

