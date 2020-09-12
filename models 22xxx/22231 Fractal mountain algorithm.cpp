
/// Project:	FMI 3D (2013)
/// File:		22231 Fractal mountain algorithm.cpp
/// Topic:		22 Fractals
/// Slide:		23
///	About:		A random fractal mountain - algorithm's steps
///
/// History:
///		2013.05.24/PB	Initial implementation
///     2013.09.13/PB   Changed button image


#include "fmi3d.h"

//#include <iostream>

/// Middle point
vect_t mid( vect_t p, vect_t q )
{
	return p.add( q ).mul( 0.5 );
}


/// Terrain
class Terrain3D : public Object3D
{
	public:
		Terrain3D( int N );
		void generateTerrain(int N);
	private:
		int N;
		void generateTerrain( int x1, int y1, int x2, int y2 );
		vect_t height[257][257];
		void ver( int i, int j );
};

Terrain3D::Terrain3D( int N ) : Object3D()
{
	if( N<1 ) N=1;
	if( N>256 ) N=256;
	setColor({1,0,0});
	generateTerrain(N);
}

void Terrain3D::generateTerrain(int N)
{
	double s=256.0/N/20;
	this->N = N;

	glDeleteLists( frameList,1 );
	glNewList( frameList,GL_COMPILE );
	{
		glColor3f(1,0,0);
		glScaled( s,s,s );
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glDepthMask(GL_FALSE);

		double lw = 5/sqrt(N);
		glLineWidth(lw);

		height[0][0]= {-N/2.0,-N/2.0,0};
		height[N][0]= {+N/2.0,-N/2.0,0};
		height[0][N]= {-N/2.0,+N/2.0,0};
		height[N][N]= {+N/2.0,+N/2.0,0};
		generateTerrain( 0,0,N,N );

		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		glDisable(GL_LIGHTING);
		glEnable(GL_LINE_SMOOTH);

		for( int i=0; i<=N; i++ )
			for( int j=0; j<=N; j++ )
			{
				height[i][j].z *= 2;
				if( height[i][j].z<-5 ) height[i][j].z = (height[i][j].z+5)/2-5;
			}
		for( int i=0; i<=N; i++ )
			{
				height[i][0].y = -N/2.0;
				height[i][N].y = N/2.0;
				height[0][i].x = -N/2.0;
				height[N][i].x = N/2.0;
			}

		/// Terrain
		for( int y=0; y<N; y++ )
		{
			glBegin( GL_QUAD_STRIP );
			for( int x=0; x<=N; x++ )
			{
				height[x][y].vertex();
				height[x][y+1].vertex();
			}
			glEnd();
		}
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

Terrain3D* terrain;


void drawScene()
{
	glPushAttrib(GL_FOG_BIT);
	glEnable(GL_FOG);
	glFogi(GL_FOG_START,viewDistance-7);
	glFogi(GL_FOG_END,viewDistance+7);
	GLfloat fc[4]={0,0,0,1};
	glFogfv(GL_FOG_COLOR,fc);
	terrain->drawFrame();
	glPopAttrib();
}


void drawFrames()
{
	drawScene();
}


int N = 1;
int K = 0;

void randomTerrain()
{
	terrain->generateTerrain(N);
}

Button2D *cS;
void changeSteps()
{
	N = 2*N;
	K = K+1;
	if( N>128 ) {N=1; K=0;}
	terrain->generateTerrain(N);
	cS->setState(K);
}

int main()
{
	/// Open window
	if( !openWindow3D( "Fractal mountain algorithm" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();

	addButton( new Button2D( "buttons/button_random.png",'R',randomTerrain ) );
	cS = new Button2D( "buttons/button_n.png",GLFW_KEY_SPACE,changeSteps,K,8 );
	addButton( new exitButton2D );

	terrain = new Terrain3D( N );
	terrain->setCenter({0,0,5});
	viewZ = 3;

	/// Main loop with automatic animation
	double t0=t;
	while( isRunning() )
	{
		viewAlpha += (t-t0)/10;
		t0=t;
	}

	return 0;
}
