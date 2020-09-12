
/// Project:	FMI 3D (2013)
/// File:		14391 Aeronavigation - Yaw.cpp
/// Topic:		14 Orientation
/// Slide:		39
///	About:		Aeronavigation: Yaw
///
/// History:
///		2013.08.12/PB	Initial implementation


#include "fmi3d.h"

PaperPlane3D *plane;
CoordSys3D *cs;

void drawScene()
{

	cs->drawImage();
	plane->drawImage();
	cs->drawFrame();
	plane->drawFrame();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glDepthMask(GL_FALSE);

	double r = 7;
	glColor4f(1,0,0,0.5);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glBegin(GL_QUAD_STRIP);
	for(int i=-30; i<=30; i+=5)
		{
			double a = i*M_PI/180;
			double m = 0.5;
			if( i==-30 || i==+30 ) m=0;
			if( i==-25 ) {m=1; a+=5*M_PI/180;}
			if( i==+25 ) {m=1; a-=5*M_PI/180;}
			double c = pow(0.5+0.5*cos(fabs(i/30.0*M_PI)),2);
			glColor4f(1-0.5*c,0.5*c,c,0.5);
			glVertex3d(r*cos(a),r*sin(a),3-m);
			glVertex3d(r*cos(a),r*sin(a),3+m);
		}
		glEnd();
	glDepthMask(GL_TRUE);

}



void drawFrames()
{
}


int main()
{
	/// Open window
	if( !openWindow3D( "Aeronavigation: Yaw" ) ) return 1;

	/// Add buttons
	addButton( new exitButton2D );
	changeGround();
	changeGround();
	changeGround();

	viewAlpha = M_PI/8;
	viewBeta = 0.3;
	viewZ = 3;

	plane = new PaperPlane3D( 7 );
	plane->setCenter( {0,0,3} );
	plane->setLighting( false );
	plane->setColor( {0.5,0.5,1} );
	plane->setScale({7,7,4});

	cs = new CoordSys3D( -0.5,4,-2,2,-0.5,3, false, 0, 0.5, L"   " );
	cs->setAxesNamesColor( {0.5,0.5,1} );
	cs->setCenter( {0,0,3} );

	/// Main loop
	isRunning();
	while( isRunning() )
	{
		plane->clearOrientation();
		plane->rotateLocal( 30*cos(2*t), {0,0,1} );
	}
	return 0;
}

