
/// Project:	FMI 3D (2013)
/// File:		26451 Knee
/// Topic:		26 Surfaces
/// Slide:		45
///	About:		Knee and NURBS skin
///
/// History:
///		2013.06.26/PB	Initial implementation


#include "fmi3d.h"

//#include <iostream>

int mode=2;	/// 0=dots 1=wireframe 2=fill
Object3D* turtle;

#define SCALE 0.15
#define HOME	turtle->identity()
#define FDT(h)	{BXT;turtle->translateLocal({h*SCALE,0,0});BXT}
#define FD(h)	{BX;turtle->translateLocal({h*SCALE,0,0});BX}
#define BK(h)	{turtle->translateLocal({-h*SCALE,0,0});}
#define SLT(h)	{BXT;turtle->translateLocal({0,h*SCALE,0});BXT}
#define SL(h)	{BX;turtle->translateLocal({0,h*SCALE,0});BX}
#define SR(h)	{BX;turtle->translateLocal({0,-h*SCALE,0});BX}
#define SU(h)	{turtle->translateLocal({0,0,h*SCALE});}
#define SD(h)	{BX;turtle->translateLocal({0,0,-h*SCALE});BX}
#define LT(h)	turtle->rotateLocal(h,{0,0,1})
#define RT(h)	turtle->rotateLocal(-h,{0,0,1})
#define UP(h)	turtle->rotateLocal(-h,{0,1,0})
#define DN(h)	turtle->rotateLocal(h,{0,1,0})
#define LR(h)	turtle->rotateLocal(-h,{1,0,0})
#define RR(h)	turtle->rotateLocal(h,{1,0,0})
#define BXT		thinbones[bt++]=turtle->getCenter();
#define BX		bones[b++]=turtle->getCenter();

Bezier3D *bez1, *bez2;

#define M 15
Sphere3D *P[M];
vect_t bones[3*M];
vect_t thinbones[3*M];
int b=0;
int bt=0;

vect_t skin(double alpha, double dist)
{
	LT(alpha);
	FDT(dist);
	BK(dist);
	RT(alpha);
	return thinbones[bt-1];
}

void skeleton()
{
	double beta=40+40*cos(t);
	b = 0; bt = 0;

	HOME;
	SU(5);
	BK(20);
	UP(30*cos(t));
	UP(90);
	{
		double f = 90;
		P[0]->setCenter(skin(+f,20));
		P[1]->setCenter(skin( 0,30));
		P[2]->setCenter(skin(-f,20));
	}
	DN(90);

	FD(20);
	UP(90+beta/2);
	{
		double f = 50;
		P[3]->setCenter(skin(+f,10));
		P[4]->setCenter(skin( 0,20));
		P[5]->setCenter(skin(-f,10));
	}
	DN(90-beta/2);

	DN(beta);
	FD(15);
	DN(beta);

	UP(90-beta/2);
	{
		double f = 90;
		P[9]->setCenter(skin(+f,10));
		P[10]->setCenter(skin( 0,10));
		P[11]->setCenter(skin(-f,10));
	}
	DN(90+beta/2);

	FD(20);
	UP(90);
	{
		double f = 90;
		P[12]->setCenter(skin(+f,5));
		P[13]->setCenter(skin( 0,5));
		P[14]->setCenter(skin(-f,5));
	}
	DN(90);

	P[6]->setCenter( P[3]->getCenter().add(P[9]->getCenter()).mul(0.5) );
	P[7]->setCenter( P[4]->getCenter().add(P[10]->getCenter()).mul(0.5) );
	P[8]->setCenter( P[5]->getCenter().add(P[11]->getCenter()).mul(0.5) );

	for(int i=0; i<9; i++)
	{
		bez1->setControlPoint(i/3,i%3,P[i]->getCenter());
		bez2->setControlPoint(i/3,i%3,P[i+6]->getCenter());
	}
}


void drawScene()
{
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(0.5);

	glLightModeli( GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE );
	bez1->drawImage();
	bez2->drawImage();
	glLightModeli( GL_LIGHT_MODEL_TWO_SIDE,GL_FALSE );

	if( mode==2 ) return;

	for( int i=0; i<15; i++ )
		P[i]->drawImage();

	glDisable(GL_LIGHTING);
	glColor3f(0,0,0);
	glLineWidth(6);
	glBegin(GL_LINES);
	for(int i=0; i<b; i++) bones[i].vertex();
	glEnd();

	glColor3f(0.5,0.5,1);
	glLineWidth(2);
	glBegin(GL_LINES);
	for(int i=0; i<bt; i++) thinbones[i].vertex();
	glEnd();
	glEnable(GL_LIGHTING);
}


void drawFrames()
{
}

Button2D* butWF;
void showWireframe()
{
	mode=( mode+1 )%3;
	butWF->setState( mode );

	bez1->setIsVisible(mode>0);
	bez2->setIsVisible(mode>0);

	bez1->setPolygonMode(mode==1?GL_LINE:GL_FILL);
	bez2->setPolygonMode(mode==1?GL_LINE:GL_FILL);

	bez1->setTolerance(mode<2?50:10);
	bez2->setTolerance(mode<2?50:10);
}


int main()
{
	/// Open window
	if( !openWindow3D( "Knee" ) ) return 1;
	noGround();

	viewZ = 0;
	viewBeta = 0.3;
	viewAlpha = 1;
	viewDistance = 15;

	/// Add buttons
	butWF = new Button2D( "buttons/button_wireframe.png",GLFW_KEY_SPACE,showWireframe,mode,3 );
	addButton( new exitButton2D );

	turtle = new Object3D();

	bez1 = new Bezier3D(3);
	bez2 = new Bezier3D(3);
	bez1->setColor({0.5,0,0});
	bez2->setColor({0.5,0,0});
	bez1->setShininess(20);
	bez2->setShininess(20);
	bez1->setTolerance(50);
	bez2->setTolerance(50);

	for( int i=0; i<M; i++ )
	{
		P[i] = new Sphere3D(0.15);
		P[i]->setColor( {0,0,1} );
		P[i]->setShininess(10);
		P[i]->setIsVisible((i<6)||(i>8));
	}
	showWireframe();


	/// Main loop
	while( isRunning() )
	{
		skeleton();
	}

	return 0;
}

