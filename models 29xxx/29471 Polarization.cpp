
/// Project:	FMI 3D (2013)
/// File:		29471 Polarization.cpp
/// Topic:		29 Stereo graphics
/// Slide:		47
///	About:		Using polarization for stereo graphics
///
/// History:
///		2013.07.19/PB	Initial implementation


#include "fmi3d.h"

//#include <iostream>


#define ANIMATION_TIME 2

Cube3D *c1,*c2;
Sphere3D *s1,*s4;
Circle2D *s2,*s3,*s5,*s6;
unsigned oddo[32];
unsigned even[32];

void drawScene()
{

	glDisable(GL_TEXTURE_2D);

	s1->drawImage();
	s4->drawImage();

	glEnable(GL_POLYGON_STIPPLE);
	glPolygonStipple((GLubyte*)oddo);
	c1->drawImage();
	if( activeObject==s1) s2->drawImage();
	glPolygonStipple((GLubyte*)even);
	c2->drawImage();
	if( activeObject==s4) s5->drawImage();
	glDisable(GL_POLYGON_STIPPLE);

	glDepthMask(GL_FALSE);
	glLineWidth(2);
	if( activeObject==s1) s3->drawImage();
	if( activeObject==s4) s6->drawImage();
	glDepthMask(GL_TRUE);

}


void drawFrames()
{
	c1->drawFrame();
	c2->drawFrame();
}


void keys( int key )
{
	if( ( key==GLFW_KEY_LEFT )     || ( key==GLFW_KEY_KP_4 ) ) activeObject->iMove( -10,0 );
	if( ( key==GLFW_KEY_RIGHT )    || ( key==GLFW_KEY_KP_6 ) ) activeObject->iMove( +10,0 );
	if( ( key==GLFW_KEY_UP )       || ( key==GLFW_KEY_KP_8 ) ) activeObject->iMove( 0,-10 );
	if( ( key==GLFW_KEY_DOWN )     || ( key==GLFW_KEY_KP_2 ) ) activeObject->iMove( 0,+10 );
}

void mice( int dX, int dY )
{
	if( glfwGetMouseButton( GLFW_MOUSE_BUTTON_LEFT ) )  activeObject->iMove( dX,dY );
}

Button2D *bE;
double cutT;
double cutE = 0;
double E = 0;
int mode = 0;
void cutFrames()
{
	cutT = t;
	mode = (mode+1)%3;
	cutE = mode*2;
	bE->setState(mode);
}

int main()
{
	/// Open window
	if( !openWindow2D( "Polarization" ) ) return 1;
	changeBrightness();
	noGround();

	/// Add buttons
	bE = new Button2D( "buttons/button_cut.png",GLFW_KEY_SPACE,cutFrames,mode,3 );
	addButton( new exitButton2D );

	GLuint tex = loadTexture( "29471 Texture.jpg" );

	c1 = new Cube3D( 8 );
	c1->setCenter({0,0,-4});
	c1->setScale({8,8,8});
	c1->setLighting( false );
	c1->setTexture(tex);
	c1->rotateLocal(180,{0,0,1});

	c2 = new Cube3D( 8 );
	c2->setCenter({0,0,-4});
	c2->setScale({8,8,8});
	c2->setLighting( false );
	c2->setTexture(tex);

	s1 = new Sphere3D(0.3);
	s1->setCenter({0,-5,1});
	s1->setColor({0.5,0.5,1});
	s1->interactive();
	s1->setKeyPress(keys);
	s1->setMouseMove(mice);
	s2 = new Circle2D(4,QUALITY_HIGH);
	s2->setColor({0,0,0.2,0.85});
	s3 = new Circle2D(4,QUALITY_HIGH);
	s3->setColor({0.5,0.5,1});
	s3->setPolygonMode(GL_LINE);

	s4 = new Sphere3D(0.3);
	s4->setCenter({0,5,1});
	s4->setColor({0.5,0.5,1});
	s4->interactive();
	s4->setKeyPress(keys);
	s4->setMouseMove(mice);
	s5 = new Circle2D(4,QUALITY_HIGH);
	s5->setColor({0,0,0.2,0.85});
	s6 = new Circle2D(4,QUALITY_HIGH);
	s6->setColor({0.5,0.5,1});
	s6->setPolygonMode(GL_LINE);

	for(int i=0; i<32; i++)
	{
		oddo[i]=(i%2)?0xaaaaaaaa:0x55555555;
		even[i]=(i%2)?0x55555555:0xaaaaaaaa;
	}

	/// Main loop
	while( isRunning() )
	{
		if( t-cutT<ANIMATION_TIME )
		{
			double k = cos( ( ( t-cutT )/ANIMATION_TIME )*M_PI/2 );
			E = E*k+cutE*( 1-k );
		}

		c1->setCenter({-E,0,-4});
		c2->setCenter({E,0,-4});

		viewX = 0;
		viewY = 0;
		viewAlpha = M_PI/2;
		viewBeta = -M_PI/2;
		s2->setCenter(s1->getCenter());
		s3->setCenter(s1->getCenter());
		s5->setCenter(s4->getCenter());
		s6->setCenter(s4->getCenter());
	}

	return 0;
}

