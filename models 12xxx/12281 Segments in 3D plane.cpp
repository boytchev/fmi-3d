
/// Project:	FMI 3D (2013)
/// File:		12281 Segments in 3D plane
/// Topic:		12 Trajectories
/// Slide:		28
///	About:		Random segments in a random plane in 3D
///
/// History:
///		2013.08.30/PB	Initial implementation


#include "fmi3d.h"

#define N 200
Vector3D *v1,*v2;
double l1,l2;
vect_t p[N],q[N],m;
int n=0;

void drawScene()
{
	v1->drawImage();
	v2->drawImage();

	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LINE_SMOOTH);
	glDepthMask(GL_FALSE);
	glLineWidth(1);
	glColor3f(1,0,0);
	glBegin(GL_LINES);
	for( int i=0; i<N; i++ )
	{
		p[i].vertex();
		q[i].vertex();
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
		l1 = random( 5,10 );
		l2 = random( 5,10 );
		v1->setCenter( {random( -5,5 ),random( -5,5 ),random( 2,8 )} );
		v2->setCenter( v1->getCenter() );
		v1->setLength( l1 );
		v2->setLength( l2 );
		v1->setDirection( {random( -1,1 ),random( -1,1 ),random( -1,1 )} );
		v2->setDirection( {random( -1,1 ),random( -1,1 ),random( -1,1 )} );
		vect_t p1 = v1->oxyz.o.add( v1->oxyz.oz.mul( l1 ) );
		vect_t p2 = v2->oxyz.o.add( v2->oxyz.oz.mul( l2 ) );
		if( p1.z>0.5 && p2.z>0.5 ) break;
	}
	m=v1->oxyz.o.add( v1->oxyz.oz.mul( l1/2) ).add( v2->oxyz.oz.mul( l2/2) );

}

int main()
{
	if( !openWindow3D( "Segments in 3D plane" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();

	addButton( new Button2D( "buttons/button_random.png",GLFW_KEY_SPACE,randomize ) );
	addButton( new exitButton2D() );

	viewZ = 5;

	v1 = new Vector3D( {1,1,1},0.5 );
	v2 = new Vector3D( {1,1,1},0.5 );
	randomize();

	isRunning();
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
			p[n]=v1->oxyz.o.add( v1->oxyz.oz.mul( random(0,1)*l1) ).add( v2->oxyz.oz.mul( random(0,1)*l2) );
			q[n]=v1->oxyz.o.add( v1->oxyz.oz.mul( random(0,1)*l1) ).add( v2->oxyz.oz.mul( random(0,1)*l2) );
			n++;
		}

		for(int i=n; i<N; i++)
		{
			p[i].z -= dt*pow(3,1*(t-randT+sin(i/10.0)+1));
			q[i].z -= dt*pow(3,1*(t-randT+sin(i/10.0)+1));
		}
	}
	return 0;
}

