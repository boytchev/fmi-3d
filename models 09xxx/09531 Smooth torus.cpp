
/// Project:	FMI 3D (2013)
/// File:		09531 Smooth torus.cpp
/// Topic:		09 Graphical primitives
/// Slide:		53
///	About:		Smooth torus
///
/// History:
///		2013.08.27/PB	Initial implementation
///     2013.09.13/PB   Changed button image, added a new phase


#include "fmi3d.h"

double gap=0;

/// Generate diaply list for unit truncate cone
GLuint unitTruncatedCone3D( double r, double h, double h0, double coef, int precision, double b1, double b2, bool smooth )
{
	double dA = 2*M_PI/precision;

	glNewList( ++UID,GL_COMPILE );
	glBegin( GL_QUAD_STRIP ); /// side

	double a = 0;
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
		Torus3D(double R, double r, int nVert, int nHor, bool smooth);
	private:
};

Torus3D::Torus3D(double R, double r, int nVert, int nHor, bool smooth) : Object3D()
{
	setShadeModel(smooth?GL_SMOOTH:GL_FLAT);
	glNewList( frameList,GL_COMPILE );
	glEndList();


	GLuint coneList[nVert];
	double dA = 2*M_PI/nVert;
	for(int i=0; i<nVert; i++)
	{
		double a1 = i*dA+M_PI/4;
		double r1 = R+r*cos(a1);
		double h1 = r*sin(a1);

		double a2 = a1+dA-gap;
		double r2 = R+r*cos(a2);
		double h2 = r*sin(a2);

		coneList[i] = unitTruncatedCone3D(r1,h2-h1,h1,r2/r1,nHor,a1,a2,smooth);
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


Torus3D* torus[3];
int mode = 0;


void drawScene()
{
	torus[mode]->drawImage();
}

void drawFrames()
{
}


Button2D *mB;
void changeMode()
{
	mode = (mode+1)%3;
	mB->setState(mode);
}

int main()
{
	if( !openWindow3D( "Smooth torus" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();

	mB = new Button2D("buttons/button_half.png",GLFW_KEY_SPACE,changeMode,0,3);
	addButton( new exitButton2D() );

	viewZ = 7.5;

    gap = 0;
    torus[0] = new Torus3D(5,1.75,80,80,true);
	torus[0]->setColor({0.5,0.5,1});
	torus[0]->setCenter({0,0,7.5});

    torus[1] = new Torus3D(5,1.75,12,200,false);
	torus[1]->setColor({0.5,0.5,1});
	torus[1]->setCenter({0,0,7.5});

    gap = 0.2;
    torus[2] = new Torus3D(5,1.75,6,200,false);
	torus[2]->setColor({0.5,0.5,1});
	torus[2]->setCenter({0,0,7.5});

	isRunning();
	double dt, t0=t;
	while( isRunning() )
    {
    	dt = t-t0;
    	t0 = t;

		torus[0]->rotateLocal(20*dt,{1,1,0});
		torus[0]->rotateLocal(27*dt,{1,0,1});

		torus[1]->rotateLocal(20*dt,{1,1,0});
		torus[1]->rotateLocal(27*dt,{1,0,1});

		torus[2]->rotateLocal(20*dt,{1,1,0});
		torus[2]->rotateLocal(27*dt,{1,0,1});
    }

	return 0;
}

