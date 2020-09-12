
/// Project:	FMI 3D (2013)
/// File:		12282 Circle in 3D plane
/// Topic:		12 Trajectories
/// Slide:		28
///	About:		A circle in a random plane in 3D
///
/// History:
///		2013.08.30/PB	Initial implementation


#include "fmi3d.h"

#define N 120
Vector3D *v1,*v2;
double l1,l2;
Sphere3D *s;
vect_t p[N],m;
int n=0;

void drawScene()
{
	v1->drawImage();
	v2->drawImage();
	s->drawImage();

	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LINE_SMOOTH);
	glDepthMask(GL_FALSE);
	glLineWidth(3);
	glColor3f(1,0,0);
	glBegin(GL_LINES);
	for( int i=0; i<n-1; i++ ) //if( p[i].z>0 && p[i+1].z>0 )
	{
		p[i].vertex();
		p[i+1].vertex();
	}
	for( int i=n+1; i<N-1; i++ ) if( p[i].distance(p[i+1])<1 )
	{
		p[i].vertex();
		p[i+1].vertex();
	}
	glEnd();
	glDepthMask(GL_TRUE);
}

void drawFrames()
{
	v1->drawFrame();
	v2->drawFrame();
}


double randT;
void randomize()
{
	n = 0;
	randT = t;
	while( true )
	{
		l1 = 8;
		l2 = 8;
		v1->setCenter( {random( -5,5 ),random( -5,5 ),random( 2,8 )} );
		v2->setCenter( v1->getCenter() );
		v1->setDirection( {random( -1,1 ),random( -1,1 ),random( -1,1 )} );
		v2->setDirection( v1->oxyz.ox );
		v1->setLength( l1 );
		v2->setLength( l2 );
		vect_t p1 = v1->oxyz.o.add( v1->oxyz.oz.mul( l1 ) );
		vect_t p2 = v2->oxyz.o.add( v2->oxyz.oz.mul( l2 ) );
		if( p1.z>0.5 && p2.z>0.5 ) break;
	}
	m=v1->oxyz.o.add( v1->oxyz.oz.mul( l1/2) ).add( v2->oxyz.oz.mul( l2/2) );
}

int main()
{
	if( !openWindow3D( "Circle in 3D plane" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();

	addButton( new Button2D( "buttons/button_random.png",GLFW_KEY_SPACE,randomize ) );
	addButton( new exitButton2D() );

	viewZ = 5;

	v1 = new Vector3D( {1,1,1},0.5 );
	v2 = new Vector3D( {1,1,1},0.5 );

	s = new Sphere3D( 0.5 );
	s->setColor({0.5,0.5,1});

	isRunning();
	randomize();

	double t0=t;
	double dt;
	double k = 0.99;
	while( isRunning() )
	{
		dt = t-t0;
		viewAlpha += dt/10;
		t0 = t;

		viewX = viewX*k+(1-k)*m.x;
		viewY = viewY*k+(1-k)*m.y;
		viewZ = viewZ*k+(1-k)*m.z;

		if( n<N && p[n].z<0 )
		{
			double a = n/(N-1.0)*2*M_PI;
			p[n]=v1->oxyz.o.add( v1->oxyz.oz.mul( (0.5+0.45*cos(a))*l1) ).add( v2->oxyz.oz.mul( (0.5+0.45*sin(a))*l2) );
			s->setCenter(p[n]);
			n++;
		}

		for(int i=n; i<N; i++)
		{
			p[i].z -= dt*( 10+3*sin(i/10+random(-0.5,0.5)))*(1+t-randT);
		}
	}
	return 0;
}

