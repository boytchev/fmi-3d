
/// Project:	FMI 3D (2013)
/// File:		27302 Texture deformation.cpp
/// Topic:		27 Textures
/// Slide:		30
///	About:		texture mapped onto deforming object
///
/// History:
///		2013.07.24/PB	Initial implementation
///     2013.09.13/PB   Attempt to fixed program crash


#include "fmi3d.h"

//#include <iostream>


#define N 3
#define B 1
GLuint texId[2];
Bezier3D* bezier[6];
vect_t P[N][N][N];
GLfloat S[N][N][N];
GLfloat O[N][N][N];

void drawScene()
{
	for( int i=0; i<6; i++ )
	{
		bezier[i]->drawImage();
	}
}


void drawFrames()
{
}


int tx=0;
void toggle()
{
	tx=1-tx;
	for(int i=0; i<6; i++) bezier[i]->setTexture( texId[tx] );
}

double f( int i, int j, int k )
{
	return sin( t*S[i][j][k]+O[i][j][k] );
}


int main()
{
	/// Open window
	if( !openWindow3D( "Texture deformation" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();

	viewZ = 8;
	viewAlpha = -M_PI/2;
	viewDistance = 25;

	/// Add buttons
	addButton( new Button2D( "buttons/button_toggle.png",GLFW_KEY_SPACE,toggle ) );
	addButton( new exitButton2D );

	texId[0] = loadTexture( "27271 Texture Grid.jpg" );
	texId[1] = loadTexture( "27271 Texture Mona Lisa.jpg" );

	for( int i=0; i<6; i++ )
	{
		bezier[i] = new Bezier3D( N );
		bezier[i]->setShininess( 2 );
		bezier[i]->setSize( 4 );
		bezier[i]->setTexture( texId[tx] );
		bezier[i]->setTolerance(20);
	}

	/// Create control points
	for( int i=0; i<N; i++ )
		for( int j=0; j<N; j++ )
			for( int k=0; k<N; k++ )
			{
				S[i][j][k] = random( 3,5 );
				O[i][j][k] = random( 0,2*M_PI );
			}

		for( int i=0; i<N; i++ )
			for( int j=0; j<N; j++ )
				for( int k=0; k<N; k++ )
				{
					double s = ( ( i<B )||( i>=N-B )||( j<B )||( j>=N-B ) )?0.25:1;
					P[i][j][k] = {i-( N-1 )/2.0+s*f(i,j,k),j-( N-1 )/2.0+s*f(j,k,i),k+s*f(k,i,j)-(N-1)/2.0};
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
					P[i][j][k] = {i-( N-1 )/2.0+s*f(i,j,k),j-( N-1 )/2.0+s*f(j,k,i),k+s*f(k,i,j)-(N-1)/2.0};
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
		for( int i=0; i<6; i++ )
		{
			bezier[i]->identity();
			bezier[i]->rotateLocal(26*t,{1,0,1});
			bezier[i]->translateGlobal({0,0,9});
		}
	}
	return 0;
}

