
/// Project:	FMI 3D (2013)
/// File:		09331 Earsticks.cpp
/// Topic:		09 Graphical primitives
/// Slide:		33
///	About:		Earsticks - demonstration of Elica object size
///
/// History:
///		2013.08.22/PB	Initial implementation
///     2013.09.11/PB   Simplified object complexity


#include "fmi3d.h"

#define N 80
Sphere3D *s[2*N];
Cylinder3D *c[N];


void randomize()
{
	for(int i=0; i<N; i++)
	{
		vect_t v1 = {random(-10,10),random(-10,10),random(2,10)};
		vect_t v2 = {random(-10,10),random(-10,10),random(2,10)};
		vect_t v = v2.sub(v1);
		v = v.mul(6/v.distance());
		v2 = v1.add(v);

		oxyz_t oxyz;
		oxyz.oz = v;
		oxyz.rebuild();
		oxyz.o = v1;
		oxyz.o.w = 1;

		s[2*i+0]->oxyz = oxyz;
		c[i]->oxyz = oxyz;
		oxyz.o = v2;
		oxyz.o.w = 1;
		s[2*i+1]->oxyz = oxyz;
	}
}


void drawScene()
{
	for(int i=0; i<N; i++)
	{
		s[2*i+0]->drawImage();
		s[2*i+1]->drawImage();
		c[i]->drawImage();
	}
}


void drawFrames()
{
}


int main()
{
	useCustomFogColor = true;
	customFogColor[0] = 0.60;
	customFogColor[1] = 0.50;
	customFogColor[2] = 0.40;
	customFogColor[3] = 1;

	/// Open window
	if( !openWindow3D( "Earsticks" ) ) return 1;
	changeGround();

	/// Add buttons
	addButton( new Button2D( "buttons/button_random.png",'R',randomize) );
	addButton( new exitButton2D );

	viewZ = 4;
	viewDistance = 25;
	viewBeta = 0.2;

	for(int i=0; i<N; i++)
	{
		s[2*i+0] = new Sphere3D(0.4,QUALITY_LOW);
		s[2*i+0]->setScale({0.3,0.3,0.6});
		s[2*i+0]->setColor({0.8,0.8,0.8});
		s[2*i+1] = new Sphere3D(0.4,QUALITY_LOW);
		s[2*i+1]->setScale({0.3,0.3,0.6});
		s[2*i+1]->setColor({0.8,0.8,0.8});
		c[i] = new Cylinder3D(0.12,6,false,QUALITY_LOWER);
		c[i]->setColor({random(0.5,0.8),random(0.5,0.8),random(0.5,0.8)});
	}
	randomize();

	/// Main loop
	double t0=t;
	while( isRunning() )
	{
		viewAlpha  += ( t-t0 )/15;
		t0=t;
	}
	return 0;
}

