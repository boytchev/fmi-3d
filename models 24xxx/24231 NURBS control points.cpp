
/// Project:	FMI 3D (2013)
/// File:		24231 NURBS control points.cpp
/// Topic:		24 Surfaces
/// Slide:		07
///	About:		Effect of control points on a NURBS surface
///
/// History:
///		2013.05.31/PB	Initial implementation


#include "fmi3d.h"

//#include <iostream>


#define N 3
#define ANIMATION_TIME 2
Bezier3D* bezier;
Object3D* P[N][N];

void drawScene()
{
	for( int i=0; i<N; i++ )
		for( int j=0; j<N; j++ )
			P[i][j]->drawImage();
	bezier->drawImage();
}


void drawFrames()
{
}


void keys( int key )
{
	if( ( key==GLFW_KEY_LEFT )     || ( key==GLFW_KEY_KP_4 ) ) activeObject->iMove( -10,0 );
	if( ( key==GLFW_KEY_RIGHT )    || ( key==GLFW_KEY_KP_6 ) ) activeObject->iMove( +10,0 );
	if( ( key==GLFW_KEY_UP )       || ( key==GLFW_KEY_KP_8 ) ) activeObject->iMove( 0,-10 );
	if( ( key==GLFW_KEY_DOWN )     || ( key==GLFW_KEY_KP_2 ) ) activeObject->iMove( 0,+10 );
}

void mice( int dX, int dY )
{
	if( glfwGetMouseButton( GLFW_MOUSE_BUTTON_LEFT ) )  activeObject->iMove( dX,dY );
}


double resetT;
void reset()
{
	resetT = t;
}

double showT;
double showBeta = maxViewBeta;
void show()
{
	showT = t;
	showBeta = minViewBeta+maxViewBeta-showBeta;
}


int main()
{
	/// Open window
	if( !openWindow3D( "NURBS control points" ) ) return 1;
	//noGround();

	resetT = -ANIMATION_TIME;
	showT = -ANIMATION_TIME;

	viewAlpha = -M_PI/2;
	viewBeta = maxViewBeta;
	viewDistance = 20;

	/// Add buttons
	addButton( new  Button2D( "buttons/button_show.png",'S',show ) );
	addButton( new  Button2D( "buttons/button_toggle.png",GLFW_KEY_SPACE,reset ) );
	addButton( new exitButton2D );

	bezier = new Bezier3D( 3 );
	bezier->setLighting( false );
	bezier->setTolerance( 50 );
	bezier->setTexture( loadTexture( "24231 Texture.jpg" ) );

	/// Create control points
	for( int i=0; i<N; i++ )
		for( int j=0; j<N; j++ )
		{
			P[i][j] = point3D( 7*( i-1 ),7*( j-1 ),1 );
			P[i][j]->setColor( {0,0.5,1} );
			P[i][j]->interactive();
			P[i][j]->setKeyPress( keys );
			P[i][j]->setMouseMove( mice );
		}


	/// Main loop
	while( isRunning() )
	{
		if( t-resetT<ANIMATION_TIME )
		{
			double k = cos( ( ( t-resetT )/ANIMATION_TIME )*M_PI/2 );

			viewAlpha = viewAlpha*k-M_PI/2*( 1-k );
			viewBeta  = viewBeta*k+maxViewBeta*( 1-k );
			viewDistance = viewDistance*k+20*( 1-k );
			for( int i=0; i<N; i++ )
				for( int j=0; j<N; j++ )
				{
					vect_t p = P[i][j]->getCenter();
					vect_t q = {7.0*( i-1 ),7.0*( j-1 ),1};
					P[i][j]->setCenter( p.mul( k ).add( q.mul( 1-k ) ) );
				}
		}

		if( t-showT<ANIMATION_TIME )
		{
			double k = cos( ( ( t-showT )/ANIMATION_TIME )*M_PI/2 );

			viewBeta  = viewBeta*k+showBeta*( 1-k );
		}

		for( int i=0; i<N; i++ )
			for( int j=0; j<N; j++ )
				bezier->setControlPoint( i,j,P[i][j]->getCenter() );
	}

	return 0;
}

