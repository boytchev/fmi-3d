
/// Project:	FMI 3D (2013)
/// File:		10422 Pringles 3D.cpp
/// Topic:		10 Composite objects
/// Slide:		42
///	About:		Pringles 3D
///
/// History:
///		2013.08.17/PB	Initial implementation
///     2013.09.13/PB  Added texture


#include "fmi3d.h"

#define ANIMATION_TIME 2

bool show = true;

class Pringles3D : public Object3D
{
	public:
		Pringles3D();
		void regenerate( double rx, double ry, double a, double b );
		vect_t pring( double r, double alpha );
		void normal( vect_t p, double r, double alpha );
		double rx, ry, a, b;
};

Pringles3D::Pringles3D( ) : Object3D()
{
	setPolygonMode(GL_FILL);
	setColor({1,0.7,0});
	setTexture(loadTexture("10422 Texture.png"));
	setTextureSize(2);
	regenerate( 1.5,1,2,1.5 );
}

vect_t Pringles3D::pring( double r, double alpha )
{
	double x = ry*r*cos( alpha );
	double y = ry*r*sin( alpha );
	double z = pow(x/a,2)-pow(y/b,2);
	return {rx/ry*x,y,z};
}

void Pringles3D::normal( vect_t pnt, double r, double alpha )
{
	vect_t nv = pring(r+0.001,alpha).sub(pnt);
	vect_t nu = pring(r,alpha+0.001).sub(pnt);
	vect_t n = nv.vmul(nu);
	n = n.mul(1/n.distance());
	glNormal3f( n.x,n.y,n.z );
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
		glFrontFace(GL_CCW);
		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

		double dA = 6;
		double dR = 0.1;
		for( double r=0.0001; r<1; r+=dR )
		{
			glBegin( GL_QUAD_STRIP );
			for( int i=0; i<=360; i+=dA )
			{
				double alpha = i*M_PI/180;
				vect_t p;
				p = pring( r,alpha );
				normal(p,r,alpha);
				glTexCoord2f(r*cos(alpha),r*sin(alpha));
				glColor3f(1,0.7,0);
				p.vertex();
				p = pring( r+dR,alpha );
				if( r+dR>=1) glColor3f(0.8,0.5,0);
				normal(p,r+dR,alpha);
				glTexCoord2f((r+dR)*cos(alpha),(r+dR)*sin(alpha));
				p.vertex();
			}
			glEnd();
		}
		glEnable(GL_DEPTH_TEST);
	}
	glEndList();


	glDeleteLists( frameList,1 );
	glNewList( frameList,GL_COMPILE );
	{
	    glDisable(GL_TEXTURE_2D);
		glLineWidth(0.5);
		glDepthMask(GL_FALSE);

		double dA = 6;
		double dR = 0.1;
		for( double r=0.0001; r<1; r+=dR )
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
		glDepthMask(GL_TRUE);
	}
	glEndList();
}


Pringles3D *p;

void drawScene()
{
	p->drawImage();
}


void drawFrames()
{
	if( show ) p->drawFrame();
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

void toggleShow()
{
	show = !show;
}

int main()
{
	/// Open window
	if( !openWindow3D( "Pringles 3D" ) ) return 1;
	changeGround();

	/// Add buttons
	addButton( new Button2D( "buttons/button_toggle.png",GLFW_KEY_SPACE,toggleAb ) );
	addButton( new Button2D( "buttons/button_wireframe.png",'W',toggleShow ) );
	addButton( new exitButton2D );

	viewDistance = 15;
	viewAlpha = -M_PI/2;
	viewBeta = 0.2;
	viewZ = 5;

	p = new Pringles3D();
	p->setCenter( {0,0,6} );
	p->setSize(3);

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
		p->clearOrientation();
		p->rotateLocal(20*t,{0,1,1});
		p->rotateLocal(25*t,{1,0,1});
		p->rotateLocal(30*t,{1,1,0});
	}
	return 0;
}

