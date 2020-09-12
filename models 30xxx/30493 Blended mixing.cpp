
/// Project:	FMI 3D (2013)
/// File:		30493 Blended mixing.cpp
/// Topic:		30 Films
/// Slide:		49
///	About:		Mixing frames with blending
///
/// History:
///		2013.07.15/PB	Initial implementation


#include "fmi3d.h"
//#include <iostream>

#define ANIMATION_TIME 1

#define K 4
#define N 21
#define M 9
Text3D *o[K];
Cube3D *a[K][N][M];
vect_t cols[N][M];

void drawScene()
{
	for( int k=0; k<K; k++ )
	{
		glPushMatrix();
		glMultMatrixd( ( GLdouble* )&o[k]->oxyz );
		glTranslated( 0,0,-0.4 );
		glRotated( -90,1,0,0 );
		for( int m=0; m<M; m++ )
			for( int n=0; n<N; n++ )
				if( a[k][n][m]->getColor().w>0.5 )
					a[k][n][m]->drawImage();
		glPopMatrix();
	}
	for( int k=0; k<K; k++ )
	{
		glPushMatrix();
		glMultMatrixd( ( GLdouble* )&o[k]->oxyz );
		glTranslated( 0,0,-0.4 );
		glRotated( -90,1,0,0 );
		for( int m=0; m<M; m++ )
			for( int n=0; n<N; n++ )
				if( a[k][n][m]->getColor().w<0.5 ) a[k][n][m]->drawImage();
		glPopMatrix();
	}

}


void drawFrames()
{
	for( int k=0; k<K; k++ )
	{
		glPushMatrix();
		glMultMatrixd( ( GLdouble* )&o[k]->oxyz );
		glTranslated( 0,0,-0.4 );
		glRotated( -90,1,0,0 );
		for( int m=0; m<M; m++ )
			for( int n=0; n<N; n++ )
				a[k][n][m]->drawFrame();
		glPopMatrix();
	}
}

int modes = 4;
int mode = modes-1;
Button2D *sB;
vect_t target[K];
double stepT;
bool merged;
double f = 0;

void nextStep()
{

	double L = -5.5-N/4.0+0.5; //left
	double R = +5.5-N/4.0+0.5; //right
	double O = -N/4.0+0.5; //middle

	stepT = t;
	mode = ( mode+1 )%modes;
	sB->setState( mode );
	double q = 2+f*random( -1.5,2 );
	double r = 2+f*random( -1.5,2 );
	switch( mode )
	{
		case 0:
			merged = false;
			target[0]= {L,0,1};
			target[1]= {R,0,1};
			target[2]= {O,0,6};
			target[3]= {R,0,100};
			for( int m=0; m<M; m++ )
				for( int n=0; n<N; n++ )
				{
					a[0][n][m]->setColor( {random( 0,0.1 ),random( 0,0.1 ),random( 0.6,1 ),1} );
					a[1][n][m]->setColor( {random( 0.6,1 ),random( 0,0.1 ),random( 0,0.1 ),1} );
					double p = sin( t+m/q )+cos( t+n/r );
					p = 0.5+p/2;
					a[2][n][m]->setColor( {1,1,p,0.1+0.9*p} );
					a[3][n][m]->setColor( {1,1,1-p,0.1+0.9*( 1-p )} );
					cols[n][m] = a[0][n][m]->getColor().mul( p ).add( a[1][n][m]->getColor().mul( 1-p ) );
				}
	f = 1;
	break;
case 1:
	target[0]= {L,0,1};
	target[1]= {R,0,1};
	target[2]= {L,0,6};
	target[3]= {R,0,6};
	break;
case 2:
	target[0]= {L,0,1};
	target[1]= {R,0,1};
	target[2]= {L,0,1};
	target[3]= {R,0,1};
	break;
case 3:
	merged = true;
	target[0]= {O,0,1};
	target[1]= {O,0,1};
	target[2]= {O,0,100};
	target[3]= {O,0,100};
	break;
}
}

int main()
{
	/// Open window
	if( !openWindow3D( "Blended mixing" ) ) return 1;
	changeBrightness();
	changeGround();
	changeGround();
	changeGround();

	/// Add buttons
	sB = new Button2D( "buttons/button_next.png",GLFW_KEY_SPACE,nextStep,0,modes );
	addButton( new exitButton2D );

	viewAlpha = 3*M_PI/2;
	viewZ = 4;

	for( int m=0; m<M; m++ )
		for( int n=0; n<N; n++ )
		{
			a[0][n][m] = new Cube3D( 0.5 );
			a[1][n][m] = new Cube3D( 0.5 );
			a[2][n][m] = new Cube3D( 0.5 );
			a[3][n][m] = new Cube3D( 0.5 );

			vect_t v = {0.5*n,0,0.5*m};

			a[0][n][m]->setCenter( v );
			a[1][n][m]->setCenter( v );
			a[2][n][m]->setCenter( v.add( {0,0,0} ) );
			a[3][n][m]->setCenter( v.sub( {0,0,0} ) );

			a[0][n][m]->setColor( {random( 0,0.1 ),random( 0,0.1 ),random( 0.6,1 ),1} );
			a[1][n][m]->setColor( {random( 0.6,1 ),random( 0,0.1 ),random( 0,0.1 ),1} );
		}

	for( int k=0; k<K; k++ )
	{
		o[k] = new Text3D( L"",systemFont3D );
		o[k]->rotateLocal( 90, {1,0,0} );
	}
	o[0]->setCenter( {-5.5-N/4.0+0.5,0,1-21} );
	o[1]->setCenter( {+5.5-N/4.0+0.5,0,1-21} );
	o[2]->setCenter( {-N/4.0+0.5,0,100} );
	o[3]->setCenter( {+5.5-N/4.0+0.5,0,100} );

	nextStep();

	/// Main loop
	t = 0;
	stepT = t;
	while( isRunning() )
	{
		if( t-stepT<ANIMATION_TIME*( 3-f ) )
		{
			double s = 0.5+0.5*cos( ( ( t-stepT )/ANIMATION_TIME )*M_PI/2 );
			for( int k=0; k<K; k++ )
			{
				vect_t c = o[k]->getCenter().mul( s ).add( target[k].mul( 1-s ) );
				if( target[k].z>20 ) c.z=20;
				o[k]->setCenter( c );
			}

			s = 1-( 1-s )*( 1-s );
			if( mode==2 )
			{
				vect_t c;
				for( int m=0; m<M; m++ )
					for( int n=0; n<N; n++ )
					{
						c = a[0][n][m]->getColor();
						c.w = c.w*( s )+( 1-s )*a[2][n][m]->getColor().w;
						a[0][n][m]->setColor( c );

						c = a[1][n][m]->getColor();
						c.w = c.w*( s )+( 1-s )*a[3][n][m]->getColor().w;
						a[1][n][m]->setColor( c );
					}
			}
			if( mode==3 )
			{
				for( int m=0; m<M; m++ )
					for( int n=0; n<N; n++ )
					{
						a[0][n][m]->setColor(a[0][n][m]->getColor().mul(s).add(cols[n][m].mul(1-s)));
						a[1][n][m]->setColor(a[1][n][m]->getColor().mul(s).add(cols[n][m].mul(1-s)));
					}
			}
		}
		else
		{
			if( ( mode==2 ) && !merged )
			{
				merged = true;
				target[2]= {0,0,100};
				target[3]= {0,0,100};
				o[2]->setCenter( {0,0,100} );
				o[3]->setCenter( {0,0,100} );
			}
			if( ( mode==3 ) && merged )
			{
				merged = false;
				target[2]= {0,0,100};
				target[3]= {0,0,100};
				o[2]->setCenter( {0,0,100} );
				o[3]->setCenter( {0,0,100} );
			}
		}
	}
	return 0;
}

