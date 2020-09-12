
/// Project:	FMI 3D (2013)
/// File:		18361 Illumination problem.cpp
/// Topic:		18 Ray tracing
/// Slide:		36
///	About:		Illumination problem - the Satan's room

#include "fmi3d.h"
//#include <iostream>

#define N 25
vect_t p[N]=
{
	{-1,0},{-1,1},{-2,1},{-2,2},{-3,1},
	{-3,0},{-4,0},{-3,-1},{-2,-1},{-2,-2},
	{-1,-1},{0,-1},{0,-2},{1,-1},{2,-2},
	{2,-1},{3,-1},{4,0},{3,0},{3,1},
	{2,2},{2,1},{1,1},{1,0},{-1,0}
};

char ch[N] = "|-|/|-\\-|/-|/\\|-/-|\\|-|-";

Sphere3D *s1,*s2;
double alpha;

vect_t isec;
double dist;
char wall;

bool intersect( vect_t R, double alpha, vect_t A, vect_t B )
{
	double s = ( ( R.x-A.x )*( B.y-A.y )-( R.y-A.y )*( B.x-A.x ) )/( ( B.x-A.x )*sin( alpha )-( B.y-A.y )*cos( alpha ) );
	if( s<0.001 ) return false;
	if( s>dist ) return false;

	if( B.x!=A.x )
		t = ( R.x+s*cos( alpha )-A.x )/( B.x-A.x );
	else
		t = ( R.y+s*sin( alpha )-A.y )/( B.y-A.y );
	if( t<0 || t>1 ) return false;
	dist = s;
	isec = {A.x+t*( B.x-A.x ),A.y+t*( B.y-A.y ),1};
	return true;
}

void drawRays()
{
	vect_t from = s1->getCenter();
	double ang = alpha;
	for( int j=0; j<50; j++ )
	{
		int c = 0;
		dist = 1000;
		wall = ' ';
		for( int i=0; i<N-1; i++ )
		{
			if( intersect( from,ang,p[i],p[i+1] ) )
			{
				wall = ch[i];
				c++;
			}
		}
		glColor4f( 1,1,0,1-j/50.0 );
		glLineWidth( 3.5-3*j/50.0 );
		glBegin( GL_LINES );
		from.vertex();
		isec.vertex();
		glEnd();
		from = isec;
		switch( wall )
		{
			case '|': ang=M_PI-ang; break;
			case '-': ang=-ang; break;
			case '\\': ang=3*M_PI/2-ang; break;
			case '/': ang=M_PI/2-ang; break;
		}
	}
}

void drawWalls()
{
	glDisable( GL_LIGHTING );
	glDisable( GL_TEXTURE_2D );
	glDepthFunc( GL_FALSE );
	glDisable( GL_DEPTH_TEST );

	glColor4f( 0.5,0.5,1,0.25 );
	glPolygonMode( GL_FRONT_AND_BACK,GL_FILL );
	glEnable( GL_POLYGON_OFFSET_FILL );
	glPolygonOffset( 1,1 );
	glBegin( GL_QUAD_STRIP );
	for( int i=0; i<N; i++ )
	{
		p[i].add( {0,0,0.01} ).vertex();
		p[i].add( {0,0,2} ).vertex();
	}
	glEnd();
	glDisable( GL_POLYGON_OFFSET_FILL );
	glDepthFunc( GL_TRUE );
	glEnable( GL_DEPTH_TEST );

	glLineWidth( 1.5 );
	glEnable( GL_LINE_SMOOTH );
	glColor3f( 0,0,1 );
	glPolygonMode( GL_FRONT_AND_BACK,GL_LINE );
	glBegin( GL_QUAD_STRIP );
	for( int i=0; i<N; i++ )
	{
		p[i].add( {0,0,0.04} ).vertex();
		p[i].add( {0,0,2} ).vertex();
	}
	glEnd();
}

void drawScene()
{
	s1->drawImage();
	s2->drawImage();

	glDisable( GL_LIGHTING );
	glDisable( GL_TEXTURE_2D );

	drawWalls();

	if( !inReflection ) drawRays();

	drawWalls();
}


void drawFrames()
{
}


void randomize()
{
	alpha = random( 0,2*M_PI );
}

int main()
{
	/// Open window
	if( !openWindow3D( "Illumination problem" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();
	changeBrightness();

	/// Add buttons
	addButton( new Button2D( "buttons/button_random.png",'R',randomize ) );
	addButton( new exitButton2D );

	viewZ = 0;
	viewAlpha = -M_PI/2;
	viewBeta = M_PI/2;
	viewDistance = 23;
	for( int i=0; i<N; i++ ) p[i]=p[i].mul( 3 );

	s1 = new Sphere3D( 0.2 );
	s1->setCenter( {-6,0,1} );
	s1->setColor( {1,0.9,0} );
	s1->setShininess( 30 );

	s2 = new Sphere3D( 0.2 );
	s2->setCenter( {6,0,1} );
	s2->setColor( {1,0,0} );
	s2->setShininess( 30 );

	randomize();

	/// Main loop
	while( isRunning() )
	{
	}
	return 0;
}

