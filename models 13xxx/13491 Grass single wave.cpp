
/// Project:	FMI 3D (2013)
/// File:		13491 Grass single wave.cpp
/// Topic:		13 Physics
/// Slide:		49
///	About:		Grass single wave
///
/// History:
///		2013.08.21/PB	Initial implementation
///     2013.09.13/PB   Changed buttons images


#include "fmi3d.h"

#define ANIMATION_TIME 2
#define N 2000
#define R 15
vect_t p[N];
double len[N];
vect_t xofs[N];
double d = 2;
int n = 200;
double nl = 3;
double wind = 1;
int mode=0, modes=3;
int node=0, nodes=3;

void drawScene()
{
	glEnable( GL_LINE_SMOOTH );
	glDisable( GL_LIGHTING );
	glDisable( GL_TEXTURE_2D );
	glDepthMask(GL_FALSE);
	glColor3f(0,0,0);
	glLineWidth( nl );
	glBegin(GL_LINES);
	for( int i=0; i<n; i++ )
	{
		vect_t v1 = p[i];
		for(int j=0; j<15; j++)
		{
			v1.vertex();
			vect_t v2 = {wind*sin(xofs[i].x*(1+j/3.0)),0,5};
			v2 = v2.mul(len[i]/v2.distance()).add(v1);
			v2.vertex();
			v1=v2;
		}
	}
	glEnd();
	glDepthMask(GL_TRUE);
}


void drawFrames()
{
}


double windT = 0;
double windMax = 1;
Button2D *wB;
void toggleWind()
{
	mode = (mode+1)%modes;
	windT = t;
	windMax = exp(mode);
	wB->setState(mode);
}

Button2D *nB;
void toggleN()
{
	node = (node+1)%nodes;
	n = int(200*pow(3.1,node));
	nl = 3-node;
	nB->setState(node);
}


int main()
{
	/// Open window
	if( !openWindow3D( "Grass single wave" ) ) return 1;

	viewZ = 1;

	/// Add buttons
	wB = new Button2D( "buttons/button_time.png",GLFW_KEY_SPACE,toggleWind,mode,modes );
	nB = new Button2D( "buttons/button_n.png",'N',toggleN,node,nodes );
	addButton( new exitButton2D );

	for( int i=0; i<N; i++ )
	{
		double a = random(0,2*M_PI);
		double r = R*pow(random(0,1),0.65);
		p[i]={r*cos(a),r*sin(a),0};
		len[i]=random(0.2,0.4);
		xofs[i]={0,p[i].x/2};
	}

	/// Main loop
	double w = 0;
	double t0=0;
	while( isRunning() )
	{
		double dwind = pow(0.95,wind);
		w = w+dwind*(t-t0);
		t0 = t;

		if( t-windT<ANIMATION_TIME )
		{
			double k = 0.5+0.5*cos( ( ( t-windT )/ANIMATION_TIME )*M_PI/2 );
			wind = wind*k+windMax*( 1-k );
		}

		for( int i=0; i<N; i++ )
		{
			xofs[i].x=0.3*sin(2*w+xofs[i].y);
		}
	}
	return 0;
}

