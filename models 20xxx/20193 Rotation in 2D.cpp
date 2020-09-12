
/// This model does not support 3D stereo
///
/// Project:	FMI 3D (2013)
/// File:		20193 Rotation in 2D
/// Topic:		20 Projections
/// Slide:		19
///	About:		Rotating the camera by changing the "Up" vector
///
/// History:
///		2013.08.10/KK	Initial implementation
///		2013.08.12/PB	Finetuning


#include "fmi3d.h"

#define ANIMATION_TIME 20

CoordSys2D* cs;
double t0;
double minx,maxx;

GLdouble    eyeX = 0.0;
GLdouble    eyeY = 0.0;
GLdouble    eyeZ = 15.0;
GLdouble    upX = 0.0;
GLdouble    upY = 1.0;
GLdouble    upZ = 0.0;

void drawScene()
{
	glPushMatrix();
	glLoadIdentity();

    gluLookAt(eyeX, eyeY, eyeZ,
              0, 0, 0,
              upX, upY, upZ);

	cs->drawImage();

    glDepthMask( GL_FALSE );

	glLineWidth( 10 );
	glEnable( GL_LINE_SMOOTH );

	glDisable(GL_LIGHTING);
	glColor3f( 1,0,0 );
	glBegin( GL_LINE_STRIP );
	for( double x=minx; x<=maxx; x+=0.002 )
	{
		double r = 3+2*sin(5*(x-minx));
		glVertex2f(r*cos(x),r*sin(x));
	}
	glEnd();

	glDepthMask( GL_TRUE );

	glPopMatrix();
}

void drawFrames()
{
	glPushMatrix();
	glLoadIdentity();

    gluLookAt(eyeX, eyeY, eyeZ,
              0, 0, 0,
              upX, upY, upZ);

	cs->drawFrame();

	glPopMatrix();
}

int main()
{
	if( !openWindow3D( "Rotation in 2D" ) ) return 1;
	noGround();

	addButton( new exitButton2D() );

	cs = new CoordSys2D(-5.5, 5.5, -5.5, 5.5,false,5,0.5,L"XY");

	isRunning();
	t0 = t;
	while( isRunning() )
	{
		maxx = 2*M_PI*(0.5-0.5*cos((t-t0)/ANIMATION_TIME*M_PI));
		minx=0;
        upX = cos(maxx);
		upY = sin(maxx);
	}

	return 0;
}
