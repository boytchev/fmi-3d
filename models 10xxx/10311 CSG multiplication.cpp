
/// Project:	FMI 3D (2013)
/// File:		10311 CSG multiplication.cpp
/// Topic:		10 Composite objects
/// Slide:		31
///	About:		Constructive solid geometry - multiplication of spheres
///
/// History:
///		2013.08.18/PB	Initial implementation


#include "fmi3d.h"

#define ANIMATION_TIME 2

Sphere3D *s1,*s2,*s1a,*s2a;
double cut=-1,top=12;

void drawSpheres(vect_t col)
{
}

void drawScene()
{
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glFrontFace(GL_CW);

	GLdouble cp3[4] = {0,0,-1,top};
	glEnable(GL_CLIP_PLANE3);
	glClipPlane(GL_CLIP_PLANE3,cp3);

	GLdouble cp1[4] = {0,-1,0,cut};
	GLdouble cp2[4] = {0,1,0,-cut};
	glEnable(GL_CLIP_PLANE2);
	glClipPlane(GL_CLIP_PLANE2,cp1);
	s1->drawImage();
	glClipPlane(GL_CLIP_PLANE2,cp2);
	s2->drawImage();
	glDisable(GL_CLIP_PLANE2);

	glDisable(GL_CLIP_PLANE3);

	glEnable(GL_LINE_SMOOTH);
	glLineWidth(0.75);
	glDepthMask(GL_FALSE);
	s1a->drawImage();
	s2a->drawImage();
	glDepthMask(GL_TRUE);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
}


void drawFrames()
{
}


double topT = 0;
double topMax = top;
void toggleTop()
{
	topT = t;
	topMax = 18-topMax;
}


int main()
{
	/// Open window
	if( !openWindow3D( "CSG multiplication" ) ) return 1;
	changeGround();
	changeGround();
	changeBrightness();

	/// Add buttons
	addButton( new Button2D( "buttons/button_cut.png",GLFW_KEY_SPACE,toggleTop ) );
	addButton( new exitButton2D );

	viewAlpha = -M_PI;
	viewZ = 6;

	GLuint tex = loadTexture("textures/Ground_Tiles.jpg");

	s1 = new Sphere3D( 5,QUALITY_HIGH );
	s1->rotateLocal(90,{1,0,0});
	s1->setTexture(tex);
	s1->setTextureScale({4,2});
	s1->setColor({1,0.5,0});
	s1->setShininess(50);

	s2 = new Sphere3D( 5,QUALITY_HIGH );
	s2->rotateLocal(90,{1,0,0});
	s2->setTexture(tex);
	s2->setTextureScale({4,2});
	s2->setColor({1,0.5,0});
	s2->setShininess(50);

	s1a = new Sphere3D( 4.95,QUALITY_HIGH );
	s1a->rotateLocal(90,{1,0,0});
	s1a->setColor({1,1,1,0.05});
	s1a->setPolygonMode(GL_LINE);

	s2a = new Sphere3D( 4.95,QUALITY_HIGH );
	s2a->rotateLocal(90,{1,0,0});
	s2a->setColor({1,1,1,0.05});
	s2a->setPolygonMode(GL_LINE);

	/// Main loop
	while( isRunning() )
	{
		if( t-topT<ANIMATION_TIME )
		{
			double k = cos( ( ( t-topT )/ANIMATION_TIME )*M_PI/2 );
			top = top*k+topMax*( 1-k );
		}

		double r1 = 4+1*sin(0.5*t);
		double r2 = 4-1*sin(0.5*t);

		s1->setRadius(r1);
		s2->setRadius(r2);
		s1a->setRadius(r1*1.01);
		s2a->setRadius(r2*1.01);

		double p1 = (2.5+1.5*sin(0.4*t));
		double p2 = -(2.5+1.5*cos(0.4*t));
		s1->setCenter({0,p1,6});
		s2->setCenter({0,p2,6});
		s1a->setCenter({0,p1,6});
		s2a->setCenter({0,p2,6});

		double d = p1-p2;
		cut = p2+(d*d-r1*r1+r2*r2)/(2*d);
	}
	return 0;
}

