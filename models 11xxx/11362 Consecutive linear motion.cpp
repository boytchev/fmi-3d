
/// Project:	FMI 3D (2013)
/// File:		11362 Consecuitive linear motion
/// Topic:		11 Animation
/// Slide:		36
///	About:		Consecuitive linear motion along a loop of segments
///
/// History:
///		2013.07.24/JB	Initial implementation
///		2013.07.26/PB	Finetuning

#include "fmi3d.h"

#define N 11
Sphere3D* point[N];
Sphere3D* p;

#define S 30
int k;
int s;
vect_t v;

void drawScene()
{

	p->drawImage();
	for( int i=0; i<N; i++ )
	{
		point[i]->drawImage();
	}

	double lw = 5-viewDistance/10;
	lw = ( lw<1 )?0.5:lw;

	glEnable( GL_LINE_SMOOTH );
	glLineWidth( lw );
	glColor3f( 0,0,0 );
	glBegin( GL_LINE_LOOP );
	for( int i=0; i<N; i++ ) point[i]->getCenter().vertex();
	glEnd();

	glLineStipple( 5, 0b1010101010101010 );
	glEnable( GL_LINE_STIPPLE );

	glLineWidth( lw/5 );
	glBegin( GL_LINES );
	for( int i=0; i<N; i++ )
	{
		glVertex3f( 0,0,2.5 );
		point[i]->getCenter().vertex();
	}
	glEnd();
	glDisable( GL_LINE_STIPPLE );
}

void drawFrames()
{
}

void randomize()
{
	for( int i=0; i<N; i++ )
	{
		double al = 2*M_PI/N*i;
		double be = random( -M_PI/6,M_PI/6 );
		point[i]->setCenter( {8*( cos( al ) )*cos( be ), 10*( sin( al ) )*cos( be ), 3.5+5*sin( be )} );
	}
	k = 0;
	s = S;
	v = point[1]->getCenter().sub( point[0]->getCenter() ).mul( 1.0/s );
	p->setCenter( point[0]->getCenter() );
}

int main()
{
	if( !openWindow3D( "Consecutive linear motion" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();

	addButton( new Button2D( "buttons/button_random.png",'R',randomize ) );
	addButton( new exitButton2D() );

	viewZ = 2;
	viewBeta = 0.2;

	for( int i=0; i<N; i++ )
	{
		point[i] = new Sphere3D( 0.2 );
		point[i]->setColor( {0.0, 0.0, 0} );
		point[i]->setShininess( 100 );
	}

	p = new Sphere3D( 0.3 );
	p->setColor( {1,0,0} );
	p->setShininess( 100 );

	randomize();

	double t0 = t;
	while( isRunning() )
	{
		viewAlpha += ( t-t0 )/10;
		t0 = t;

		if( s )
		{
			p->setCenter( p->getCenter().add( v ) );
			s--;
		}
		else
		{
			k++;
			s = S;
			v = point[( k+1 )%N]->getCenter().sub( point[k%N]->getCenter() ).mul( 1.0/s );
		}
	}
	return 0;
}

