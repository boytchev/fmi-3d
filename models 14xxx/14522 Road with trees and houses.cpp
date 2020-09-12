
/// Project:	FMI 3D (2013)
/// File:		14522 Road with trees and houses.cpp
/// Topic:		14 Orientation
/// Slide:		52
///	About:		A random road with trees and houses
///
/// History:
///		2013.08.14/PB	Initial implementation
///     2013.09.13/PB   Fixed shininess


#include "fmi3d.h"


#define dN 20
#define N 500
#define T (N/dN)

Sphere3D *p;
bool isTree1[N];
bool isTree2[N];

vect_t p1[N],p2[N];
int n=0;
double t0,k=0.99;

#define S 10
double s[S],o[S];

void drawHouse(vect_t v)
{
	if( inShadow ) return;
	if( v.distance()<0.1 ) return;

	glPushMatrix();
	glTranslated(v.x,v.y,0);
	glRotated(180*cos(v.x*v.y),0,0,1);
	glScaled(0.75,0.75,0.75);
	{
		// walls
		glColor3f(1,1,0.9);
		glTranslated(0,0,0.5);
		glCallList(idUnitCube3D);

		glDisable(GL_LIGHTING);

		// sand
		glColor4f(1,1,0,0.3);
		glPushMatrix();
		glTranslated(0,0,-0.48);
		glScaled(1.6,1.6,0);
		glCallList(idUnitCube3D);
		glPopMatrix();

		// front window
		glColor3f(0,0.5,1);
		glPushMatrix();
		glTranslated(0.5,0.2,0);
		glScaled(0.1,0.3,0.4);
		glCallList(idUnitCube3D);
		glPopMatrix();

		// side windows
		glColor3f(0,0.5,1);
		glPushMatrix();
		glTranslated(0.2,0,0);
		glScaled(0.3,1.05,0.4);
		glCallList(idUnitCube3D);
		glPopMatrix();
		glPushMatrix();
		glTranslated(-0.2,0,0);
		glScaled(0.3,1.05,0.4);
		glCallList(idUnitCube3D);
		glPopMatrix();

		// door
		glColor3f(0.5,0,0);
		glPushMatrix();
		glTranslated(0.5,-0.2,-0.5);
		glScaled(0.1,0.2,1);
		glCallList(idUnitCube3D);
		glPopMatrix();

		// chimney
		glColor3f(0.5,0,0);
		glPushMatrix();
		glTranslated(0,0.3,0);
		glScaled(0.1,0.1,2.5);
		glCallList(idUnitCube3D);
		glPopMatrix();

		// roof
		glEnable(GL_LIGHTING);
		glColor3f(0.5,0,0);
		glScaled(0.6,0.6,1);
		glBegin(GL_TRIANGLE_FAN);
		glNormal3f(0,0,1);
		glVertex3f(0,0,1.3);
		glNormal3f(+1,+1,0.5);
		glVertex3f(+1,+1,0.5);
		glNormal3f(-1,+1,0.5);
		glVertex3f(-1,+1,0.5);
		glNormal3f(-1,-1,0.5);
		glVertex3f(-1,-1,0.5);
		glNormal3f(+1,-1,0.5);
		glVertex3f(+1,-1,0.5);
		glNormal3f(+1,+1,0.5);
		glVertex3f(+1,+1,0.5);
		glEnd();
	}
	glPopMatrix();
}

void drawTree(vect_t v)
{
	if( inShadow ) return;
	if( v.distance()<0.1 ) return;

	double h = 1.3+0.2*sin(v.x+v.y);

	glPushMatrix();
	glTranslated(v.x,v.y,0);
	glColor3f(0.5,0,0);
	{
		glPushMatrix();
		glScaled(0.1*h,0.1*h,h);
		glCallList(idUnitCone3D[QUALITY_LOW]);
		glPopMatrix();
	}
	glTranslated(0,0,h);
	glColor3f(0.3+0.3*sin(3*v.y),0.8+0.2*cos(2*v.x),0);
	{
		glPushMatrix();
		glScaled(0.3*h,0.3*h,0.3*h);
		glCallList(idUnitSphere3D[QUALITY_LOW]);
		glPopMatrix();
	}
	glPopMatrix();
}

void drawScene()
{
	glDepthMask( GL_FALSE );
	glDisable(GL_DEPTH_TEST);
	glDisable( GL_TEXTURE_2D );
	glDisable( GL_LIGHTING );
	glColor3f( 0,0,0 );
	glEnable( GL_POLYGON_SMOOTH );
	glBegin( GL_QUAD_STRIP );
	for( int i=n; i<n+N; i++ )
	{
		p1[i%N].vertex();
		p2[i%N].vertex();
	}
	glEnd();

	glEnable(GL_LINE_SMOOTH);
	glLineWidth(1.5);
	glColor3f( 1,1,1 );
	glBegin( GL_LINES );
	int n5 = 6*(n/6);
	for( int i=n5+6; i<n+N; i+=3 )
	{
		p1[i%N].add(p2[i%N]).mul(0.5).vertex();
	}
	glEnd();
	glLineWidth(1);
	glBegin( GL_LINE_STRIP );
	for( int i=n+1; i<n+N; i++ )
	{
		p1[i%N].add(p2[i%N].mul(12)).mul(1.0/13).vertex();
	}
	glEnd();
	glBegin( GL_LINE_STRIP );
	for( int i=n+1; i<n+N; i++ )
	{
		p2[i%N].add(p1[i%N].mul(12)).mul(1.0/13).vertex();
	}
	glEnd();

	glDepthMask( GL_TRUE );
	glEnable(GL_DEPTH_TEST);

	glEnable( GL_LIGHTING );
	for( int i=n; i<n+N; i++ ) if(i%dN==0)
	{
		vect_t v = p1[i%N].sub(p2[i%N]).mul(0.3);
		vect_t v1 = p1[i%N].add(v);
		vect_t v2 = p2[i%N].sub(v);
		if( isTree1[(i%N)/dN] ) drawTree(v1); else drawHouse(v1.add(v.mul(3)));
		if( isTree2[(i%N)/dN] ) drawTree(v2); else drawHouse(v2.sub(v.mul(3)));
	}


	glEnable( GL_CULL_FACE );
	glCullFace( GL_FRONT );
	p->drawImage();
	glDisable( GL_CULL_FACE );
}


void drawFrames()
{
}


void fd( double dist )
{
	p->translateLocal( {dist/10.0,0,0} );
}

void rt( double dist )
{
	p->rotateLocal( dist, {0,0,-1} );
}

void lt( double dist )
{
	p->rotateLocal( dist, {0,0,1} );
}

double f()
{
	double y = 0;
	for(int i=0; i<S; i++)
		y = y+sin(s[i]*t+o[i]);
	return 22*y;
}

int main()
{
	useCustomFogColor = true;
	customFogColor[0] = 0.5;
	customFogColor[1] = 0.7;
	customFogColor[2] = 1;

	/// Open window
	if( !openWindow3D( "Road with trees and houses" ) ) return 1;
	changeGround();

	/// Add buttons
	addButton( new exitButton2D );

	p = new Sphere3D( 0.5, QUALITY_HIGH );
	p->setColor( {0.5,0.75,1,0.75} );
	p->setShininess( 30 );
	p->setCenter( {0,0,0} );
	p->rotateLocal( 90, {0,0,1} );

	for(int i=0; i<S; i++)
	{
		s[i]=random(0.5,5);
		o[i]=random(0,2*M_PI);
	}
	/// Main loop
	isRunning();
	t0=t;
	while( isRunning() )
	{
		double dt = (t-t0);
		viewAlpha += (t-t0)/10;
		t0 = t;
		fd(15*dt);
		lt(f()*dt);

		Object3D o;
		o.oxyz = p->oxyz;
		o.translateLocal({0,0.6,0});
		p1[n%N] = o.getCenter();
		o.translateLocal({0,-1.2,0});
		p2[n%N] = o.getCenter();

		if( n%dN==0 )
		{
			isTree1[(n%N)/dN] = (random(0,100)<80);
			isTree2[(n%N)/dN] = (random(0,100)<80);
		}
		n++;

		viewX = viewX*k+(1-k)*p->getCenter().x;
		viewY = viewY*k+(1-k)*p->getCenter().y;
		viewDistance = viewDistance*k+(1-k)*(16-8*sin(t/4));
	}

	return 0;
}

