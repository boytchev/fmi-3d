
/// Project:	FMI 3D (2013)
/// File:		28191 Smooth shading.cpp
/// Topic:		28 Light
/// Slide:		19
///	About:		Smooth shading
///
/// History:
///		2013.07.06/PB	Initial implementation
///     2013.09.13/PB  Changed button image


#include "fmi3d.h"

#define ANIMATION_TIME 4

GLuint s[3];
Text3D *tx[3];
double ti,k=0;
bool shine = false;

void drawScene()
{
	for( int i=0; i<3; i++ ) tx[i]->drawImage();

	if( shine )
	{
		GLfloat sp[4]= {0.15,0.15,1,0};
		glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, 10 );
		glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, sp );
	}
	glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, noEmission );

	glPushMatrix();
	glDisable( GL_TEXTURE_2D );
	glShadeModel( GL_SMOOTH );
	glColor3f( 0.75,0.75,1 );
	glScaled( 3.5,3.5,3.5 );
	glTranslated( 0,-2.5,1.5 );
	for( int i=0; i<3; i++ )
	{
		glPushMatrix();
		glRotated( 41*ti,0,1,0 );
		glRotated( 52*ti,1,1,0 );
		glRotated( 63*ti,0,0,1 );
		glCallList( s[i] );
		glPopMatrix();
		glTranslated( 0,2.5,0 );
	}
	glShadeModel( GL_SMOOTH );
	glPopMatrix();

	if( shine )
	{
		glMaterialf( GL_FRONT_AND_BACK,GL_SHININESS,0.0 );
		glMaterialfv( GL_FRONT_AND_BACK,GL_SPECULAR, noSpecular );
		glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, defaultEmission );
	}

}


void drawFrames()
{
}

GLuint unitSphere3D( int precision );

double toggleT;
double toggleK  = 0;
void toggle()
{
	toggleT = t;
	toggleK = 1-toggleK;
}

void changeShine()
{
	shine = !shine;
}

int main()
{
	/// Open window
	if( !openWindow3D( "Smooth shading" ) ) return 1;

	/// Add buttons
	addButton( new Button2D( "buttons/button_light.png",'S',changeShine ) );
	addButton( new Button2D( "buttons/button_time.png",GLFW_KEY_SPACE,toggle ) );
	addButton( new exitButton2D );

	viewAlpha = 0;
	viewBeta = 0;
	viewZ = 5;
	viewDistance = 20;

	s[0] = unitSphere3D( 10 );
	s[1] = unitSphere3D( 32 );
	s[2] = unitSphere3D( 100 );

	for( int i=0; i<3; i++ )
	{
		tx[i] = new Text3D( L"",systemFont3D );
		tx[i]->setColor( {0,0,1} );
		tx[i]->setCenter( {0.4,( i-1.0 )*8.75-( i+3 )*0.475,0.3} );
		tx[i]->rotateLocal( 90, {0,0,1} );
		tx[i]->rotateLocal( 90, {1,0,0} );
		tx[i]->setScale( {2,1.5,0.1} );
	}
	tx[0]->setText( L"1OO" );
	tx[1]->setText( L"1OOO" );
	tx[2]->setText( L"1OOOO" );

	/// Main loop
	double t0 = t;
	ti = 0;
	while( isRunning() )
	{
		if( t-toggleT<ANIMATION_TIME )
		{
			double kk = cos( ( ( t-toggleT )/ANIMATION_TIME )*M_PI/2 );
			k = k*kk+toggleK*( 1-kk );
		}

		ti = ti+( t-t0 )*k;
		t0 = t;
	}
	return 0;
}

