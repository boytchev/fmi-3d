
/// Project:	FMI 3D (2013)
/// File:		12391 Motion on a sphere.cpp
/// Topic:		12 Trajectories
/// Slide:		39
///	About:		Motion on a sphere with dominant speed
///
/// History:
///		2013.08.30/PB	Initial implementation


#include "fmi3d.h"

#define ANIMATION_TIME 2

Sphere3D *s,*w;

void drawScene()
{
	s->setPolygonMode( GL_LINE );
	s->setColor( {0.2,0.2,0.4} );
	s->drawImage();
	w->drawImage();

	s->setColor( {0.5,0.5,1,0.65} );
	s->setPolygonMode( GL_FILL );
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	s->drawImage();
	glCullFace(GL_BACK);
	s->drawImage();
	glDisable(GL_CULL_FACE);
}


void drawFrames()
{
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
	if( !openWindow3D( "Motion on a sphere" ) ) return 1;

	/// Add buttons
	dB = new Button2D( "buttons/button_next.png",GLFW_KEY_SPACE,toggleDuv,mode,3 );
	addButton( new exitButton2D );
	changeGround();
	changeGround();
	changeGround();

	viewAlpha = -M_PI;
	viewZ = 6;

	s = new Sphere3D( 5 );
	s->setCenter( {0,0,7} );

	w = new Sphere3D( 0.7 );
	w->setColor( {1,0.8,0} );
	w->setCenter( {0,0,11} );

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

		w->setCenter({5*cos(u)*cos(v),5*sin(u)*cos(v),7+5*sin(v)});

		viewAlpha += dt/20;
	}
	return 0;
}

