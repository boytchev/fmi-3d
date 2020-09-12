
/// Project:	FMI 3D (2013)
/// File:		27101 Colour transparency.cpp
/// Topic:		27 Textures
/// Slide:		10
///	About:		A colour-based transparency
///
/// History:
///		2013.07.23/PB	Initial implementation
///     2013.09.13/PB   Changed button image


#include "fmi3d.h"

#define ANIMATION_TIME 2
#define N 10
Sphere3D *s[N];
Circle2D *c[2];
bool order = false;

void drawScene()
{
	if( order )
	{
		for( int i=N-1; i>=0; i-- ) s[i]->drawImage();
		for( int i=1; i>=0; i-- ) c[i]->drawImage();
	}
	else
	{
		for( int i=0; i<2; i++ ) c[i]->drawImage();
		for( int i=0; i<N; i++ ) s[i]->drawImage();
	}
}


void drawFrames()
{
	for( int i=0; i<2; i++ ) c[i]->drawFrame();
}


void toggleOrder()
{
	order = !order;
}


double speedT = 0;
double speedS = 50;
double speed = 50;
void toggleSpeed()
{
	speedT = t;
	speedS = 60-speedS;
}


int main()
{
	/// Open window
	if( !openWindow3D( "Colour transparency" ) ) return 1;
	changeBrightness();
	changeGround();

	viewZ = 2;

	/// Add buttons
	addButton( new Button2D( "buttons/button_time.png",'S',toggleSpeed ) );
	addButton( new Button2D( "buttons/button_xray.png",GLFW_KEY_SPACE,toggleOrder ) );
	addButton( new exitButton2D );

	for( int i=0; i<N; i++ )
	{
		s[i] = new Sphere3D( 0.75 );
		s[i]->setColor( {1,0.8,0} );
	}

	c[0] = new Circle2D( 6,QUALITY_HIGH );
	c[0]->setColor( {1,0,0,0.5} );
	c[0]->setCenter( {0,2,4} );

	c[1] = new Circle2D( 6,QUALITY_HIGH );
	c[1]->setColor( {0,0,1,0.5} );
	c[1]->setCenter( {0,-2,3} );

	/// Main loop
	double q = 0;
	double t0 = t;
	while( isRunning() )
	{
		q += speed*(t-t0);
		t0 = t;

		if( t-speedT<ANIMATION_TIME )
		{
			double k = cos( ( ( t-speedT )/ANIMATION_TIME )*M_PI/2 );
			speed = speed*k+speedS*( 1-k );
		}

		c[0]->setCenter( {0,3*cos(q*0.01),4} );
		c[1]->setCenter( {0,-3*cos(q*0.01),3} );
		for( int i=0; i<N; i++ )
		{
			double a = double( i )/N*2*M_PI;
			double h = 1+fabs( 6*sin( ( 2.5+0.7*sin( i*i ) )*q/50+23*i));
			s[i]->identity();
			s[i]->setCenter( {4*cos(a),4*sin(a),h} );
			s[i]->rotateGlobal(q,{0,0,1});
		}
	}
	return 0;
}

