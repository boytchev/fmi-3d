
/// This model does not support 3D stereo
///
/// Project:	FMI 3D (2013)
/// File:		20151 Traditional 2D viewpoint
/// Topic:		20 Projections
/// Slide:		15
///	About:		Traditional 2D viewpoint
///
/// History:
///		2013.08.10/KK	Initial implementation
///		2013.08.12/PB	Finetuning


#include "fmi3d.h"

#define ANIMATION_TIME 5

CoordSys2D* cs;
Circle2D*   unitCircle;
double t0;

void drawScene()
{
	glPushMatrix();
	glLoadIdentity();

	gluLookAt( 0, 0, 8,
			   0, 0, 0,
			   0, 1, 0 );

	cs->drawImage();

	glDepthMask( GL_FALSE );
	glEnable(GL_LINE_WIDTH);
	glLineWidth(5);
	unitCircle->drawImage();
	glDisable(GL_LINE_WIDTH);

	glDepthMask( GL_FALSE );
	glLineWidth( 10 );
	glEnable( GL_LINE_SMOOTH );

	glColor3f( 1,0,0 );
	glBegin( GL_LINE_STRIP );
	double min = -4;
	double max = min+8*(t-t0)/ANIMATION_TIME ;
	if( max>4 ) max=4;
	for( double x=min; x<=max; x+=0.02 ) glVertex2f(x,sin(x));
	glEnd();

	glDepthMask( GL_TRUE );

	glPopMatrix();
}


void drawFrames()
{
	glPushMatrix();
	glLoadIdentity();

	gluLookAt( 0, 0, 8,
			   0, 0, 0,
			   0, 1, 0 );

	cs->drawFrame();

	glPopMatrix();
}


int main()
{
	if( !openWindow3D( "Traditional 2D viewpoint" ) ) return 1;
	noGround();

	addButton( new exitButton2D() );

	cs = new CoordSys2D( -4.5, 4.5,-2.5,2.5,false,1,0.3,L"XY" );
	unitCircle = new Circle2D( 1, QUALITY_HIGH );
	unitCircle->setPolygonMode(GL_LINE);
	unitCircle->setColor({0.5,0.5,1});
	unitCircle->setLighting(false);

	isRunning();
	t0 = t;
	while( isRunning() )
	{
	}

	return 0;
}

