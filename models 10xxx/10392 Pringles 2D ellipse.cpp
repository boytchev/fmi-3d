
/// Project:	FMI 3D (2013)
/// File:		10392 Pringles 2D ellipse.cpp
/// Topic:		10 Composite objects
/// Slide:		39
///	About:		Pringles 2D ellipse
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
		void regenerate( double rx, double ry );
		vect_t pring( double r, double a );
		double rx, ry;
};

Pringles3D::Pringles3D( ) : Object3D()
{
	setPolygonMode(GL_LINE);
	regenerate( 1.5*3,3 );
}

vect_t Pringles3D::pring( double r, double a )
{
	return {rx*r*cos( a ),ry*r*sin( a )};
}

void Pringles3D::regenerate( double rx, double ry )
{
	this->rx = rx;
	this->ry = ry;

	glDeleteLists( imageList,1 );
	glNewList( imageList,GL_COMPILE );
	{
		glEnable(GL_LINE_SMOOTH);
		double dA = 15;
		double dR = 0.2;
		for( double r=0; r<1; r+=dR )
		{
			glBegin( GL_QUAD_STRIP );
			for( int i=0; i<=360; i+=dA )
			{
				double a = i*M_PI/180;
				vect_t p;
				p = pring( r,a );
				p.vertex();
				p = pring( r+dR,a );
				p.vertex();
			}
			glEnd();
		}
	}
	glEndList();
}


Pringles3D *p;
CoordSys2D *cs;

void drawScene()
{
	cs->drawImage();
    double lw=7-viewDistance/5;
    lw = (lw<0.5)?0.5:lw;
    glLineWidth(lw);
	p->drawImage();
}


void drawFrames()
{
}


double eccT = 0;
double eccMax = 1;
double ecc = 1;
void toggleEcc()
{
	eccT = t;
	eccMax = 2.5-ecc;
}

int main()
{
	/// Open window
	if( !openWindow2D( "Pringles 2D ellipse" ) ) return 1;
	noGround();

	/// Add buttons
	addButton( new Button2D( "buttons/button_toggle.png",GLFW_KEY_SPACE,toggleEcc ) );
	addButton( new exitButton2D );

	viewDistance = 20;

	p = new Pringles3D();
	p->setCenter( {0,0,4} );
	p->setColor( {0,0,0} );

	cs = new CoordSys2D( -5,5,-5,5,true,5,0.5,L"XY" );
	cs->setColor({0.5,0.5,1});

	/// Main loop
	double t0=t;
	while( isRunning() )
	{
		viewAlpha += ( t-t0 )/10;
		t0 = t;

		if( t-eccT<ANIMATION_TIME )
		{
			double k = cos( ( ( t-eccT )/ANIMATION_TIME )*M_PI/2 );
			ecc = ecc*k+eccMax*( 1-k );
		}
		p->regenerate(ecc*3,3);
	}
	return 0;
}

