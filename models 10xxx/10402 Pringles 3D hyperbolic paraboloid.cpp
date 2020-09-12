
/// Project:	FMI 3D (2013)
/// File:		10402 Pringles 3D hyperbolic paraboloid.cpp
/// Topic:		10 Composite objects
/// Slide:		40
///	About:		Pringles 3D hyperbolic paraboloid
///
/// History:
///		2013.08.17/PB	Initial implementation
///     2013.09.13/PB   Fixed width of lines


#include "fmi3d.h"

#define ANIMATION_TIME 2

class Pringles3D : public Object3D
{
	public:
		Pringles3D();
		void regenerate( double rx, double ry, double a, double b );
		vect_t pring( double r, double alpha );
		double rx, ry, a, b;
};

Pringles3D::Pringles3D( ) : Object3D()
{
	setPolygonMode(GL_LINE);
	regenerate( 1.5,1,2,1.5 );
}

vect_t Pringles3D::pring( double r, double alpha )
{
	double x = ry*r*cos( alpha );
	double y = ry*r*sin( alpha );
	double z = pow(x/a,2)-pow(y/b,2);
	return {rx/ry*x,y,z};
}

void Pringles3D::regenerate( double rx, double ry, double a, double b )
{
	this->rx = rx;
	this->ry = ry;
	this->a = a;
	this->b = b;

	glDeleteLists( imageList,1 );
	glNewList( imageList,GL_COMPILE );
	{
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_LINE_SMOOTH);
		double dA = 5;
		double dR = 0.1;
		for( double r=0; r<1; r+=dR )
		{
			glBegin( GL_QUAD_STRIP );
			for( int i=0; i<=360; i+=dA )
			{
				double alpha = i*M_PI/180;
				vect_t p;
				p = pring( r,alpha );
				p.vertex();
				p = pring( r+dR,alpha );
				p.vertex();
			}
			glEnd();
		}
	}
	glEnable(GL_DEPTH_TEST);
	glEndList();
}


Pringles3D *p;
CoordSys2D *cs;

void drawScene()
{
	cs->drawImage();
    double lw=5-viewDistance/5;
    lw = (lw<0.5)?0.5:lw;
    glLineWidth(lw);
	p->drawImage();
}


void drawFrames()
{
}

double abT = 0;
double aMax = 2;
double a = 2;
double bMax = 1.5;
double b = 1.5;
void toggleAb()
{
	abT = t;
	aMax = 3.0-aMax;
	bMax = 2.5-bMax;
}


int main()
{
	/// Open window
	if( !openWindow3D( "Pringles 3D hyperbolic paraboloid" ) ) return 1;
	noGround();

	/// Add buttons
	addButton( new Button2D( "buttons/button_toggle.png",GLFW_KEY_SPACE,toggleAb ) );
	addButton( new exitButton2D );

	viewDistance = 15;
	viewAlpha = -M_PI/2;
	viewBeta = 0.7;

	p = new Pringles3D();
	p->setCenter( {0,0,3} );
	p->setColor( {0,0,0} );
	p->setSize(3);

	cs = new CoordSys2D( -5,5,-5,5, true,5,0.5,L"XY" );
	cs->setColor({0.5,0.5,1});

	/// Main loop
	isRunning();
	double t0=t;
	while( isRunning() )
	{
		viewAlpha += ( t-t0 )/10;
		t0 = t;

		if( t-abT<ANIMATION_TIME )
		{
			double k = cos( ( ( t-abT )/ANIMATION_TIME )*M_PI/2 );
			a = a*k+aMax*( 1-k );
			b = b*k+bMax*( 1-k );
		}
		p->regenerate(1.5,1,a,b);
	}
	return 0;
}

