
/// Project:	FMI 3D (2013)
/// File:		28271 Simple shadow.cpp
/// Topic:		28 Light
/// Slide:		27
///	About:		Simple shadow of spheres
///
/// History:
///		2013.07.11/PB	Initial implementation


#include "fmi3d.h"

#define N 30
Sphere3D* ball[N];
Circle2D* shadow[N];
vect_t v[N];
double p[N];
double s[N];

void drawScene()
{
	for( int i=0; i<N; i++ )
	{
		ball[i]->drawImage();
		shadow[i]->drawImage();
	}
}


void drawFrames()
{
}


int main()
{
	/// Open window
	if( !openWindow3D( "Simple shadow" ) ) return 1;
	changeBrightness();
	changeGround();

	viewZ = 3;

	/// Add buttons
	addButton( new exitButton2D );

	for(int i=0; i<N; i++)
	{
		ball[i] = new Sphere3D(0.7);
		ball[i]->setColor({1,0.6,0});
		ball[i]->setShininess(50);
		double alpha = double(i)/N*2*M_PI;
		double r = random(0.07,0.15);
		v[i] = {r*cos(alpha),r*sin(alpha),0,0};
		ball[i]->setCenter({cos(alpha),sin(alpha),0});
		p[i] = random(0,2*M_PI);
		s[i] = random(1.5,2.5);

		shadow[i] = new Circle2D(0.7);
		shadow[i]->setColor({0,0,0,0.5});
		shadow[i]->setLighting(false);
		shadow[i]->setCenter({ball[i]->getCenter().x,ball[i]->getCenter().y,0.1});
	}

	/// Main loop
	while( isRunning() )
	{
		for(int i=0; i<N; i++)
		{
			vect_t w = ball[i]->getCenter();
			w = w.add(v[i]);
			if( fabs(w.x)>14-0.5 ) v[i].x*=-1;
			if( fabs(w.y)>14-0.5 ) v[i].y*=-1;
			w.z = 0.71+fabs(7*sin(s[i]*t));
			ball[i]->setCenter(w);
			shadow[i]->setCenter({w.x,w.y,0.1});
		}
	}
	return 0;
}

