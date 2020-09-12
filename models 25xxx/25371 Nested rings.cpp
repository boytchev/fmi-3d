
/// Project:	FMI 3D (2013)
/// File:		25371 Nested rings.cpp
/// Topic:		25 Skeletons
/// Slide:		37
///	About:		Nested rings
///
/// History:
///		2013.07.13/PB	Initial implementation


#include "fmi3d.h"

#define ANIMATION_TIME 10
double q;

GLuint unitStone( )
{
	#define PRECA 4
	#define PRECB 2
	double alpha;
	double dalpha = 2.0*M_PI/PRECA;
	double beta = M_PI/2;
	double dbeta = 1.0*M_PI/PRECB;

	glNewList( ++UID,GL_COMPILE );

	for( int j=0; j<PRECB; j++, beta-=dbeta )
	{
		alpha = 0;
		for( int i=0; i<PRECA; i++, alpha+=dalpha )
		{
			vect_t p;
			glBegin( GL_QUADS );

			p = p.s3d( alpha,beta,1 );
			glNormal3f( p.x,p.y,p.z );
			glVertex3f( p.x,p.y,p.z );

			p = p.s3d( alpha+dalpha,beta,1 );
			glNormal3f( p.x,p.y,p.z );
			glVertex3f( p.x,p.y,p.z );

			p = p.s3d( alpha+dalpha,beta-dbeta,1 );
			glNormal3f( p.x,p.y,p.z );
			glVertex3f( p.x,p.y,p.z );

			p = p.s3d( alpha,beta-dbeta,1 );
			glNormal3f( p.x,p.y,p.z );
			glVertex3f( p.x,p.y,p.z );

			glEnd( );
		}
	}

	glEndList();
	return UID;
}

class Ring3D : public Object3D
{
	private:
	public:
		Ring3D( );
		void drawImage();
};

Ring3D::Ring3D( ) : Object3D()
{
	GLuint stone = unitStone();

	/// Create display list of the cylinder side
	glNewList( imageList,GL_COMPILE );
	glPushMatrix();
	glColor3f( 1,0.8,0 );
	glScaled( 1,-1,0.2 );
	glTranslated( 0,0,-0.5 );
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glCallList( idUnitCylinder3D[QUALITY_HIGH] );
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	glPopMatrix();
	for( int i=0; i<36; i++ )
	{
		glPushMatrix();
		glRotated( 10*i,0,0,1 );
		if( ( i==0 ) || ( i==18 ) )
		{
			glPushMatrix();
			glTranslated( 0,1,0 );
			glColor3f( 1,0.5,0 );
			glScaled( 0.03,0.2,0.03 );
			glRotated( 90,1,0,0 );
			glCallList( idUnitCylinder3D[QUALITY_MEDIUM] );
			glPopMatrix();
		}
		{
			glTranslated( 0,1,0 );
			glRotated( 90,1,0,0 );
			glColor3f( random( 0,1 ),random( 0,0.5 ),random( 0,0.5 ) );
			glScaled( 0.05,0.05,0.03 );
			glCallList( stone );
		}
		glPopMatrix();
	}
	glEndList();
}


void Ring3D::drawImage()
{
	prepareImageProps();
	glRotated( 30*q,0,0,1 );
	glRotated( 20*q,1,0,0 );
	glCallList( imageList );

	double s = 0.8;
	for( int i=0; i<15; i++ )
	{
		glScaled( s,s,s );
		glRotated( 90,0,0,1 );
		glRotated( (20+4.345678*i)*q,1,0,0 );
		glCallList( imageList );
	}

	restoreImageProps();
}

Ring3D *r;
Sphere3D *s;

void drawScene()
{
	s->drawImage();
	r->drawImage();
}


void drawFrames()
{
}


int main()
{
	/// Open window
	if( !openWindow3D( "Nested rings" ) ) return 1;
	changeBrightness();
	changeGround();
	changeGround();
	changeGround();

	/// Add buttons
	addButton( new exitButton2D );

	viewZ = 6;
	viewDistance = 20;

	r = new Ring3D();
	r->setSize( 6 );
	r->setCenter( {0,0,6.5} );
	r->setShininess( 10 );

	s = new Sphere3D( 0.17, QUALITY_HIGH );
	s->setColor({0,0,0});
	s->setCenter({0,0,6.5});
	s->setShininess(50);

	/// Main loop
	q = 0; t=0;
	while( isRunning() )
	{
		viewDistance = 12+5*sin(t/5.4321);
		if( t<ANIMATION_TIME )
			q = t*(0.5-0.5*cos(t/ANIMATION_TIME*M_PI));
		else
			q = t;
	}
	return 0;
}

