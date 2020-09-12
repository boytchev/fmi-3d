
/// Project:	FMI 3D (2013)
/// File:		13391 Jumps.cpp
/// Topic:		13 Physics
/// Slide:		39
///	About:		Rotating barrier makes cones jump over it.
///
/// History:
///		2013.08.24/KK	Initial implementation
///		2013.08.24/PB	Finetuned
///     2013.09.11/PB   Reduced complexity of graphical objects
///     2013.09.13/PB   More reduced complexity of graphical objects


#include "fmi3d.h"
#include "math.h" // fmod

#define MAX_CONES 25

Cone3D*     cones[MAX_CONES];
double		spd[MAX_CONES];
Cylinder3D* base;
Sphere3D*   sp;
Cone3D*     arrow;

double      offsets[MAX_CONES];
double      rotations[MAX_CONES];

Button2D*   speedButton;
int         speed = 1;
void adjustSpeed()
{
    speed = (speed + 1) % 3;
	speedButton->setState(speed);
}

void drawScene()
{
	base->drawImage();
	sp->drawImage();
	arrow->drawImage();

    for(int i = 0; i < MAX_CONES; i++)
        cones[i]->drawImage();
}

void drawFrames()
{
	base->drawFrame();
	sp->drawFrame();
	arrow->drawFrame();

    for(int i = 0; i < MAX_CONES; i++)
        cones[i]->drawFrame();
}

double  dist(double a, double b)
{
	double x = fmod(a-b,360);
	if( x>180 ) x = 360-x;
	if( x<-180 ) x = 360+x;

	return x;
}

int main()
{
	if( !openWindow3D( "Jumps" ) ) return 1;
	changeGround();

    speedButton = new Button2D( "buttons/button_time.png", GLFW_KEY_SPACE, adjustSpeed, speed, 3);
	addButton( new exitButton2D() );

	base = new Cylinder3D(8, 8, true, QUALITY_LOWER);
	base->setTexture(loadTexture("textures/Material_Scratch.jpg"));
	base->setTextureScale({1,1,2*M_PI,1});
	base->setCenter({0,0,-7.5});
	sp = new Sphere3D(0.8);

	sp->translateGlobal({0, 0, 0.5});
	sp->setColor({1, 1, 0});

	arrow = new Cone3D(0.4, 8, false, QUALITY_LOW);
	arrow->rotateGlobal(90, {0, 1, 0});
	arrow->translateGlobal({0, 0, 0.5});
	arrow->setColor({1, 1, 0});

	GLuint tex = loadTexture("textures/Ground_Marble.jpg");
    for(int i = 0; i < MAX_CONES; i++)
    {
        cones[i] = new Cone3D(0.5, 2.0, true, QUALITY_LOWER);
        cones[i]->setTexture(tex);

        offsets[i] = random(2.5, 7.5);
        cones[i]->translateGlobal({offsets[i], 0, 0.5});

        rotations[i] = random(0, 360);
        cones[i]->rotateGlobal(rotations[i], {0, 0, 1});
        spd[i] = random(0.5,1.5);
    }

    double rotationSpeed;
    double reactionDelta = 25.0;
    double currentRoatation = 0;

	isRunning();
	double dt, t0=t;
	while( isRunning() )
    {
    	dt = t-t0;
    	t0 = t;
		viewAlpha -= dt/10;

        rotationSpeed = 15 + 10 * pow(speed+1,3);

        arrow->rotateGlobal(rotationSpeed*dt, {0, 0, 1});

        currentRoatation += rotationSpeed*dt;

        if(currentRoatation >= 360)
            currentRoatation -= 360;

        for(int i = 0; i < MAX_CONES; i++)
        {
            double jump = 0;
            double twist1 = 0;
            double twist2 = 0;

			double d = dist(currentRoatation,rotations[i]);
	            if(fabs(d) <= reactionDelta*(speed+1))
            {
                jump = 2*(0.5+0.5*cos(d/reactionDelta/(speed+1)*M_PI));

                twist1 = jump * sin(2*t+i) * 30;
                twist2 = jump * cos(3*t-i) * 30;
            }

            cones[i]->identity();
            cones[i]->rotateGlobal(twist1, {1, 0, 1});
            cones[i]->rotateGlobal(twist2, {0, -1, 1});
            cones[i]->translateGlobal({offsets[i], 0, 0.5 + jump});
            cones[i]->rotateGlobal(rotations[i], {0, 0, 1});
            rotations[i] = (rotations[i]-spd[i]+0.8*sin(3*t+i));
            if( rotations[i]<0 ) rotations[i] +=360;
        }
    }

	return 0;
}

