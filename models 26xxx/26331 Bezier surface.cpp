
/// Project:	FMI 3D (2013)
/// File:		26331 Bezier surface.cpp
/// Topic:		26 Deformations
/// Slide:		33
///	About:		Bezier surface
///
/// History:
///		2013.06.16/PB	Initial implementation


#include "fmi3d.h"

//#include <iostream>


#define N 3
#define B 1
GLfloat S[N][N];
GLfloat O[N][N];
Bezier3D* bezier;
GLuint texId[2];

void drawScene()
{
	bezier->drawImage();
}


void drawFrames()
{
}


int tx=0;
void toggle()
{
	tx=1-tx;
	bezier->setTexture( texId[tx] );
}

double f( int i, int j )
{
	return sin( t*S[i][j]+O[i][j] );
}
double g( int i, int j )
{
	return cos( t*S[N-i-1][j]+O[i][j] );
}


int main()
{
	/// Open window
	if( !openWindow3D( "Bezier surface" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();

	viewBeta = M_PI/2;
	viewAlpha = -M_PI/2;
	viewDistance = 20;

	/// Add buttons
	addButton( new Button2D("buttons/button_next.png",GLFW_KEY_SPACE,toggle));
	addButton( new exitButton2D );

	texId[0] = loadTexture("26331 Texture1.png");
	texId[1] = loadTexture("26331 Texture2.png");

	bezier = new Bezier3D( N );
	bezier->setShininess( 2 );
	bezier->setSize( 5 );
	bezier->setTolerance( 40 );
	bezier->setTexture( texId[tx] );

	/// Create control points
	for( int i=0; i<N; i++ )
		for( int j=0; j<N; j++ )
		{
			S[i][j] = random( 3,5 );
			O[i][j] = random( 0,2*M_PI );
		}


	/// Main loop
	while( isRunning() )
	{
		for( int i=0; i<N; i++ )
			for( int j=0; j<N; j++ )
			{
				double k = ( ( i<B )||( i>=N-B )||( j<B )||( j>=N-B ) )?0.25:1;
				bezier->setControlPoint( i,j, {i-( N-1 )/2.0+k*f( i,j ),j-( N-1 )/2.0+k*g( i,j ),1} );
			}
	}
	return 0;
}

