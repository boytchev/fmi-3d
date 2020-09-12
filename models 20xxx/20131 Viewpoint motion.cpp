
/// This model does not support 3D stereo
///
/// Project:	FMI 3D (2013)
/// File:		20131 Viewpoint motion
/// Topic:		20 Projections
/// Slide:		20
///	About:		Interpolating between camera with different "up" and "target" vectors.
///
/// History:
///		2013.08.17/KK	Initial implementation
///		2013.08.19/PB	Finetuning


#include "fmi3d.h"

#define N 10
#define ANIMATION_TIME 5

Cube3D* s[N][ N];
Sphere3D *p[3];
Cone3D *co[2];
Cylinder3D *cy[2];
double k=0;

Cylinder3D *cameraDirectionVector;
Vector3D *cameraUpVector;

GLdouble    eyeX = 15.0;
GLdouble    eyeY = 15.0;
GLdouble    eyeZ = 15.0;
GLdouble    targetX = 0.0;
GLdouble    targetY = 0.0;
GLdouble    targetZ = 0.0;
GLdouble    upX = 0.0;
GLdouble    upY = 0.0;
GLdouble    upZ = 1.0;

void drawScene()
{
	glDisable(GL_FOG);
	glPushMatrix();
	glLoadIdentity();

	gluLookAt( eyeX, eyeY, eyeZ,
			   targetX, targetY, targetZ,
			   upX, upY, upZ );
	for(int i=0; i<3; i++) p[i]->drawImage();
	for( int i=0; i<N; i++ )
		for( int j=0; j<N; j++ )
			s[i][j]->drawImage();

	cameraDirectionVector->drawImage();
	cameraUpVector->drawImage();

	glPopMatrix();
}

void drawFrames()
{
	glDisable(GL_FOG);
	glPushMatrix();
	glLoadIdentity();

	gluLookAt( eyeX, eyeY, eyeZ,
			   targetX, targetY, targetZ,
			   upX, upY, upZ );

	for( int i=0; i<N; i++ )
		for( int j=0; j<N; j++ )
			s[i][j]->drawFrame();

	cameraDirectionVector->drawFrame();

	glPopMatrix();
}

void moveCameraVectors()
{
	double q = t/2;
	p[0]->setCenter({5+3*sin(q),5+3*cos(q+1),12+4*sin(q*1.1)});
	p[1]->setCenter({-4+3*cos(q),-4+3*sin(q-1),4+1*sin(q*1.4-2)});
	p[2]->setCenter({-7+3*cos(q+1),-7+3*sin(q),3+1*sin(q*1.4-1)});

	vect_t v = p[1]->getCenter().sub(p[0]->getCenter());
	vect_t tar = p[1]->getCenter();
	vect_t eye = p[0]->getCenter();
	vect_t upp = p[2]->getCenter().sub(tar);

	oxyz_t oxyz;
	oxyz.oz = tar.sub(eye);
	oxyz.ox = upp;
	oxyz.oy = oxyz.oz.vmul(oxyz.ox);
	oxyz.ox = oxyz.oy.vmul(oxyz.oz);
	oxyz.ox = oxyz.ox.mul(1/oxyz.ox.distance());
	oxyz.oy = oxyz.oy.mul(1/oxyz.oy.distance());
	oxyz.oz = oxyz.oz.mul(1/oxyz.oz.distance());
	oxyz.o = eye;
	oxyz.o.w = 1;
	cameraDirectionVector->oxyz = oxyz;

	cameraUpVector->setCenter(p[1]->getCenter());
	cameraUpVector->setDirection(p[2]->getCenter().sub(p[1]->getCenter()));

	targetX = tar.x*k+(1-k)*0;
	targetY = tar.y*k+(1-k)*0;
	targetZ = tar.z*k+(1-k)*0;

	v = eye.sub(v.mul(3/v.distance()));
	eyeX = v.x*k+(1-k)*20*sin(t/5);
	eyeY = v.y*k+(1-k)*20*cos(t/5);
	eyeZ = v.z*k+(1-k)*25;

	upX = upp.x*k+(1-k)*0;
	upY = upp.y*k+(1-k)*0;
	upZ = upp.z*k+(1-k)*1;

	v = {eyeX,eyeY,eyeZ};
	viewDistance = v.distance();
	if(viewDistance>20) viewDistance=20;
}

double kT = 0;
double kMax = 0;
void toggleK()
{
	kT = t;
	kMax = 1-kMax;
}

int main()
{
	if( !openWindow3D( "20131 Viewpoint motion" ) ) return 1;
	noGround();

	addButton( new Button2D( "buttons/button_show.png",GLFW_KEY_SPACE,toggleK ) );
	addButton( new exitButton2D() );

	p[0] = new Sphere3D(0.3);
	p[0]->setIsVisible(false);

	p[1] = new Sphere3D(0.2);
	p[1]->setColor({1,0,0});
	p[1]->setShininess(50);

	p[2] = new Sphere3D(0.05);
	p[2]->setIsVisible(false);

	for( int i=0; i<N; i++ )
		for( int j=0; j<N; j++ )
		{
			s[i][j] = new Cube3D( 5 );
			double z = random( 0.1,2 );
			s[i][j]->setScale( {2,2,z} );
			s[i][j]->setCenter( {( 2.0*i-( N-1 ) ),( 2.0*j-( N-1 ) ),z/2} );
			s[i][j]->setColor( {1,random(0.8,1),random(0.6,1)} );
		}

	cameraDirectionVector = new Cylinder3D( 0.75,3,false,QUALITY_HIGH );
	cameraDirectionVector->setColor({0.5,0.5,1});

	cameraUpVector = new Vector3D( {0, 0, 0}, 0.35 );
	cameraUpVector->setColor( {1,0,0} );
	cameraUpVector->setLength( 1 );

	while( isRunning() )
	{
		if( t-kT<ANIMATION_TIME )
		{
			double kk = 0.5+0.5*cos( ( ( t-kT )/ANIMATION_TIME )*M_PI/2 );
			k = k*kk+kMax*( 1-kk );
		}

		cameraUpVector->setLength(3-1.5*k);
		cameraUpVector->setScale({2-0.5*k,2-0.5*k,2-0.5*k});
		moveCameraVectors();
	}

	return 0;
}

