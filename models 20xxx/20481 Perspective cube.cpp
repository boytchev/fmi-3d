
/// Project:	FMI 3D (2013)
/// File:		20481 Perspective cube.cpp
/// Topic:		20 Projections
/// Slide:		48
///	About:		Perspective image of a cube with matrix
///
/// History:
///		2013.05.31/PB	Initial implementation
///     2013.09.13/PB   Simplified objects complexity


#include "fmi3d.h"

#define ANIMATION_TIME 1

#define matrix oxyz_t
#define vector vect_t

double w = 7;
double h = 12;
double d = 0.3;
double z = d;
double dz = 0;

Sphere3D *s[5];
Cylinder3D *c[4];
Cube3D *b[2];
Sphere3D *p[8];
Circle2D *q[8];

double f = 5;

matrix perspMatrix( )
{
	return
	{
		{ 1, 0,  0,  0 },
		{ 0, 1,  0,  0 },
		{ 0, 0,  1,  0 },
		{ 0, 0, 1/f, 0 },
	};
}

void drawScene()
{
	glPushMatrix();
	glTranslated(0,0,dz);
	for(int i=0; i<5; i++) s[i]->drawImage();
	for(int i=0; i<4; i++) c[i]->drawImage();
	b[0]->drawImage();
	b[1]->drawImage();

	glTranslated(0,0,2*d-f+0.05);
	for(int i=0; i<8; i++) p[i]->drawImage();
	for(int i=0; i<8; i++) q[i]->drawImage();

	glDisable(GL_LIGHTING);
	glColor3f(1,0,0);
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(2);
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glBegin(GL_QUAD_STRIP);
	p[0]->getCenter().vertex();
	p[1]->getCenter().vertex();
	p[3]->getCenter().vertex();
	p[2]->getCenter().vertex();
	p[7]->getCenter().vertex();
	p[6]->getCenter().vertex();
	p[4]->getCenter().vertex();
	p[5]->getCenter().vertex();
	p[0]->getCenter().vertex();
	p[1]->getCenter().vertex();
	glEnd();

	glColor3f(0,0,1);
	glBegin(GL_QUAD_STRIP);
	q[0]->getCenter().vertex();
	q[1]->getCenter().vertex();
	q[3]->getCenter().vertex();
	q[2]->getCenter().vertex();
	q[7]->getCenter().vertex();
	q[6]->getCenter().vertex();
	q[4]->getCenter().vertex();
	q[5]->getCenter().vertex();
	q[0]->getCenter().vertex();
	q[1]->getCenter().vertex();
	glEnd();

	glColor4f(0,0,0,dz/6.0);
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1,0xAAAA);
	glLineWidth(0.75);
	glBegin(GL_LINES);
	for(int i=0; i<8; i++)
	{
		p[i]->getCenter().vertex();
		glVertex3f(0,0,0);
	}
	glEnd();
	glDisable(GL_LINE_STIPPLE);
	glPopMatrix();
}


void drawFrames()
{
//	b[1]->drawFrame();
}


double raiseT;
double raiseZ = 0;
void raiseUp()
{
	raiseT = t;
	raiseZ = 6-raiseZ;
}

int main()
{
	/// Open window
	if( !openWindow3D( "Perspective cube" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();
//	changeBrightness();
//	noGround();

	/// Add buttons
	addButton( new Button2D("buttons/button_show.png",GLFW_KEY_SPACE,raiseUp));
	addButton( new exitButton2D );


	vect_t col = {0,0,0};
	double shi = 40;

	for(int i=0; i<5; i++)
	{
		s[i] = new Sphere3D(d,QUALITY_HIGH);
		s[i]->setColor(col);
		s[i]->setShininess(shi);
	}

	s[0]->setCenter({-h/2,-w/2,z});
	s[1]->setCenter({-h/2,+w/2,z});
	s[2]->setCenter({+h/2,-w/2,z});
	s[3]->setCenter({+h/2,+w/2,z});
	s[4]->setCenter({+h/2-0.75,0,z+d});
	s[4]->setScale({1.5*d,1.5*d,d/2});
	s[4]->setShininess(10);

	for(int i=0; i<4; i++)
	{
		c[i] = new Cylinder3D(d,(i<2)?h:w,false,QUALITY_HIGH);
		c[i]->setColor(col);
		c[i]->setShininess(shi);
	}

	c[0]->setCenter({-h/2,-w/2,z});
	c[2]->setCenter({-h/2,+w/2,z});
	c[3]->setCenter({+h/2,-w/2,z});
	c[1]->setCenter({+h/2,+w/2,z});
	c[0]->rotateLocal(90,{0,1,0});
	c[1]->rotateLocal(-90,{0,1,0});
	c[2]->rotateLocal(90,{1,0,0});
	c[3]->rotateLocal(-90,{1,0,0});

	b[0] = new Cube3D(w);
	b[0]->setScale({h,w,2*d});
	b[0]->setColor(col);
	b[0]->setCenter({0,0,z});
	b[0]->setLighting(false);

	b[1] = new Cube3D(w);
	b[1]->setScale({h-1.4-0.4,w-0.4,2*d});
	b[1]->setColor({1,1,1});
	b[1]->setCenter({0-0.7,0,z+0.01});
	b[1]->setLighting(false);

	for(int i=0; i<8; i++)
	{
		p[i] = new Sphere3D(0.1,QUALITY_LOWER);
		p[i]->setColor({1,0,0});
		p[i]->setShininess(40);
		q[i] = new Circle2D(0.1,QUALITY_LOWER);
		q[i]->setColor({0,0,1});
	}

	matrix M = perspMatrix();

	/// Main loop
	double t0 = t;
	while( isRunning() )
	{
		viewAlpha += (t-t0)/30;
		t0 = t;

		if( t-raiseT<ANIMATION_TIME )
		{
			double k;
			k = cos( ( ( t-raiseT )/ANIMATION_TIME )*M_PI/2 );
			dz = dz*k+raiseZ*( 1-k );
		}

		viewZ = dz+2;

		double r = 2;
		for(int i=0; i<8; i++)
		{
			p[i]->identity();
			p[i]->setCenter({0,0,10});
			p[i]->rotateLocal(41*t,{0,0,1});
			p[i]->rotateLocal(27*t,{0,1,0});
		}

		p[0]->translateLocal({+r,+r,+r});
		p[1]->translateLocal({+r,+r,-r});
		p[2]->translateLocal({+r,-r,-r});
		p[3]->translateLocal({+r,-r,+r});
		p[4]->translateLocal({-r,+r,+r});
		p[5]->translateLocal({-r,+r,-r});
		p[6]->translateLocal({-r,-r,-r});
		p[7]->translateLocal({-r,-r,+r});

		for(int i=0; i<8; i++)
		{
			q[i]->setCenter( p[i]->getCenter().mmul(M) );
		}
	}
	return 0;
}

