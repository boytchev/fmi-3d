
/// Project:	FMI 3D (2013)
/// File:		18281 Heart.cpp
/// Topic:		18 Ray tracing
/// Slide:		28
///	About:		Generation of 3D heart shape

#include "fmi3d.h"

#define ANIMATION_TIME 20
#define N 10000
#define I 600
vect_t p1[N+1],n1[N+1],p2[N+1],n2[N+1];
double stripT;

bool closer(double a, double b)
{
	return fabs(a)<fabs(b);
}

int modes = 4;
int mode = 0;

double inHeart( vect_t p )
{
	double m;
	switch( mode )
	{
		case 0:
			m = 2*p.x*p.x+p.y*p.y+p.z*p.z-1;
			return m*m*m-p.z*p.z*p.z*(p.x*p.x/10+p.y*p.y);
		case 1:
			m = 2;
			return pow(p.x,m)+pow(p.y,m)+pow(p.z,m)-1;
		case 2:
			m = 64;
			return pow(p.x,m)+pow(p.y,m)+pow(p.z,m)-1;
		case 3:
			return fabs(p.x*0.8)+fabs(p.y*0.8)+fabs(p.z)-1.4;
	}
	return 1;
}

vect_t onHeart(double a, double b)
{
	if( b>+M_PI/2 ) b=+M_PI/2;
	if( b<-M_PI/2 ) b=-M_PI/2;

	vect_t p = {cos(a)*cos(b),sin(a)*cos(b),sin(b)};
	double m1 = 0.01;
	double m2 = 5;
	double m = (m1+m2)/2;
	while( m2-m1>0.00001 )
	{
		if( inHeart(p.mul(m))>0 )
			m2 = m;
		else
			m1 = m;
		m = (m1+m2)/2;
	}
	return p.mul(m);
}

void onVertex(int i, double a, double b, double bb)
{
	vect_t p = onHeart(a,b);
	vect_t u = onHeart(a+0.01,b).sub(p);
	vect_t v = onHeart(a,b+0.01).sub(p);
	vect_t n = u.vmul(v);

	n1[i] = n;//.mul(1/n.distance());
	p1[i] = p.mul(4).add({0,0,5});

	p = onHeart(a,bb);
	u = onHeart(a+0.01,bb).sub(p);
	v = onHeart(a,bb+0.01).sub(p);
	n = u.vmul(v);

	n2[i] = n;//.mul(1/n.distance());
	p2[i] = p.mul(4).add({0,0,5});
}


void regenerate()
{
	double db = 0.07;
	for(int i=0; i<=N; i++)
	{
		double b = (double(i)/N-0.5)*M_PI;
		double a = i*0.017;
		onVertex(i,a,b,b+db);
	}
}

void drawScene()
{
	glColor3f(1,0,0);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);

	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glBegin(GL_QUAD_STRIP);
	int n;
	if( t-stripT>ANIMATION_TIME )
		n = N;
	else
		n = int(N*(0.5+0.5*sin(((t-stripT)/ANIMATION_TIME-0.5)*M_PI)));
	for(int i=0; i<n; i++)
	{
		double c=1;
		if(i<=I) c=double(i)/I;
		if(i>=N-I) c=double((N-i))/I;

		double z = 0;
		double k = N-n;
		if( k>100 ) k=100;
		if( i>n-k ) z = 0.2*(i-n+k);

		glColor4f(1,0,0,c);
		glNormal3dv(&n1[i].x);
		vect_t p = p1[i].add({0,0,z});
		glVertex3dv(&p.x);
		glNormal3dv(&n2[i].x);
		p = p2[i].add({0,0,z});
		glVertex3dv(&p.x);
	}
	glEnd();
}


void drawFrames()
{
}


void sameShape()
{
	stripT = t;
}

Button2D *newS;
void nextShape()
{
	mode = (mode+1)%modes;
	newS->setState(mode);
	regenerate();
	sameShape();
}

int main()
{
	/// Open window
	if( !openWindow3D( "Heart" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();
	changeBrightness();

	/// Add buttons
	addButton( "buttons/button_show.png",GLFW_KEY_SPACE,sameShape);
	newS = new Button2D("buttons/button_next.png",'N',nextShape,mode,modes);
	addButton( new exitButton2D );

	viewZ = 5;

	regenerate();

	/// Main loop
	stripT = t;
	while( isRunning() )
	{
	}
	return 0;
}

