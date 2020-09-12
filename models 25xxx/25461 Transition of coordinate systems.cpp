
/// Project:	FMI 3D (2013)
/// File:		25461 Transition of coordinate systems.cpp
/// Topic:		25 Skeletons
/// Slide:		46
///	About:		Transition of coordinate systems
///
/// History:
///		2013.07.13/PB	Initial implementation
///     2013.09.13/PB   Fixed the colours, reduced speed


#include "fmi3d.h"

#define N 5
CoordSys3D *cs1,*cs2;
Sphere3D *s[2*N];

void drawScene()
{
	glPushMatrix();
	glTranslated( 0,-6,1 );
	cs1->drawImage();
	cs2->drawImage();
	for( int i=0; i<2*N; i++ ) s[i]->drawImage();
	glPopMatrix();
}


void drawFrames()
{
	glPushMatrix();
	glTranslated( 0,-6,1 );
	cs1->drawFrame();
	cs2->drawFrame();
	glPopMatrix();
}


int main()
{
	/// Open window
	if( !openWindow3D( "Transition of coordinate systems" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();
	changeGround();

	/// Add buttons
	addButton( new exitButton2D );

	viewZ = 4;

	cs1 = new CoordSys3D( -3,3,-3,3,-0.2,2,true,3,0.35,L"XYZ" );
	cs2 = new CoordSys3D( -3,3,-3,3,-0.2,2,true,3,0.35,L"UVW" );
	cs2->setAxesNamesColor( {1,0,0} );

	for( int i=0; i<2*N; i++ )
	{
		s[i] = new Sphere3D( 0.4 );
		if( i<N )
		{
			s[i]->setColor( {0.5,0.5,1} );
			//s[i]->setShininess( 20 );
		}
		else
		{
			s[i]->setColor( {1,0,0} );
			//s[i]->setShininess( 10 );
		}
		if( (i%N)==0 )
		{
			s[i]->setColor({1,0.7,0});
			//s[i]->setShininess( 10 );
		}
	}

	/// Main loop
	while( isRunning() )
	{
		double q = t/2;
		cs1->identity();
		cs1->setCenter( {0,0,0} );

		cs2->oxyz = cs1->oxyz;
		cs2->translateLocal( {1*cos( 1.3*q ),1*sin( 1*q )+12,1*sin( 1.5*q )+5} );
		cs2->rotateLocal( 21*q, {1,0,-1} );
		cs2->rotateLocal( 13*q, {1,1,0} );

		for( int i=0; i<N; i++ )
		{
			double a = double( i )/N*2*M_PI;
			s[i]->setCenter( {2*sin( a ),2*cos( a ),0.2+fabs( ( 4-i/4.0 )*sin( 2.5*t+i ) )} );

			s[i+N]->oxyz = s[i]->oxyz.multiply( cs2->oxyz );
		}
	}
	return 0;
}

