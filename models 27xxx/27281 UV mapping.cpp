
/// Project:	FMI 3D (2013)
/// File:		27281 UV mapping.cpp
/// Topic:		27 Textures
/// Slide:		28
///	About:		UV mapping onto 3D objects
///
/// History:
///		2013.07.24/PB	Initial implementation
///     2013.09.13/PB   Improved speed


#include "fmi3d.h"

//#include <iostream>

#define ANIMATION_TIME 2
#define N 8
GLfloat S[N][N];
GLfloat O[N][N];

Bezier3D* b;
Sphere3D* s;
Cylinder3D* c;
Cube3D* e;

double ez,bz,sz,cz,atime;

void drawScene()
{
	if( b->getCenter().z>-6 ) b->drawImage();
	if( s->getCenter().z>-6 ) s->drawImage();
	if( c->getCenter().z>-6 ) c->drawImage();
	if( e->getCenter().z>-6 ) e->drawImage();
}


void drawFrames()
{
}


double f( int i, int j )
{
	return sin( t*S[i][j]+O[i][j] );
}


Button2D *bF;
int figure = 0;
int figures = 4;
void nextFigure()
{
	atime = t;
	figure = (figure+1)%figures;
	bF->setState(figure);
	switch( figure )
	{
		case 0:
			bz = -16;
			ez = 6.5;
			break;
		case 1:
			ez = -16;
			cz = 3;
			break;
		case 2:
			cz = -16;
			sz = 6;
			break;
		case 3:
			sz = -16;
			bz = 6;
			break;
	}
}


int main()
{
	/// Open window
	if( !openWindow3D( "UV mapping" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();

	viewZ = 5;

	/// Add buttons
	bF = new Button2D("buttons/button_next.png",GLFW_KEY_SPACE,nextFigure,figure,figures);
	addButton( new exitButton2D );

	GLuint tex = loadTexture("27271 Texture Grid.jpg");

	b = new Bezier3D( N );
	b->setShininess( 2 );
	b->setSize( 1.75 );
	b->setTolerance( 100 );
	b->setTexture( tex );
	b->setCenter({0,0,-10});
	b->setTextureScale({2,2});

	s = new Sphere3D(4.5,QUALITY_MEDIUM);
	s->setCenter({0,0,-10});
	s->setTexture(tex);
	s->setTextureScale({6,-3});

	c = new Cylinder3D(3.5,7,false);
	c->setCenter({0,0,-10});
	c->setTexture(tex);
	c->setTextureScale({0,0,4,-1});

	e = new Cube3D(7);
	e->setCenter({0,0,-10});
	e->setTexture(tex);
	e->setTextureScale({1,-1});


	/// Create control points
	for( int i=0; i<N; i++ )
		for( int j=0; j<N; j++ )
		{
			S[i][j] = random( 1.5,2.5 );
			O[i][j] = random( 0,2*M_PI );
		}


	ez = 6.5;
	bz = -16;
	sz = -16;
	cz = -16;

	/// Main loop
	isRunning();
	double t0=t;
	atime=t;
	while( isRunning() )
	{
		viewAlpha += (t-t0)/20;
		t0 = t;

		if( t-atime<ANIMATION_TIME )
		{
			double k = 0.5+0.5*cos( ( ( t-atime )/ANIMATION_TIME )*M_PI/2 );
			double p = 0.5+0.5*cos( ( ( t-atime )/ANIMATION_TIME )*M_PI );
			p=0;
			e->oxyz.o.z = e->oxyz.o.z*k+(1-k)*ez-p*cos(13*(t-atime))*((ez>0)?1:0);
			c->oxyz.o.z = c->oxyz.o.z*k+(1-k)*cz-p*cos(13*(t-atime))*((cz>0)?1:0);
			s->oxyz.o.z = s->oxyz.o.z*k+(1-k)*sz-p*cos(13*(t-atime))*((sz>0)?1:0);
			b->oxyz.o.z = b->oxyz.o.z*k+(1-k)*bz-p*cos(13*(t-atime))*((bz>0)?1:0);
		}

		for( int i=0; i<N; i++ )
			for( int j=0; j<N; j++ )
			{
				double k = ( ( i==0 )||( i==N-1 )||( j==0 )||( j==N-1 ) )?1:3;
				b->setControlPoint( i,j, {i-( N-1 )/2.0,j-( N-1 )/2.0,k*f( i,j )} );
			}

		e->clearOrientation();
		e->rotateLocal(37*t,{0,0,1});

		s->clearOrientation();
		s->rotateLocal(37*t,{0,0,1});

		c->clearOrientation();
		c->rotateLocal(37*t,{0,0,1});

		b->clearOrientation();
		b->rotateLocal(37*t,{0,0,1});

	}
	return 0;
}

