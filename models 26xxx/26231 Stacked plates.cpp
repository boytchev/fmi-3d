
/// Project:	FMI 3D (2013)
/// File:		26231 Stacked plates.cpp
/// Topic:		26 Deformations
/// Slide:		23
///	About:		Deformation by layers of stacked plates
///
/// History:
///		2013.06.06/PB	Initial implementation


#include "fmi3d.h"

//#include <iostream>

#define ANIM_TIME 0.5
#define STATES 3

int state = 0;
double targetS;
double k=0;

/// Plates
class Plates3D : public Object3D
{
	public:
		Plates3D( );
		void drawImage();
		double d;
		double m;
		double r;
	private:

};

Plates3D::Plates3D( ) : Object3D()
{
	setColor( {0.5,0.5,1} );

	glNewList( imageList,GL_COMPILE );
	{
		glPushMatrix();
		glScaled( 21.9,21.9,2.5 );
		glCallList( idUnitCube3D );
		glPopMatrix();
	}
	glEndList();

}


double q;
void Plates3D::drawImage( )
{
	if( !isVisible ) return;
	if( inShadow ) return;


	prepareImageProps();
	glScaled( 0.38,0.38,0.38 );

	glTranslated( 0,0,1.25 );
	for( int i=0; i<12; i++ )
	{
		glCallList( imageList );
		glTranslated( 0,0,d );
		glScaled( m,m,1 );
		switch( (state==2)?int( ( q+M_PI/2 )/2/M_PI )%4:0 )
		{
			case 0: glRotated( r,1,0,0 ); break;
			case 1: glRotated( r,0,1,0 ); break;
			case 2: glRotated( r,-1,0,0 ); break;
			case 3: glRotated( r,0,-1,0 ); break;
		}
	}
	restoreImageProps();
}

Plates3D* plates;

void drawScene()
{
	plates->drawImage();
}


void drawFrames()
{
//	water->drawFrame();
}

Button2D* cA;
double tA = -ANIM_TIME;
void changeAnimation()
{
	state = ( state+1 )%STATES;
	switch( state )
	{
		case 0: tA=t; break;
		case 1: tA=t; break;
		case 2: k=1; break;
	}
	cA->setState( state );
}

int main()
{
	/// Open window
	if( !openWindow3D( "Stacked plates" ) ) return 1;
	viewZ = 8;
	viewBeta = 0.2;
	viewDistance = 30;

	cA = new Button2D( "buttons/button_next.png",GLFW_KEY_SPACE,changeAnimation,state,STATES );
	addButton( new exitButton2D );

	plates = new Plates3D( );

	double k1 = 0.5*( pow( 324.0/219,1.0/11 )-1 );
	double k2 = 0.5*( pow( 324.0/219,1.0/11 )+1 );

	/// Main loop with automatic animation
	double t0=t;
	double s=1;
	targetS = -1;
	q = 0;
	while( isRunning() )
	{
		viewAlpha += ( t-t0 )/10;
		q += 5*( t-t0 );
		t0=t;
		s = sin( q );

		if( state==0 )
		{
			if( t-tA<=ANIM_TIME )
				k = 0.5-0.5*cos( ( t-tA )/ANIM_TIME * M_PI-M_PI );
			else
				{k=0; q=0;}

			s = s*k-1*( 1-k );
		}

		if( state==1 )
		{
			if( t-tA<=ANIM_TIME )
				k = 0.5-0.5*cos( ( t-tA )/ANIM_TIME * M_PI-M_PI );
			else
				{k=0; q=M_PI/2;}
			s = s*k+1*( 1-k );
		}

		plates->d = 0.5*( 3.75+2.5 )+0.5*( 3.75-2.5 )*s;
		plates->m = k2+k1*s;
		plates->r = 1.5+1.5*s;
	}

	return 0;
}
