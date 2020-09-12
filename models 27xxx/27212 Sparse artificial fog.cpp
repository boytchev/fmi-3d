
/// Project:	FMI 3D (2013)
/// File:		27212 Sparse artificial fog.cpp
/// Topic:		27 Textures
/// Slide:		21
///	About:		A dense artificial fog effect
///
/// History:
///		2013.07.24/PB	Initial implementation


#include "fmi3d.h"

#define ANIMATION_TIME 2
#define N 8
Cube3D *c;
Sphere3D *s;

double fogD = 10;
void drawScene()
{
	s->drawImage();

	glEnable(GL_LINE_SMOOTH);
	glLineWidth(2);
	c->drawImage();
	glDisable(GL_LINE_SMOOTH);

	glColor4f(0.9,0.9,1,0.3);
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
	if( !openWindow3D( "Sparse artificial fog" ) ) return 1;
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

	s = new Sphere3D( 1, QUALITY_HIGH );
	s->setColor({1,0.5,0});
	s->setScale({3,1.5,1.5});
	s->setShininess(50);

	/// Main loop
	double a=0;
	while( isRunning() )
	{
		viewAlpha = sin(t/3);
		s->setCenter({8*cos(t),0,5});
		s->clearOrientation();

		double dT = t-M_PI*int(t/M_PI);
		if (dT>M_PI/2) dT-=M_PI;

		if( fabs(dT)<0.7 ) a = 90*sin(dT/0.7*M_PI/2)+90;
		s->rotateLocal(a,{0,0,1});
	}
	return 0;
}

