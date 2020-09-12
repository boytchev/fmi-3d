
/// Project:	FMI 3D (2013)
/// File:		28071 Shaded complex object.cpp
/// Topic:		28 Light
/// Slide:		07
///	About:		A shaded complex object
///
/// History:
///		2013.07.06/PB	Initial implementation


#include "fmi3d.h"

GLuint idBilightSphere3D;
GLuint unitBilightSphere3D( int precision )
{
	double alpha;
	double dalpha = 2.0*M_PI/precision;
	double beta = M_PI/2;
	double dbeta = 1.0*M_PI/precision;
	double tx;
	double ty;
	double dprec = 1.0/precision;

	glNewList( ++UID,GL_COMPILE );

	ty = 1;
	for( int j=0; j<precision; j++, beta-=dbeta )
	{
		alpha = 0;
		tx = 0;
		for( int i=0; i<precision; i++, alpha+=dalpha )
		{
			vect_t p;
			glBegin( GL_QUADS );

			p = p.s3d( alpha,beta,1 );
			glTexCoord2d( tx,ty );
			glNormal3f( -p.x,-p.y,-p.z );
			glVertex3f( p.x,p.y,p.z );

			p = p.s3d( alpha+dalpha,beta,1 );
			glTexCoord2d( tx+dprec,ty );
			glNormal3f( -p.x,-p.y,-p.z );
			glVertex3f( p.x,p.y,p.z );

			p = p.s3d( alpha+dalpha,beta-dbeta,1 );
			glTexCoord2d( tx+dprec,ty-dprec );
			glNormal3f( -p.x,-p.y,-p.z );
			glVertex3f( p.x,p.y,p.z );

			p = p.s3d( alpha,beta-dbeta,1 );
			glTexCoord2d( tx,ty-dprec );
			glNormal3f( -p.x,-p.y,-p.z );
			glVertex3f( p.x,p.y,p.z );

			glEnd( );

			tx+=dprec;
		}
		ty-=dprec;
	}

	glEndList();
	return UID;
}

class Complexo3D : public Object3D
{
	public:
		Complexo3D( );
		void drawImage();
};

Complexo3D::Complexo3D( ) : Object3D()
{
	setColor( {0.5,0.5,1} );
	setLighting( false );

	glNewList( frameList,GL_COMPILE );
	for( int i=0; i<10; i++ )
	{
		glPushMatrix();
		glRotated( 36*i,1,0,0 );
		glTranslated( 0,0,0.6 );
		glScaled( 0.3,0.1,0.4 );
		glCallList( idUnitSphere3D[QUALITY_MEDIUM] );
		glPopMatrix();
	}
	glEndList();

	glNewList( imageList,GL_COMPILE );

	glLightModeli( GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE );

	GLdouble cp[2][4]= { {-1,0,0,0.5}, {1,0,0,0.5} };
	glEnable( GL_CLIP_PLANE0 );
	glEnable( GL_CLIP_PLANE1 );
	glClipPlane( GL_CLIP_PLANE0,cp[0] );
	glClipPlane( GL_CLIP_PLANE1,cp[1] );
	glPushMatrix();
	glCallList( idBilightSphere3D );
	glTranslated( 1,0,0 );
	glCallList( idBilightSphere3D );
	glTranslated( -2,0,0 );
	glCallList( idBilightSphere3D );
	glPopMatrix();
	glDisable( GL_CLIP_PLANE1 );
	glDisable( GL_CLIP_PLANE0 );

	glLightModeli( GL_LIGHT_MODEL_TWO_SIDE,GL_FALSE );

	for( int i=0; i<10; i++ )
	{
		glPushMatrix();
		glRotated( 36*i,1,0,0 );
		glTranslated( 0,0,0.95 );
		glScaled( 0.3,0.3,0.1 );
		glCallList( idUnitSphere3D[QUALITY_MEDIUM] );
		glPopMatrix();
	}

	glPushMatrix();
	glScaled( 0.5,0.5,0.5 );
	glCallList( idUnitSphere3D[QUALITY_MEDIUM] );
	glPopMatrix();


	glEndList();
}

void Complexo3D::drawImage()
{
	prepareImageProps( );
	glCallList( imageList );
	glRotated(150*t,1,0,0);
	glCallList( frameList );
	restoreImageProps( );
}

Complexo3D *c;

void drawScene()
{
	c->drawImage();
}


void drawFrames()
{
}


int light = false;
void changeLight()
{
	light = !light;
	c->setLighting(light);
}

int main()
{
	/// Open window
	if( !openWindow3D( "Shaded complex object" ) ) return 1;

	/// Add buttons
	addButton( new Button2D("buttons/button_light.png",GLFW_KEY_SPACE,changeLight) );
	addButton( new exitButton2D );

	viewAlpha = 0;
	viewZ = 5;
	viewDistance = 20;

	idBilightSphere3D = unitBilightSphere3D( 32 );

	c = new Complexo3D();
	c->setSize( 5 );

	/// Main loop
	while( isRunning() )
	{
		c->identity();
		c->rotateGlobal( 21*t, {0,0,1} );
		c->rotateGlobal( 23*t, {0,1,0} );
		c->rotateGlobal( 19*t, {1,0,0} );
		c->setCenter( {0,0,6} );
	}
	return 0;
}

