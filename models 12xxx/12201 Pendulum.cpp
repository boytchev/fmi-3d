
/// Project:	FMI 3D (2013)
/// File:		12201 Pendulum
/// Topic:		12 Trajectories
/// Slide:		20
///	About:		A chain of pendulums
///
/// History:
///		2013.08.23/JB	Initial implementation


#include "fmi3d.h"

#define N 3
Sphere3D* p[N];
Sphere3D* pp[N];
Cylinder3D* c[N];
double r[N],s[N];
double k = 0.75;

int mode=0,modes=3;

void drawScene()
{
	for( int i=0; i<=mode; i++ )
	{
		c[i]->drawImage();
		p[i]->drawImage();
		pp[i]->drawImage();
	}
}

void drawFrames()
{
}

Button2D *mB;
void nextMode()
{
	mode = (mode+1)%modes;
	mB->setState(mode);
}

int main()
{
	if( !openWindow3D( "Pendulum" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();

	mB = new Button2D("buttons/button_n.png",GLFW_KEY_SPACE,nextMode,mode,modes);
	addButton( new exitButton2D() );

	viewDistance = 15;
	viewZ = 3.5;
	viewBeta = 0.2;
	viewAlpha = -0.5;

	for( int i=0; i<N; i++ )
	{
		double q = pow(k,i);
		r[i] = 4*q;
		s[i] = 2*pow(2,i);
		p[i] = new Sphere3D( 1, QUALITY_HIGH );
		p[i]->setScale({q/3,q,q});
		p[i]->setColor( {1,0.5,0} );
		p[i]->setCenter( {0,0,10} );
		p[i]->setShininess(50);
		pp[i] = new Sphere3D( 1, QUALITY_HIGH );
		pp[i]->setScale({q/4,q*1.03,q*1.03});
		pp[i]->setColor( {0,0,0} );
		pp[i]->setCenter( {0,0,10} );
		pp[i]->setShininess(50);
		c[i] = new Cylinder3D( 1,1,false );
		c[i]->setScale({q/6,q/6,r[i]});
		c[i]->setColor( {0,0,0} );
		c[i]->setCenter( {0,0,12} );
		c[i]->setShininess(50);
	}

	isRunning();
	double t0=t;
	while( isRunning() )
	{
		double dt=t-t0;
		t0 = t;

		viewAlpha += dt/20;

		vect_t o = {0,0,10};
		double a = 0;
		for( int i=0; i<N; i++ )
		{
			a = a+(1-0/N)*sin(t*s[i]);
			c[i]->setCenter(o);
			c[i]->clearOrientation();
			c[i]->rotateLocal(180+a*180/M_PI,{1,0,0});
			o = o.add({0,r[i]*sin(a),-r[i]*cos(a)});
			p[i]->setCenter(o);
			pp[i]->setCenter(o);
		}
	}
	return 0;
}

