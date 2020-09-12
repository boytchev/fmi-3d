
/// Project:	FMI 3D (2013)
/// File:		10261 Lissajous curve.cpp
/// Topic:		10 Composite objects
/// Slide:		26
///	About:		Lissajous curve
///
/// History:
///		2013.08.17/PB	Initial implementation


#include "fmi3d.h"

class Lissajous3D : public Object3D
{
	public:
		Lissajous3D();
		void regenerate(int n, double bx, double by, double bz, int cx, int cy, int cz);
		vect_t liss(double x);
		double bx, by, bz, cx, cy, cz;
};

Lissajous3D::Lissajous3D( ) : Object3D()
{
	regenerate(2, 3,2,5, 0,0,30);
}

vect_t Lissajous3D::liss(double p)
{
	return {sin(bx*p+cx),sin(by*p+cy),sin(bz*p+cz)};
}

void Lissajous3D::regenerate(int n, double bx, double by, double bz, int cx, int cy, int cz)
{
	this->bx = bx;
	this->by = by;
	this->bz = bz;
	this->cx = cx*M_PI/180;
	this->cy = cy*M_PI/180;
	this->cz = cz*M_PI/180;

	glDeleteLists( imageList,1 );
	glNewList( imageList,GL_COMPILE );
	{
		double dA = 0.3;
		double dA2 = dA*M_PI/180;
		for(double i=0; i<360*n; i+=dA )
		{
			double a = i*M_PI/180;
			vect_t p = liss(a);
			vect_t q = liss(a+dA2);

			oxyz_t oxyz = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
			oxyz.oz = q.sub(p);
			oxyz.rebuild();
			oxyz.o = p;
			oxyz.o.w = 1;

			glColor3f( 0.5+0.5*p.x,0.5+0.5*p.y,0.5+0.5*p.z);
			glPushMatrix();
			glMultMatrixd((GLdouble*)&oxyz);
			glScaled(0.1,0.1,0.05);
			glCallList(idUnitCylinder3D[QUALITY_LOW]);
			glPopMatrix();
		}
	}
	glEndList();
}


Lissajous3D* liss;
Cube3D *cube;

void drawScene()
{
	liss->drawImage();
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(2);
	cube->drawImage();
}


void drawFrames()
{
}


void randomize()
{
	liss->regenerate(2, irandom(2,8)/2.0,irandom(2,8)/2.0,irandom(2,8)/2.0, 60*irandom(0,6),60*irandom(0,6),60*irandom(0,6));
}


int main()
{
	/// Open window
	if( !openWindow3D( "Lissajous curve" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();
	//changeBrightness();

	/// Add buttons
	addButton( new Button2D("buttons/button_random.png",GLFW_KEY_SPACE,randomize));
	addButton( new exitButton2D );

	viewZ = 5;
	viewDistance = 25;

	liss = new Lissajous3D();
	liss->setShininess( 100 );
	liss->setCenter({0,0,6});
	liss->setSize(4.5);

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
	}
	return 0;
}

