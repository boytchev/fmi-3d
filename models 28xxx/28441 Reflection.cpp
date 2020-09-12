
/// Project:	FMI 3D (2013)
/// File:		28441 Reflection.cpp
/// Topic:		28 Light
/// Slide:		44
///	About:		Reflection
///
/// History:
///		2013.07.11/PB	Initial implementation


#include "fmi3d.h"

#define N 20
Sphere3D* ball[N];
vect_t v[N];
double p[N];
double s[N];

GLuint texg,texf;

void drawCenter()
{
	double psize = 5;
	double tsize = 1;
	glDisable( GL_LIGHTING );
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D,texg );
	glBegin( GL_QUADS );
	glNormal3f( 0,0,1 );
	glTexCoord2f( -tsize,-tsize );
	glVertex3f( -psize,-psize,0 );
	glTexCoord2f( -tsize,+tsize );
	glVertex3f( +psize,-psize,0 );
	glTexCoord2f( +tsize,+tsize );
	glVertex3f( +psize,+psize,0 );
	glTexCoord2f( +tsize,-tsize );
	glVertex3f( -psize,+psize,0 );
	glEnd();
	glDisable( GL_TEXTURE_2D );
}

void drawOutskirts()
{
	double psize = 5;
	double tsize = 1;
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D,texf );
	glDisable( GL_LIGHTING );
	for( int x=-1; x<2; x++ )
		for( int y=-1; y<2; y++ ) if( x!=0 || y!=0 )
			{
				glPushMatrix();
				glTranslated( 2*x*psize,2*y*psize,0 );
				glBegin( GL_QUADS );
				glNormal3f( 0,0,1 );
				glTexCoord2f( -tsize,-tsize );
				glVertex3f( -psize,-psize,0 );
				glTexCoord2f( -tsize,+tsize );
				glVertex3f( +psize,-psize,0 );
				glTexCoord2f( +tsize,+tsize );
				glVertex3f( +psize,+psize,0 );
				glTexCoord2f( +tsize,-tsize );
				glVertex3f( -psize,+psize,0 );
				glEnd();
				glPopMatrix();
			}
	glDisable( GL_TEXTURE_2D );
}

void drawBalls()
{
	for( int i=0; i<N; i++ )
	{
		ball[i]->drawImage();
	}
}

void drawReflectedBalls( GLuint op )
{
	glEnable( GL_COLOR_LOGIC_OP );
	glLogicOp( op );
	glPushMatrix();
	glScaled( 1,1,-1 );
	for( int i=0; i<N; i++ )
	{
		ball[i]->drawImage();
	}
	glPopMatrix();
	glDisable( GL_COLOR_LOGIC_OP );
}

bool showReflection = true;

void drawScene()
{
	if( inReflection ) return;
	if( inShadow ) return;

	// draw original balls
	drawBalls();

	// draw ground with reflection of balls in black
	glColor4f( 1,1,1,1 );
	glDepthMask( GL_FALSE );
	drawCenter();
	if( showReflection ) drawOutskirts();
	drawReflectedBalls( GL_CLEAR );
	glDepthMask( GL_TRUE );

	// put balls' images in the black areas in the ground
	drawReflectedBalls( GL_OR );

	// put thin layer of ground
	glColor4f( 1,1,1,0.5 );
	drawCenter();
	glColor4f( 1,1,1,( showReflection?0.6:1 ) );
	drawOutskirts();
}


void drawFrames()
{
}


void toggleReflection()
{
	showReflection = !showReflection;
}

int main()
{
	/// Open window
	if( !openWindow3D( "Simple shadow" ) ) return 1;
	noGround();

	viewZ = 3;
	texg = loadTexture( "Textures/Material_Scratch_Round.jpg" );
	texf = loadTexture( "Textures/Material_Wood.jpg" );

	/// Add buttons
	addButton( new Button2D( "buttons/button_show.png",GLFW_KEY_SPACE,toggleReflection ) );
	addButton( new exitButton2D );


	for( int i=0; i<N; i++ )
	{
		ball[i] = new Sphere3D( 0.7 );
		ball[i]->setShininess( 50 );
		double alpha = double( i )/N*2*M_PI;
		double r = random( 0.07,0.15 );
		v[i] = {r*cos( alpha ),r*sin( alpha ),0,0};
		ball[i]->setCenter( {cos( alpha ),sin( alpha ),0} );
		ball[i]->setColor( {1,0.6,0} );
		p[i] = random( 0,2*M_PI );
		s[i] = random( 1.5,2.5 );
	}

	/// Main loop
	while( isRunning() )
	{
		for( int i=0; i<N; i++ )
		{
			vect_t w = ball[i]->getCenter();
			w = w.add( v[i] );
			if( fabs( w.x )>14-0.5 ) v[i].x*=-1;
			if( fabs( w.y )>14-0.5 ) v[i].y*=-1;
			w.z = 0.71+fabs( 7*sin( s[i]*t ) );
			ball[i]->setCenter( w );
		}
	}
	return 0;
}

