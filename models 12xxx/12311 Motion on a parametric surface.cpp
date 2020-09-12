
/// Project:	FMI 3D (2013)
/// File:		12311 Motion on a parametric surface.cpp
/// Topic:		12 Trajectories
/// Slide:		31
///	About:		12311 Motion on a parametric surface
///
/// History:
///		2013.08.30/PB	Initial implementation
///     2013.09.13/PB   Simplifiedspheres


#include "fmi3d.h"

#define N 9
#define NA 120
#define NR 40

vect_t v[NA][NR];
vect_t n[NA][NR];
vect_t c[NA][NR];

Sphere3D *p[N];

void drawScene()
{
	glFrontFace(GL_CCW);
	for(int i=0; i<N; i++) p[i]->drawImage();

	glDisable(GL_TEXTURE_2D);

	glFrontFace(GL_CCW);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glEnable( GL_POLYGON_OFFSET_FILL );
	glPolygonOffset( 1,-1 );
	for(int r1 = 0; r1<NR-1; r1++)
	{
		int r2 = (r1+1)%NR;
		glBegin( GL_QUAD_STRIP);
		for(int a1 = 0; a1<=NA; a1++)
		{
			int a = a1%NA;
			glColor3f(  c[a][r1].x,c[a][r1].y,c[a][r1].z );
			glNormal3f( n[a][r1].x,n[a][r1].y,n[a][r1].z );
			glVertex3f( v[a][r1].x,v[a][r1].y,v[a][r1].z );

			glColor3f(  c[a][r2].x,c[a][r2].y,c[a][r2].z );
			glNormal3f( n[a][r2].x,n[a][r2].y,n[a][r2].z );
			glVertex3f( v[a][r2].x,v[a][r2].y,v[a][r2].z );
		}
		glEnd();
	}

	glDisable(GL_LIGHTING);
	glDepthMask(GL_FALSE);
	glColor4f(1,1,1,0.3);
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(0.5);
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	for(int r1 = 0; r1<NR-1; r1++)
	{
		int r2 = (r1+1)%NR;
		glBegin( GL_QUAD_STRIP);
		for(int a1 = 0; a1<=NA; a1++)
		{
			int a = a1%NA;
			glVertex3f( v[a][r1].x,v[a][r1].y,v[a][r1].z+0.03 );
			glVertex3f( v[a][r2].x,v[a][r2].y,v[a][r2].z+0.03 );
		}
		glEnd();
	}
	glDepthMask(GL_TRUE);

}


void drawFrames()
{
}


vect_t surfC(double u, double v)
{
	double a = 10*u*2*M_PI;
	double r = 5*v*2*M_PI;
	vect_t c = {0.5+0.5*sin(a+r),0.5+0.5*cos(a-r),sin(r)};
	c = c.mul(1-v/1.5);
	if( v>0.9) c = c.mul(0);
	return c;
}

vect_t surfV(double u, double v)
{
	double a = u*2*M_PI;
	double r = 10*v;
	double z = 3+(2.5*sin(6*a)+cos(1.5*r))/3.5+12/r;
	if( z>12 ) z = 12;
	if( v>0.9 ) { z = z+0.5; }
	if( v>0.95 ) { z = z-1; }
	return {r*cos(a),r*sin(a),z};
}

vect_t surfN(double u, double v)
{
	vect_t p = surfV(u,v);
	vect_t p1 = p.sub(surfV(u+0.1,v));
	vect_t p2 = p.sub(surfV(u,v+0.1));
	p = p1.vmul(p2);
	p = p.mul(-1/p.distance());
	return p;
}

int main()
{
	/// Open window
	if( !openWindow3D( "Motion on a parametric surface" ) ) return 1;
	changeBrightness();
	changeGround();

	/// Add buttons
	addButton( new exitButton2D );

	viewAlpha = 0;
	viewBeta = 0.5;
	viewZ = 4.5;
	viewDistance = 20;

	for(int i=0; i<N; i++)
	{
		p[i] = new Sphere3D(0.5,QUALITY_LOW);
		p[i]->setColor({0.9,0.9,1});
		p[i]->setShininess(100);
	}

	for(int r = 0; r<NR; r++)
		for(int a = 0; a<NA; a++)
		{
			v[a][r] = surfV(double(a)/NA,double(r)/NR);
			n[a][r] = surfN(double(a)/NA,double(r)/NR);
			c[a][r] = surfC(double(a)/NA,double(r)/NR);
		}
	/// Main loop
	isRunning();
	double t0=t,dt;
	while( isRunning() )
	{
		dt = t-t0;
		t0 = t;

		viewAlpha += dt/10;
		viewBeta += sin(t/2)/400;

		for(int i=0; i<N; i++)
		{
			double u = t/10+double(i)/N;
			double v = 0.6+0.25*sin(t+3*2*M_PI*i/N);
			p[i]->setCenter(surfV(u,v));
		}
	}
	return 0;
}

