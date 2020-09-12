
/// Project:	FMI 3D (2013)
/// File:		26332 Bezier space.cpp
/// Topic:		26 Deformations
/// Slide:		33
///	About:		Bezier space
///
/// History:
///		2013.06.16/PB	Initial implementation


#include "fmi3d.h"

//#include <iostream>


#define N 3
#define B 1
vect_t P[N][N][N];
GLfloat S[N][N][N];
GLfloat O[N][N][N];
Bezier3D* bezier[6];
GLuint texId[2];

void drawScene()
{
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	for( int i=0; i<6; i++ )
		bezier[i]->drawImage();
	glCullFace(GL_BACK);
	for( int i=0; i<6; i++ )
		bezier[i]->drawImage();
	glDisable(GL_CULL_FACE);
}


void drawFrames()
{
}


int tx=0;
void toggle()
{
	tx=1-tx;
	bezier[0]->setIsVisible(tx==0);
	bezier[1]->setIsVisible(tx==0);
	bezier[2]->setTexture( texId[tx] );
	bezier[3]->setTexture( texId[tx] );
}

double f( int i, int j, int k )
{
	return sin( t*S[i][j][k]+O[i][j][k] );
}


int main()
{
	/// Open window
	if( !openWindow3D( "Bezier space" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();

	viewZ = 10;
	viewAlpha = -M_PI/2;
	viewDistance = 25;

	/// Add buttons
	addButton( new Button2D( "buttons/button_next.png",GLFW_KEY_SPACE,toggle ) );
	addButton( new exitButton2D );

	texId[0] = loadTexture( "26331 Texture1.png" );
	texId[1] = loadTexture( "26331 Texture2.png" );

	for( int i=0; i<6; i++ )
	{
		bezier[i] = new Bezier3D( N );
		bezier[i]->setShininess( 2 );
		bezier[i]->setSize( 5 );
		bezier[i]->setTolerance( 40 );
		bezier[i]->setTexture( texId[tx] );
		//bezier[i]->setLighting(false);
	}

	/// Create control points
	for( int i=0; i<N; i++ )
		for( int j=0; j<N; j++ )
			for( int k=0; k<N; k++ )
			{
				S[i][j][k] = random( 3,5 );
				O[i][j][k] = random( 0,2*M_PI );
			}


	/// Main loop
	double t0 = t;
	while( isRunning() )
	{
		viewAlpha += (t-t0)/10;
		t0 = t;

		for( int i=0; i<N; i++ )
			for( int j=0; j<N; j++ )
				for( int k=0; k<N; k++ )
				{
					double s = ( ( i<B )||( i>=N-B )||( j<B )||( j>=N-B ) )?0.25:1;
					P[i][j][k] = {i-( N-1 )/2.0+s*f(i,j,k),j-( N-1 )/2.0+s*f(j,k,i),1+k+s*f(k,i,j)};
				}

		for( int i=0; i<N; i++ )
			for( int j=0; j<N; j++ )
			{
				bezier[0]->setControlPoint( i,j,P[i][N-1-j][0] );
				bezier[1]->setControlPoint( i,j,P[i][j][N-1] );
				bezier[2]->setControlPoint( i,j,P[i][0][j] );
				bezier[3]->setControlPoint( i,j,P[i][N-1][N-1-j] );
				bezier[4]->setControlPoint( i,j,P[0][i][N-1-j] );
				bezier[5]->setControlPoint( i,j,P[N-1][i][j] );
			}
	}
	return 0;
}

