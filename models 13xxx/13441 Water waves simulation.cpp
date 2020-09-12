
/// Project:	FMI 3D (2013)
/// File:		13441 Water waves simulation.cpp
/// Topic:		13 Physics
/// Slide:		44
///	About:		A grid of nodes and simulated water waves
///
/// History:
///		2013.08.22/PB	Initial implementation
///     2013.09.13/PB   Reduced complexity of objects, changed button image


#include "fmi3d.h"

#define N 11
Sphere3D *p[N][N];
double ofs[N][N];
bool show = false;

void drawScene()
{
	if( !show )
	{
		for( int i=0; i<N; i++ )
			for( int j=0; j<N; j++ )
				p[i][j]->drawImage();

		glDisable( GL_LIGHTING );
		glDisable( GL_TEXTURE_2D );
		glColor3f( 0,0,0 );
		glLineWidth( 4 );
		glEnable( GL_LINE_SMOOTH );
		glDepthMask( GL_FALSE );
		for( int i=0; i<N; i++ )
		{
			glBegin( GL_LINE_STRIP );
			for( int j=0; j<N; j++ ) p[i][j]->getCenter().vertex();
			glEnd();
			glBegin( GL_LINE_STRIP );
			for( int j=0; j<N; j++ ) p[j][i]->getCenter().vertex();
			glEnd();
		}
		glDepthMask( GL_TRUE );
	}
	else
	{

		vect_t n[N][N];

		for( int i=0; i<N; i++ )
			for( int j=0; j<N; j++ )
				n[i][j]= {0,0,1};
		for( int i=1; i<N-1; i++ )
			for( int j=1; j<N-1; j++ )
			{
				vect_t v = p[i+1][j]->getCenter().sub( p[i-1][j]->getCenter() );
				vect_t u = p[i][j+1]->getCenter().sub( p[i][j-1]->getCenter() );
				v = v.vmul( u );
				v = v.mul( 1/v.distance() );
				n[i][j]=v;
			}

		glEnable( GL_LIGHTING );
		glColor3f( 0.5,0.5,1 );
		for( int i=0; i<N-1; i++ )
		{
			glBegin( GL_QUAD_STRIP );
			for( int j=0; j<N; j++ )
			{
				glNormal3f( n[i][j].x,n[i][j].y,n[i][j].z );
				p[i][j]->getCenter().vertex();
				glNormal3f( n[i+1][j].x,n[i+1][j].y,n[i][j].z );
				p[i+1][j]->getCenter().vertex();
			}
			glEnd();
		}
	}
}


void drawFrames()
{
}


void hideShow()
{
	show = !show;
}


int main()
{
	/// Open window
	if( !openWindow3D( "Water waves simulation" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();

	/// Add buttons
	addButton( new Button2D( "buttons/button_wireframe.png",GLFW_KEY_SPACE,hideShow ) );
	addButton( new exitButton2D );

	for( int i=0; i<N; i++ )
		for( int j=0; j<N; j++ )
		{
			p[i][j] = new Sphere3D( 0.2,QUALITY_LOWER );
			p[i][j]->setCenter( {2.0*i-( N-1 ),2.0*j-( N-1 ),3} );
			p[i][j]->setColor( {0.5,0.5,1} );
			ofs[i][j] = random( 0,2*M_PI );
		}

	viewDistance = 20;

	/// Main loop
	double t0 = t;
	while( isRunning() )
	{
		viewAlpha -= ( t-t0 )/20;
		t0 = t;

		for( int i=0; i<N; i++ )
			for( int j=0; j<N; j++ )
				p[i][j]->oxyz.o.z=4+0.7*sin( 3*t+ofs[i][j] );
	}
	return 0;
}

