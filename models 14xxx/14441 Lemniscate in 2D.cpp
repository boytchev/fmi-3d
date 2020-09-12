
/// Project:	FMI 3D (2013)
/// File:		14441 Lemniscate in 2D.cpp
/// Topic:		14 Orientation
/// Slide:		44
///	About:		Lemniscate of Bernoulli in 2D
///
/// History:
///		2013.08.13/PB	Initial implementation
///     2013.09.13/PB   Fixed speed


#include "fmi3d.h"


#define N 361

CoordSys2D *cs;
PaperPlane3D *p;
vect_t q[N];
int n=0;

void drawScene()
{
	cs->drawImage();
	cs->drawFrame();
	glFrontFace(GL_CCW);
	p->drawImage();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glDepthMask(GL_FALSE);
	glEnable(GL_LINE_SMOOTH);
	glColor3f(1,0,0);
	glLineWidth(7);
	glBegin(GL_LINE_STRIP);
	for(int i=0; i<n; i++) q[i].vertex();
	glEnd();
	glDepthMask(GL_TRUE);
}


void drawFrames()
{
	p->drawFrame();
}


int main()
{
	/// Open window
	if( !openWindow2D( "Lemniscate in 2D" ) ) return 1;
	noGround();

	/// Add buttons
	addButton( new exitButton2D );

	p = new PaperPlane3D( 2 );
	p->setScale({2,1,1});
	p->setCenter({0.14,-0.14,1});
	p->rotateLocal(-48,{0,0,1});
	p->setColor({0.5,0.5,1});

	cs = new CoordSys2D(-8,8,-4,4,true,4,0.4,L"XY");

	/// Main loop
	isRunning();
	double t0=t;
	int fr=0;
	while( isRunning() )
	{
        if( t-t0>1/100.0 )
		{
		    fr++;
		    t0 = t;
		    p->rotateLocal(2.405*sin(fr*M_PI/180),{0,0,1});
            p->translateLocal({0.1,0,0});
            if(n<N)
            {
                q[n]=p->getCenter();
                n++;
            }
		}
	}

	return 0;
}

