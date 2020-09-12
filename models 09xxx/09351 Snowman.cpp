
/// Project:	FMI 3D (2013)
/// File:		09351 Snowman.cpp
/// Topic:		09 Graphical primitives
/// Slide:		35
///	About:		Snowman - demonstration of Elica object orientation
///
/// History:
///		2013.08.22/PB	Initial implementation
///     2013.09.11/PB   Simplified object complexity


#include "fmi3d.h"

#define N 150
Sphere3D *s[5];
Cone3D *c[3][N];
Cylinder3D *hat[2];


void randomize()
{
	for( int j=0; j<3; j++ )
		for( int i=0; i<N-10*j; i++ )
		{
			c[j][i]->clearOrientation();
			c[j][i]->rotateLocal( random( 0,360 ), {0,0,1} );
			c[j][i]->rotateLocal( random( 90-70,90+70 ), {0,1,0} );
		}
	c[2][0]->clearOrientation();
	c[2][0]->rotateLocal( 90, {0,1,0} );
}


void drawScene()
{
	for( int j=0; j<5; j++ )
		s[j]->drawImage();
	for( int j=0; j<2; j++ )
		hat[j]->drawImage();
	for( int j=0; j<3; j++ )
		for( int i=0; i<N; i++ )
			c[j][i]->drawImage();
}


void drawFrames()
{
}


int main()
{
	useCustomFogColor = true;
	customFogColor[0] = 0.5;
	customFogColor[1] = 0.6;
	customFogColor[2] = 1;
	customFogColor[3] = 1;

	/// Open window
	if( !openWindow3D( "Snowman" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();

	/// Add buttons
	addButton( new Button2D( "buttons/button_random.png",'R',randomize ) );
	addButton( new exitButton2D );

	viewZ = 6;
	viewDistance = 20;
	viewBeta = 0.2;

	s[0] = new Sphere3D( 3,QUALITY_HIGH ); s[0]->setCenter( {0,0,3} );
	s[1] = new Sphere3D( 2,QUALITY_HIGH ); s[1]->setCenter( {0,0,8-0.25} );
	s[2] = new Sphere3D( 1,QUALITY_HIGH ); s[2]->setCenter( {0,0,11-0.5} );

	s[3] = new Sphere3D( 0.2,QUALITY_LOWER );
	s[3]->setCenter( {0.8,-0.3,11-0.3} );
	s[3]->setColor({0.1,0.1,0.1});
	s[3]->setShininess(50);

	s[4] = new Sphere3D( 0.2,QUALITY_LOWER );
	s[4]->setCenter( {0.8,+0.3,11-0.3} );
	s[4]->setColor({0.1,0.1,0.1});
	s[4]->setShininess(50);

	for(int i=0; i<2; i++)
	{
		hat[i] = new Cylinder3D(i?0.8:1.7,i?1.5:0.1,true,QUALITY_MEDIUM);
		hat[i]->setColor({0.1,0.1,0.1});
		hat[i]->setShininess(50);
	}

	for( int j=0; j<3; j++ )
		for( int i=0; i<N; i++ )
		{
			c[j][i] = new Cone3D( 0.5-j/10.0,s[j]->getRadius()*1.5,false,QUALITY_LOWER );
			c[j][i]->setCenter( s[j]->getCenter() );
			c[j][i]->setColor({0.5,0.75,1,0.4});
		}
	c[2][0]->setColor({1,0.4,0});
	c[2][0]->setScale({0.3,0.3,s[2]->getRadius()*2});
	randomize();

	/// Main loop
	double t0=t;
	while( isRunning() )
	{
		viewAlpha  += ( t-t0 )/15;
		t0=t;
		for(int i=0; i<2; i++)
		{
			hat[i]->setCenter( s[2]->getCenter() );
			hat[i]->clearOrientation();
			hat[i]->rotateLocal( 30*sin(t), {1,0,0} );
			hat[i]->translateLocal( {0,0,0.6+0.05*i} );
		}
	}
	return 0;
}

