
/// Project:	FMI 3D (2013)
/// File:		14481 Lemniscate flight with tilt.cpp
/// Topic:		14 Orientation
/// Slide:		48
///	About:		Lemniscate flight with tilt
///
/// History:
///		2013.08.13/PB	Initial implementation


#include "fmi3d.h"

#define ANIMATION_TIME 10
#define N 10
Cone3D *c[2][N];
#define P 12
#define dP 15
PaperPlane3D *p[P];
vect_t o[P];
int show = 1;

void drawScene()
{
	for( int i=0; i<show; i++ )
	{
		p[i]->rotateLocal( -60*sin( ( frame-dP*i )*M_PI/180 ), {1,0,0} );
		p[i]->translateLocal({-1,0,0});
		p[i]->drawImage();
		p[i]->drawFrame();
		p[i]->translateLocal({+1,0,0});
		p[i]->rotateLocal( 60*sin( ( frame-dP*i )*M_PI/180 ), {1,0,0} );
	}
	for( int i=0; i<2; i++ )
		for( int n=0; n<N; n++ )
			c[i][n]->drawImage();
}


void drawFrames()
{
}


void toggleShow()
{
	show = ( P+1 )-show;
}


int main()
{
	useCustomFogColor = true;
	customFogColor[0]=0.75;
	customFogColor[1]=0.85;
	customFogColor[2]=1;
	customFogColor[3]=1;

	/// Open window
	if( !openWindow3D( "Lemniscate flight with tilt" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();

	/// Add buttons
	addButton( new Button2D( "buttons/button_n.png",GLFW_KEY_SPACE,toggleShow ) );
	addButton( new exitButton2D );

	viewDistance = 15;
	viewZ = 2;

	for( int i=0; i<P; i++ )
	{
		p[i] = new PaperPlane3D( 2 );
		p[i]->setScale( {1.5,1,1} );
		p[i]->setCenter( {0.14,-0.14,3} );
		p[i]->rotateLocal( -48, {0,0,1} );
		if( i )
			p[i]->setColor( {random( 0.5,1 ),random( 0.5,1 ),random( 0,1 )} );
		else
			p[i]->setColor( {1,1,1} );
		p[i]->setLighting( false );
		p[i]->setIsVisible( false );
		o[i] = {random( 0.1,1 ),random( 0,2*M_PI )};
	}

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

		viewX = viewX*0.99+p[0]->getCenter().x*0.01;
		viewY = viewY*0.99+p[0]->getCenter().y*0.01;

		for( int i=0; i<P; i++ )
		{
			if( dP*i<frame )
			{
				p[i]->setIsVisible( true );
				p[i]->rotateLocal( 2.405*sin( ( frame-dP*i )*M_PI/180 ), {0,0,1} );
				p[i]->translateLocal( {0.1,0,0} );
				p[i]->oxyz.o.z = 4+2*sin( o[i].x*t+o[i].y );
			}
		}
	}

	return 0;
}

