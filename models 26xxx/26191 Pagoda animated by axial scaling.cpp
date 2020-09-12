
/// Project:	FMI 3D (2013)
/// File:		26191 Pagoda animated by axial scaling.cpp
/// Topic:		26 Deformations
/// Slide:		19
///	About:		A pagoda animated by differnt scaling along axes
///
/// History:
///		2013.06.06/PB	Initial implementation


#include "fmi3d.h"

#define N 50
#define STATES 3

//#include <iostream>

GLuint gold;
int state = 0;

/// Pagoda
class Pagoda3D : public Object3D
{
	public:
		Pagoda3D( int floors );
		void drawImage();
	private:
		int floors;
		double k;
		double ofs;
		double spd;
		GLuint roofList;
};

Pagoda3D::Pagoda3D( int floors ) : Object3D()
{
	this->floors = floors;
	ofs = random( 0,2*M_PI );
	spd = random( 1,3 );

	setLighting( false );

	glNewList( imageList,GL_COMPILE );
	{
		glBegin( GL_QUAD_STRIP );
		glColor3f( 1,1,1 );

		glNormal3f( 1,1,0 );
		glTexCoord2d( 0,3.5 );
		glVertex3f( 4,4,-1 );

		glNormal3f( 1,-1,0 );
		glTexCoord2d( 1,3.5 );
		glVertex3f( 4,-4,-1 );

		glNormal3f( 1,1,0 );
		glTexCoord2d( 0,2 );
		glVertex3f( 4,4,3 );

		glNormal3f( 1,-1,0 );
		glTexCoord2d( 1,2 );
		glVertex3f( 4,-4,3 );

		glColor3f( 1,0,0 );

		glNormal3f( 1,1,-2 );
		glTexCoord2d( 0,1 );
		glVertex3f( 6,6,6 );

		glNormal3f( 1,-1,-2 );
		glTexCoord2d( 1,1 );
		glVertex3f( 6,-6,6 );

		glColor3f( 1,1,0 );

		glNormal3f( 1,1,2 );
		glTexCoord2d( 0,0 );
		glVertex3f( 4,4,5 );

		glNormal3f( 1,-1,2 );
		glTexCoord2d( 1,0 );
		glVertex3f( 4,-4,5 );

		glColor3f( 1,1,1 );

		glNormal3f( 0,0,2 );
		glTexCoord2d( 0,-1 );
		glVertex3f( 0,0,5 );

		glNormal3f( 0,0,2 );
		glTexCoord2d( 1,-1 );
		glVertex3f( 0,0,5 );

		glEnd();
	}
	glEndList();
}


void Pagoda3D::drawImage( )
{
	if( !isVisible ) return;
	if( inShadow ) return;

	glEnable( GL_FOG );
	glFogi( GL_FOG_MODE, GL_LINEAR );
	glFogf( GL_FOG_START,viewDistance-10 );
	glFogf( GL_FOG_END,viewDistance+40 );

	prepareImageProps();
	glScaled( 0.2,0.2,0.2 );

	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D,gold );

	k = 0.8+0.19*sin( t*spd+ofs );
	for( int i=0; i<floors; i++ )
	{
		for( int j=0; j<4; j++ )
		{
			glCallList( imageList );
			glRotated( 90,0,0,1 );
		}
		switch( state )
		{
			case 0:
				glScaled( 1,1,k );
				break;
			case 1:
				glScaled( k,k,1 );
				break;
			case 2:
				k = 0.9+0.4*sin( t*spd+ofs+i/1.5 );
				glScaled( k,k,0.93 );
				break;
		}
		glTranslated( 0,0,5 );
	}
	restoreImageProps();
}

Pagoda3D* pagoda[N];

void drawScene()
{
	for( int i=0; i<N; i++ )
		pagoda[i]->drawImage();
}


void drawFrames()
{
//	water->drawFrame();
}

Button2D* cS;
void changeScaling()
{
	state = ( state+1 )%STATES;
	cS->setState( state );
}

int main()
{
	/// Open window
	if( !openWindow3D( "Pagodas animated by axial scaling" ) ) return 1;
	changeGround();
	changeGround();
	changeBrightness();

	cS = new Button2D( "buttons/button_next.png",GLFW_KEY_SPACE,changeScaling,state,STATES );
	addButton( new exitButton2D );

	gold = loadTexture( "26141 Texture.jpg" );

	for( int i=0; i<N; i++ )
	{
		pagoda[i] = new Pagoda3D( irandom( 5,30 ) );
		pagoda[i]->setCenter( {random( -25,25 ),random( -25,25 ),0.2} );
		pagoda[i]->rotateLocal( random( 0,360 ), {0,0,1} );
	}

	/// Main loop with automatic animation
	double t0=t;
	while( isRunning() )
	{
		viewAlpha += ( t-t0 )/10;
		t0=t;
	}

	return 0;
}
