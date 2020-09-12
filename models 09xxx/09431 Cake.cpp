
/// Project:	FMI 3D (2013)
/// File:		09431 Cake.cpp
/// Topic:		09 Graphical primitives
/// Slide:		43
///	About:		A shape of a cake made of truncated cones
///
/// History:
///		2013.08.22/PB	Initial implementation
///     2013.09.10/PB   Changed button image
///     2013.09.13/PB   Changed motion and colours


#include "fmi3d.h"

#define ANIMATION_TIME 3
#define N 9
GLuint tc;
double coef = 0.5;

/// Generate diaply list for unit truncate cone
GLuint unitTruncatedCone3D( int precision )
{
	double dalpha = 2*M_PI/precision;
	double dprec = 1.0/precision;
	double dx = cos( dalpha );
	double dy = sin( dalpha );

	double x1 = 1;
	double y1 = 0;

	glNewList( ++UID,GL_COMPILE );

	glBegin( GL_QUAD_STRIP ); /// side
	glNormal3f( x1, y1, 0 );
	glTexCoord2d( -dprec, 0 );
	glVertex3f( x1, y1, 0 );
	glNormal3f( x1, y1, 1 );
	glTexCoord2d( -dprec, 1 );
	glVertex3f( x1*coef, y1*coef, 1 );

	for( int i=0; i<precision; i++ )
	{
		double x2 = x1*dx-y1*dy;
		double y2 = y1*dx+x1*dy;

		glNormal3f( x2, y2, 0 );
		glTexCoord2d( i*dprec, 0 );
		glVertex3f( x2, y2, 0 );
		glNormal3f( x2, y2, 1 );
		glTexCoord2d( i*dprec, 1 );
		glVertex3f( x2*coef, y2*coef, 1 );

		x1 = x2;
		y1 = y2;
	}

	glEnd();
	glEndList();
	return UID;
}

class TruncatedCone3D : public Object3D
{
	public:
		TruncatedCone3D( );
};

TruncatedCone3D::TruncatedCone3D( ) : Object3D()
{
	glNewList( frameList,GL_COMPILE );
	glEndList();

	glNewList( imageList,GL_COMPILE );
	glDisable( GL_POLYGON_OFFSET_FILL );
	glCallList( tc );
	glScaled( 1,1,-1 );
	glCallList( idUnitCircle2D[QUALITY_HIGH] );
	glScaled( coef,coef,-1 );
	glTranslated( 0,0,1 );
	glCallList( idUnitCircle2D[QUALITY_HIGH] );
	glEndList();
}

TruncatedCone3D *c;

double colT = 0;
double colMax = 0;
double col[N];
void toggleCol()
{
	colT = t;
	colMax = -8-colMax;
}


void drawScene()
{
	c->clearOrientation();
	for( int i=0; i<N; i++ )
	{
		double m = 1+col[i]/8;
		m = pow(m,2);
		c->setColor({1-0.5*m,0.5*m,1*m});
		c->oxyz.o.z = col[i]+2;
		c->drawImage();
		c->rotateLocal( 180.0/N, {0,0,1} );
	}
}


void drawFrames()
{
}


int main()
{
	/// Open window
	if( !openWindow3D( "Cake" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();

	/// Add buttons
	addButton( new Button2D( "buttons/button_cut.png",GLFW_KEY_SPACE,toggleCol ) );
	addButton( new exitButton2D );

	for( int i=0; i<N; i++ ) col[i]=0;

	viewDistance = 20;
	viewBeta = 1;

	tc = unitTruncatedCone3D( quality[QUALITY_HIGH] );
	c = new TruncatedCone3D( );
	c->setScale( {5,2,5} );
	c->setColor( {1,0.8,0} );
	c->setCenter( {0,0,10} );

	/// Main loop
	double t0 = t;
	while( isRunning() )
	{
		viewAlpha -= (t-t0)/20;
		t0 = t;

		for( int i=0; i<N-1; i++ )
		{
			double iT = t-colT-i*0.2;
			if( 0<=iT && iT/3.0<ANIMATION_TIME )
			{
				double k = 0.5+0.51*cos( iT/ANIMATION_TIME*M_PI/2 );
				col[i] = col[i]*k+colMax*( 1-k );
			}
		}
	}
	return 0;
}

