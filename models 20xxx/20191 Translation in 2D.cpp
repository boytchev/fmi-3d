
/// This model does not support 3D stereo
///
/// Project:	FMI 3D (2013)
/// File:		20191 Translation in 2D
/// Topic:		20 Projections
/// Slide:		19
///	About:		Translation in 2D
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
GLdouble    eyeZ = 10.0;
GLdouble    targetX = 0.0;
GLdouble    targetY = 0.0;
GLdouble    targetZ = 0.0;

void drawScene()
{
	glPushMatrix();
	glLoadIdentity();

	gluLookAt( eyeX, eyeY, eyeZ,
			   targetX, targetY, targetZ,
			   0, 1, 0 );

	cs->drawImage();

	glDepthMask( GL_FALSE );
	glLineWidth( 10 );
	glEnable( GL_LINE_SMOOTH );

	glDisable(GL_LIGHTING);
	glColor3f( 1,0,0 );
	glBegin( GL_LINE_STRIP );
	for( double x=minx; x<=maxx; x+=0.02 ) glVertex2f(x,sin(x));
	glEnd();

	glDepthMask( GL_TRUE );

	glPopMatrix();
}


void drawFrames()
{
	glPushMatrix();
	glLoadIdentity();

	gluLookAt( eyeX, eyeY, eyeZ,
			   targetX, targetY, targetZ,
			   0, 1, 0 );

	cs->drawFrame();

	glPopMatrix();
}

int main()
{
	if( !openWindow3D( "Translation in 2D" ) ) return 1;
	noGround();

	addButton( new exitButton2D() );

	cs = new CoordSys2D( -2, 50, -3, 3, false, 10, 0.2, L"XY" );

	isRunning();
	t0=t;
	while( isRunning() )
	{
		maxx = 49*(0.5-0.5*cos((t-t0)/ANIMATION_TIME*M_PI));
		minx = maxx-12;
		if( minx<0 ) minx=0;
		eyeX = targetX = maxx-4;
	}

	return 0;
}
