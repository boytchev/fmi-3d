
/// Project:	FMI 3D (2013)
/// File:		28571 Specular light.cpp
/// Topic:		28 Light
/// Slide:		57
///	About:		Specular light with different colours
///
/// History:
///		2013.07.11/PB	Initial implementation


#include "fmi3d.h"

#define N 5
GLfloat specular[4] = {0,0,0,1};

void drawScene()
{
	glColor3f(0.1,0.1,0.1);
	glDisable( GL_TEXTURE_2D );
	glEnable( GL_LIGHTING);
	glShadeModel( GL_SMOOTH );
	glEnable(GL_AUTO_NORMAL);

	glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, specular );
	glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, noEmission );
	for( int i=0; i<N; i++ )
	{
		glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, exp(i)-0.8 );
		glPushMatrix();
		glTranslated(0,4.1*(i-N/2.0+0.5),3.5);
		glScaled(1.7,1.7,1.7);
		glCallList(idUnitSphere3D[QUALITY_HIGH]);
		glPopMatrix();
	}
}


void drawFrames()
{
}


int main()
{
	/// Open window
	if( !openWindow3D( "Specular light" ) ) return 1;

	/// Add buttons
	addButton( new exitButton2D );

	/// Main loop
	while( isRunning() )
	{
		specular[0]=0.5+0.5*sin(2*t);
		specular[1]=0.5+0.5*sin(2*t+2*M_PI/3);
		specular[2]=0.5+0.5*sin(2*t+2*M_PI/3*2);
	}
	return 0;
}

