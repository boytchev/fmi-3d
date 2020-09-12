
/// Project:	FMI 3D (2013)
/// File:		25081 Linear motions at DOF=2.cpp
/// Topic:		25 Skeletons
/// Slide:		08
///	About:		Linear motions at DOF=2
///
/// History:
///		2013.07.12/PB	Initial implementation
///     2013.09.14/PB   Change button image


#include "fmi3d.h"
#include "25000 Mecho.h"

#define ANIMATION_TIME 3

#define N 400
Rail3D *r[3];
Holder3D *h;
Holder3D *h2[4];
Pencil3D *p;
Object3D *o;
double cloud = 0;
int n=0;
int maxn = 0;
vect_t path[N];
double k = 0;
double dk;
double q;

void drawScene()
{
	double lw = 5-viewDistance/20;
	lw = ( lw<1 )?0.5:lw;

	for( int i=0; i<4; i++ ) h2[i]->drawImage();
	h->drawImage();
	for( int i=0; i<3; i++ ) r[i]->drawImage();
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
		path[i].x +=-cloud*random( -0.01,0.01 );
		path[i].y +=-cloud*random( -0.01,0.01 );
		path[i].z -=cloud*random( -0.2,0.5 )-0.1*sin(10*q);
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
double speedR = 3;
double speedH = 0;
int mode = 3;
void speedOn()
{
	speedT = t;
	speedH = 1-speedH;
	if( speedH>0.5 ) n=0;
	mode = ( mode+1 )%4;
	if( mode%2 )
		maxn=n;
	else
	{
		q = 0;
		speedL = int( random( 2,8 ) )/5.0;
		speedR = ( speedL*random(1.5,2.5) )/3.0;
	}
	cloud = 0;
}

int main()
{
	/// Open window
	if( !openWindow3D( "Linear motions at DOF=2" ) ) return 1;
	//changeBrightness();
	changeGround();
	changeGround();
	changeGround();

	/// Add buttons
	addButton( new Button2D( "buttons/button_start.png",GLFW_KEY_SPACE,speedOn ) );
	addButton( new exitButton2D );

	wood = loadTexture( "textures/Material_Wood.jpg" );
	viewZ = 2;

	for( int i=0; i<3; i++ ) r[i] = new Rail3D( 14 );
	r[0]->setCenter( {-7,-7,5} );
	r[1]->setCenter( {-7,+7,5} );
	r[2]->setCenter( {0,0,5} );
	r[2]->rotateLocal( 90, {0,0,1} );
	h = new Holder3D( 2 );
	for( int i=0; i<4; i++ )
	{
		h2[i]= new Holder3D( 7 );
		h2[i]->setCenter( {14*( i%2-0.5 ),14*( i/2-0.5 ),2} );
	}
	p = new Pencil3D( 9 );
	o = new Object3D();

	/// Main loop
	t = 0;
	double t0=t;
	dk = 0;
	double ofsL = 0;
	double ofsR = 0;
	while( isRunning() )
	{
		double dt = t-t0; t0=t;
		q += dt;

		if( t-speedT<ANIMATION_TIME )
		{
			k = 0.5+0.5*cos( ( ( t-speedT )/ANIMATION_TIME )*M_PI/2 );
			dk = dk*k+speedH*( 1-k );
		}
		ofsL += speedL*dk/20;
		ofsR += speedR*dk/20;
		viewAlpha+=dt/12;

		vect_t v = {6*sin(ofsL)*cos(ofsR-q*dk/3),6*sin(ofsR)*cos(ofsL+dk*q/3),5};

		h->clearOrientation();
		h->setCenter( v );

		p->clearOrientation();
		p->setCenter( v );
		p->translateLocal( {0,0,-5} );

		r[2]->setCenter( {v.x,-7,5} );
		o->identity();
		o->setCenter( v );
		o->translateLocal( {0,0,-5} );
		if( ( dk>0.002 ) && ( n<N ) )
		{
			path[n]=o->getCenter().add({0,0,0.02});
			n++;
		}

		if( n==N-90 )
		{
			speedOn();
		}
		if( n>maxn ) maxn=n;
		cloud = cloud*1.05+0.01;
	}
	return 0;
}

