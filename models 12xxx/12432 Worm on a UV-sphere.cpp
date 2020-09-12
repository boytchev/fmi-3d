
/// Project:	FMI 3D (2013)
/// File:		12432 Worm on a UV-sphere.cpp
/// Topic:		12 Trajectories
/// Slide:		43
///	About:		Worm on a UV-sphere
///
/// History:
///		2013.08.23/PB	Initial implementation


#include "fmi3d.h"

#define N 40
Sphere3D *s,*w[N];

void drawScene()
{
	s->setPolygonMode( GL_LINE );
	s->setColor( {0,0,0} );
	s->drawImage();
	for(int i=0; i<N; i++) w[i]->drawImage();

	s->setPolygonMode( GL_FILL );
	s->setColor( {0.5,0.5,1,0.85} );
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


int main()
{
	/// Open window
	if( !openWindow3D( "Worm on a UV-sphere" ) ) return 1;

	/// Add buttons
	addButton( new exitButton2D );
	changeGround();
	changeGround();
	changeGround();

	viewAlpha = -M_PI;
	viewZ = 5;

	s = new Sphere3D( 5,QUALITY_MEDIUM);
	s->setCenter( {0,0,6} );
	s->setColor( {0.5,0.5,1,0.85} );

	for(int i=0; i<N; i++)
	{
		w[i] = new Sphere3D( 1*pow(0.96,i),QUALITY_LOW );
		w[i]->setColor( {1,0.5+0.5*i/(N-1),0} );
		w[i]->setCenter( {0,0,11} );
	}

	/// Main loop
	isRunning();
	double t0=t,dt,al=0,u=0,v=M_PI/2;
	while( isRunning() )
	{
		dt=t-t0;
		t0=t;

		s->clearOrientation();
		s->rotateLocal(30*t,{0,0,1});
		s->rotateLocal(23*t,{1,0,0});

		for(int i=N-1; i>=1; i--) w[i]->setCenter(w[i-1]->getCenter());

		al += dt*random(-5,5);
		double r = 2*dt;
		u += r*cos(al);
		v += r*sin(al);

		w[0]->setCenter({5*cos(u)*cos(v),5*sin(u)*cos(v),6+5*sin(v)});

		double a = M_PI/2-atan2(w[0]->oxyz.o.x,w[0]->oxyz.o.y);
		while (a<viewAlpha-M_PI) a+=2*M_PI;
		while (a>viewAlpha+M_PI) a-=2*M_PI;
		viewAlpha = viewAlpha*0.98+0.02*a;
	}
	return 0;
}

