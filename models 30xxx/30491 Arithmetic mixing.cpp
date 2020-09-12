
/// Project:	FMI 3D (2013)
/// File:		30491 Arithmetic mixing.cpp
/// Topic:		30 Films
/// Slide:		49
///	About:		Mixing frames with arithmetic operations
///
/// History:
///		2013.07.15/PB	Initial implementation


#include "fmi3d.h"
//#include <iostream>

#define ANIMATION_TIME 4

#define K 4
#define N 21
#define M 9
Text3D *o[K];
Cube3D *a[K][N][M];

void drawScene()
{
	for( int k=0; k<K; k++ )
	{
		o[k]->drawImage();
		glPushMatrix();
		glMultMatrixd( ( GLdouble* )&o[k]->oxyz );
		glTranslated( 0,0,-0.4 );
		glRotated( -90,1,0,0 );
		for( int m=0; m<M; m++ )
			for( int n=0; n<N; n++ )
				if( a[k][n][m]->getColor().w>0.5 ) a[k][n][m]->drawImage();
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

int modes = 6;
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
	switch( mode )
	{
		case 0:
			target[0]= {L,0,1};
			target[1]= {R,0,1};
			target[2]= {O,0,6};
			target[3]= {R,0,-6};
			for( int k=0; k<K; k++ )
			{
				o[k]->setColor({1,0.9,0,1});
				o[k]->setScale( {2.5,2,4} );

				vect_t c;
				for( int m=0; m<M; m++ )
					for( int n=0; n<N; n++ )
						if( k<2 )
						{
							c = a[k][n][m]->getColor();
							c.w = 1;
							a[k][n][m]->setColor(c);
						}
						else
						{
							double p = ( n-10+5*f*sin(t) )*( n-10+5*f*cos(t) )+( m-4+2*f*cos(t) )*( m-4 )-13+f*random(-20,20);
							if( ( p>=0 ) && ( n!=N/2 ) && ( m!=M/2 ) )
							{
								a[2][n][m]->setColor( {1,1,0,0.1} );
								a[3][n][m]->setColor( {1,1,1,1} );
							}
							else
							{
								a[2][n][m]->setColor( {1,1,1,1} );
								a[3][n][m]->setColor( {1,1,0,0.1} );
							}
						}

			}
			o[0]->setText(L"A");
			o[1]->setText(L"B");
			f = 1;
			break;
		case 1:
			target[0]= {L,4,1};
			target[1]= {R,4,1};
			target[2]= {L,-4,1};
			target[3]= {R,-4,-6};
			break;
		case 2:
			target[0]= {L,4,1};
			target[1]= {R,4,1};
			target[2]= {L,-4,1};
			target[3]= {R,-4,1};
			break;
		case 3:
			target[0]= {L,0,1};
			target[1]= {R,0,1};
			target[2]= {L,0,1};
			target[3]= {R,0,1};
			break;
		case 4:
			target[0]= {O,-2,1};
			target[1]= {O,+2,1};
			target[2]= {L,0,-100};
			target[3]= {R,0,-100};
			break;
		case 5:
			target[0]= {O,0,1};
			target[1]= {O,0,1};
			target[2]= {L,0,-100};
			target[3]= {R,0,-100};
			break;
	}
}

int main()
{
	/// Open window
	if( !openWindow3D( "Arithmetic mixing" ) ) return 1;
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

			a[0][n][m]->setColor( {random( 0,0.1 ),random( 0,0.2 ),random( 0.4,1 ),1} );
			a[1][n][m]->setColor( {random( 0.4,1 ),random( 0,0.2 ),random( 0,0.1 ),1} );

			double p = ( n-10 )*( n-10 )+( m-4 )*( m-4 )-13;
			if( ( p>=0 ) && ( n!=N/2 ) && ( m!=M/2 ) )
			{
				a[2][n][m]->setColor( {1,1,0,0.1} );
				a[3][n][m]->setColor( {1,1,1,1} );
			}
			else
			{
				a[2][n][m]->setColor( {1,1,1,1} );
				a[3][n][m]->setColor( {1,1,0,0.1} );
			}
		}

	for( int k=0; k<K; k++ )
	{
		o[k] = new Text3D( L"A",systemFont3D );
		o[k]->setColor( {1,0.9,0,1} );
		o[k]->setScale( {2.5,2,4} );
		o[k]->rotateLocal( 90, {1,0,0} );
	}
	o[0]->setCenter( {-5.5-N/4.0+0.5,0,1-21} );
	o[1]->setCenter( {+5.5-N/4.0+0.5,0,1-21} );
	o[2]->setCenter( {-N/4.0+0.5,0,6-11} );
	o[3]->setCenter( {+5.5-N/4.0+0.5,0,-6-11} );
	o[1]->setText( L"B" );
	o[2]->setText( L"M" );
	o[3]->setText( L"1-M" );

	nextStep();

	/// Main loop
	stepT = t;
	merged = false;
	while( isRunning() )
	{
		if( t-stepT<ANIMATION_TIME )
		{
			double s = 0.5+0.5*cos( ( ( t-stepT )/ANIMATION_TIME )*M_PI/2 );
			for( int k=0; k<K; k++ )
			{
				o[k]->setCenter( o[k]->getCenter().mul( s ).add( target[k].mul( 1-s ) ) );
			}

			s = 1-(1-s)*(1-s);
			if( mode==3 )
			{
				vect_t c;
				for( int m=0; m<M; m++ )
					for( int n=0; n<N; n++ )
					{
						c = a[0][n][m]->getColor();
						c.w = c.w*(s)+(1-s)*a[2][n][m]->getColor().w;
						a[0][n][m]->setColor(c);

						c = a[1][n][m]->getColor();
						c.w = c.w*(s)+(1-s)*a[3][n][m]->getColor().w;
						a[1][n][m]->setColor(c);
					}

				c = o[2]->getColor();
				c.w = c.w*(s)+(1-s)*0.01;
				o[2]->setColor(c);

				c = o[3]->getColor();
				c.w = c.w*(s)+(1-s)*0.01;
				o[3]->setColor(c);
			}
		}
		else
		{
			if( (mode==3) && !merged )
			{
				merged = true;
				target[2]={0,0,-100};
				target[3]={0,0,-100};
				o[2]->setCenter({0,0,-100});
				o[3]->setCenter({0,0,-100});
				o[0]->setText(L"AM");
				o[1]->setText(L"B(1-M)");
				o[0]->setScale( {2,2,4} );
				o[1]->setScale( {2,2,4} );
			}
			if( mode == 4 ) merged = true;
			if( (mode==5) && merged )
			{
				merged = false;
				o[0]->setText(L"AM+B(1-M)");
				o[1]->setText(L"");
				o[0]->setScale( {1.2,1.2,4} );
				o[1]->setScale( {1.2,1.2,4} );
			}
		}
	}
	return 0;
}

