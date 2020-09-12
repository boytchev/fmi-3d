
/// This model does not support 3D stereo
///
/// Project:	FMI 3D (2013)
/// File:		20261 Slalom without tilt
/// Topic:		20 Projections
/// Slide:		26
///	About:		Zig-zagging the camera around an infinite line of cones.
///
/// History:
///		2013.08.11/KK	Initial implementation
///		2013.08.12/PB	Finetuning
///     2013.09.13/PB   Changed button image and key


#include "fmi3d.h"

#define ANIMATION_TIME 4
#define MAX_CONES 10

Cone3D* cone[MAX_CONES];
double groundY;
double speed = 5;

GLdouble    eyeX = 0.0;
GLdouble    eyeY = 0.0;
GLdouble    eyeZ = 2.5;
GLdouble    targetX = 0.0;
GLdouble    targetY = 1.0;
GLdouble    targetZ = 2.5;
GLdouble    upX = 0.0;
GLdouble    upY = 0.0;
GLdouble    upZ = 1.0;

void drawScene()
{
	if( inReflection ) return;

	glClear(GL_COLOR_BUFFER_BIT+GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glLoadIdentity();
    gluLookAt(eyeX, eyeY, eyeZ,
              targetX, targetY, targetZ,
              upX, upY, upZ);

    for(int i=0; i<MAX_CONES; i++) cone[i]->drawImage();
	glScaled(1,1,-1);
    for(int i=0; i<MAX_CONES; i++) cone[i]->drawImage();

	glTranslated(0,groundY,0);
	ground->drawImage();
	glTranslated(0,-groundY,0);

	glPopMatrix();
}

void drawFrames()
{
}

Button2D *sB;
double speedMax = speed, speedT;
void changeSpeed()
{
	speedT = t;
	int ti = (int((speedMax-5)/5)+1)%3;
	speedMax = 5+5*ti;
	sB->setState(ti);
}

int main()
{
	if( !openWindow3D( "Slalom1" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();

	sB = new Button2D( "buttons/button_time.png",GLFW_KEY_SPACE,changeSpeed,0,3 );
	addButton( new exitButton2D() );

    GLdouble distanceTraveled = 0;

	for(int i = 0; i < MAX_CONES; i++)
    {
		cone[i] = new Cone3D(1,5,false);
		cone[i]->setCenter({0,4*M_PI*(i-0.3),0});
		cone[i]->setColor({1,0,0});
    }

	isRunning();
	double t0=t;
	int c=0;
	while( isRunning() )
    {
		if( t-speedT<ANIMATION_TIME )
		{
			double k = cos( ( ( t-speedT )/ANIMATION_TIME )*M_PI/2 );
			speed = speed*k+speedMax*( 1-k );
		}

        eyeX = 2.5 * sin(distanceTraveled/4);
        eyeY = distanceTraveled;

        targetX = eyeX;
        targetY = eyeY + 1;

        distanceTraveled += speed*(t-t0);
        t0=t;

		if( eyeY > cone[c]->getCenter().y)
		{
			cone[c]->oxyz.o.y += MAX_CONES*4*M_PI;
			c = (c+1)%MAX_CONES;
		}

		double k=5.3*M_PI;
		groundY = k*int(eyeY/k);
    }


	return 0;
}

