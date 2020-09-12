
/// Project:	FMI 3D (2013)
/// File:		27331 Texture scale.cpp
/// Topic:		27 Textures
/// Slide:		33
///	About:		Using texture scale
///
/// History:
///		2013.07.24/PB	Initial implementation
///     2013.09.14/PB   Changed button image


#include "fmi3d.h"

//#include <iostream>

Cube3D *c[27];

void drawScene()
{
	GLfloat fc[4]={0,0,0,1};

	glPushAttrib(GL_FOG_BIT);
	glEnable(GL_FOG);
	glFogf(GL_FOG_START,viewDistance-1);
	glFogf(GL_FOG_END,viewDistance+4);
	glFogfv(GL_FOG_COLOR,fc);
	for( int i=0; i<27; i++ ) c[i]->drawImage();
	glDisable(GL_FOG);
	glPopAttrib();
}


void drawFrames()
{
}


int mode = 0;
int modes = 6;
Button2D *mB;
void toggle()
{
	mode = (mode+1)%modes;
	for(int i=0; i<27; i++) c[i]->setTextureSize(mode+1);
	mB->setState(mode);
}



int main()
{
	/// Open window
	if( !openWindow3D( "Texture scale" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();

	viewZ = 6;
	viewDistance = 15;

	/// Add buttons
	mB = new Button2D( "buttons/button_n.png",GLFW_KEY_SPACE,toggle,mode,modes );
	addButton( new exitButton2D );

	GLuint tex = loadTexture( "27331 Texture.jpg" );

	for( int i=0; i<27; i++ )
	{
		c[i] = new Cube3D( 1 );
		c[i]->setTexture(tex);
		c[i]->setSize(2);
		c[i]->setIsVisible( abs(i%3-1)+abs((i/3)%3-1)+abs((i/9)%3-1)>1 );
		c[i]->setColor({0.5,0.5,1});
	}

	/// Main loop
	double t0 = t;
	while( isRunning() )
	{
		viewAlpha += (t-t0)/10;
		t0 = t;

		for( int i=0; i<27; i++ )
		{
			c[i]->identity();
			c[i]->setCenter({2.0*(i%3-1),2.0*((i/3)%3-1),2.0*((i/9)%3-1)});
			c[i]->rotateGlobal(26*t,{1,0,1});
			c[i]->translateGlobal({0,0,6});
		}
	}
	return 0;
}

