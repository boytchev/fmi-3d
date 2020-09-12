
/// Project:	FMI 3D (2013)
/// File:		13421 Water waves grid.cpp
/// Topic:		13 Physics
/// Slide:		42
///	About:		A grid of nodes for water waves
///
/// History:
///		2013.08.22/PB	Initial implementation
///     2013.09.13/PB   Reduced complexity of objects


#include "fmi3d.h"

#define N 11
Sphere3D *p[N][N];


void drawScene()
{
	for( int i=0; i<N; i++ )
		for( int j=0; j<N; j++ )
			p[i][j]->drawImage();
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glColor3f(0,0,0);
	glLineWidth(4);
	glEnable(GL_LINE_SMOOTH);
	glDepthMask(GL_FALSE);
	for( int i=0; i<N; i++ )
	{
		glBegin(GL_LINE_STRIP);
		for( int j=0; j<N; j++ ) p[i][j]->getCenter().vertex();
		glEnd();
		glBegin(GL_LINE_STRIP);
		for( int j=0; j<N; j++ ) p[j][i]->getCenter().vertex();
		glEnd();
	}

	glDepthMask(GL_TRUE);
}


void drawFrames()
{
}


int main()
{
	/// Open window
	if( !openWindow3D( "Water waves grid" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();

	/// Add buttons
	addButton( new exitButton2D );

	for( int i=0; i<N; i++ )
		for( int j=0; j<N; j++ )
		{
			p[i][j] = new Sphere3D(0.2,QUALITY_LOWER);
			p[i][j]->setCenter({2.0*i-(N-1),2.0*j-(N-1),3});
			p[i][j]->setColor({0.5,0.5,1});
		}

	viewDistance = 20;

	/// Main loop
	double t0 = t;
	while( isRunning() )
	{
		viewAlpha -= ( t-t0 )/20;
		t0 = t;
	}
	return 0;
}

