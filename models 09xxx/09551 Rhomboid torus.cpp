
/// Project:	FMI 3D (2013)
/// File:		09551 Rhomboid torus.cpp
/// Topic:		09 Graphical primitives
/// Slide:		55
///	About:		Torus with rhomboidal cross-section
///
/// History:
///		2013.08.27/PB	Initial implementation
///     2013.09.13/PB   Changed button image


#include "fmi3d.h"

#define ANIMATION_TIME 2

/// Generate diaply list for unit truncate cone
GLuint unitTruncatedCone3D( double r, double h, double h0, double coef, int precision, double b1, double b2, bool smooth, double k1, double k2 )
{
	double dA = (k2-k1)*M_PI/precision;

	glNewList( ++UID,GL_COMPILE );
	glBegin( GL_QUAD_STRIP ); /// side

	double a = k1*M_PI;
	for( int i=0; i<=precision; i++ )
	{
		double x = cos(a);
		double y = sin(a);
		a +=dA;

		if( smooth )
			glNormal3f( x*cos(b1), y*cos(b1), sin(b1) );
		else
			glNormal3f( cos(a-dA/2)*cos((b1+b2)/2), sin(a-dA/2)*cos((b1+b2)/2), sin((b1+b2)/2) );
		glVertex3f( r*x, r*y, h0 );

		if( smooth )
			glNormal3f( x*cos(b2), y*cos(b2), sin(b2) );
		glVertex3f( r*x*coef, r*y*coef, h0+h );
	}

	glEnd();
	glEndList();
	return UID;
}

class Torus3D : public Object3D
{
	public:
		Torus3D(double R, double r, int nVert, int nHor, bool smooth, double k1, double k2);
	private:
};

Torus3D::Torus3D(double R, double r, int nVert, int nHor, bool smooth, double k1, double k2) : Object3D()
{
	setShadeModel(smooth?GL_SMOOTH:GL_FLAT);
	glNewList( frameList,GL_COMPILE );
	glEndList();


	GLuint coneList[nVert];
	double dA = 2*M_PI/nVert;
	for(int i=0; i<nVert; i++)
	{
		double a1 = i*dA;
		double r1 = R+r*cos(a1);
		double h1 = r*sin(a1);

		double a2 = a1+dA;
		double r2 = R+r*cos(a2);
		double h2 = r*sin(a2);

		coneList[i] = unitTruncatedCone3D(r1,h2-h1,h1,r2/r1,nHor,a1,a2,smooth,k1,k2);
	}

	glNewList( imageList,GL_COMPILE );
	glFrontFace(GL_CW);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
	for(int i=0; i<nVert; i++)
	{
		glCallList(coneList[i]);
	}
	glEndList();
}


Torus3D* torus[2];


void drawScene()
{
	torus[1]->drawImage();
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	torus[0]->drawImage();
	glCullFace(GL_BACK);
	torus[0]->drawImage();
	glDisable(GL_CULL_FACE);
}

void drawFrames()
{
}


double transT = 0;
double transMax = 1;
double trans = 1;
void toggleTrans()
{
	transT = t;
	transMax = 1.25-transMax;
}

int main()
{
	if( !openWindow3D( "Rhomboid torus" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();

	addButton( new Button2D("buttons/button_xray.png",GLFW_KEY_SPACE,toggleTrans));
	addButton( new exitButton2D() );

	viewZ = 7.5;

    torus[0] = new Torus3D(5,1.75,4,50,false,1.5,2);
	torus[0]->setColor({0.5,0.5,1});
	torus[0]->setCenter({0,0,7.5});

    torus[1] = new Torus3D(5,1.75,4,150,false,0,1.5);
	torus[1]->setColor({0.5,0.5,1});
	torus[1]->setCenter({0,0,7.5});

	isRunning();
	double dt, t0=t;
	while( isRunning() )
    {
    	dt = t-t0;
    	t0 = t;

		if( t-transT<ANIMATION_TIME )
		{
			double k = 0.5+0.5*cos( ( ( t-transT )/ANIMATION_TIME )*M_PI/2 );
			trans = trans*k+transMax*( 1-k );
		}

		torus[0]->clearOrientation();
		torus[0]->rotateLocal(30*t,{1,0,0});
		torus[0]->rotateLocal(23*dt,{0,1,0});
		torus[0]->rotateLocal(47*t,{0,0,1});
		torus[0]->setColor({1-0.5*trans,0.5,1,trans});

		torus[1]->clearOrientation();
		torus[1]->rotateLocal(30*t,{1,0,0});
		torus[1]->rotateLocal(23*dt,{0,1,0});
		torus[1]->rotateLocal(47*t,{0,0,1});
    }

	return 0;
}

