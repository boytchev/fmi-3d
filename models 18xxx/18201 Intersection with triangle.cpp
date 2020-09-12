
/// Project:	FMI 3D (2013)
/// File:		18201 Intersection with triangle.cpp
/// Topic:		18 Ray tracing
/// Slide:		20
///	About:		Intersection of line and triangle
///
/// History:
///		2013.08.06/PB	Initial implementation
///     2013.09.13/PB   Changed buttons images



#include "fmi3d.h"

#define ANIMATION_TIME 2
#define N  80
int n = 10;
Sphere3D *s[3];
Sphere3D *q[N];
vect_t p1[N],p2[N],o[N],w;
double tt;
double speed = 0;

void drawScene()
{
	for( int i=0; i<n; i++ ) q[i]->drawImage();
	//for( int i=0; i<3; i++ ) s[i]->drawImage();

	glDisable( GL_FOG );
	glDisable( GL_LIGHTING );
	glEnable( GL_LINE_SMOOTH );
	glDisable( GL_TEXTURE_2D );

	glLineWidth( 2 );
	glColor3f( 1,0,0 );
	glPolygonMode( GL_FRONT_AND_BACK,GL_LINE );
	glBegin( GL_TRIANGLES );
	for( int i=0; i<3; i++ ) s[i]->getCenter().vertex();
	glEnd();

	glLineWidth( 1 );
	glColor3f( 1,1,0 );
	glBegin( GL_LINES );
	for( int i=0; i<n; i++ )
	{
		p1[i].vertex();
		p2[i].vertex();
	}
	glEnd();

	glColor4f( 1,0,0,0.3 );
	glPolygonMode( GL_FRONT_AND_BACK,GL_FILL );
	glBegin( GL_TRIANGLES );
	for( int i=0; i<3; i++ ) s[i]->getCenter().vertex();
	glEnd();

}


void drawFrames()
{
}


void generatePlane()
{
	s[0]->setCenter( {3+2*sin( 0.8*tt+0.6 ),4*cos( 0.9*tt+0.3 ),2+1.5*sin( 1.1*tt+1.0 )} );
	s[1]->setCenter( {-3+2*cos( 0.9*tt+0.3 ),4*cos( 1.1*tt+1.5 ),2+1.5*sin( 0.8*tt )} );
	s[2]->setCenter( {3*sin( 0.7*tt+1.1 ),4*cos( 0.8*tt+2.0 ),6+1.5*cos( 0.9*tt+0.5 )} );

	double x1 = s[0]->getCenter().x;
	double x2 = s[1]->getCenter().x;
	double x3 = s[2]->getCenter().x;

	double y1 = s[0]->getCenter().y;
	double y2 = s[1]->getCenter().y;
	double y3 = s[2]->getCenter().y;

	double z1 = s[0]->getCenter().z;
	double z2 = s[1]->getCenter().z;
	double z3 = s[2]->getCenter().z;

	w.x = y1*( z2-z3 )+y2*( z3-z1 )+y3*( z1-z2 );
	w.y = z1*( x2-x3 )+z2*( x3-x1 )+z3*( x1-x2 );
	w.z = x1*( y2-y3 )+x2*( y3-y1 )+x3*( y1-y2 );
	w.w = -( x1*( y2*z3-y3*z2 )+x2*( y3*z1-y1*z3 )+x3*( y1*z2-y2*z1 ) );
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
	if( !openWindow3D( "Intersection with triangle" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();
	changeBrightness();

	/// Add buttons
	addButton( new Button2D( "buttons/button_time.png",GLFW_KEY_SPACE,changeSpeed ) );
	nB = new Button2D( "buttons/button_n.png",'N',changeCount,0,4 );
	addButton( new exitButton2D );

	for( int i=0; i<3; i++ )
	{
		s[i] = new Sphere3D( 0.2 );
		s[i]->setColor( {0.2,0.2,1} );
	}

	for( int i=0; i<N; i++ )
	{
		o[i] = {random( 0,2 ),random( 0,2 ),random( 0.1,0.3 ),random( 0.1,0.3 )};
		q[i] = new Sphere3D( 0.15,QUALITY_LOW );
		q[i]->setColor( {1,1,1} );
		q[i]->setShininess( 10 );
	}

	/// Main loop
	double t0=t;

	while( isRunning() )
	{
		tt = tt+( t-t0 )*speed;
		t0 = t;

		if( t-speedT<ANIMATION_TIME )
		{
			double k = cos( ( ( t-speedT )/ANIMATION_TIME )*M_PI/2 );
			speed  = speed*k+speedMax*( 1-k );
		}

		generatePlane();

		for( int i=0; i<n; i++ )
		{
			p1[i] = {3.5*sin( o[i].z*tt+i+o[i].x ),-7,4+3.5*cos( o[i].w*tt-i+o[i].y )};
			p2[i] = {3.5*cos( o[i].w*tt+i*i+o[i].y ),+7,4+3.5*sin( o[i].z*tt-i*i+o[i].x )};

			// intersect line-plane
			vect_t v = p2[i].sub( p1[i] );
			double k = -( w.x*p1[i].x+w.y*p1[i].y+w.z*p1[i].z+w.w )/( w.x*v.x+w.y*v.y+w.z*v.z );
			q[i]->setCenter( p1[i].add( v.mul( k ) ) );

			// find internal points of triangle
			vect_t m1 = q[i]->getCenter().sub( s[0]->getCenter() ); //q
			vect_t m2 = s[1]->getCenter().sub( s[0]->getCenter() ); //p2
			vect_t m3 = s[2]->getCenter().sub( s[0]->getCenter() ); //p3
			double m12 = m1.smul( m2 );
			double m13 = m1.smul( m3 );
			double m22 = m2.smul( m2 );
			double m23 = m2.smul( m3 );
			double m33 = m3.smul( m3 );

			double d = ( m22*m33-m23*m23 );
			double w2 = ( m33*m12-m23*m13 )/d;
			double w3 = ( m22*m13-m23*m12 )/d;
			double w1 = 1-w2-w3;

			if( ( 0<=w1 ) & ( w1<=1 ) & ( 0<=w2 ) & ( w2<=1 ) & ( 0<=w3 ) & ( w3<=1 ) )
				q[i]->setColor( {1,0,0} );
			else
				q[i]->setColor( {1,1,1} );

			p1[i] = p1[i].add( v.mul( -100 ) );
			p2[i] = p2[i].add( v.mul( 100 ) );
		}
	}
	return 0;
}

