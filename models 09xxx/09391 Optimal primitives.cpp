
/// Project:	FMI 3D (2013)
/// File:		09391 Optimal primitives.cpp
/// Topic:		09 Graphical primitives
/// Slide:		39
///	About:		Optimal number of primitives
///
/// History:
///		2013.08.22/PB	Initial implementation
///     2013.09.10/PB   Changed button image


#include "fmi3d.h"

#define ANIMATION_TIME 2

Cube3D *c[4];

void drawScene()
{
	for( int i=1; i<4; i++ )
		c[i]->drawImage();

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	c[0]->drawImage();
	glCullFace(GL_BACK);
	c[0]->drawImage();
	glDisable(GL_CULL_FACE);
}


void drawFrames()
{
	for( int i=0; i<1; i++ )
		c[i]->drawFrame();
}


double colT = 0;
double colMax = 1;
double col = 1;
void toggleCol()
{
	colT = t;
	colMax = 1.5-colMax;
}


int main()
{
	/// Open window
	if( !openWindow3D( "Optimal primitives" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();

	/// Add buttons
	addButton( new Button2D( "buttons/button_xray.png",GLFW_KEY_SPACE,toggleCol ) );
	addButton( new exitButton2D );

	viewZ = 5;
	viewDistance = 20;

	for(int i=0; i<4; i++)
	{
		c[i] = new Cube3D( 6 );
		c[i]->setCenter( {0,0,6} );
		c[i]->setColor( {1,i?1.0:0,0} );
	}

	c[1]->setScale( {8,3,3} );
	c[2]->setScale( {3,8,3} );
	c[3]->setScale( {3,3,8} );

	/// Main loop
	while( isRunning() )
	{
		if( t-colT<ANIMATION_TIME )
		{
			double k = 0.5+0.5*cos( ( ( t-colT )/ANIMATION_TIME )*M_PI/2 );
			col = col*k+colMax*( 1-k );
		}
		c[0]->setColor({1,0,0,col});

		for(int i=0; i<4; i++)
		{
			c[i]->clearOrientation();
			c[i]->rotateLocal(25*t,{-1,0,1});
			c[i]->rotateLocal(35*t,{0,1,-1});
			c[i]->rotateLocal(45*t,{1,-1,0});
		}
	}
	return 0;
}

