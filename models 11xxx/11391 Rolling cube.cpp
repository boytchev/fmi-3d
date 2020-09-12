
/// Project:	FMI 3D (2013)
/// File:		11391 Rolling cube
/// Topic:		11 Animation
/// Slide:		39
///	About:		Rolling cube
///
/// History:
///		2013.08.12/JB	Initial implementation
///		2013.08.15/PB	Finetuning
///		2013.09.09/PB	Fixed speed of animation


#include "fmi3d.h"

#define N 5
Cube3D* cube;
Cube3D* fl;
Cylinder3D *cy[N];


void drawScene()
{
	cube->drawImage();
	fl->drawImage();
	for( int i=0; i<N; i++ )
	{
		cy[i]->drawImage();
	}
}


void drawFrames()
{
	cube->drawFrame();
}

void spinCube( double alpha )
{
	cube->translateLocal( {0,-1,-1} );
	cube->rotateLocal( alpha, {-1,0,0} );
	cube->translateLocal( {0,+1,+1} );
}

int main()
{
	if( !openWindow3D( "Rolling cube" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();
	changeBrightness();

	addButton( new exitButton2D() );

	cube = new Cube3D( 2 );
	cube->setCenter( {0,-10,20} );
	cube->setColor( {1,1,0} );

	GLuint tex = loadTexture( "textures/Material_Wood.jpg" );

	fl = new Cube3D( 0 );
	fl->setScale( {3,30.5,0.2} );
	fl->setCenter( {0,0,3.9} );
	fl->setTexture( tex );
	fl->setTextureScale( {1,10} );

	for( int i=0; i<N; i++ )
	{
		cy[i] = new Cylinder3D( 0.5,4,false,QUALITY_LOW );
		cy[i]->setCenter( {0,-14+28.0*i/( N-1.0 ),-0.1} );
		cy[i]->setTexture( tex );
		cy[i]->setTextureScale( {1,10,1,2} );
	}

	double speed = 0;
	double k = 0.95;

	isRunning();
	double t0 = t;
	double angle = 0;
	spinCube( -90 );
	int phase=0; // 0=fall, 1=roll, 2=drop
	double amp = 0;
	while( isRunning() )
	{
		double dt = 130*( t-t0 );
		t0 = t;

		viewAlpha += dt/900;

		if( phase==0 )
		{
			cube->oxyz.o.z -= speed;
			//speed *= 1.05;
			speed += dt/500;
			if( cube->getCenter().z < 5 )
			{
				cube->oxyz.o.z = 5;
				phase = 1;
			};
			cube->clearOrientation();
			cube->rotateLocal(-(cube->oxyz.o.z-5)*10-90,{-1,0,0});
		}
		else if( phase == 1 )
		{
			if( angle+dt<=90 )
			{
				angle = angle+dt;
				spinCube( dt );
			}
			else
			{
				spinCube( -angle );
				angle = 0;
				cube->translateLocal( {0,0,-2} );
				amp = 2;
			}
			if( cube->getCenter().y>16 )
			{
				phase = 2;
				speed = 0;
			}
		}
		else if( phase == 2 )
		{
			angle = angle+dt;
			spinCube( dt );
			cube->oxyz.o.z -= speed;
			speed += dt/500;
			if( cube->getCenter().z < -15 )
			{
				angle = 0;
				cube->clearOrientation();
				cube->setCenter( {0,-10,20} );
				spinCube( -90 );
				speed = 0.1;
				phase = 0;
			};
		}

		viewX = viewX*k+( 1-k )*cube->getCenter().x;
		viewY = viewY*k+( 1-k )*cube->getCenter().y;
		viewZ = viewZ*k+( 1-k )*( cube->getCenter().z+amp*sin( 50*t ) );
		amp = amp*k*k;
	}
	return 0;
}

