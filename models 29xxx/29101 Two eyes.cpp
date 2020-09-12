
/// Project:	FMI 3D (2013)
/// File:		29101 Two eyes.cpp
/// Topic:		29 Stereo graphics
/// Slide:		10
///	About:		Two eyes focusing on the mouse cursor
///
/// History:
///		2013.07.19/PB	Initial implementation


#include "fmi3d.h"

#define ANIMATION_TIME 1
#define N 2
Sphere3D *s[N];

void drawScene()
{
	GLfloat fc[4]={0,0,0,1};
	glEnable(GL_FOG);
	glFogfv(GL_FOG_COLOR,&fc[0]);
	glFogf( GL_FOG_START,viewDistance-2 );
	glFogf( GL_FOG_END,viewDistance );
	for(int i=0; i<N; i++) s[i]->drawImage();
}


void drawFrames()
{
}


double nearT;
double nearZ = 22;
double Z = 22;
void nearFar()
{
	nearT = t;
	nearZ = 30-nearZ;
}


int main()
{
	/// Open window
	if( !openWindow3D( "Two eyes" ) ) return 1;
	changeBrightness();
	noGround();

	/// Add buttons
	addButton( new Button2D( "buttons/button_show.png",GLFW_KEY_SPACE,nearFar ));
	addButton( new exitButton2D );

	GLuint tex = loadTexture("29101 Texture.jpg");
	for(int i=0; i<N; i++)
	{
		s[i] = new Sphere3D(2,QUALITY_HIGH);
		s[i]->setTexture(tex);
		s[i]->setTextureScale({1,0.98});
		s[i]->setCenter({-4,0,0});
	}
	s[1]->setCenter({+4,0,0});

	/// Main loop
	while( isRunning() )
	{
		if( t-nearT<ANIMATION_TIME )
		{
			double k = cos( ( ( t-nearT )/ANIMATION_TIME )*M_PI/2 );
			Z = Z*k+nearZ*( 1-k );
		}

		int x,y,wx,wy;
		glfwGetMousePos(&x,&y);
		glfwGetWindowSize(&wx,&wy);

		vect_t fly = {-x+wx/2.0,wy/2.0-y,Z*50};
		fly = fly.mul(1/50.0);

		for(int i=0; i<N; i++)
		{
			vect_t d = fly.sub(s[i]->oxyz.o);
			s[i]->clearOrientation();
			s[i]->rotateLocal(+atan2(d.y,d.z)*180/M_PI,{1,0,0});
			s[i]->rotateLocal(+atan2(d.x,d.z)*180/M_PI,{0,1,0});
		}

		viewX = 0;
		viewY = 0;
		viewZ = 0;
		viewAlpha = M_PI/2;
		viewBeta = M_PI/2;
	}
	return 0;
}

