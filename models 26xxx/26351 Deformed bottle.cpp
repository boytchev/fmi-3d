
/// Project:	FMI 3D (2013)
/// File:		26351 Deformed bottle.cpp
/// Topic:		26 Deformations
/// Slide:		35
///	About:		Deforming a bottle in Bezier space
///
/// History:
///		2013.05.29/PB	Initial implementation
///     2013.09.14/PB   Simplified objects and calculations


#include "fmi3d.h"

//#include <iostream>


#define N 2
vect_t    P[N+1][N+1][N+1];	/// control points
GLfloat   S[N+1][N+1][N+1];
GLfloat   O[N+1][N+1][N+1];

int step=0;
double k1=0;

double fact[N+1];		/// factorials
double nk[N+1][N+1];	/// (n k)
GLint tex;

void setFactoriels()
{
	fact[0] = 1;
	for( int i=1; i<=N; i++ ) fact[i]=i*fact[i-1];
	for( int i=0; i<=N; i++ )
		for( int k=0; k<=i; k++ )
			nk[i][k] = fact[i]/fact[k]/fact[i-k];
}

double B( int n, int i, double u )
{
	return nk[n][i]*pow( u,i )*pow( 1-u,n-i );
}

/// Calculate point on a Bezier surface
vect_t Bp( double u, double v, double w )
{
	vect_t r = {0,0,0};
	for( int i=0; i<=N; i++ )
		for( int j=0; j<=N; j++ )
			for( int k=0; k<=N; k++ )
			{
				double s=B( N,i,u )*B( N,j,v )*B( N,k,w );
				r = r.add( P[i][j][k].mul( s ) );
			}
	return r;
}

vect_t bottlep( double u, double v )
{
	double v0=v;
	v = v/100.0;
	u = u/180.0;
	double r = 0.15+0.1*sin( 8.5*v-0.5 );
	if( v0>=80 ) {r=0.05; v = v-0.08;}
	if( v0==86 ) {v=v+0.04;}
	if( v0==92 ) {r=0; v=v-0.04;}

	u = M_PI*u;
	return Bp( 0.5+r*cos( u ),0.5+r*sin( u ),v );
}

/// Estimate normal vector to a point on the bottle
vect_t Np( double u )
{
    return {cos(u*M_PI/180),sin(u*M_PI/180),0};
}

class FlexBottle3D : public Object3D
{
	public:
		FlexBottle3D();
		void regenerate();
};

FlexBottle3D::FlexBottle3D( ) : Object3D()
{
	regenerate();
}

void FlexBottle3D::regenerate()
{
	glDeleteLists( imageList,1 );
	glNewList( imageList,GL_COMPILE );
	{
		int dV = 4;
		for( int v=0; v<92; v+=dV )
		{
			glBegin( GL_QUAD_STRIP );
			for( int u=0; u<=360; u+=24 )
			{
				vect_t n = Np( u );
				glNormal3f( n.x,n.y,n.z );

				glTexCoord2d( u/360.0,1-( v-0 )/92.0 );
				bottlep( u,v ).vertex();
				glTexCoord2d( u/360.0,1-( v+dV-0 )/92.0 );
				bottlep( u,v+dV ).vertex();
			}
			glEnd();
		}
	}
	glEndList();
}

//Bezier3D* cube;
//Bezier3D* wall[4];
FlexBottle3D* bottle;


void drawScene()
{
    if( inShadow ) return;
//	glRotated( 30*sin( 0.45*t ),sin( 0.5*t ),cos( 0.5*t ),0 );
//	if( step )
//		bottle->setColor({0,0,0});
//	else
		bottle->setColor({1,1,1});
//	cube->drawImage();
	bottle->drawImage();
//	wall[0]->drawImage();
//	wall[1]->drawImage();
//	wall[2]->drawImage();
//	wall[3]->drawImage();
}


void drawFrames()
{
}

double f( int i, int j, int k )
{
	return sin( t*S[i][j][k]+O[i][j][k] );
}

Button2D* cS;
void changeStep()
{
	step = ( step+1 )%2;
	cS->setState( step );
	if( step==0 )
		bottle->setPolygonMode( GL_FILL );
	else
		bottle->setPolygonMode( GL_LINE );
}

void randomize()
{
	double s = 1;
	double c = sin( t/3 );
	for( int i=0; i<=N; i++ )
	{
		for( int j=0; j<=N; j++ )
		{
			P[i][j][0]={5*( i-( N )/2.0 ),5*( j-( N )/2.0 ),0};
			for( int k=1; k<=N; k++ )
			{
				double cc = c*k/2.0;
				vect_t p = {5*( i-( N )/2.0 )+s*f( i,j,k ),5*( j-( N )/2.0 )+s*f( j,k,i ),5*k+0+s*f( k,i,j )};
				vect_t q = {5-p.z,p.y,p.x+10};
				P[i][j][k] = ( p.mul( 1-cc ) ).add( q.mul( cc ) );
			}
//			cube->setControlPoint( i,j,P[i][j][0] );
		}
	}
//	for( int i=0; i<=N; i++ )
//	{
//		wall[0]->setControlPoint( i,0,P[2][i][0] );
//		wall[1]->setControlPoint( i,0,P[0][i][0] );
//		wall[2]->setControlPoint( i,0,P[i][0][0] );
//		wall[3]->setControlPoint( i,0,P[i][2][0] );
//	}
	bottle->regenerate();
}

int main()
{
	/// Open window
	if( !openWindow3D( "Deformed bottle" ) ) return 1;
//	noGround();
    changeGround();
    changeGround();
	changeBrightness();

	/// Add buttons
	cS = new Button2D( "buttons/button_wireframe.png",GLFW_KEY_SPACE,changeStep );
	addButton( new exitButton2D );

	viewZ = 4;
	viewDistance = 20;
	viewAlpha = M_PI;
	viewBeta = 0;

	tex = loadTexture( "26351 Texture.jpg" );

	/// Create control points
	for( int i=0; i<=N; i++ )
		for( int j=0; j<=N; j++ )
			for( int k=0; k<=N; k++ )
			{
				S[i][j][k] = random( 2,4 );
				O[i][j][k] = random( 0,2*M_PI );
			}

	setFactoriels();

//	GLint wood = loadTexture( "textures/material_wood.jpg" );

//	cube = new Bezier3D( 3 );
//	cube->setTexture( wood );
//	cube->setTextureSize( 3 );
	bottle = new FlexBottle3D();
	bottle->setTexture( tex );
//	bottle->setShininess( 100 );

//	for( int i=0; i<4; i++ )
//	{
//		wall[i] = new Bezier3D( 3 );
//		wall[i]->setTexture( wood );
//		wall[i]->setTextureSize( 3 );
//		wall[i]->setLighting( false );
//	}
//
//	for( int i=0; i<3; i++ )
//		for( int j=0; j<3; j++ )
//		{
//			wall[0]->setControlPoint( i,j, {5,5*( i-1 ),0-5*j*j} );
//			wall[1]->setControlPoint( i,j, {-5,5*( i-1 ),0-5*j*j} );
//			wall[2]->setControlPoint( i,j, {5*( i-1 ),-5,0-5*j*j} );
//			wall[3]->setControlPoint( i,j, {5*( i-1 ),5,0-5*j*j} );
//		}

	randomize();
	changeStep();

	/// Main loop
	double t0=t;
	while( isRunning() )
	{
		viewAlpha += ( t-t0 )/10;
		t0 = t;
		randomize();
	}
	return 0;
}

