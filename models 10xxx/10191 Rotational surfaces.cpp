
/// Project:	FMI 3D (2013)
/// File:		10191 Rotational surfaces.cpp
/// Topic:		10 Composite objects
/// Slide:		19
///	About:		Generation of rotational surfaces
///
/// History:
///		2013.08.16/PB	Initial implementation
///     2013.09.13/PB   Changed button image


#include "fmi3d.h"

#define ANIMATION_TIME 4
int modes = 9;
int mode = 0;

double minp = 0;
double maxp = 1;

vect_t curve( double p )
{
	switch( mode )
	{//					h				r
		case 0: return {1-exp(p*p)/exp(1),p/2};
		case 1: return {p,0.005*p*p*p-1.5*p*p+1.5*p};
		case 2: return {0.5+0.2*cos(p*2*M_PI),0.4+0.2*sin(p*2*M_PI)};
		case 3: return {p,0.2+0.1*sin(22*p)};
		case 4: return {0.75+0.08*sin(22*p)-p/2,p/2};
		case 5: return {1-p,0.4*sin(10*p*p)*p};
		case 6: return {p,(0.2+0.1*cos(p*20))*sin(p*M_PI*1.5)};
		case 7: return {0.5+0.5*cos(p*2*M_PI),p*cos(7.2*p)/2};
		case 8: return {p,random(0.3,0.5)};
	}
	return {};
}

vect_t surface( double p, double a )
{
	vect_t c = curve(p);
	double z = c.x;
	double r = fabs(c.y);
	a = a*M_PI/180;
	return {r*cos(a),r*sin(a),z};
}

void normal( vect_t pnt, double p, double a )
{
	vect_t nv = surface(p+0.001,a).sub(pnt);
	vect_t nu = surface(p,a+0.001).sub(pnt);
	vect_t n = nv.vmul(nu);
	n = n.mul(1/n.distance());
	glNormal3f( n.x,n.y,n.z );
}

class Rotational3D : public Object3D
{
	public:
		Rotational3D();
		void regenerate();
		void drawImage();
};

Rotational3D::Rotational3D( ) : Object3D()
{
	regenerate();
}

void Rotational3D::regenerate()
{
	glDeleteLists( imageList,1 );
	glNewList( imageList,GL_COMPILE );
	{
//		glFrontFace(GL_CCW);
		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

		vect_t c[2] = {{random(0.4,1),random(0.4,1),random(0.4,1)},{0,0,0}};
		c[1] = c[0].mul(0.9);

		int i = 0;
		float dV = (maxp-minp)/50;
		for( float v=minp; v<maxp-0.01; v+=dV )
		{
			glColor3dv(&c[i].x);
			i = 1-i;

			glBegin( GL_QUAD_STRIP );
			for( int u=0; u<=360; u+=5 )
			{
				vect_t p = surface(v,u);
				normal( p,v,u );
				p.vertex();

				p = surface(v+dV,u);
				normal( p,v+dV,u );
				p.vertex();
			}
			glEnd();
		}

		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	}
	glEndList();
}


void Rotational3D::drawImage()
{
	if( !isVisible ) return;
	if( inReflection )
		glFrontFace(GL_CW);
	else
		glFrontFace(GL_CCW);
	drawList( imageList );
}


Rotational3D* rot;
Cube3D *cube;

double cutT = 0;
double cutMax = 6;
double cut = 6;
void toggleCut()
{
	cutT = t;
	cutMax = 6-cutMax;
}

void drawScene()
{
	GLdouble cp[4] = {1,0,0,cut};
	glEnable(GL_CLIP_PLANE2);
	glClipPlane(GL_CLIP_PLANE2,cp);
	rot->drawImage();
	glDisable(GL_CLIP_PLANE2);
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(2);
	cube->drawImage();
}


void drawFrames()
{
}

Button2D* mB;
void changeMode()
{
	mode = ( mode+1 )%modes;
	mB->setState( mode );
	rot->regenerate();
}

int main()
{
	/// Open window
	if( !openWindow3D( "Rotational surfaces" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();
	//changeBrightness();

	/// Add buttons
	mB = new Button2D( "buttons/button_next.png",GLFW_KEY_SPACE,changeMode,mode,modes );
	addButton( new Button2D("buttons/button_cut.png",'S',toggleCut));
	addButton( new exitButton2D );

	viewZ = 5;
	viewDistance = 25;

	rot = new Rotational3D();
	rot->setShininess( 100 );
	rot->setColor({0,1,0});
	rot->setCenter({0,0,1});
	rot->setSize(10);

	cube = new Cube3D( 10 );
	cube->setPolygonMode( GL_LINE );
	cube->setCenter( {0,0,6} );
	cube->setColor( {0,0,0} );

	/// Main loop
	double t0=t;
	while( isRunning() )
	{
		viewAlpha += ( t-t0 )/10;
		t0 = t;

		if( t-cutT<ANIMATION_TIME )
		{
			double k = cos( ( ( t-cutT )/ANIMATION_TIME )*M_PI/2 );
			cut = cut*k+cutMax*( 1-k );
		}
	}
	return 0;
}

