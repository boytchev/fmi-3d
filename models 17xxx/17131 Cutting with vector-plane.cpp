
/// Project:	FMI 3D (2013)
/// File:		17131 Cutting with vector-plane.cpp
/// Topic:		17 Cutting
/// Slide:		13
///	About:		Cutting with plane defined by point and normal vector
///
/// History:
///		2013.08.08/PB	Initial implementation


#include "fmi3d.h"

Cylinder3D *c;
Sphere3D *s,*sm;
CoordSys3D *cs;
GLdouble cp[4]= {1,0,0,0};
GLdouble cp2[4]= {0,0,1,-6};
GLdouble w[4];
Vector3D *v;
Circle2D *ci;

void drawStick()
{
	c->drawImage();
	glEnable( GL_CLIP_PLANE2 );
	glClipPlane( GL_CLIP_PLANE2,cp2 );
	s->drawImage();
	glDisable( GL_CLIP_PLANE2 );
	glScaled( 1,1,-1 );
	c->drawImage();
	glEnable( GL_CLIP_PLANE2 );
	glClipPlane( GL_CLIP_PLANE2,cp2 );
	s->drawImage();
	glDisable( GL_CLIP_PLANE2 );
}

void drawScene()
{
	glEnable( GL_CLIP_PLANE3 );
	glClipPlane( GL_CLIP_PLANE3,w );

	glPushMatrix();
	drawStick();
	glRotated( 90,0,1,0 );
	drawStick();
	glRotated( 90,1,0,0 );
	drawStick();
	glPopMatrix();
	glDisable( GL_CLIP_PLANE3 );

	glDisable(GL_LIGHTING);

	cs->drawImage();
	v->drawImage();
	ci->drawImage();
	sm->drawImage();
}


void drawFrames()
{
	cs->drawFrame();
	ci->drawFrame();
}


int main()
{
	/// Open window
	if( !openWindow3D( "Cutting with vector-plane" ) ) return 1;
	noGround();

	/// Add buttons
	addButton( new exitButton2D );

	viewZ = 1;
	viewDistance = 22;

	c = new Cylinder3D( 0.75,6,false,QUALITY_HIGH );
	c->setColor( {0.5,0.5,1} );
	c->setShininess( 100 );

	s = new Sphere3D( 0.75,QUALITY_HIGH );
	s->setColor( {0.5,0.5,1} );
	s->setShininess( 100 );
	s->setCenter( {0,0,6} );

	sm = new Sphere3D( 0.15 );
	sm->setColor( {1,1,1} );

	ci = new Circle2D(4,QUALITY_HIGH);
	ci->setColor({1,0,0,0.4});
	ci->setLighting(false);

	cs = new CoordSys3D( -8,8,-8,8,-8,8,false,1,0.5 );

	v = new Vector3D({0,0,1},0.5);
	v->setColor({1,0,0});

	/// Main loop
	double t0=t;
	while( isRunning() )
	{
		viewAlpha -= (t-t0)/5;
		t0=t;

		double al = 0.5*t;
		double be = 0.6789*t-0.2;

		vect_t dir = {cos(al)*cos(be),sin(al)*cos(be),sin(be)};
		dir = dir.mul(1/dir.distance());
		double len = 2.5+1.0*sin(0.789*t);
		v->setLength(len-0.5);
		v->setDirection(dir);

		w[0] = -dir.x;
		w[1] = -dir.y;
		w[2] = -dir.z;
		w[3] = len;

		ci->identity();
		ci->rotateLocal(90,{0,1,0});
		ci->rotateLocal(180*al/M_PI,{-1,0,0});
		ci->rotateLocal(180*be/M_PI,{0,-1,0});
		ci->translateLocal({0,0,len});
		sm->setCenter(ci->getCenter());
	}
	return 0;
}

