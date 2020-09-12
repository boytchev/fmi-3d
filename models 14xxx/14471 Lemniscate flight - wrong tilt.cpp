
/// Project:	FMI 3D (2013)
/// File:		14471 Lemniscate flight - wrong tilt.cpp
/// Topic:		14 Orientation
/// Slide:		47
///	About:		Lemniscate flight with wrong tilt
///
/// History:
///		2013.08.13/PB	Initial implementation


#include "fmi3d.h"

#define ANIMATION_TIME 10
#define N 10
Cone3D *c[2][N];
PaperPlane3D *p;

void drawScene()
{
	p->drawImage();
	p->drawFrame();
	for( int i=0; i<2; i++ )
		for( int n=0; n<N; n++ )
			c[i][n]->drawImage();
}


void drawFrames()
{
}


int main()
{
	/// Open window
	if( !openWindow3D( "Lemniscate flight - wrong tilt" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();

	/// Add buttons
	addButton( new exitButton2D );

	viewDistance = 15;

	p = new PaperPlane3D( 2 );
	p->setScale( {2.5,1.5,1.5} );
	p->setCenter( {0.14,-0.14,3} );
	p->rotateLocal( -48, {0,0,1} );
	p->setColor( {1,1,1} );
	p->setLighting( false );

	GLuint tex = loadTexture( "textures/Material_Wood.jpg" );

	for( int i=0; i<2; i++ )
		for( int n=0; n<N; n++ )
		{
			c[i][n] = new Cone3D( 0,0,false,QUALITY_HIGH );
			c[i][n]->setCenter( {10*( i-0.5 ),0,0} );
			c[i][n]->setTexture( tex );
			c[i][n]->setTextureScale( {2,2,2,2} );
		}

	/// Main loop
	isRunning();
	double t0=t;
	while( isRunning() )
	{
		viewAlpha += ( t-t0 )/10;
		t0=t;

		double k=1;
		if( t0<ANIMATION_TIME )
		{
			k = 1-cos( t0/ANIMATION_TIME*M_PI/2 );
		}

		for( int i=0; i<2; i++ )
			for( int n=0; n<N; n++ )
			{
				c[i][n]->setCenter( {10*( i-0.5 ),0,-0.05+0.05*k} );
				double r = 4.0*n/N;
				double h = k*k*k*( 8-7.0*n/N );
				c[i][n]->setScale( {r,r,h} );
				c[i][n]->rotateLocal( r*( n%2-0.5 ), {0,0,1} );
			}

		viewX = viewX*0.99+p->getCenter().x*0.01;
		viewY = viewY*0.99+p->getCenter().y*0.01;

		p->rotateLocal( 2.405*sin( frame*M_PI/180 ), {0,0,1} );
		if( frame%360 == 0 ) p->rotateLocal(45, {1,0,0} );
		p->translateLocal( {0.1,0,0} );
	}

	return 0;
}

