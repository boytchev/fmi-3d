
/// Project:	FMI 3D (2013)
/// File:		17271 Painter's algorithm.cpp
/// Topic:		17 Cutting
/// Slide:		27
///	About:		Painter's algorithm
///
/// History:
///		2013.08.09/PB	Initial implementation


#include "fmi3d.h"

#define ANIMATION_TIME 1

#define N 6
#define S 5
vect_t p[N][S];
vect_t col[N];
int n[N];
Vector3D *v[S];
double z[N],z0[N];

double fallT;
int fallN;

void drawScene()
{
	glEnable( GL_DEPTH_TEST );

	glDisable( GL_LIGHTING );
	glDisable( GL_TEXTURE_2D );
	glPolygonMode( GL_FRONT_AND_BACK,GL_FILL );
	glEnable( GL_POLYGON_OFFSET_FILL );
	glPolygonOffset( 1,1 );
	for( int i=0; i<N; i++ )
	{
		if( inShadow )
			glColor4f( 0,0,0,1 );
		else
			glColor4f( col[i].x,col[i].y,col[i].z,1 );
		glBegin( GL_POLYGON );
		for( int j=0; j<n[i]; j++ )
			glVertex3f(p[i][j].x,p[i][j].y,(i<fallN)?z0[i]:p[i][j].z);
		glEnd();
	}
	glDisable( GL_POLYGON_OFFSET_FILL );
	glEnable( GL_LINE_SMOOTH );
	glLineWidth( 1.5 );
	glPolygonMode( GL_FRONT_AND_BACK,GL_LINE );
	for( int i=0; i<N; i++ )
	{
		glColor4f( 0,0,0,1 );
		glBegin( GL_POLYGON );
		for( int j=0; j<n[i]; j++ )
			glVertex3f(p[i][j].x,p[i][j].y,(i<fallN)?z0[i]:p[i][j].z);
		glEnd();
	}

	for( int i=0; i<S; i++ ) v[i]->drawImage();
}


void drawFrames()
{
}

Button2D *nB;
void randomize()
{
	fallT = t-1000;
	fallN = -1;
	nB->setState(fallN);
	for( int i=0; i<N; i++ )
	{
		n[i] = irandom( 3,S );
		col[i] = {random( 0,1 ),random( 0,1 ),random( 0,1 )};
		z0[i] = (0.1+i/10.0);
		double x = random( -5,5 );
		double y = random( -5,5 );
		z[i] = random( 3,10 );
		for( int j=0; j<n[i]; j++ )
		{
			double a = double( j )/n[i]*M_PI*2;
			double r = random( 2,4 );
			p[i][j]= {x+r*cos( a ),y+r*sin( a ),z[i]};
		}
	}
	for( int i=0; i<S; i++ )
	{
		v[i]->setCenter( {0,0,-10} );
	}
}

void fall()
{
	if( fallN<N-1 )
	{
		fallN++;
		fallT=t;
		nB->setState(fallN);
	}
}


int main()
{
	/// Open window
	if( !openWindow3D( "Painter's algorithm" ) ) return 1;
//	changeGround();
//	changeBrightness();

	/// Add buttons
	nB = new Button2D( "buttons/button_next.png",GLFW_KEY_SPACE,fall,0,N );
	addButton( new Button2D( "buttons/button_random.png",'R',randomize));
	addButton( new exitButton2D );

	viewZ = 4;

	for( int i=0; i<S; i++ )
	{
		v[i] = new Vector3D( {0,0,-1},0.3 );
		v[i]->setCenter( {0,0,-10} );
		v[i]->setColor( {0,0,0} );
	}

	randomize();

	/// Main loop
	double t0=t;
	double kk = 0.01;
	while( isRunning() )
	{
		viewAlpha -= (t-t0)/10;
		t0 = t;

		if( t-fallT<ANIMATION_TIME )
		{
			double k=1+sin( ( ( t-fallT )/ANIMATION_TIME-1 )*M_PI/2 );
			for( int i=0; i<S; i++ )
			{
				if( i<n[fallN] )
				{
					v[i]->setCenter( p[fallN][i] );
					v[i]->setLength( p[fallN][0].z*k-0.3 );
					if( p[fallN][0].z*k-0.3<0.3 ) v[i]->setCenter( {0,0,-10} );
				}
				else
					v[i]->setCenter( {0,0,-10} );
			}
		}
		else if( t-fallT<2*ANIMATION_TIME )
		{
			double k=1+sin( ( ( t-fallT-ANIMATION_TIME )/ANIMATION_TIME-1 )*M_PI/2 );
			if( fallN==N-1 ) viewBeta = viewBeta*(1-kk)+(M_PI/2)*(kk);
			for( int i=0; i<S; i++ )
			{
				if( i<n[fallN] )
				{
					p[fallN][i].z = (z[fallN]-z0[fallN])*( 1-k )+z0[fallN];
					if( p[fallN][i].z<0.5 )
					{
						v[i]->setLength( 0 );
						v[i]->setCenter( {0,0,-10} );
					}
					else
					{
						v[i]->setLength( p[fallN][0].z-0.3 );
						v[i]->setCenter( p[fallN][i] );
					}
				}
			}
		}
		else
		{
			if( fallN==N-1 ) viewBeta = viewBeta*(1-kk)+(M_PI/2)*(kk);
			if( fallN>=0 )
			for( int i=0; i<S; i++ )
			{
				v[i]->setCenter( {0,0,-10} );
				p[fallN][i].z = z0[fallN];
			}
		}
		if( fallN<2 ) viewBeta = viewBeta*(1-kk)+0.3*(kk);
	}
	return 0;
}

