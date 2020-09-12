
/// Project:	FMI 3D (2013)
/// File:		13511 Grass comples wave.cpp
/// Topic:		13 Physics
/// Slide:		51
///	About:		Grass double wave with more complex dynamics
///
/// History:
///		2013.08.21/PB	Initial implementation


#include "fmi3d.h"

#define ANIMATION_TIME 2
#define N 2000
#define R 15
vect_t p[N];
double len[N];
vect_t xofs[N];
vect_t yofs[N];
double d = 2;
int n = 200;
double nl = 3;
double wind = 1;
int mode=0, modes=3;
int node=0, nodes=3;

Cube3D *c1,*c2;

void drawScene()
{
	if( inShadow ) return;

	for(int i=0; i<4; i++)
	{
		c1->drawImage();
		c1->rotateGlobal(90,{0,0,1});
		c1->oxyz.o.z = 0.3+i%2/5.0;
	}
	c2->drawImage();

	glEnable(GL_FOG);
	glFogf(GL_FOG_START,viewDistance-10);
	glFogf(GL_FOG_END,viewDistance+10);

	glEnable( GL_LINE_SMOOTH );
	glDisable( GL_LIGHTING );
	glDisable( GL_TEXTURE_2D );
	//glDepthMask(GL_FALSE);
	glColor3f(0,1,0);
	glLineWidth( nl );
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 1-n/3000.0);

	glBegin(GL_LINES);
	for( int i=0; i<n; i++ )
	{
		vect_t v1 = p[i];
		for(int j=0; j<15; j++)
		{
			v1.vertex();
			vect_t v2 = {wind*sin(xofs[i].x*(1+j/13.0)),wind*sin(yofs[i].x*(1+j/13.0)),5};
			v2 = v2.mul(len[i]/v2.distance()).add(v1);
			v2.vertex();
			v1=v2;
		}
	}
	glEnd();
	glDisable(GL_ALPHA_TEST);
	glDepthMask(GL_TRUE);
	glDisable(GL_FOG);
//	glDisable(GL_BLEND);
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
	if( !openWindow3D( "Grass comples wave" ) ) return 1;
	changeGround();
	changeBrightness();

	viewZ = 0;
	viewDistance = 45;
	viewBeta = 0.6;

	/// Add buttons
	wB = new Button2D( "buttons/button_time.png",GLFW_KEY_SPACE,toggleWind,mode,modes );
	nB = new Button2D( "buttons/button_n.png",'N',toggleN,node,nodes );
	addButton( new exitButton2D );

	for( int i=0; i<N; i++ )
	{
		p[i]={random(-R,R),random(-R,R)};
		len[i]=random(0.4,0.8);
		xofs[i]={0,20*sin(p[i].x/35-p[i].y/39)};
		yofs[i]={0,20*cos(p[i].x/29+p[i].y/25)};
	}

	c1 = new Cube3D(1);
	c1->setScale({2*R+5,1,1});
	c1->setCenter({0,R+2,1});
	c1->setTexture(loadTexture("textures/material_wood.jpg"));

	c2 = new Cube3D(2*R+4);
	c2->setCenter({0,0,0.2-R-2});
	c2->setTexture(loadTexture("textures/ground_rock.jpg"));
	c2->setTextureSize(2);
	c2->setColor({0.4,0.3,0.3});

	/// Main loop
	double w = 0;
	double t0=0;
	while( isRunning() )
	{
		viewAlpha += (t-t0)/10;
		double dwind = pow(0.95,wind);
		w = w+dwind*(t-t0);
		t0 = t;

		if( t-windT<ANIMATION_TIME )
		{
			double k = 0.5+0.5*cos( ( ( t-windT )/ANIMATION_TIME )*M_PI/2 );
			wind = wind*k+windMax*( 1-k );
		}

		double nt = 1+0.03*cos(t);
		double mt = 1+0.03*sin(1.233*t);

		for( int i=0; i<N; i++ )
		{
			xofs[i].x=0.3*sin(2*w+xofs[i].y*nt/3);
			yofs[i].x=0.3*sin(3*w+yofs[i].y*mt/3);
		}
	}
	return 0;
}

