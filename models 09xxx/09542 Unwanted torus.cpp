
/// Project:	FMI 3D (2013)
/// File:		09542 Unwanted torus.cpp
/// Topic:		09 Graphical primitives
/// Slide:		54
///	About:		Unwanted solution of a rought shape of a torus
///
/// History:
///		2013.08.27/PB	Initial implementation


#include "fmi3d.h"


class Torus3D : public Object3D
{
	public:
		Torus3D(double R, double r);
	private:
};

Torus3D::Torus3D(double R, double r) : Object3D()
{
	GLfloat col[4][3] = { {0.5,0.5,1}, {1,0.5,0.5}, {0.5,1,0.5}, {1,1,0.5} };

	glNewList( frameList,GL_COMPILE );
	glEndList();

	double s = sqrt(2);

	glNewList( imageList,GL_COMPILE );
	glDisable(GL_POLYGON_OFFSET_FILL);
	for(int i=0; i<4; i++)
	{
		glColor3fv(col[i]);
		glPushMatrix();
		glRotated(90*i,0,0,1);
		glTranslated(R/s,0.97*r/s,0);
		glScaled(s*r,2*(2*R/s-2*r),s*r);
		glCallList(idUnitCube3D);
		glPopMatrix();
	}
	glEndList();
}


Torus3D* torus;


void drawScene()
{
	torus->drawImage();
}

void drawFrames()
{
}


int main()
{
	if( !openWindow3D( "Unwanted torus" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();

	addButton( new exitButton2D() );

	viewZ = 7.5;

    torus = new Torus3D(5,1.75);
	torus->setCenter({0,0,7.5});

	isRunning();
	double dt, t0=t;
	while( isRunning() )
    {
    	dt = t-t0;
    	t0 = t;

		torus->clearOrientation();
		torus->rotateLocal(30*t,{1,0,0});
		torus->rotateLocal(23*dt,{0,1,0});
		torus->rotateLocal(47*t,{0,0,1});
    }

	return 0;
}

