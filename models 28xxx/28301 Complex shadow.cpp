
/// Project:	FMI 3D (2013)
/// File:		28301 Complex shadow.cpp
/// Topic:		28 Light
/// Slide:		30
///	About:		Shadow of complex object
///
/// History:
///		2013.07.11/PB	Initial implementation
///     2013.09.15/PB   Added fix for 16-bit Z-buffers


#include "fmi3d.h"

GLuint tex;

void drawObject()
{
	glTranslated( 0,0,7+sin(7*t) );
	glScaled( 5,5,5 );
	glRotated( 50*t,1,0,1 );
	glRotated( 50*t,0,-1,1 );
	glCallList( idUnitCube3D );
	glRotated( 45,0,0,1 );
	glTranslated( 0,0,0.01 );
	glCallList( idUnitCube3D );
	glTranslated( 0,0,-0.01 );
	glRotated( 90,0,1,0 );
	glTranslated( 0.3,0,0 );

	for( int j=0; j<3; j++ )
	{
		for( int i=0; i<8; i++ )
		{
			glPushMatrix();
			glRotated( 45*i+22.5,1,0,0 );
			double m = 1.1+0.3*sin( 4*t+i+j*j );
			glScaled( 0.2,0.2,m );
			glCallList( idUnitCone3D[QUALITY_LOW] );
			glTranslated(0,0,1);
			glScaled(0.25,0.25,0.05/m);
			glCallList( idUnitSphere3D[QUALITY_LOW] );
			glPopMatrix();
		}
		glTranslated( -0.3,0,0 );
	}
}

void drawScene()
{
	if( inShadow ) return;
	if( inReflection ) return;

	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, tex );
	glColor3f( 1,1,1 );
	glPushMatrix();
	drawObject();
	glPopMatrix();

	glDisable( GL_TEXTURE_2D );
	glColor3f( 0,0,0 );
	glPushMatrix();
	glTranslated( 0,0,0.1 );
	glScaled( 1,1,0 );
	drawObject();
	glPopMatrix();
}


void drawFrames()
{
}


int main()
{
	/// Open window
	if( !openWindow3D( "Complex shadow" ) ) return 1;
	changeBrightness();
	changeGround();

	viewZ = 5;
	tex = loadTexture("textures/Material_Wood.jpg");

	/// Add buttons
	addButton( new exitButton2D );

	/// Main loop
	while( isRunning() )
	{
	}
	return 0;
}

