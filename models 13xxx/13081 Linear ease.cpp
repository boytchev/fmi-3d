
/// Project:	FMI 3D (2013)
/// File:		13081 Linear ease
/// Topic:		13 Physics
/// Slide:		08
///	About:		Linear ease-in and ease-out
///
/// History:
///		2013.07.28/JB	Initial implementation
///		2013.08.16/PB	Finetuning


#include "fmi3d.h"

#define N 1000

Sphere3D *point;
CoordSys2D *cs;
int n = 0;
vect_t p[N];

void drawScene()
{
    point->drawImage();
    cs->drawImage();

    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_LINE_WIDTH);
    glColor3f(1,0,0);
    glLineWidth(3);
    glBegin(GL_LINE_STRIP);
    for(int i=0; i<n; i++) p[i].vertex();
    glEnd();
}

void drawFrames()
{
	cs->drawFrame();
}

int main()
{
	if( !openWindow3D( "Linear ease" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();

	viewZ = 4;
	viewAlpha = -1;

	addButton( new exitButton2D() );

    point = new Sphere3D(0.8, QUALITY_HIGH);
	point->setColor( {1,0,0} );
	point->setCenter( {0,0,2} );
    point->setShininess(50);

	double m=5*5/6.5;
	cs = new CoordSys2D( -0.1,8, -0.1,1.5, true, 10);
	cs->rotateLocal(90,{1,0,0});
	cs->rotateGlobal(90,{0,0,1});
	cs->setCenter(point->getCenter().add({-2,0,0}));
	cs->setSize(m);

	isRunning();
	double t0=t,t1=t0+random(0.3,2),t2=t1+random(1,4),t3=t2+random(0.3,2);
	double v0=0,v1=1;
	double dir=1;
	double tt=t;
	n=0;
	while( isRunning() )
	{
		double dt=t-tt;
		tt=t;

		viewAlpha += dt/20;

		double v=v1;
		if( t0<=t && t<=t1 )
		{
			v=(v1-v0)/(t1-t0)*(t-t0)+v0;
		}
		if( t2<=t && t<=t3 )
		{
			v=(v0-v1)/(t3-t2)*(t-t2)+v1;
		}
		if( t3<t ) v=v0;

		if( t<=t3 && n<N-1 )
		{
			p[n] = cs->getCenter().add({0,m*dir*(t-t0),m*v});
			n++;
		}

		if( t3+2<t && t<=t3+4 )
		{
			for(int i=0; i<n; i++) p[i] = p[i].add({dt*random(-3,3),0,dt*random(-3,-7)});
			cs->oxyz.o = cs->oxyz.o.add({0,0,dt*random(-9,-15)});
		}

		if( t3+4<t )
		{
			t0=t;
			t1=t0+random(1,2);
			t2=t1+random(2,4);
			t3=t2+random(1,2);
			dir=-dir;
			cs->setCenter(point->getCenter().add({-2,0,0}));
			cs->rotateLocal(180,{0,1,0});
			n=0;
		}
        point->oxyz.o.y += 5*dt*dir*v;

		double k=0.95;
		viewX = viewX*k+( 1-k )*point->getCenter().x;
		viewY = viewY*k+( 1-k )*point->getCenter().y;
	}
	return 0;
}

