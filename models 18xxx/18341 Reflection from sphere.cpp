
/// Project:	FMI 3D (2013)
/// File:		18341 Reflection from sphere.cpp
/// Topic:		18 Ray tracing
/// Slide:		34
///	About:		Reflection from sphere
///
/// History:
///		2013.08.06/PB	Initial implementation
///     2013.09.13/PB   Changed buttons images


#include "fmi3d.h"

#define ANIMATION_TIME 2
#define N  80
int n = 10;
Sphere3D *s;
vect_t p1[N],p2[N],p3[N],o[N],w,q1[N],q2[N],co[N];
double tt;
double speed = 0;

void drawScene()
{
	s->drawImage();

	glDisable( GL_FOG );
	glDisable( GL_LIGHTING );
	glEnable( GL_LINE_SMOOTH );
	glDisable( GL_TEXTURE_2D );

	glLineWidth( 2 );
	glBegin( GL_LINES );
	for( int i=0; i<n; i++ )
	{
		glColor3f( co[i].x,co[i].y,co[i].z );
		p1[i].vertex();
		p2[i].vertex();
		p2[i].vertex();
		p3[i].vertex();
	}
	glEnd();

}


void drawFrames()
{
}


Button2D *nB;
void changeCount()
{
	switch( n )
	{
		case 10: n = 20; nB->setState( 1 ); break;
		case 20: n = 40; nB->setState( 2 ); break;
		case 40: n = 80; nB->setState( 3 ); break;
		case 80: n = 10; nB->setState( 0 ); break;
	}
}

double speedT,speedMax=1.5;
void changeSpeed()
{
	speedT = t;
	speedMax = 1.7-speedMax;
}

int main()
{
	/// Open window
	if( !openWindow3D( "Reflection from sphere" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();
	changeBrightness();

	/// Add buttons
	addButton( new Button2D( "buttons/button_time.png",GLFW_KEY_SPACE,changeSpeed ) );
	nB = new Button2D( "buttons/button_n.png",'N',changeCount,0,4 );
	addButton( new exitButton2D );

	viewZ = 5;

	double rad = 4;
	s = new Sphere3D( rad-0.15,QUALITY_HIGH );
	s->setColor( {1,0,0} );
	s->setCenter( {0,0,4} );
	s->setShininess(1);

	for( int i=0; i<N; i++ )
	{
		o[i] = {random( 0,2 ),random( 0,2 ),random( 0.1,0.3 ),random( 0.1,0.3 )};
		co[i] = {random(0,1),random(0,1),random(0,1)};
	}

	/// Main loop
	double t0=t;
	double ttt = t;

	while( isRunning() )
	{
		tt = tt+( t-t0 )*speed;
		ttt = ttt+2*( t-t0 )*speed;
		t0 = t;

		if( t-speedT<ANIMATION_TIME )
		{
			double k = cos( ( ( t-speedT )/ANIMATION_TIME )*M_PI/2 );
			speed  = speed*k+speedMax*( 1-k );
		}

		s->setCenter( {0,0,4+fabs( 3*sin( ttt ) )} );

		for( int i=0; i<n; i++ )
		{
			p1[i] = {3.5*sin( o[i].z*tt+i+o[i].x ),-7,5+4.5*cos( o[i].w*tt-i+o[i].y )};
			p2[i] = {3.5*cos( o[i].w*tt+i*i+o[i].y ),+7,5+4.5*sin( o[i].z*tt-i*i+o[i].x )};
			p3[i] = p2[i];

			// intersect line-sphere
			vect_t v = p2[i].sub( p1[i] );

			vect_t ps = p1[i].sub( s->getCenter() );
			double A = v.smul( v );
			double B = ps.smul( v );
			double C = ps.smul( ps )-rad*rad;
			double D = B*B-A*C;
			q1[i]={0,0,-1};
			q2[i]={0,0,-1};

			if( D>=0 )
			{
				double k1 = ( -B+sqrt( D ) )/A;
				double k2 = ( -B-sqrt( D ) )/A;
				double k;
				if( k1<k2 ) k=k1; else k=k2;

				q1[i] = ( p1[i].add( v.mul( k ) ) );
				p2[i] = q1[i];

				vect_t v = p2[i].sub(p1[i]);
				v = v.mul(1.0/v.distance());

				vect_t n = p2[i].sub(s->getCenter());
				n = n.mul(1.0/n.distance());

				double nv = n.smul(v);
				vect_t u = n.mul(2*nv).sub(v);
				p3[i] = p2[i].add(u.mul(-1));
			}
			else
			p3[i] = p2[i].add(p2[i].sub(p1[i]).mul(100));

			p1[i] = p1[i].sub(p2[i].sub(p1[i]).mul(100));
		}
	}
	return 0;
}

