
/// Project:	FMI 3D (2013)
/// File:		21441 Subpixels.cpp
/// Topic:		21 Dithering and antialiasing
/// Slide:		49
///	About:		Using pixels and subpixels for antialiasing
///
/// History:
///		2013.07.30/PB	Initial implementation
///		2013.09.13/PB	Reduced the number of cubes


#include "fmi3d.h"

#define ANIMATION_TIME 2

#define N 50
#define M 30
#define S 2
int subpix[N][M];
Cube3D *a[S+1][N][M];

int modes = S;
int mode = 0;

double A[3] = {1,1,-3};
double B[3] = {1,-2,1};
double C[3] = {-10,5,-28};
double D[3] = {2,2.3,5};

void drawScene()
{
	int s = mode+1;
	for( int m=0; m<M/s; m++ )
		for( int n=0; n<N/s; n++ )
			a[s][n][m]->drawImage();
}


void drawFrames()
{
	int s = 2;
	for( int m=0; m<M/s; m++ )
		for( int n=0; n<N/s; n++ )
			a[s][n][m]->drawFrame();
}

Button2D *sB;

void nextStep()
{

	mode = ( mode+1 )%modes;
	sB->setState( mode );
}

void regenerate()
{
	for( int s=1; s<=S; s++ )
		for( int m=0; m<M/s; m++ )
			for( int n=0; n<N/s; n++ )
			{
				vect_t v = {0.5*s*( n-( N/s-1 )/2.0 ),0,0.5*s*m+0.25*s};

				if( s==1 )
				{
					subpix[n][m]=1;

					for( int k=0; k<3; k++ )
					{
						double c = A[k]*v.x+B[k]*v.z+C[k];
						if( fabs( c )<D[k] )
							subpix[n][m]=0;
					}
				}

				int sum = 0;
				for( int x=0; x<s; x++ )
					for( int y=0; y<s; y++ )
						sum+=subpix[s*n+x][s*m+y];
				double c = double( sum )/s/s;
				a[s][n][m]->setColor( {1,c,c} );
			}
}

void randomize()
{
	for( int k=0; k<3; k++ )
	{
		A[k] = random( -4,4 );
		B[k] = random( -4,4 );
		C[k] = random( -30,30 );
		D[k] = random( 2,5 );
	}
	regenerate();
}



int main()
{

	/// Open window
	if( !openWindow3D( "Subpixels" ) ) return 1;
	changeBrightness();
	changeGround();
	changeGround();
	changeGround();

	/// Add buttons
	sB = new Button2D( "buttons/button_show.png",GLFW_KEY_SPACE,nextStep );
	addButton( new Button2D( "buttons/button_random.png",'R',randomize ) );
	addButton( new exitButton2D );

	viewAlpha = 3*M_PI/2;
	viewBeta = 0.1;
	viewZ = 8;
	viewDistance = 30;

	for( int s=1; s<=S; s++ )
		for( int m=0; m<M/s; m++ )
			for( int n=0; n<N/s; n++ )
			{
				vect_t v = {0.5*s*( n-( N/s-1 )/2.0 ),0,0.5*s*m+0.25*s};

				a[s][n][m] = new Cube3D( 0.5 );
				a[s][n][m]->setCenter( v );
				a[s][n][m]->setScale( {0.5*s,0.5,0.5*s} );
			}
	regenerate();

	/// Main loop
	t = 0;
//	stepT = t;
	while( isRunning() )
	{
//		if( t-stepT<ANIMATION_TIME*( 3-f ) )
//		{
//			double s = 0.5+0.5*cos( ( ( t-stepT )/ANIMATION_TIME )*M_PI/2 );
//			for( int k=0; k<K; k++ )
//			{
//				vect_t c = o[k]->getCenter().mul( s ).add( target[k].mul( 1-s ) );
//				if( target[k].z>20 ) c.z=20;
//				o[k]->setCenter( c );
//			}
//
//			s = 1-( 1-s )*( 1-s );
//			if( mode==2 )
//			{
//				vect_t c;
//				for( int m=0; m<M; m++ )
//					for( int n=0; n<N; n++ )
//					{
//						c = a[0][n][m]->getColor();
//						c.w = c.w*( s )+( 1-s )*a[2][n][m]->getColor().w;
//						a[0][n][m]->setColor( c );
//
//						c = a[1][n][m]->getColor();
//						c.w = c.w*( s )+( 1-s )*a[3][n][m]->getColor().w;
//						a[1][n][m]->setColor( c );
//					}
//			}
//			if( mode==3 )
//			{
//				for( int m=0; m<M; m++ )
//					for( int n=0; n<N; n++ )
//					{
//						a[0][n][m]->setColor(a[0][n][m]->getColor().mul(s).add(cols[n][m].mul(1-s)));
//						a[1][n][m]->setColor(a[1][n][m]->getColor().mul(s).add(cols[n][m].mul(1-s)));
//					}
//			}
//		}
//		else
//		{
//			if( ( mode==2 ) && !merged )
//			{
//				merged = true;
//				target[2]= {0,0,100};
//				target[3]= {0,0,100};
//				o[2]->setCenter( {0,0,100} );
//				o[3]->setCenter( {0,0,100} );
//			}
//			if( ( mode==3 ) && merged )
//			{
//				merged = false;
//				target[2]= {0,0,100};
//				target[3]= {0,0,100};
//				o[2]->setCenter( {0,0,100} );
//				o[3]->setCenter( {0,0,100} );
//			}
//		}
	}
	return 0;
}

