
/// Project:	FMI 3D (2013)
/// File:		11363 Liner motion on a cube
/// Topic:		11 Animation
/// Slide:		36
///	About:		Liner motion along the edges of a cube
///
/// History:
///		2013.07.24/JB	Initial implementation
///		2013.07.28/PB	Finetuning
///     2013.09.13/PB   Speed is adjusted dynamically


#include "fmi3d.h"

Cube3D* cube;
Sphere3D* p;

int K=50;
double S=(-2.0/K);

void drawScene()
{
	glPushMatrix();
	glTranslated(0,0,5.2);
	glRotated(10*t,1,0,0);
	glRotated(21*t,0,1,1);
	p->drawImage();

    glLineWidth(3);
    cube->drawImage();
    glLineWidth(1);
    glPopMatrix();
}

void drawFrames()
{
	glPushMatrix();
	glTranslated(0,0,5.2);
	glRotated(10*t,1,0,0);
	glRotated(21*t,0,1,1);
    cube->drawFrame();
    glPopMatrix();
}

int main()
{
	if( !openWindow3D( "Linear motion on a cube" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();

	viewZ = 5;
	viewDistance = 15;

	addButton( new exitButton2D() );

	p = new Sphere3D(0.5,QUALITY_HIGH);
	p->setColor({1,0,0});
	p->setShininess(100);
    p->setCenter( {3,3,3} );

    cube = new Cube3D(6);
    cube->setCenter({0,0,0});
    cube->setPolygonMode(GL_LINE);
    cube->setColor({0,0,0});

	vect_t v;
	int k = 0;
	int dir = 2;

    double t0=0;
	while( isRunning() )
	{
		if( k==0 )
		{
		    t0 = t;
			k = K;
			dir = ((random(0,2)>1)?dir+1:dir+2)%3;
			switch( dir )
			{
				case 0: v = {p->getCenter().x*S,0,0}; break;
				case 1: v = {0,p->getCenter().y*S,0}; break;
				case 2: v = {0,0,p->getCenter().z*S}; break;
			}
		}
		p->setCenter( p->getCenter().add(v) );
		k--;
		if( k==0 )
        {
            t0 = t-t0;
            if( t0>2.0 ) K-=2;
            if( t0<1.5 ) K+=2;
            S=(-2.0/K);
        }
	}
	return 0;
}

