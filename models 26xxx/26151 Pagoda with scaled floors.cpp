
/// Project:	FMI 3D (2013)
/// File:		26151 Pagoda with scaled floors.cpp
/// Topic:		26 Deformations
/// Slide:		15
///	About:		A pagoda with scaled floors
///
/// History:
///		2013.06.06/PB	Initial implementation


#include "fmi3d.h"

#define N 50

//#include <iostream>

GLuint gold;

/// Pagoda
class Pagoda3D : public Object3D
{
	public:
		Pagoda3D( int floors );
		void drawImage();
	private:
		int floors;
		double k;
		GLuint roofList;
};

Pagoda3D::Pagoda3D( int floors ) : Object3D()
{
	this->floors = floors;
	k = pow(0.1,1.0/floors);

	setLighting(false);

	glNewList(imageList,GL_COMPILE);
	{
		glBegin(GL_QUAD_STRIP);
		glColor3f(1,1,1);

		glNormal3f(1,1,0);
		glTexCoord2d(0,3.5);
		glVertex3f(4,4,-1);

		glNormal3f(1,-1,0);
		glTexCoord2d(1,3.5);
		glVertex3f(4,-4,-1);

		glNormal3f(1,1,0);
		glTexCoord2d(0,2);
		glVertex3f(4,4,3);

		glNormal3f(1,-1,0);
		glTexCoord2d(1,2);
		glVertex3f(4,-4,3);

		glColor3f(1,0,0);

		glNormal3f(1,1,-2);
		glTexCoord2d(0,1);
		glVertex3f(6,6,6);

		glNormal3f(1,-1,-2);
		glTexCoord2d(1,1);
		glVertex3f(6,-6,6);

		glColor3f(1,1,0);

		glNormal3f(1,1,2);
		glTexCoord2d(0,0);
		glVertex3f(4,4,5);

		glNormal3f(1,-1,2);
		glTexCoord2d(1,0);
		glVertex3f(4,-4,5);

		glColor3f(1,1,1);

		glNormal3f(0,0,2);
		glTexCoord2d(0,-1);
		glVertex3f(0,0,5);

		glNormal3f(0,0,2);
		glTexCoord2d(1,-1);
		glVertex3f(0,0,5);

		glEnd();
	}
	glEndList();
}


void Pagoda3D::drawImage( )
{
	if( !isVisible ) return;
	if( inShadow ) return;

	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_LINEAR );
	glFogf( GL_FOG_START,viewDistance-10 );
	glFogf( GL_FOG_END,viewDistance+40 );

	prepareImageProps();
	glScaled(0.2,0.2,0.2);

	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D,gold );

	for(int i=0; i<floors; i++)
	{
		for(int j=0; j<4; j++)
		{
			glCallList(imageList);
			glRotated(90,0,0,1);
		}
		glScaled(k,k,k);
		glTranslated(0,0,5);
	}
	restoreImageProps();
}

Pagoda3D* pagoda[N];

void drawScene()
{
	for(int i=0; i<N; i++)
		pagoda[i]->drawImage();
}


void drawFrames()
{
//	water->drawFrame();
}


int main()
{
	/// Open window
	if( !openWindow3D( "Pagodas with scaled floors" ) ) return 1;
	changeGround();
	changeGround();
	changeBrightness();

	addButton( new exitButton2D );

	gold = loadTexture("26141 Texture.jpg");

	for(int i=0; i<N; i++)
	{
		pagoda[i] = new Pagoda3D( irandom(5,30) );
		pagoda[i]->setCenter({random(-25,25),random(-25,25),0.2});
		pagoda[i]->rotateLocal(random(0,360),{0,0,1});
	}

	/// Main loop with automatic animation
	double t0=t;
	while( isRunning() )
	{
		viewAlpha += (t-t0)/10;
		t0=t;
	}

	return 0;
}
