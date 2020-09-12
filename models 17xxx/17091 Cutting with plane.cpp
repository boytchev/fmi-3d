
/// Project:	FMI 3D (2013)
/// File:		17091 Cutting with plane.cpp
/// Topic:		17 Cutting
/// Slide:		07
///	About:		Cutting with plane defined by three points
///
/// History:
///		2013.08.08/PB	Initial implementation


#include "fmi3d.h"

Cylinder3D *c;
Sphere3D *s,*p[3];
CoordSys3D *cs;
GLdouble cp[4]= {1,0,0,0};
GLdouble cp2[4]= {0,0,1,-6};
GLdouble w[4];

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
	cs->drawImage();
	for( int i=0; i<3; i++ ) p[i]->drawImage();

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

	glColor4f(1,0,0,0.4);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glBegin(GL_TRIANGLES);
	for(int i=0; i<3; i++) p[i]->getCenter().vertex();
	glEnd();

}


void drawFrames()
{
	cs->drawFrame();

	glDisable(GL_LIGHTING);
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(3);
	glColor3f(1,0,0);

	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glBegin(GL_TRIANGLES);
	for(int i=0; i<3; i++) p[i]->getCenter().vertex();
	glEnd();
}


int main()
{
	/// Open window
	if( !openWindow3D( "Cutting with plane" ) ) return 1;
	noGround();

	/// Add buttons
	addButton( new exitButton2D );

	c = new Cylinder3D( 0.75,6,false,QUALITY_HIGH );
	c->setColor( {0.5,0.5,1} );
	c->setShininess( 100 );

	s = new Sphere3D( 0.75,QUALITY_HIGH );
	s->setColor( {0.5,0.5,1} );
	s->setShininess( 100 );
	s->setCenter( {0,0,6} );

	for( int i=0; i<3; i++ )
	{
		p[i] = new Sphere3D( 0.2,QUALITY_HIGH );
		p[i]->setColor( {1,0,0} );
		p[i]->setShininess( 20 );
	}
	cs = new CoordSys3D( -8,8,-8,8,-8,8,false,1,0.5 );

	/// Main loop
	double t0=t;
	while( isRunning() )
	{
		viewAlpha -= (t-t0)/3;
		t0=t;

		p[0]->setCenter( {5*sin( 0.25*t ),0,0} );
		p[1]->setCenter( {0,5*cos( 0.5*t ),0} );
		p[2]->setCenter( {0,0,5*sin( 0.35*t+0.3 )} );

		vect_t m={0,0,0,0};
		for(int i=0; i<3; i++)
			m = m.add(p[i]->getCenter());
		m = m.mul(1.0/3);
		for(int i=0; i<3; i++)
			p[i]->setCenter(p[i]->getCenter().add(p[i]->getCenter().sub(m).mul(2)));

		double x1 = p[0]->getCenter().x;
		double x2 = p[1]->getCenter().x;
		double x3 = p[2]->getCenter().x;

		double y1 = p[0]->getCenter().y;
		double y2 = p[1]->getCenter().y;
		double y3 = p[2]->getCenter().y;

		double z1 = p[0]->getCenter().z;
		double z2 = p[1]->getCenter().z;
		double z3 = p[2]->getCenter().z;

		w[0] = y1*( z2-z3 )+y2*( z3-z1 )+y3*( z1-z2 );
		w[1] = z1*( x2-x3 )+z2*( x3-x1 )+z3*( x1-x2 );
		w[2] = x1*( y2-y3 )+x2*( y3-y1 )+x3*( y1-y2 );
		w[3] = -( x1*( y2*z3-y3*z2 )+x2*( y3*z1-y1*z3 )+x3*( y1*z2-y2*z1 ) );
	}
	return 0;
}

