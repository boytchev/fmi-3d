
/// Project:	FMI 3D (2013)
/// File:		19101 Types of Geometries.cpp
/// Topic:		19 Matrices
/// Slide:		20
///	About:		Types of Geometries depending on the transformation
///
/// History:
///		2013.05.28/PB	Initial implementation


#include "fmi3d.h"

//#include <iostream>


#define N 3
GLfloat S[N][N];
GLfloat O[N][N];
GLuint texId[5];
double scales[5]={4,3,3,3,3};
int geometry=4;
Bezier3D* bezier;

void drawScene()
{
	bezier->drawImage();
}


void drawFrames()
{
}


double f(int i, int j)
{
	return sin(t*S[i][j]+O[i][j]);
}

double g(int i, int j)
{
	return sin(t*S[i][j]/4+O[i][j]);
}

void geomTopological()
{
	double x = 0.15*(f(2,1)+f(0,2));
	for( int i=0; i<N; i++ )
		for( int j=0; j<N; j++ )
		{
			double k = ( ( i==1 ) || ( j==1 ) )?0.5:0.1;
			bezier->setControlPoint(i,j,{1.3*(i+k*f(i,j)-1)+x,1.3*(j+k*f(j,i)-1),1});
		}
}

void geomProjective()
{
	for( int i=0; i<N; i++ )
		for( int j=0; j<N; j++ )
		{
			double k = 1.2;
			bezier->setControlPoint(i,j,{i+k*0.5*f(i,j)-1,j+k*0.5*f(j,i)-1,1});
		}

	vect_t p;
	p = (bezier->getControlPoint(0,0)).add(bezier->getControlPoint(2,0)).mul(0.5);
	bezier->setControlPoint(1,0,p);

	p = (bezier->getControlPoint(0,2)).add(bezier->getControlPoint(2,2)).mul(0.5);
	bezier->setControlPoint(1,2,p);

	p = (bezier->getControlPoint(0,0)).add(bezier->getControlPoint(0,2)).mul(0.5);
	bezier->setControlPoint(0,1,p);

	p = (bezier->getControlPoint(2,0)).add(bezier->getControlPoint(2,2)).mul(0.5);
	bezier->setControlPoint(2,1,p);

	p = (bezier->getControlPoint(0,0)).add(bezier->getControlPoint(2,0)).add(bezier->getControlPoint(0,2)).add(bezier->getControlPoint(2,2)).mul(0.25);
	bezier->setControlPoint(1,1,p);
}

void geomAffine(double k=1, double s=1, int m=2)
{
	double a = M_PI*g(0,0)+M_PI;
	double b = a+M_PI/2+k*M_PI/3.2*f(1,1);
	double dxa=cos(a)*(1+s*0.5*g(0,1));
	double dya=sin(a)*(1+s*0.5*g(0,1));
	double dxb=cos(b)*(1+s*0.5*g(0,m));
	double dyb=sin(b)*(1+s*0.5*g(0,m));
	double x = 1+g(2,1);
	for( int i=0; i<N; i++ )
		for( int j=0; j<N; j++ )
			bezier->setControlPoint(i,j,{x+(i-1)*dxa+(j-1)*dxb-1,1+(i-1)*dya+(j-1)*dyb-1,1});
}

void geomConformal()
{
	geomAffine(0,1,1);
}


void geomCongruent()
{
	geomAffine(0,0,2);
}

Button2D* cG;
void changeGeometry()
{
	geometry = (geometry+1)%5;
	cG->setState(geometry);
	bezier->setTexture(texId[geometry]);
	bezier->setSize(scales[geometry]);
}

int main()
{
	/// Open window
	if( !openWindow2D( "Types of Geometries" ) ) return 1;
	//noGround();

	/// Add buttons
	cG = new Button2D("buttons/button_next.png", GLFW_KEY_SPACE, changeGeometry, geometry, 5);
	addButton( new exitButton2D );

	texId[0] = loadTexture("19101 Texture-0.jpg");
	texId[1] = loadTexture("19101 Texture-1.jpg");
	texId[2] = loadTexture("19101 Texture-2.jpg");
	texId[3] = loadTexture("19101 Texture-3.jpg");
	texId[4] = loadTexture("19101 Texture-4.jpg");

	bezier = new Bezier3D(N);
	changeGeometry();

	/// Create control points
	for( int i=0; i<N; i++ )
		for( int j=0; j<N; j++ )
		{
			bezier->setControlPoint(i,j,{i+random(-0.3,0.3),j+random(-0.3,0.3),1});
			S[i][j] = random( 1,2 );
			O[i][j] = random( 0,2*M_PI );
		}


	/// Main loop
	while( isRunning() )
	{
		switch( geometry )
		{
			case 0: geomCongruent(); break;
			case 1: geomConformal(); break;
			case 2: geomAffine(); break;
			case 3: geomProjective(); break;
			case 4: geomTopological(); break;
		}
	}
	return 0;
}

