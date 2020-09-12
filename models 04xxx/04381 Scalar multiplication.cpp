
/// Project:	FMI 3D (2013)
/// File:		04381 Scalar multiplication.cpp
/// Topic:		04 Vectors
/// Slide:		38
///	About:		Scalar multiplication of vectors
///
/// History:
///		2013.08.14/PB	Initial implementation


#include "fmi3d.h"

#define ANIMATION_TIME 4

Vector3D *v,*u;
CoordSys3D *cs;
Cube3D *c;
vect_t p,dv,du,dz;

void drawScene()
{
	c->drawImage();
	c->drawFrame();
	cs->drawImage();

	glDepthMask( GL_FALSE );
	glDisable( GL_LIGHTING );
	glDisable( GL_TEXTURE_2D );
	glEnable( GL_LINE_SMOOTH );
	glEnable( GL_LINE_STIPPLE );
	glLineStipple( 3,0b0101010101010101 );
	glLineWidth( 1.5 );
	glColor3f(1,0,0);

	glBegin( GL_LINES );
	{
		glVertex3f( dv.x,dv.y,dv.z );
		glVertex3f( dv.x,dv.y,0 );
		glVertex3f( dv.x,dv.y,0 );
		glVertex3f( dv.x,0,0 );
		glVertex3f( dv.x,dv.y,0 );
		glVertex3f( 0,dv.y,0 );

		glVertex3f( du.x,du.y,du.z );
		glVertex3f( du.x,du.y,0 );
		glVertex3f( du.x,du.y,0 );
		glVertex3f( du.x,0,0 );
		glVertex3f( du.x,du.y,0 );
		glVertex3f( 0,du.y,0 );
	}
	glEnd();
	glDisable( GL_LINE_STIPPLE );
	glDepthMask( GL_TRUE );

	viewDistance /=1.5;
	v->drawImage();
	u->drawImage();
	viewDistance *=1.5;



}


void drawFrames()
{
}


oxyz_t showO;

oxyz_t fixOxyz(oxyz_t oxyz)
{
	oxyz.oz = oxyz.ox.vmul(oxyz.oy);
	oxyz.oy = oxyz.oz.vmul(oxyz.ox);

	oxyz.ox = oxyz.ox.mul(1/oxyz.ox.distance());
	oxyz.oy = oxyz.oy.mul(1/oxyz.oy.distance());
	oxyz.oz = oxyz.oz.mul(1/oxyz.oz.distance());
	oxyz.o.w = 1;
	return oxyz;
}


bool show = false;
double showT = -10;
void showCube()
{
	show = !show;
	if( show )
	{
		showO.oy = du;
		showO.ox = dv;
		showO.oz = dz;
		showO = fixOxyz(showO);
		showO.o = showO.ox.add(showO.oy).add(showO.oz).mul(2);
	}
	else
	{
		showO = cs->oxyz;
		showO.o = {8,-8,2};
	}
	showT = t;
}

double r()
{
	return ( random( -1,1 )>0 )?2:-2;
}

void randomize()
{
	dv = {r()*random( 1,6 ),r()*random( 1,6 ),r()*random( 0,6 )};
	du = {r()*random( 1,6 ),r()*random( 1,6 ),r()*random( 0,6 )};

	du.z=-(dv.x*du.x+dv.y*du.y)/dv.z;
	dz = dv.vmul(du);

	du = du.mul(8/du.distance());
	dv = dv.mul(8/dv.distance());

	v->setDirection( dv );
	v->setLength( dv.distance()-0.7 );

	u->setDirection( du );
	u->setLength( du.distance()-0.7 );
	if( show )
	{
		showO.oy = du;
		showO.ox = dv;
		showO.oz = dz;
		showO = fixOxyz(showO);
		showO.o = showO.ox.add(showO.oy).add(showO.oz).mul(2);
		showT = t;
	}
}

int main()
{
	/// Open window
	if( !openWindow3D( "Scalar multiplication" ) ) return 1;
	noGround();

	/// Add buttons
	addButton( "buttons/button_show.png",GLFW_KEY_SPACE,showCube );
	addButton( "buttons/button_random.png",'R',randomize );
	addButton( new exitButton2D );

	viewZ = 2;
	viewDistance = 25;
	minViewBeta = -maxViewBeta;

	cs = new CoordSys3D( -10,10,-10,10,-7,7,false,1,0.7,L"XYZ" );
	cs->setCenter( {0,0,0} );

	v = new Vector3D( {0,0,1},0.7 );
	v->setColor( {1,0,0} );
	v->setCenter( cs->getCenter() );

	u = new Vector3D( {0,0,1},0.7 );
	u->setColor( {1,0,0} );
	u->setCenter( cs->getCenter() );

	c = new Cube3D(4);
	c->setColor({0.5,0.5,1});
	c->setCenter({8,-8,2});

	randomize();

	isRunning();
	/// Main loop
	while( isRunning() )
	{
		if( t-showT<ANIMATION_TIME )
		{
			double k = cos( ( ( t-showT )/ANIMATION_TIME )*M_PI/2 );
			c->oxyz.o = c->oxyz.o.mul(k).add(showO.o.mul( 1-k ));
			c->oxyz.ox = c->oxyz.ox.mul(k).add(showO.ox.mul( 1-k ));
			c->oxyz.oy = c->oxyz.oy.mul(k).add(showO.oy.mul( 1-k ));
			c->oxyz.oz = c->oxyz.oz.mul(k).add(showO.oz.mul( 1-k ));
			c->oxyz = fixOxyz(c->oxyz);
		}
	}
	return 0;
}

