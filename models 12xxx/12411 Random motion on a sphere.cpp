
/// Project:	FMI 3D (2013)
/// File:		12411 Random motion on a sphere.cpp
/// Topic:		12 Trajectories
/// Slide:		41
///	About:		Random motion on the surface of a sphere
///
/// History:
///		2013.08.30/PB	Initial implementation


#include "fmi3d.h"

Sphere3D *s,*w;

void drawScene()
{
	s->setPolygonMode( GL_LINE );
	s->setColor( {0.2,0.2,0.4} );
	s->drawImage();
	w->drawImage();

	s->setPolygonMode( GL_FILL );
	s->setColor( {0.5,0.5,1,0.65} );
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
	if( !openWindow3D( "Random motion on a sphere" ) ) return 1;

	/// Add buttons
	addButton( new exitButton2D );
	changeGround();
	changeGround();
	changeGround();

	viewAlpha = -M_PI;
	viewZ = 5;

	s = new Sphere3D( 5 );
	s->setCenter( {0,0,6} );

	w = new Sphere3D( 0.7 );
	w->setColor( {1,0.8,0} );
	w->setCenter( {0,0,11} );

	/// Main loop
	isRunning();
	double t0=t,dt,al=0,u=M_PI,v=0;
	while( isRunning() )
	{
		dt=t-t0;
		t0=t;

		al += dt*random(-10,10);
		double r = 3*dt;
		u += r*cos(al);
		v += r*sin(al);

		w->setCenter({5*cos(u)*cos(v),5*sin(u)*cos(v),6+5*sin(v)});

		viewAlpha += dt/20;
	}
	return 0;
}

