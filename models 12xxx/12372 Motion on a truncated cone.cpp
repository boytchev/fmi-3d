
/// Project:	FMI 3D (2013)
/// File:		12372 Motion on a truncated cone.cpp
/// Topic:		12 Trajectories
/// Slide:		37
///	About:		Motion on a truncated cone with dominant speed
///
/// History:
///		2013.08.30/PB	Initial implementation


#include "fmi3d.h"

#define ANIMATION_TIME 2

Cone3D *c;
Sphere3D *w;
Circle2D *k1,*k2;

void drawScene()
{
	w->drawImage();
	k1->drawImage();
	k2->drawImage();
	k2->drawFrame();

	GLdouble cp[4]={0,0,-1,5+2};
	glEnable(GL_CLIP_PLANE2);
	glClipPlane(GL_CLIP_PLANE2,cp);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	c->drawImage();
	glCullFace(GL_BACK);
	c->drawImage();
	glDisable(GL_CULL_FACE);
	glDisable(GL_CLIP_PLANE2);
}


void drawFrames()
{
	k1->drawFrame();
}


Button2D *dB;
double duvT = 0;
double duMax = 2;
double dvMax = 3;
double du = 0;
double dv = 0;
int mode = 0;
void toggleDuv()
{
	mode = (mode+1)%3;
	duvT = t;
	switch( mode )
	{
		case 0: duMax=random(2,3); dvMax=random(2,3); break;
		case 1: duMax=5.5; dvMax=0.5; break;
		case 2: duMax=0.5; dvMax=5.5; break;
	}
	dB->setState(mode);
}


int main()
{
	/// Open window
	if( !openWindow3D( "Motion on a truncated cone" ) ) return 1;

	/// Add buttons
	dB = new Button2D( "buttons/button_next.png",GLFW_KEY_SPACE,toggleDuv,mode,3 );
	addButton( new exitButton2D );
	changeGround();
	changeGround();
	changeGround();

	viewAlpha = -M_PI;
	viewZ = 3;

	c = new Cone3D( 4,10,false,QUALITY_HIGH );
	c->setCenter( {0,0,2} );
	c->setColor( {0.5,0.5,1,0.65} );

	k1 = new Circle2D( 2,QUALITY_HIGH );
	k1->setCenter( {0,0,5+2} );
	k1->setColor( {0.5,0.5,1,0.65} );

	k2 = new Circle2D( 4,QUALITY_HIGH );
	k2->setCenter( {0,0,2} );
	k2->setColor( {0.5,0.5,1,0.65} );

	w = new Sphere3D( 0.7 );
	w->setColor( {1,0.8,0} );

	/// Main loop
	isRunning();
	double t0=t,dt,u=M_PI,v=0;
	while( isRunning() )
	{
		dt=t-t0;
		t0=t;

		if( t-duvT<ANIMATION_TIME )
		{
			double k = 0.5+0.5*cos( ( ( t-duvT )/ANIMATION_TIME )*M_PI/2 );
			du = du*k+duMax*( 1-k );
			dv = dv*k+dvMax*( 1-k );
		}

		u += du*dt;
		v += dv*dt;

		double z = 2.5+2.5*sin(v);
		double r = 4*(1-z/10);

		w->setCenter({r*cos(u),r*sin(u),z+2});

		viewAlpha += dt/20;
	}
	return 0;
}

