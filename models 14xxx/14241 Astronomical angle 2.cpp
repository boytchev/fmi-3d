
/// Project:	FMI 3D (2013)
/// File:		14241 Astronomical angle 2.cpp
/// Topic:		14 Orientation
/// Slide:		24
///	About:		Astronomical angle 2
///
/// History:
///		2013.08.10/PB	Initial implementation


#include "fmi3d.h"
#include "14000 Mecho.h"

Rail3D *r1,*r2;
Holder3D *h,*h1;


void drawScene()
{
	h->drawImage();
	h1->drawImage();
	r1->drawImage();
	r2->drawImage();
}


void drawFrames()
{
}


int main()
{
	/// Open window
	if( !openWindow3D( "Astronomical angle 2" ) ) return 1;
	changeBrightness();
	changeGround();
	changeGround();

	/// Add buttons
	addButton( new exitButton2D );

	wood = loadTexture( "textures/Material_Wood.jpg" );
	viewZ = 5;

	r1 = new Rail3D( 6 );
	r1->setCenter( {0,0,4} );
	r2 = new Rail3D( 6 );
	r2->setCenter( {0,0,2.25} );
	r2->rotateLocal(90,{0,1,0});
	h = new Holder3D( 6 );
	h1 = new Holder3D( 4 );

	/// Main loop
	t = 0;
	double t0=t;
	double ofsR;
	while( isRunning() )
	{
		double dt = t-t0; t0=t;
		viewAlpha+=dt/12;

		ofsR = 300*sin(t*0.5);

		r1->clearOrientation();
		r1->rotateGlobal( ofsR,{0,0,1} );
		r1->rotateLocal(-30,{0,1,0});

		h1->identity();
		h1->rotateGlobal( ofsR,{0,0,1} );
		h1->rotateLocal(90,{1,0,0});
		h1->translateLocal({0,4,0});
		h1->rotateLocal(30,{0,0,1});
		h1->translateLocal({6,0,0});
	}
	return 0;
}

