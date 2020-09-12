
/// Project:	FMI 3D (2013)
/// File:		26262 Hand.cpp
/// Topic:		26 Deformations
/// Slide:		26
///	About:		A hand
///
/// History:
///		2013.06.16/PB	Initial implementation


#include "fmi3d.h"

//#include <iostream>

#define N 5
#define ANIMATION_TIME 2

double k = 0.8;

/// Finger
class Finger3D : public Object3D
{
	public:
		Finger3D( double length, double roty );
		void drawImage();
		double angle;
		double roty;
	private:
		double length;
};

Finger3D::Finger3D( double length, double roty ) : Object3D()
{
	this->angle = 0;
	this->length = length*0.8;
	this->roty = roty;
	setColor( {1,0.80,0.65} );

	/// imageList = truncated cone
	glNewList( imageList,GL_COMPILE );
	{
#define PREC 64
		double dalpha = 2*M_PI/PREC;
		double dx = cos( dalpha );
		double dy = sin( dalpha );

		double x1 = 1;
		double y1 = 0;

		glBegin( GL_QUAD_STRIP ); /// side
		for( int i=0; i<=PREC; i++ )
		{
			glNormal3f( x1, y1, 0 );
			glVertex3f( x1, y1, 0 );
			glVertex3f( k*x1, k*y1, this->length );

			double x2 = x1*dx-y1*dy;
			double y2 = y1*dx+x1*dy;
			x1 = x2;
			y1 = y2;
		}
		glEnd();
		glEndList();
	}
}

void Finger3D::drawImage( )
{
	if( !isVisible ) return;
	if( inShadow ) return;

	prepareImageProps();
	glRotated( roty,0,1,0 );
	glRotated( angle/2,1,0,0 );
	glCallList( idUnitSphere3D[QUALITY_HIGH] );
	for( int i=0; i<3; i++ )
	{
		glCallList( imageList );
		glTranslated( 0,0,length );
		glScaled( k,k,k );
		glRotated( angle/2,1,0,0 );
		glCallList( idUnitSphere3D[QUALITY_HIGH] );
		glRotated( angle/2,1,0,0 );
	}

	glRotated( 8-angle,1,0,0 );
	glTranslated( 0,0.7,0 );
	glScaled( 0.8,0.4,1.8 );
	glColor3f( 1,0.2,0.2 );
	glCallList( idUnitSphere3D[QUALITY_HIGH] );

	restoreImageProps();
}

Finger3D* finger[N];

double sz;
void drawScene()
{
	sz = sz+0.5+sin( 2*t );
	glTranslated( 0,0,2 );
	glRotated( 20*sin( 1.5*t ),1,0,0 );
	glRotated( 20*sin( 3*t ),0,1,0 );
	glRotated( sz,0,0,1 );
	for( int i=0; i<N; i++ )
		finger[i]->drawImage();
}


void drawFrames()
{
}

Button2D* mB;
double motionT;
double ma = 1;
double mb = 0;
double showMA = 1;
double showMB = 0;
int state = 0;
void motion()
{
	state = (state+1)%3;
	mB->setState(state);
	motionT = t;
	showMA = state==2?0:1;
	showMB = state==1?1:0;
}

int main()
{
	/// Open window
	if( !openWindow3D( "A hand" ) ) return 1;
	changeGround();
	changeGround();
	changeBrightness();

	mB = new  Button2D( "buttons/button_next.png",GLFW_KEY_SPACE,motion,state,3 );
	addButton( new exitButton2D );

	viewZ = 7;
	viewDistance = 25;

	finger[0] = new Finger3D( 4,10 );
	finger[0]->setCenter( {3.6,0.2,5-0.4} );

	finger[1] = new Finger3D( 4,3 );
	finger[1]->setCenter( {1.8,0.5,5+0.4} );

	finger[2] = new Finger3D( 4,-3 );
	finger[2]->setSize( 0.9 );
	finger[2]->setCenter( {0,0.4,5} );

	finger[3] = new Finger3D( 4,-10 );
	finger[3]->setSize( 0.8 );
	finger[3]->setCenter( {-1.44,0,5-0.8} );

	finger[4] = new Finger3D( 4*5.5/10,90 );
	finger[3]->setSize( 0.8 );
	finger[4]->setCenter( {4.3,-1,5-3} );

	/// Main loop
	while( isRunning() )
	{
		if( t-motionT<ANIMATION_TIME )
		{
			double k = cos( ( ( t-motionT )/ANIMATION_TIME )*M_PI/2 );
			mb = mb*k+showMB*( 1-k );
			ma = ma*k+showMA*( 1-k );
		}

		for( int i=0; i<N; i++ )
		{
			finger[i]->angle = (i==4?0.65:1)*50*( 1.1+sin( 4*t+mb*i/1.5 ) );
			finger[i]->angle = finger[i]->angle*ma+(i==0?10:90)*(i==4?0.65:1)*(1-ma);
		}

		finger[0]->roty = 10+(1-ma)*(10+10*sin(10*t));
	}

	return 0;
}
