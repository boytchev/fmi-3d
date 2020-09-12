
/// Project:	FMI 3D (2013)
/// File:		08131 Rasterisation - interger division.cpp
/// Topic:		08 Rasterisation
/// Slide:		13
///	About:		Integer rasterisation of segment
///
/// History:
///		2013.08.15/PB	Initial implementation


#include "fmi3d.h"

#define N 30
#define M 18
Cube3D *a[N][M];
int px,py,qx,qy;
bool show = false;

void drawScene()
{
	for( int m=0; m<M; m++ )
		for( int n=0; n<N; n++ )
			a[n][m]->drawImage();

	if( show )
	{
		glColor4f( 1,0,0,0.8 );
		glEnable( GL_LINE_SMOOTH );
		glLineWidth( 5 );
		glBegin( GL_LINES );
		a[px][py]->getCenter().add( {0,+0.3,0} ).vertex();
		a[qx][qy]->getCenter().add( {0,+0.3,0} ).vertex();
		a[px][py]->getCenter().add( {0,-0.3,0} ).vertex();
		a[qx][qy]->getCenter().add( {0,-0.3,0} ).vertex();
		glEnd();
	}
}


void drawFrames()
{
	for( int m=0; m<M; m++ )
		for( int n=0; n<N; n++ )
			a[n][m]->drawFrame();
}

void drawLine( int px,int py,int qx,int qy )
{
	if( fabs( px-qx )<2 && fabs( py-qy )<2 ) return;
	int mx = ( px+qx )/2;
	int my = ( py+qy )/2;
	a[mx][my]->setColor( {0.5,0.5,1} );
	a[mx][my]->setScale( {0.6,0.1,0.6} );
	drawLine( px,py,mx,my );
	drawLine( mx,my,qx,qy );
}

void randomize()
{
	px = irandom( 1,N/2 );
	py = irandom( 1,M/2 );
	qx = N-irandom( 2,N/2 );
	qy = M-irandom( 2,M/2 );

	for( int m=0; m<M; m++ )
		for( int n=0; n<N; n++ )
		{
			a[n][m]->setColor( {1,1,1,0.8} );
			a[n][m]->setScale( {1,0,1} );
		}
	drawLine( px,py,qx,qy );
	a[px][py]->setColor( {0.5,0.5,1} );
	a[px][py]->setScale( {1,1,1} );
	a[qx][qy]->setColor( {0.5,0.5,1} );
	a[qx][qy]->setScale( {1,1,1} );
}


void hideShow()
{
	show = !show;
}


int main()
{

	/// Open window
	if( !openWindow3D( "Rasterisation: interger division" ) ) return 1;
	changeBrightness();
	changeGround();
	changeGround();
	changeGround();

	/// Add buttons
	addButton( new Button2D( "buttons/button_show.png",GLFW_KEY_SPACE,hideShow ) );
	addButton( new Button2D( "buttons/button_random.png",'R',randomize ) );
	addButton( new exitButton2D );

	viewAlpha = 3*M_PI/2;
	viewBeta = 0.1;
	viewZ = 8;
	viewDistance = 30;

	for( int m=0; m<M; m++ )
		for( int n=0; n<N; n++ )
		{
			vect_t v = {( n-( N-1 )/2.0 ),0,m+0.5};

			a[n][m] = new Cube3D( 1 );
			a[n][m]->setCenter( v );
			a[n][m]->setLighting( false );
		}
	randomize();

	/// Main loop
	t = 0;
	while( isRunning() )
	{
	}
	return 0;
}

