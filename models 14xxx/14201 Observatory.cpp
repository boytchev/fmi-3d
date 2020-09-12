
/// Project:	FMI 3D (2013)
/// File:		14201 Observatory.cpp
/// Topic:		14 Orientation
/// Slide:		20
///	About:		Observatory
///
/// History:
///		2013.08.10/PB	Initial implementation
///     2013.09.13/PB   Simplified complexity

#include "fmi3d.h"

#define ANIMATION_TIME 4

Cylinder3D *cyl,*cov,*cov2;
Circle2D *cir;
Sphere3D *sph,*glass,*stars;
Vector3D *vec;

GLdouble cp2[4]={0,0,1,-4};
GLdouble cp3[4];
GLdouble cp4[4]={0,1,1,-8};


double a = 0;

void drawDome()
{
	glEnable(GL_CLIP_PLANE2);
	glClipPlane(GL_CLIP_PLANE2,cp2);

	glEnable(GL_CLIP_PLANE3);
	cp3[0]=1;
	cp3[1]=0;
	cp3[2]=0;
	cp3[3]=-0.8;
	glClipPlane(GL_CLIP_PLANE3,cp3);
	sph->drawImage();
	cp3[0]=-1;
	cp3[1]=0;
	cp3[2]=0;
	cp3[3]=-0.8;
	glClipPlane(GL_CLIP_PLANE3,cp3);
	sph->drawImage();

	glClipPlane(GL_CLIP_PLANE3,cp4);
	sph->drawImage();

	glDisable(GL_CLIP_PLANE3);

	glDisable(GL_CLIP_PLANE2);
}

void drawScene()
{
	//glEnable(GL_POINT_SMOOTH);
	glDisable(GL_FOG);
	if( !inShadow )
	{
	    glPushMatrix();
        for(int i=0; i<20; i++)
        {
            glColor4f(0.9,0.9,1,random(0.3,1));
            glPointSize(1.5+sin(i));
            glRotated(60*i,1,1,0);
            glRotated(70*i,1,0,1);
            glRotated(50*i,0,1,1);
            stars->drawImage();
        }
        glPopMatrix();
	}
	glEnable(GL_FOG);

	cyl->drawImage();

	glPushMatrix();
	glRotated(a,0,0,1);

	if( !inShadow ) vec->drawImage();

	cov->drawImage();
	cov2->drawImage();
	cir->drawImage();

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	drawDome();
	glCullFace(GL_BACK);
	drawDome();
	glDisable(GL_CULL_FACE);

	if( !inShadow ) vec->drawImage();
	glass->drawImage();
	if( !inShadow ) vec->drawImage();

	glPopMatrix();
}


void drawFrames()
{
}


double transT = 0;
double trans = 1, transS = 1;
void toggleTrans()
{
	transT = t;
	transS = 1.4-transS;
}

int main()
{
	/// Open window
	if( !openWindow3D( "Observatory" ) ) return 1;
	changeBrightness();
	changeGround();

	/// Add buttons
	addButton( new Button2D("buttons/button_xray.png",GLFW_KEY_SPACE,toggleTrans));
	addButton( new exitButton2D );

	viewZ = 5;
	viewAlpha = -M_PI/2;
	viewBeta = 0;

	cyl = new Cylinder3D(4,4.3,false);
	cyl->setShadeModel(GL_FLAT);
	cyl->setColor({0.65,0.65,0.65});
	cyl->setCenter({0,0,0});

	cir = new Circle2D(3.9);
	cir->setColor({0,0,0});
	cir->setCenter({0,0,2});

	cov = new Cylinder3D(0.5,4,false);
	cov->setColor({0.4,0.4,0.4});
	cov->setCenter({0,0,3});

	cov2 = new Cylinder3D(0.495,3.95,false);
	cov2->setColor({0,0,0});
	cov2->setCenter({0,0,3});

	sph = new Sphere3D(4.03,QUALITY_HIGH);
	sph->setCenter({0,0,4});

	stars = new Sphere3D(500,QUALITY_LOW);
	stars->setCenter({0,0,0});
	stars->setPolygonMode(GL_POINT);
	stars->hasNoColor();
	stars->setLighting(false);

	glass = new Sphere3D(1,QUALITY_HIGH);
	glass->setColor({0.4,0.6,1,0.8});
	glass->setShininess(50);
	glass->setScale({0.49,0.49,0.2});

	vec = new Vector3D({0,0,1},0.3);
	vec->setColor({0.5,0.6,1});
	vec->setLength(9);
	vec->setCenter({0,0,3});

	/// Main loop
	transT = 0;
	double t0=t;
	while( isRunning() )
	{
        if( frame==3 )t0=t;
        if( frame==4 )
		{
            if( t-t0<1/25.0 )
                glEnable(GL_POINT_SMOOTH);
            else
                glDisable(GL_POINT_SMOOTH);
		}

		if( t-transT<ANIMATION_TIME )
		{
			double k = cos( ( ( t-transT )/ANIMATION_TIME )*M_PI/2 );
			trans = trans*k+transS*( 1-k );
		}

		a = 70*sin(t/3);
		viewAlpha = viewAlpha*0.99+0.01*(a*M_PI/180-M_PI/2);
		double b = 40+30*sin(t/1.123);
		cov->clearOrientation();
		cov->rotateLocal(b,{1,0,0});
		cov2->clearOrientation();
		cov2->rotateLocal(b,{1,0,0});
		glass->setCenter(cov->getCenter());
		glass->clearOrientation();
		glass->rotateLocal(b,{1,0,0});
		glass->translateLocal({0,0,3.7});
		vec->clearOrientation();
		vec->rotateLocal(b,{1,0,0});
		sph->setColor({0.8,0.8,1,trans});
	}
	return 0;
}

