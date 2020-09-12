
/// Project:	FMI 3D (2013)
/// File:		25092 Mixed motion at DOF=2.cpp
/// Topic:		25 Skeletons
/// Slide:		09
///	About:		Linear and circular motion at DOF=2
///
/// History:
///		2013.07.13/PB	Initial implementation
///     2013.09.14/PB   Change button image


#include "fmi3d.h"
#include "25000 Mecho.h"

#define ANIMATION_TIME 3

#define N 800
Rail3D *r;
Holder3D *h;
Holder3D *h2;
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
double speedL = 1;
double speedR = 4;

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
	if( mode%2 )
		maxn=n;
	else
	{
		speedL = random(1,5);
		speedR = random(1,5);
	}
	cloud = 0;
}

int main()
{
	/// Open window
	if( !openWindow3D( "Mixed motion at DOF=2" ) ) return 1;
	//changeBrightness();
	changeGround();
	changeGround();
	changeGround();

	/// Add buttons
	sB = new Button2D( "buttons/button_start.png",GLFW_KEY_SPACE,speedOn );
	addButton( new exitButton2D );

	wood = loadTexture( "textures/Material_Wood.jpg" );
	viewZ = 4;

	r = new Rail3D( 9 );
	r->setCenter( {0,0,5} );
	h = new Holder3D( 2 );
	h2= new Holder3D( 7 );
	p = new Pencil3D( 9 );
	o = new Object3D();

	h2->setCenter({0,0,2});
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
		ofsR += speedR*dk;
		viewAlpha+=dt/12;

		r->clearOrientation();
		r->rotateGlobal( ofsR,{0,0,1} );
		r->rotateLocal(-35,{0,1,0});

		h->identity();
		h->rotateLocal( ofsR, {0,0,1} );
		h->rotateLocal(-35,{0,1,0});
		h->translateLocal( {4.5+3*sin( ofsL ),0,0} );
		h->translateGlobal( {0,0,5} );


		p->identity();
		p->rotateLocal( ofsR, {0,0,1} );
		p->rotateLocal(-35,{0,1,0});
		p->translateLocal( {4.5+3*sin( ofsL ),0,-5} );
		p->translateGlobal( {0,0,5} );

		o->identity();
		o->rotateLocal( ofsR, {0,0,1} );
		o->rotateLocal(-35,{0,1,0});
		o->translateLocal( {4.5+3*sin( ofsL ),0,-5} );
		o->translateGlobal( {0,0,5} );
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

