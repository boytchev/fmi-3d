
/// Project:	FMI 3D (2013)
/// File:		27211 Dense artificial fog.cpp
/// Topic:		27 Textures
/// Slide:		21
///	About:		A dense artificial fog effect
///
/// History:
///		2013.07.24/PB	Initial implementation


#include "fmi3d.h"

#define ANIMATION_TIME 2
#define N 100
Cube3D *c;

double fogD = 10;
void drawScene()
{
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(2);
	c->drawImage();
	glDisable(GL_LINE_SMOOTH);

	glColor4f(0.9,0.9,1,0.02);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glBegin(GL_QUADS);
	for(int i=0; i<N; i++)
	{
		double s = 8*double(i)/(N-1);
		glVertex3d(-4+s,-4,1);
		glVertex3d(-4+s,+4,1);
		glVertex3d(-4+s,+4,9);
		glVertex3d(-4+s,-4,9);
	}
	glEnd();
}


void drawFrames()
{
}


int main()
{
	/// Open window
	if( !openWindow3D( "Dense artificial fog" ) ) return 1;
	changeBrightness();
	changeGround();
	changeGround();

	/// Add buttons
	addButton( new exitButton2D );

	maxViewBeta = 1;
	viewBeta = 0.4;
	viewZ = 4;

	c = new Cube3D( 8 );
	c->setCenter({0,0,5});
	c->setPolygonMode(GL_LINE);
	c->setColor({0.5,0.5,1});
	c->setLighting(false);

	/// Main loop
	while( isRunning() )
	{
		viewAlpha = sin(t/2);
	}
	return 0;
}

