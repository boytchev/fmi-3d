
/// Project:	FMI 3D (2013)
/// File:		17071 Vertical cutting.cpp
/// Topic:		17 Cutting
/// Slide:		07
///	About:		Vertical cutting with plane
///
/// History:
///		2013.08.08/PB	Initial implementation


#include "fmi3d.h"

#define N 15
Sphere3D *s[N];
vect_t r[N];
vect_t col[N];
GLdouble cp[4] = {1,0,0,0};

void drawScene()
{
	glEnable(GL_CLIP_PLANE2);
	glClipPlane(GL_CLIP_PLANE2,cp);
	for( int i=0; i<N; i++ )
		s[i]->drawImage();
	glDisable(GL_CLIP_PLANE2);

	glDepthFunc(GL_FALSE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	for( int i=0; i<N; i++ )
	{
		col[i].w = 0.25;
		s[i]->setColor(col[i]);
		s[i]->drawImage();
		col[i].w = 1;
		s[i]->setColor(col[i]);
	}
	glDisable(GL_CULL_FACE);
	glDepthFunc(GL_TRUE);

}


void drawFrames()
{
}


void togglePlane()
{
	cp[0] = -cp[0];
}

int main()
{
	/// Open window
	if( !openWindow3D( "Vertical cutting" ) ) return 1;

	/// Add buttons
	addButton( new Button2D("buttons/button_toggle.png",GLFW_KEY_SPACE,togglePlane));
	addButton( new exitButton2D );

	viewAlpha = M_PI/2;
	viewDistance = 15;
	viewZ = 3;

	for( int i=0; i<N; i++ )
	{
		s[i] = new Sphere3D( 1.5 );
		col[i] = {random(0,1),random(0,1),random(0,1)};
		r[i] = {random(1,2),random(0,2*M_PI),random(1,2),random(0,2*M_PI)};
	}

	/// Main loop
	while( isRunning() )
	{
		double q = t/15;
		for( int i=0; i<N; i++ )
		{
			double x = 4*sin(q*r[i].x+r[i].y);
			double y = 3*cos(q*r[i].z+r[i].w);
			double z = 4+2*cos(q*r[i].x+r[i].w+i);
			s[i]->setCenter({x,y,z});
		}
	}
	return 0;
}

