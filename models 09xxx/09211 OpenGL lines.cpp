
/// Project:	FMI 3D (2013)
/// File:		09211 OpenGL lines.cpp
/// Topic:		9 Graphical Primitives
/// Slide:		21
///	About:		Independent lines in OpenGL
///
/// History:
///		2013.06.28/MD	Initial implementation
///     2013.09.11/PB   Fixed segment smoothness


#include "fmi3d.h"

#define N 30
const double radius=0.2;
Sphere3D* points[N];

void drawScene()
{
	for(int i=0; i<N; i++)
		points[i]->drawImage();

    glDisable(GL_LIGHTING);
    glEnable(GL_LINE_SMOOTH);
    glDepthMask(GL_FALSE);
    for(int i=0; i<N/2; i++)
	{
        vect_t centerA= points[i*2]->getCenter();
        vect_t centerB= points[i*2+1]->getCenter();

        glLineWidth(3);
        glColor3f(0, 0, 0.5);
        glBegin( GL_LINES );

        glVertex3f(centerA.x,centerA.y,centerA.z);
        glVertex3f(centerB.x,centerB.y,centerB.z);
        glEnd();
    }
    glDepthMask(GL_TRUE);
}


void drawFrames()
{
}

void randomize()
{
    double range=8.0;
	for(int i=0; i<N/2; i++)
	{
		points[i*2]->setCenter({random(-range,range),
                       random(-range,range),random(0,range)});

        points[i*2+1]->setCenter({random(-range,range),
                       random(-range,range),random(0,range)});
    }
}

int main()
{
	/// Open window
	if( !openWindow3D( "OpenGL lines" ) ) return 1;

	addButton( new Button2D("buttons/button_random.png",'R',randomize) );
	addButton( new exitButton2D );


	for(int i=0; i<N; i++)
	{
		points[i] = new Sphere3D(radius);
		points[i]->setColor({0,0,0});
		points[i]->setShininess(20);
    }

	randomize();

	/// Main loop with automatic animation
	double t0=t;
	while( isRunning() )
	{
		viewAlpha += (t-t0)/20;
		t0=t;
	}

	return 0;
}
