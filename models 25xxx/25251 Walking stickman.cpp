
/// Project:	FMI 3D (2013)
/// File:		25251 Walking stickman.cpp
/// Topic:		25 Skeletons
/// Slide:		25
///	About:		A walking stickman
///
/// History:
///		2013.07.13/PB	Initial implementation


#include "fmi3d.h"

//#include <iostream>

Object3D* turtle;

#define SCALE 0.03
#define HOME	turtle->identity()
#define FD(h)	{BX;turtle->translateLocal({h*SCALE,0,0});BX}
#define BK(h)	{BX;turtle->translateLocal({-h*SCALE,0,0});BX}
#define SL(h)	{BX;turtle->translateLocal({0,h*SCALE,0});BX}
#define SR(h)	{BX;turtle->translateLocal({0,-h*SCALE,0});BX}
#define SLX(h)	turtle->translateLocal({0,h*SCALE,0})
#define SRX(h)	turtle->translateLocal({0,-h*SCALE,0})
#define SU(h)	{BX;turtle->translateLocal({0,0,h*SCALE});BX}
#define SD(h)	{BX;turtle->translateLocal({0,0,-h*SCALE});BX}
#define LT(h)	turtle->rotateLocal(h,{0,0,1})
#define RT(h)	turtle->rotateLocal(-h,{0,0,1})
#define UP(h)	turtle->rotateLocal(-h,{0,1,0})
#define DN(h)	turtle->rotateLocal(h,{0,1,0})
#define LR(h)	turtle->rotateLocal(-h,{1,0,0})
#define RR(h)	turtle->rotateLocal(h,{1,0,0})
#define BX		bones[b++]=turtle->getCenter();
#define GET		turtle->oxyz
#define SET(o)	turtle->oxyz = o

#define M 140
vect_t bones[M];
int b=0;
Sphere3D *gr;

Sphere3D *s1,*s2, *s3;

double minz = -5;
double minx = -5;
oxyz_t el,er,ee;
vect_t lf,rf;
double q;
double grSpin=0;
double rsin( double x )
{
	return sin( x*M_PI/180 );
}

double oldlfx,oldrfx;
void drawScene()
{
	if( inShadow ) return;

	glEnable( GL_FOG );
	glFogf( GL_FOG_START,viewDistance );
	glFogf( GL_FOG_END,viewDistance+20 );

	s1->setCenter( {lf.x+0.3,lf.y,-5.5} );
	s2->setCenter( {rf.x+0.3,rf.y,-5.5} );
	s3->oxyz = ee;
	s3->rotateLocal( 100, {0,1,0} );
	s3->translateLocal( {0,0,-0.4} );

	double newminz = 100;
	for( int i=0; i<b; i++ )
	{
		if( newminz>bones[i].z ) newminz=bones[i].z-0.2;
	}
	minz = 0.9*minz+0.1*newminz;

	if( lf.x<oldlfx )
		grSpin -= 1.16*( lf.x-oldlfx );
	else
		grSpin -= 1.16*( rf.x-oldrfx );
	gr->drawImage();

	oldlfx = lf.x;
	oldrfx = rf.x;

	glTranslated( 0,0.3*rsin( 40*q ),-minz );
	s1->drawImage();
	s2->drawImage();
	glPushMatrix();
	s3->translateLocal( {0.3,0,0.4} );
	for( int i=0; i<6; i++ )
	{
		s3->drawImage();
		s3->setScale( {0.7-i/20.0,0.6-i/15.0,0.4+i/30.0} );
		s3->translateLocal( {-0.03,0,-0.05} );
	}
	s3->translateLocal( {0.2,0,0.55} );
	s3->rotateLocal( -15, {0,1,0} );
	s3->scaleLocal( {0.7,0.3,0.3} );
	s3->drawImage();
	s3->setScale( {0.7,0.6,0.4} );
	glPopMatrix();

	glEnable( GL_LINE_SMOOTH );
	glLineWidth( 5 );

	glDisable( GL_LIGHTING );
	glDisable( GL_TEXTURE_2D );
	glColor3f( 1,1,1 );
	glBegin( GL_LINES );
	for( int i=0; i<28; i++ )
	{
		if( ( i==7 )||( i==13 ) ) bones[i].z=-5.1;
		if( i==26 )
		{
			glEnd();
			glLineWidth( 3 );
			glBegin( GL_LINES );
		}
		bones[i].vertex();
	}
	glEnd();
	glColor3f( 1,0,0 );
	glEnable( GL_LIGHTING );
	for( int i=0; i<26; i++ )
	{
		glPushMatrix();
		glTranslated( bones[i].x,bones[i].y,bones[i].z );
		glScaled( 0.2,0.2,0.2 );
		glCallList( idUnitSphere3D[QUALITY_LOW] );
		glPopMatrix();
	}

	glColor3f( 0.5,0.5,1 );
	glDisable( GL_LIGHTING );
	glEnable( GL_POINT_SMOOTH );
	glPointSize( 9 );
	glBegin( GL_POINTS );
	el.o.vertex();
	er.o.vertex();
	glEnd();
}


void drawFrames()
{
}

double m = 0;
void skeleton()
{
	oxyz_t m1,m2;
	double a;

	b=0;
	HOME;

	// backbone
	m1 = GET;
	DN( 90 ); FD( 65 );

	// right leg
	m2 = GET;
	a = 5*rsin( 180+40*q );
	RT( 90 ); UP( a ); LT( a ); FD( 20 ); LT( 90 );
	a = 15+30*rsin( 180+40*q );
	UP( a ); FD( 60 );
	if( a<5 ) a=5;
	DN( a ); FD( 50 ); rf=turtle->getCenter();

	// left leg
	SET( m2 );
	a = 5*rsin( 40*q );
	LT( 90 ); UP( a ); RT( a ); FD( 20 ); RT( 90 );
	a = 15+30*rsin( 40*q );
	UP( a ); FD( 60 );
	if( a<5 ) a=5;
	DN( a ); FD( 50 ); lf=turtle->getCenter();

	double w = t-8*int( t/8 );
	double it = int( w );
	if( it==3 )
		m=80*( w-it );
	else if( it==5 )
		m=80-80*( w-it );

	// right hand
	SET( m1 );
	a = 15*rsin( 40*q );
	RT( 90+a ); DN( 10+a ); FD( 25 ); LT( 90 ); DN( 90 ); RT( 20-m );
	a = 10+30*rsin( 40*q );
	UP( a ); FD( 50 );
	if( a<5 ) a=5;
	UP( a-m ); FD( 40 );

	// left hand
	SET( m1 );
	a = 15*rsin( 180+40*q );
	LT( 90-a ); DN( 10+a ); FD( 25 ); RT( 90 ); DN( 90 ); LT( 20+m );
	a = 10+30*rsin( 180+40*q );
	UP( a ); FD( 50 );
	if( a<5 ) a=5;
	UP( a-m ); FD( 40 );

	// head
	SET( m1 );
	LT( 60*rsin( 15*q ) );
	DN( 15*rsin( 5*q ) );
	LR( 25*rsin( 12*q ) );
	SU( 20 ); DN( 30 ); BK( 10 ); FD( 10 ); UP( 30 ); FD( 10 ); DN( 10 ); SU( 8 );
	SL( 5 ); SR( 10 ); SL( 5 ); // mouth
	SU( 8 ); UP( 30 ); SU( 10 );
	ee=GET; FD( 11 ); SD( 10 ); SLX( 7 ); el=GET; SRX( 14 ); er=GET; SLX( 7 ); // eyes
}

int main()
{
	/// Open window
	if( !openWindow3D( "Walking skickman" ) ) return 1;
	changeBrightness();
	noGround();

	/// Add buttons
	addButton( new exitButton2D );

	double rad = 50;
	gr = new Sphere3D( rad,QUALITY_MEDIUM );
	gr->setTexture( loadTexture( "textures/Material_Wood.jpg" ) );
	gr->setTextureScale( {80,40,1,1} );
	gr->setScale( {rad,rad,rad} );

	s1 = new Sphere3D( 1 );
	s1->setColor( {1,0,0} );
	s1->setScale( {0.8,0.4,0.5} );
	s2 = new Sphere3D( 1 );
	s2->setColor( {1,0,0} );
	s2->setScale( {0.8,0.4,0.5} );

	s3 = new Sphere3D( 1 );
	s3->setColor( {1,1,1} );
	s3->setScale( {0.7,0.6,0.4} );

	turtle = new Object3D();
	viewZ = 3;
	viewDistance = 15;

	/// Main loop
	double t0=t;
	while( isRunning() )
	{
		viewAlpha+=( t-t0 )/10.0;
		t0=t;
		q = 5*t;
		skeleton();
		gr->identity();
		gr->rotateLocal( 90, {0,1,0} );
		gr->rotateLocal( -grSpin, {0,1,0} );
		gr->rotateLocal( 360/32.0/2.0, {0,0,1} );
		gr->setCenter( {0,0,-rad} );
	}

	return 0;
}

