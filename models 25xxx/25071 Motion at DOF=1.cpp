
/// Project:	FMI 3D (2013)
/// File:		25071 Motion at DOF=1.cpp
/// Topic:		25 Skeletons
/// Slide:		07
///	About:		Linear and circular motion at DOF=1
///
/// History:
///		2013.07.12/PB	Initial implementation
///     2013.09.14/PB   Change button image


#include "fmi3d.h"
#include "25000 Mecho.h"

#define ANIMATION_TIME 3

#define N 300
Rail3D *r;
Holder3D *h;
Holder3D *h2,*h3;
Pencil3D *p;
Object3D *o;
double cloud = 0;
int n=0;
int maxn = 0;
vect_t path[N];
double k = 0;
double dk;

void drawScene()
{
	double lw = 5-viewDistance/20;
	lw = ( lw<1 )?0.5:lw;

	h2->drawImage();
	h3->drawImage();
	h->drawImage();
	r->drawImage();
	p->drawImage();
	glColor3f( 1,0,0 );
	glDisable( GL_LIGHTING );
	glLineWidth( lw );
	glDisable( GL_LINE_SMOOTH );
	glBegin( GL_LINE_STRIP );
	for( int i=0; i<n; i++ )
	{
		path[i].vertex();
	}
	glEnd();

	glLineWidth( lw+0.5 );
	glEnable( GL_LINE_SMOOTH );
	glBegin( GL_LINE_STRIP );
	for( int i=0; i<n; i++ )
	{
		path[i].vertex();
	}
	glEnd();
	glPointSize( lw*0.8 );
	glEnable( GL_POINT_SMOOTH );
	glBegin( GL_LINE_STRIP );
	int maxi = 0;
	for( int i=n; i<maxn; i++ )
	{
		path[i].x +=-cloud*random(-0.01,0.01);
		path[i].y +=-cloud*random(-0.01,0.01);
		if( path[i].z<0.2 )
			path[i].z-=0.008;
		else
			path[i].z +=-cloud*random(0.1,0.2);
		path[i].vertex();
		if( path[i].z>0 ) maxi++;
	}
	glEnd();
	glDisable( GL_POINT_SMOOTH );
}


void drawFrames()
{
}

double speedT = -10*ANIMATION_TIME;
double speedL = 2;
double speedR = 0;

double speedH = 0;
int mode = 3;
Button2D *sB;
void speedOn()
{
	speedT = t;
	speedH = 1-speedH;
	if( speedH>0.5 ) n=0;
	mode = ( mode+1 )%4;
	sB->setState( mode/2 );
	if( mode%2 ) maxn=n;
	switch( mode/2 )
	{
		case 0: speedL = 2; speedR = 0; break;
		case 1: speedL = 0; speedR = 90; break;
	}
	cloud = 0;
}

int main()
{
	/// Open window
	if( !openWindow3D( "Motion at DOF=1" ) ) return 1;
	//changeBrightness();
	changeGround();
	changeGround();
	changeGround();

	/// Add buttons
	sB = new Button2D( "buttons/button_start.png",GLFW_KEY_SPACE,speedOn,0,2 );
	addButton( new exitButton2D );

	wood = loadTexture( "textures/Material_Wood.jpg" );
	viewZ = 4;

	r = new Rail3D( 14 );
	r->setCenter( {-7,0,5} );
	h = new Holder3D( 2 );
	h2= new Holder3D( 7 );
	h3= new Holder3D( 7 );
	p = new Pencil3D( 9 );
	o = new Object3D();

	h2->setCenter({-7,0,2});
	h3->setCenter({+7,0,2});
	/// Main loop
	t = 0;
	double t0=t;
	dk = 0;
	double ofsL = 0;
	double ofsR = 0;
	while( isRunning() )
	{
		double dt = t-t0; t0=t;

		if( t-speedT<ANIMATION_TIME )
		{
			k = 0.5+0.5*cos( ( ( t-speedT )/ANIMATION_TIME )*M_PI/2 );
			dk = dk*k+speedH*( 1-k );
		}
		ofsL += speedL*dk/30;
		ofsR += speedR*dk/30;
		viewAlpha+=dt/12;

		h->clearOrientation();
		h->setCenter( {5*sin( ofsL ),0,5} );
		h->rotateLocal( ofsR, {1,0,0} );

		p->clearOrientation();
		p->setCenter( {5*sin( ofsL ),0,5} );
		p->rotateLocal( ofsR, {1,0,0} );
		p->translateLocal( {0,0,-5} );

		o->identity();
		o->setCenter( {5*sin( ofsL ),0,5} );
		o->rotateLocal( ofsR, {1,0,0} );
		o->translateLocal( {0,0,-5} );
		if( ( dk>0.01 ) && ( n<N ) )
		{
			path[n]=o->getCenter();
			n++;
		}

		if( n==N-110 )
		{
			speedOn();
		}
		if( n>maxn ) maxn=n;
		cloud = cloud*1.1+0.01;
	}
	return 0;
}

