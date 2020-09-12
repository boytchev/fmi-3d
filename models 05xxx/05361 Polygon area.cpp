
/// Project:	FMI 3D (2013)
/// File:	    05361 Polygon area.cpp
/// Topic:		05 Lines and polygons
/// Slide:		36
///	About:		Calculating polygon area
///
/// History:
///		2013.08.23/VH	Initial implementation
///		2013.08.24/PB	Finetuning

#include "fmi3d.h"

#define N 10

Object3D *pts[N];
CoordSys2D *cs;
Text3D *txt;
int n,k;

#define EPS 1E-9
double sgn( double w )
{
	return fabs( w ) < EPS ? 0 : w;
}
double vp( vect_t f , vect_t s )
{
	return f.x*s.y - f.y*s.x;
}
bool intersect( vect_t lo1 , vect_t hi1 ,
				vect_t lo2 , vect_t hi2 )
{
	if( sgn( vp( hi1.sub( lo1 ) , lo2.sub( lo1 ) ) ) *
			sgn( vp( hi1.sub( lo1 ) , hi2.sub( lo1 ) ) ) >= 0 )return false;
	if( sgn( vp( hi2.sub( lo2 ) , lo1.sub( lo2 ) ) ) *
			sgn( vp( hi2.sub( lo2 ) , hi1.sub( lo2 ) ) ) >= 0 )return false;
	return true;
}

void drawScene()
{
	cs->drawImage();
	for( int i=0; i<n; ++i )
		pts[i]->drawImage();

	glLineWidth( 4 );
	glColor3f( 0,0,0 );
	glBegin( GL_LINE_LOOP );
	for( int i=0; i<n; i++ )
		pts[i]->getCenter().sub( {0,0,1} ).vertex();
	glEnd();

	for(int i=0; i<n-2; i++)
		for(int j=i+2; j<n; j++)
		if(intersect(pts[i]->getCenter(),pts[i+1]->getCenter(),pts[j]->getCenter(),pts[(j+1)%n]->getCenter()))
			return;

	glDepthMask( GL_FALSE );
	for( int i=0,j=1; i<k; ++i,j=(i+1)%n )
	{
		if( pts[i]->getCenter().x<pts[j]->getCenter().x )
		{
			glColor4f(0,0,1,0.3);
			txt->setColor({0,0,1});
			txt->setText(L"-");
		}
		else
		{
			glColor4f(1,0,0,0.3);
			txt->setColor({1,0,0});
			txt->setText(L"+");
		}
		txt->setCenter(pts[i]->getCenter().add(pts[j]->getCenter()).mul(0.5).sub({0.8,2,-5}));


		glBegin( GL_QUADS );
		pts[j]->getCenter().vertex();
		pts[i]->getCenter().vertex();
		glColor4f( 1,1,1,0 );
		glVertex2d( pts[i]->getCenter().x,0 );
		glVertex2d( pts[j]->getCenter().x,0 );
		glEnd();
	}

	glLineWidth( 2 );
	glBegin( GL_LINES );
	for( int i=0; i<n; i++ )
	{
		glColor4f( 0,0,0,1 );
		pts[i]->getCenter().sub( {0,0,1} ).vertex();
		glColor4f( 1,1,1,0 );
		glVertex2d( pts[i]->getCenter().x,0 );
	}
	glEnd();

	glDepthMask( GL_TRUE );

	txt->drawImage();
	glFrontFace(GL_CW);
}

void drawFrames()
{
	for( int i=0; i<n; i++ )
		pts[i]->drawFrame();
}

void create_figure()
{
	k = 0;
	n = irandom( 5,N );
	double dA = 2*M_PI/n;
	for( int i=0; i<n; i++ )
	{
		double a = ( i+random( -0.3,0.3 ) )*dA;
		double r = random( 2,5 );
		pts[i]->setCenter( {2*r*cos( a ),r*sin( a )+6,1} );
	}
	txt->setIsVisible(false);
}

void next()
{
	txt->setIsVisible(true);
	if( k<n )
		k++;
	else
		txt->setIsVisible(false);
}

int main()
{
	if( !openWindow2D( "Polygon area" ) ) return 1;
	noGround();

	viewDistance = 30;

	addButton( new Button2D( "buttons/button_next.png", GLFW_KEY_SPACE, next ) );
	addButton( new Button2D( "buttons/button_random.png", 'R' , create_figure ) );
	addButton( new exitButton2D() );

	cs = new CoordSys2D( -12,12,-1,15,true,1,0.5,L"XY" );
	for( int i=0; i<N; i++ ) pts[i] = interactivePoint2D( 0,0 );
	txt = new Text3D(L"",systemFont3D);
	txt->setSize(4);

	create_figure();

	while( isRunning() )
	{
		viewX = 0;
		viewY = 7;
		if( glfwGetMouseButton( GLFW_MOUSE_BUTTON_LEFT ) )
			for( int i=0; i<n; i++ )
				if( pts[i]->oxyz.o.y<1 )
					pts[i]->oxyz.o.y = 1;
	}
	return 0;
}
