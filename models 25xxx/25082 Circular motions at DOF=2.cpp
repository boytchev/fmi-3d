
/// Project:	FMI 3D (2013)
/// File:		25082 Circular motions at DOF=2.cpp
/// Topic:		25 Skeletons
/// Slide:		08
///	About:		Circular motions at DOF=2
///
/// History:
///		2013.07.13/PB	Initial implementation
///     2013.09.14/PB   Change button image


#include "fmi3d.h"
#include "25000 Mecho.h"

#define ANIMATION_TIME 3

#define N 400
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
double q;

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
double speedL = 15;
double speedR = 20;
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
		speedL = random( 1,50 );
		speedR = random( 1,50 );
	}
	cloud = 0;
}

int main()
{
	/// Open window
	if( !openWindow3D( "Circular motions at DOF=2" ) ) return 1;
	//changeBrightness();
	changeGround();
	changeGround();
	changeGround();

	/// Add buttons
	addButton( new Button2D( "buttons/button_start.png",GLFW_KEY_SPACE,speedOn ) );
	addButton( new exitButton2D );

	wood = loadTexture( "textures/Material_Wood.jpg" );
	viewZ = 4;
	viewDistance = 25;

	r = new Rail3D( 4 );
	r->setCenter( {0,0,5} );
	h = new Holder3D( 2 );
	h2= new Holder3D( 7 );
	h2->setCenter( {0,0,0.5} );
	p = new Pencil3D( 5 );
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
		ofsL += speedL*dk/220;
		ofsR += speedR*dk/220;
		viewAlpha+=dt/12;

		vect_t v = {50*sin(ofsL),-20+40*cos(ofsR)};

		h->identity();
		h->rotateLocal(v.x,{0,0,1});
		h->rotateLocal(v.y,{0,1,0});
		h->translateLocal({5,0,0});
		h->translateGlobal({0,0,5});
		h->rotateLocal(90,{0,1,0});


		p->identity();
		p->rotateLocal(v.x,{0,0,1});
		p->rotateLocal(v.y,{0,1,0});
		p->translateLocal({9,0,0});
		p->translateGlobal({0,0,5});
		p->rotateLocal(-90,{0,1,0});

		r->clearOrientation();
		r->rotateLocal(v.x,{0,0,1});
		r->rotateLocal(v.y,{0,1,0});

		o->identity();
		o->rotateLocal(v.x,{0,0,1});
		o->rotateLocal(v.y,{0,1,0});
		o->translateLocal({9,0,0});
		o->translateGlobal({0,0,5});
		o->rotateLocal(-90,{0,1,0});

		if( ( dk>0.002 ) && ( n<N ) )
		{
			path[n]=o->getCenter();
			n++;
		}

		if( n==N-90 )
		{
			speedOn();
		}
		if( n>maxn ) maxn=n;
		cloud = cloud*1.1+0.01;
	}
	return 0;
}

