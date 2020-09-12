
/// Project:	FMI 3D (2013)
/// File:		10232 Deformed spheres Z
/// Topic:		10 Composite objects
/// Slide:		23
///	About:		Deformed spheres along Z axis
///
/// History:
///		2013.08.16/PB	Initial implementation
///     2013.09.11/PB   Changed button image


#include "fmi3d.h"

#define ANIMATION_TIME 2
#define N 30
Sphere3D *s,*s2,*c;

double k = 0.95;

void drawScene()
{
	glEnable(GL_LINE_SMOOTH);

	double sx=2,sy=2,sz=2;
	for( int i=0; i<N; i++ )
	{
		if( i==N-1 )
		{
			s2->setScale( {sx,sy,sz} );
			s2->drawImage();
		}
		else
		{
			s->setScale( {sx,sy,sz} );
			s->drawImage();
		}
		sx /= k;
		sy /= k;
		sz *= k;
	}

	glEnable(GL_LINE_SMOOTH);
	glLineWidth(0.5);

	sx=2,sy=2,sz=2;
	for( int i=0; i<N; i++ )
	{
		if( i==0 || i==9 || i==19 || i==29 )
		{
			c->setScale( {sx*0.94,sy*0.94,sz*0.94} );
			c->drawImage();
		}
		sx /= k;
		sy /= k;
		sz *= k;
	}
}


void drawFrames()
{
}


double offsetT = 0;
double offsetMax = 0;
double offset = 0;
void toggleOffset()
{
	offsetT = t;
	offsetMax = 2-offsetMax;
}


int main()
{
	if( !openWindow3D( "Deformed spheres Z" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();

	addButton( new Button2D( "buttons/button_wireframe.png",GLFW_KEY_SPACE,toggleOffset ) );
	addButton( new exitButton2D() );

	viewDistance = 15;
	viewZ = 3;

	s = new Sphere3D( 4 );
	s->setColor( {0.5,0.5,1} );
	s->setCenter( {0,0,3} );

	s2 = new Sphere3D( 4, QUALITY_HIGH );
	s2->setColor( {0.5,0.5,1} );
	s2->setCenter( {0,0,3} );

	c = new Sphere3D( 4, QUALITY_LOW );
	c->setColor( {0,0,0} );
	c->setCenter( {0,0,3} );
	c->setPolygonMode( GL_LINE );

	double t0=t;
	while( isRunning() )
	{
		viewAlpha += ( t-t0 )/10;
		t0 = t;

		if( t-offsetT<ANIMATION_TIME )
		{
			double k = cos( ( ( t-offsetT )/ANIMATION_TIME )*M_PI/2 );
			offset = offset*k+offsetMax*( 1-k );
		}

		s->oxyz.o.z = 4+offset;
		s->clearOrientation();
		s->rotateLocal( 10*sin( 1.3*t ), {0,1,0} );
		s->rotateLocal( 0+10*sin( t ), {1,0,0} );

		s2->oxyz.o.z = 4+offset;
		s2->clearOrientation();
		s2->rotateLocal( 10*sin( 1.3*t ), {0,1,0} );
		s2->rotateLocal( 0+10*sin( t ), {1,0,0} );

		c->oxyz.o.z = 4-offset;
		c->clearOrientation();
		c->rotateLocal( 10*sin( 1.3*t ), {0,1,0} );
		c->rotateLocal( 0+10*sin( t ), {1,0,0} );

		k = 0.98+0.02*sin( t );
	}
	return 0;
}

