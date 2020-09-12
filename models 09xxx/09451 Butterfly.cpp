
/// Project:	FMI 3D (2013)
/// File:		09451 Butterfly.cpp
/// Topic:		09 Graphical primitives
/// Slide:		45
///	About:		Butterfly - parametric definition
///
/// History:
///		2013.08.22/PB	Initial implementation
///     2013.09.10/PB   Changed button image


#include "fmi3d.h"

#define ANIMATION_TIME 5
#define N 960
#define M 6
int mode=0, modes=M;
int steps[M] = {1,20,32,60,96,240};
int step = steps[0];
vect_t p[N];
double flip = 1;


void drawScene()
{
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LINE_SMOOTH);
	glDepthMask(GL_FALSE);
	glColor3f(0,0,0);
	glLineWidth(3);
	double al = pow(fabs((90*M_PI/180)*(cos(4*t)+cos(3.1*t)+sin(2.3*t))/3),2);
	al = al*0.95+0.05;
	al = M_PI/2*flip+(1-flip)*al;
	double co = cos(al);
	double si = sin(al);
	glBegin(GL_LINE_LOOP);
	for( int i=0; i<N; i+=step )
	{
		glVertex3f(fabs(p[i].y)*co,p[i].y*si,p[i].z);
	}
	glEnd();
	glDepthMask(GL_TRUE);
}


void drawFrames()
{
}


Button2D *sB;
void nextStep()
{
	mode = (mode+1)%modes;
	sB->setState(mode);
	step = steps[mode];
}

double flipT = 0;
double flipMax = 1;
void toggleFlip()
{
	flipT = t;
	flipMax = 1-flipMax;
}

int main()
{
	useCustomFogColor = true;
	customFogColor[0] = 0.5;
	customFogColor[1] = 0.6;
	customFogColor[2] = 1;
	customFogColor[3] = 1;

	/// Open window
	if( !openWindow3D( "Butterfly" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();

	viewDistance = 10;
	viewZ = 3.5;

	/// Add buttons
	sB = new Button2D( "buttons/button_n.png",GLFW_KEY_SPACE,nextStep,mode,modes );
	addButton( new Button2D( "buttons/button_time.png",'F',toggleFlip ) );
	addButton( new exitButton2D );

	for(int i=0; i<N; i++)
	{
		double a = (2*M_PI*i)/(N-1);
		double r = exp(cos(a)) - 2*cos(4*a) + pow(sin(a/12),5);
		p[i] = {0,r*sin(a),r*cos(a)+3};
	}

	/// Main loop
	double t0=t;
	while( isRunning() )
	{
		viewAlpha  -= ( t-t0 )/15;
		t0=t;

		if( t-flipT<ANIMATION_TIME )
		{
			double k = 0.5+0.5*cos( ( ( t-flipT )/ANIMATION_TIME )*M_PI/2 );
			flip = flip*k+flipMax*( 1-k );
		}
	}
	return 0;
}

