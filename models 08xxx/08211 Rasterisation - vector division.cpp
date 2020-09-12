
/// Project:	FMI 3D (2013)
/// File:		08211 Rasterisation - vector division.cpp
/// Topic:		08 Rasterisation
/// Slide:		21
///	About:		Rasterisation of segment with vector
///
/// History:
///		2013.08.15/PB	Initial implementation


#include "fmi3d.h"

#define N 30
#define M 18
Cube3D *a[N][M];
int px,py,qx,qy;
bool show = false;
Vector3D *w;

void drawScene()
{
	for( int m=0; m<M; m++ )
		for( int n=0; n<N; n++ )
			a[n][m]->drawImage();

	if( show )
	{
		w->drawImage();

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
	if( show )
	{
		//w->drawFrame();
	}

	for( int m=0; m<M; m++ )
		for( int n=0; n<N; n++ )
			a[n][m]->drawFrame();
}

void drawPixel( double px,double py )
{
	a[int( px+0.5 )][int( py+0.5 )]->setColor( {0.5,0.5,1} );
	a[int( px+0.5 )][int( py+0.5 )]->setScale( {0.6,0.1,0.6} );
}

void randomize()
{
	px = irandom( 1,N/2 );
	py = irandom( 1,M/2 );
	qx = N-irandom( 2,N/2 );
	qy = M-irandom( 2,M/2 );

	int n;
	double vx=qx-px,vy=qy-py;

	if( fabs( vx )>fabs( vy ) )
	{
		n=fabs( vx );
		w->setCenter( a[px][py]->getCenter().add( {-0.5,0,1.5} ) );
	}
	else
	{
		n=fabs( vy );
		w->setCenter( a[px][py]->getCenter().add( {-1.5,0,0.5} ) );
	}
	vx = vx/n;
	vy = vy/n;

	w->setDirection( {double(qx-px),0,double(qy-py)} );
	w->setLength( sqrt( vx*vx+vy*vy )-0.4 );

	for( int m=0; m<M; m++ )
		for( int n=0; n<N; n++ )
		{
			a[n][m]->setColor( {1,1,1,0.8} );
			a[n][m]->setScale( {1,0,1} );
		}
	for( int i=1; i<n; i++ )
		drawPixel( px+i*vx,py+i*vy );
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
	if( !openWindow3D( "Rasterisation: vector division" ) ) return 1;
	changeBrightness();
	changeGround();
	changeGround();
	changeGround();

	/// Add buttons
	addButton( new Button2D( "buttons/button_show.png",GLFW_KEY_SPACE,hideShow ) );
	addButton( new Button2D( "buttons/button_random.png",'R',randomize ) );
	addButton( new exitButton2D );

	w = new Vector3D( {0,0,1},0.4 );
	w->setColor( {1,0,0} );

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

