
/// Project:	FMI 3D (2013)
/// File:		03061 Cartesian coordinate system
///	About:		Sphere moving within cartesian coordinates
///
/// History:
///		2013.06.16/JB	Initial implementation
///		2013.06.17/PB	Fine-tuning


#include "fmi3d.h"

Sphere3D* point;
Text3D* text1;
Text3D* text2;
Text3D* text3;
CoordSys3D* cs1;
Cube3D* cube;

void drawScene()
{

    point->drawImage();

	text1->drawImage();
	text2->drawImage();
	text3->drawImage();

    cs1->drawImage();

	glLineWidth(1);
	cube->setColor({1,0,0});
	cube->setPolygonMode(GL_LINE);
	cube->drawImage();
	cube->setColor({1,0,0,0.1});
	cube->setPolygonMode(GL_FILL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	cube->drawImage();
	glCullFace(GL_FRONT);
	cube->drawImage();
	glDisable(GL_CULL_FACE);

	cube->setColor({1,0,0});
	glPointSize(7);
	glEnable(GL_POINT_SMOOTH);
	cube->setPolygonMode(GL_POINT);
	cube->drawImage();
}

void drawFrames()
{
    cs1->drawFrame();
}

int main()
{
	if( !openWindow3D( "Cartesian coordinate system" ) ) return 1;
	noGround();

	viewZ = 3;
	minViewBeta = -maxViewBeta;

	addButton( new exitButton2D() );

    point = new Sphere3D( 0.3 );
    point->setColor({0.5,0.5,1});
    point->setShininess(100);

    cs1 = new CoordSys3D(-6,6,-6,6,0,6,true,1,0.5,L"XYZ");
	cs1->setAxesNamesColor({0.2,0.2,0.2});
	cs1->setCenter({0,0,2});

	cube = new Cube3D(0);
	cube->setColor({1,0,0});
	cube->setLighting(false);

	text1 = new Text3D( L"x",systemFont3D );
	text1->setColor( {1,0,0} );
	text1->rotateLocal( 90, {1,0,0} );

	text2 = new Text3D( L"y",systemFont3D );
	text2->setColor( {1,0,0} );
	text2->rotateLocal( 90, {0,0,1} );
	text2->rotateLocal( 90, {1,0,0} );

	text3 = new Text3D( L"z",systemFont3D );
	text3->setColor( {1,0,0} );
	text3->rotateLocal( 90, {0,0,1} );
	text3->rotateLocal( 90, {1,0,0} );


	while( isRunning() )
	{
        double x=5*sin(0.8*t);
        double y=5*cos(0.5*t);
        double z=3+2*cos(0.4*t);

		cube->setCenter({x/2,y/2,2+z/2});
		cube->setScale({x,y,z});

        text1->setCenter({ x/2 , y-0.1 , 2+z } );
        text2->setCenter({ x+0.1 , y/2 ,2+z } );
        text3->setCenter( { x , y , 2+z/2-0.2 } );

		text3->clearOrientation();
		text3->rotateLocal( 90, {0,0,1} );
		text3->rotateLocal( 90, {1,0,0} );
		text3->rotateLocal(180/M_PI*viewAlpha,{0,1,0});

        point->setCenter({ x , y , 2+z } );
	}
	return 0;
}

