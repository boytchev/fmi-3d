
/// Project:	FMI 3D (2013)
/// File:		22232 Fractal mountain.cpp
/// Topic:		22 Fractals
/// Slide:		23
///	About:		A random fractal mountain
///
/// History:
///		2013.05.24/PB	Initial implementation


#include "fmi3d.h"

//#include <iostream>
#define S 5

/// Middle point
vect_t mid( vect_t p, vect_t q )
{
	return p.add( q ).mul( 0.5 );
}

double f( double x,double x1,double x2,double y1,double y2 ) { return ( x-x1 )/( x2-x1 )*( y2-y1 )+y1; }


/// Terrain
class Terrain3D : public Object3D
{
	public:
		Terrain3D( int N );
		void generateTerrain();
	private:
		int N;
		void generateTerrain( int x1, int y1, int x2, int y2 );
		vect_t height[257][257];
		void ver( int i, int j );
};

Terrain3D::Terrain3D( int N ) : Object3D()
{
	if( N<2 ) N=2;
	if( N>256 ) N=256;
	this->N = N;
	generateTerrain();
}

void Terrain3D::generateTerrain()
{
	double s=256.0/N/20;

	glDeleteLists( imageList,1 );
	glNewList( imageList,GL_COMPILE );
	{
		glScaled( s,s,s );

		height[0][0]= {-N/2.0,-N/2.0,random( -N/20,N/10 )};
		height[N][0]= {+N/2.0,-N/2.0,random( -N/20,N/10 )};
		height[0][N]= {-N/2.0,+N/2.0,random( -N/20,N/10 )};
		height[N][N]= {+N/2.0,+N/2.0,random( -N/20,N/10 )};
		generateTerrain( 0,0,N,N );

		for( int i=0+S; i<=N-S; i++ )
			for( int j=0+S; j<=N-S; j++ )
			{
				height[i][j].z *= 2;
				if( height[i][j].z<-5 ) height[i][j].z = (height[i][j].z+5)/2-5;
			}
		for( int i=0; i<=N; i++ )
			{
				height[i][S].y = S-N/2;
				height[i][N-S].y = N/2-S;
				height[S][i].x = S-N/2;
				height[N-S][i].x = N/2-S;
			}

		/// Terrain
		for( int y=0+S; y<N-S; y++ )
		{
			glBegin( GL_QUAD_STRIP );
			for( int x=0+S; x<=N-S; x++ )
			{
				ver( x,y );
				ver( x,y+1 );
			}
			glEnd();
		}

		glDisable(GL_LIGHTING);
		/// Wall 1
		glBegin( GL_QUAD_STRIP );
		for( int y=0+S; y<=N-S; y++ )
		{
			glColor3f(0,0,0.2);
			glVertex3d( height[0+S][y].x,height[0+S][y].y,height[0+S][y].z );
			glColor3f(0,0,0);
			glVertex3d( 0+S-N/2,y-N/2,-3/s );
		}
		glEnd();

		/// Wall 2
		glBegin( GL_QUAD_STRIP );
		for( int y=0+S; y<=N-S; y++ )
		{
			glColor3f(0,0,0.2);
			glVertex3d( height[N-S][y].x,height[N-S][y].y,height[N-S][y].z );
			glColor3f(0,0,0);
			glVertex3d( N-S-N/2,y-N/2,-3/s );
		}
		glEnd();

		/// Wall 3
		glBegin( GL_QUAD_STRIP );
		for( int x=0+S; x<=N-S; x++ )
		{
			glColor3f(0,0,0.2);
			glVertex3d( height[x][0+S].x,height[x][0+S].y,height[x][0+S].z );
			glColor3f(0,0,0);
			glVertex3d( x-N/2,S-N/2,-3/s );
		}
		glEnd();

		/// Wall 4
		glBegin( GL_QUAD_STRIP );
		for( int x=0+S; x<=N-S; x++ )
		{
			glColor3f(0,0,0.2);
			glVertex3d( height[x][N-S].x,height[x][N-S].y,height[x][N-S].z );
			glColor3f(0,0,0);
			glVertex3d( x-N/2,N-S-N/2,-3/s );
		}
		glEnd();
	}
	glEndList();
}

void Terrain3D::generateTerrain( int x1, int y1, int x2, int y2 )
{
	if( x1+1==x2 ) return;

	int xm=( x1+x2 )/2;
	int ym=( y1+y2 )/2;

	height[xm][y1]=height[x1][y1].add( height[x2][y1] ).mul( 0.5 );
	height[x1][ym]=height[x1][y1].add( height[x1][y2] ).mul( 0.5 );
	height[x2][ym]=height[x2][y1].add( height[x2][y2] ).mul( 0.5 );
	height[xm][y2]=height[x1][y2].add( height[x2][y2] ).mul( 0.5 );

	height[xm][ym]=height[xm][y1].add( height[xm][y2] ).add( height[x1][ym] ).add( height[x2][ym] ).mul( 0.25 );

	double m=x2-x1;
	height[xm][ym].x += random( m/8,m/5 )*1.1*( irandom( 0,1 )-0.5 );
	height[xm][ym].y += random( m/8,m/5 )*1.1*( irandom( 0,1 )-0.5 );
	height[xm][ym].z += random( m/8,m/5 )*1.5*( irandom( 0,1 )-0.5 );

	generateTerrain( x1,y1,xm,ym );
	generateTerrain( xm,y1,x2,ym );
	generateTerrain( x1,ym,xm,y2 );
	generateTerrain( xm,ym,x2,y2 );
}

void Terrain3D::ver( int i, int j )
{
	vect_t n= {0,0,1};
	if( ( i>0 )&&( i<N )&&( j>0 )&&( j<N ) )
	{
		vect_t vx = height[i-1][j-1].sub( height[i+1][j+1] );
		vect_t vy = height[i+1][j-1].sub( height[i-1][j+1] );
		n = vx.vmul( vy );
	}

	double K = -10;
	double B = -5;
	double Y = 0;
	double G = 5;
	double R = 25;
	double W = 55;

	double z = height[i][j].z;

	if( z<K ) glColor3f(0,0,0.4);
	if( (K<=z) && (z<B) ) glColor3f( 0,0,f( z,K,B,0.4,1 ) );
	if( (B<=z) && (z<Y) ) glColor3f( f( z,B,Y,0,0.7 ),f( z,B,Y,0,0.7 ),f( z,B,Y,1,0 ) );
	if( (Y<=z) && (z<G) ) glColor3f( f( z,Y,G,0.7,0 ),f( z,Y,G,0.7,0.5 ),f( z,Y,G,0,0 ) );
	if( (G<=z) && (z<R) ) glColor3f( f( z,G,R,0,0.85 ),f( z,G,R,0.5,0.5 ),f( z,G,R,0,0 ) );
	if( (R<=z) && (z<W) ) glColor3f( f( z,R,W,0.85,1 ),f( z,R,W,0.5,1 ),f( z,R,W,0,1 ) );
	if( z>=W ) glColor3f(1,1,1);

	glTexCoord3d( height[i][j].x/500,height[i][j].y/500,1 );
	glNormal3d( n.x,n.y,n.z );
	double x=height[i][j].x;
	double y=height[i][j].y;
	glVertex3d( x,y,height[i][j].z );
}


Terrain3D* terrain;
Cube3D* water;

void drawScene()
{
	terrain->drawImage();
	water->drawImage();
}


void drawFrames()
{
//	water->drawFrame();
}


void randomTerrain()
{
	terrain->generateTerrain();
}


int main()
{
	/// Open window
	if( !openWindow3D( "Fractal mountain" ) ) return 1;
	//noGround();
	changeGround();
	changeGround();
	changeGround();
	changeBrightness();

	addButton( new Button2D( "buttons/button_random.png",'R',randomTerrain ) );
	addButton( new exitButton2D );

	water = new Cube3D( 1 );
	water->setScale({12.8-S/10.0-0.05,12.8-S/10.0-0.05,6});
	water->setColor({0,0.25,0.5,0.5});

	terrain = new Terrain3D( 256 );
	terrain->setCenter({0,0,3});

	/// Main loop with automatic animation
	double t0=t;
	while( isRunning() )
	{
		viewAlpha += (t-t0)/10;
		t0=t;
	}

	return 0;
}
