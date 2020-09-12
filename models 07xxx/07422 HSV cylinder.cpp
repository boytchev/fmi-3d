
/// Project:	FMI 3D (2013)
/// File:		07422 HSV cylinder.cpp
/// Topic:		07 Colours
/// Slide:		42
///	About:		HSV cylinder
///
/// History:
///		2013.06.29/PB	Initial implementation
///     2013.09.13/PB   Changed arrow sizes


#include "fmi3d.h"

//#include <iostream>

#define N 10.0
#define ANIMATION_TIME 2
int mode=0;
bool showVectors = false;
double h = 0;

void HSVtoRGB( double h, double s, double v )
{
	const double k = 0.85*(mode?v:1);
	double c = pow(v,0.5)*s; // chroma
	double m = v-c;

	glColor3f
	(
		m+c*(cos((h)*M_PI/180)+1)/2,
		m+c*(cos((h+120)*M_PI/180)+1)/2,
		m+c*(cos((h+240)*M_PI/180)+1)/2
	);
	glVertex3f( k*s*cos( h*M_PI/180.0 ),k*s*sin( h*M_PI/180.0 ),1.5*v );
}


class ColorCyl3D : public Object3D
{
	public:
		ColorCyl3D( int fromH, int toH );
	private:
};

ColorCyl3D::ColorCyl3D( int fromH, int toH ) : Object3D()
{
	setSize( 6 );
	setLighting( false );

	glNewList( imageList, GL_COMPILE );
	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glBegin( GL_QUADS );
	for( int j=0; j<N; j++ )
	{
		for( int i=fromH; i<toH; i+=5 )
		{
			// outer wall
			HSVtoRGB( i,1,double( j )/N );
			HSVtoRGB( i+5,1,double( j )/N );
			HSVtoRGB( i+5,1,double( j+1 )/N );
			HSVtoRGB( i,1,double( j+1 )/N );
			// top
			HSVtoRGB( i,double( j )/N,1 );
			HSVtoRGB( i+5,double( j )/N,1 );
			HSVtoRGB( i+5,double( j+1 )/N,1 );
			HSVtoRGB( i,double( j+1 )/N,1 );
			// bottom
			HSVtoRGB( i,double( j )/N,0 );
			HSVtoRGB( i+5,double( j )/N,0 );
			HSVtoRGB( i+5,double( j+1 )/N,0 );
			HSVtoRGB( i,double( j+1 )/N,0 );
		}
		for( int i=0; i<N; i++ )
		{
			// start wall
			HSVtoRGB( fromH,double( i )/N,double( j )/N );
			HSVtoRGB( fromH,double( i+1 )/N,double( j )/N );
			HSVtoRGB( fromH,double( i+1 )/N,double( j+1 )/N );
			HSVtoRGB( fromH,double( i )/N,double( j+1 )/N );
			// end wall
			HSVtoRGB( toH,double( i )/N,double( j )/N );
			HSVtoRGB( toH,double( i+1 )/N,double( j )/N );
			HSVtoRGB( toH,double( i+1 )/N,double( j+1 )/N );
			HSVtoRGB( toH,double( i )/N,double( j+1 )/N );
		}
	}
	glEnd();
	glEndList();
}

ColorCyl3D *q[6][2];
Vector3D *vs;
Text3D *ts;
Vector3D *vl;
Text3D *tl;
Vector3D *vh;
Text3D *th;

void drawScene()
{
	glPushMatrix();
	glRotated(30,0,0,1);
	for( int i=0; i<6; i++ )
		q[i][mode]->drawImage();
	glPopMatrix();

	if( !showVectors ) return;
	vs->drawImage();
	ts->drawImage();
	vl->drawImage();
	tl->drawImage();
	vh->drawImage();
	th->drawImage();

	glColor3f(0,0,0);
	glDisable(GL_LINE_SMOOTH);
	glBegin(GL_LINE_STRIP);
	for(float i=10*M_PI/180; i<355*M_PI/180; i=i+3*M_PI/180)
		glVertex3f((5.5+0.9*h)*cos(-i),(5.5+0.9*h)*sin(-i),9);
	glEnd();
}


void drawFrames()
{
	if( !showVectors ) return;

	vs->drawFrame();
	vl->drawFrame();
	vh->drawFrame();

	glColor3f(0,0,0);
	glEnable(GL_LINE_SMOOTH);
	glBegin(GL_LINE_STRIP);
	for(float i=10*M_PI/180; i<355*M_PI/180; i=i+3*M_PI/180)
		glVertex3f((5.5+0.9*h)*cos(-i),(5.5+0.9*h)*sin(-i),9);
	glEnd();
}

double splitT;
double splitH = 0;
void splitUnsplit()
{
	splitT = t;
	splitH = 4-splitH;
}

void nextMode()
{
	mode = 1-mode;
	ts->setText((mode?L"C":L"S"));
}

void showHide()
{
	showVectors = !showVectors;
}

int main()
{
	/// Open window
	if( !openWindow3D( "HSV cylinder" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();

	addButton( new Button2D( "buttons/button_toggle.png",'T',nextMode ) );
	addButton( new Button2D( "buttons/button_show.png",'S',showHide ) );
	addButton( new Button2D( "buttons/button_cut.png",GLFW_KEY_SPACE,splitUnsplit ) );
	addButton( new exitButton2D );

	mode=1;
	for(int i=0; i<6; i++)
		q[i][mode] = new ColorCyl3D( 60*i, 60*i+60 );
	mode=0;
	for(int i=0; i<6; i++)
		q[i][mode] = new ColorCyl3D( 60*i, 60*i+60 );

	vs = new Vector3D({1,0,0},0.4);
	vs->setColor({0,0,0});
	vs->setLength(4.8);

	vl = new Vector3D({0,0,1},0.4);
	vl->setColor({0,0,0});
	vl->setLength(8.6);

	vh = new Vector3D({sin(10*M_PI/180),cos(10*M_PI/180),0},0.4);
	vh->setColor({0,0,0});
	vh->setLength(0.1);

	ts = new Text3D(L"S",systemFont3D);
	ts->setColor({0,0,1});
	ts->rotateGlobal(90,{1,0,0});

	tl = new Text3D(L"V",systemFont3D);
	tl->setColor({0,0,1});
	tl->rotateGlobal(90,{1,0,0});

	th = new Text3D(L"H",systemFont3D);
	th->setColor({0,0,1});
	th->rotateGlobal(90,{1,0,0});
	th->rotateGlobal(60,{0,0,1});

	viewZ = 4;
	viewAlpha = -0.6;
	viewBeta = 0.45;
	minViewBeta = 0;
	h=0;

	/// Main loop with automatic animation
	while( isRunning() )
	{
		double k = 1;
		if( t-splitT<ANIMATION_TIME )
		{
			k = cos( ( ( t-splitT )/ANIMATION_TIME )*M_PI/2 );
			h = h*k+splitH*( 1-k );
		}

		double dx = -0.6*h*cos(330*M_PI/180);
		double dy = -0.6*h*sin(330*M_PI/180);
		for(int i=0; i<6; i++)
		{
			double a = (i+0.5)*60.0*M_PI/180;
			double r = (i==5)?1.5:0.2;
			q[i][mode]->setCenter( {r*h*cos(a)+dx,r*h*sin(a)+dy,0} );
		}

		h/=4;
		vs->setCenter({0+3.7*h,0,9.4});
		ts->setCenter({2.4+3.7*h,-0.1,9.6});

		vl->setCenter({5.5+3.7*h,0,0});
		tl->setCenter({5.7+3.7*h,-0.1,6});

		vh->setCenter({(5.5+3.6*h)*cos(-10*M_PI/180),(5.5+3.6*h)*sin(-10*M_PI/180),9});
		th->setCenter({4.7+3*h,-3.1-2*h,9.2});

		h*=4;
	}

	return 0;
}
