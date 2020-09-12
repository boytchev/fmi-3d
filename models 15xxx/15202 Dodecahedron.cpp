
/// Project:	FMI 3D (2013)
/// File:		15201 Dodecahedron.cpp
/// Topic:		15 Convex hulls
/// Slide:		20
///	About:		Dodecahedron decomposed to vertices and edges
///
/// History:
///		2013.08.20/PB	Initial implementation
///     2013.09.13/PB   Increased animation time


#include "fmi3d.h"

#define ANIMATION_TIME 8
#define V 20
#define E 30
#define F 12

double ve[3*V] = { 0.0, 0.8090, 0.5, -0.5, -0.8090, 0.0, 1.3095, 0.8093, -0.8093, -1.3095,
		0.8093, 1.3095, 0.0, -1.3095, -0.8093, 0.5, 0.8090, 0.0, -0.8090, -0.5,
		0.8507, 0.2629, -0.6882, -0.6882, 0.2629, 1.3769, 0.4255, -1.1139,
		-1.1139, 0.4255, 1.1139, -0.4255, -1.3769, -0.4255, 1.1139, 0.6882,
		-0.2629, -0.8507, -0.2629, 0.6882, 1.1135, 1.1135, 1.1135, 1.1135,
		1.1135, 0.2065, 0.2065, 0.2065, 0.2065, 0.2065, -0.2065, -0.2065,
		-0.2065, -0.2065, -0.2065, -1.1135, -1.1135, -1.1135, -1.1135, -1.1135
				 };
int ed[2*E]    = { 17, 18, 18, 19, 19, 20, 20, 16, 16, 17, 17, 12, 12, 8, 8, 13, 13, 18,
		19, 14, 14, 9, 9, 13, 1, 2, 2, 3, 3, 4, 4, 5, 5, 1, 1, 6, 6, 15, 15, 10,
		10, 5, 20, 15, 10, 14, 9, 4, 3, 8, 2, 7, 7, 11, 7, 12, 11, 6, 11, 16
				 };

Object3D o0,o1;
double k = 0;

int mode = 0;
int mode2 = -1;
int modes = 5;

class Platonic3D : public Object3D
{
	public:
		Platonic3D( double sc );
		void drawImage();
	private:
		vect_t v0[V];
		vect_t v1[V];
		Sphere3D *v[V];
		oxyz_t e0[E];
		oxyz_t e1[E];
		Cylinder3D *e[E];
};

oxyz_t fixOxyz( oxyz_t oxyz )
{
	oxyz.oz = oxyz.ox.vmul( oxyz.oy );
	oxyz.oy = oxyz.oz.vmul( oxyz.ox );

	oxyz.ox = oxyz.ox.mul( 1/oxyz.ox.distance() );
	oxyz.oy = oxyz.oy.mul( 1/oxyz.oy.distance() );
	oxyz.oz = oxyz.oz.mul( 1/oxyz.oz.distance() );
	oxyz.o.w = 1;
	return oxyz;
}

void Platonic3D::drawImage()
{
	if( !isVisible ) return;
	prepareImageProps();
	for( int i=0; i<V; i++ )
	{
		v[i]->setCenter( v0[i].mul(1-k).add(v1[i].mul(k)) );
		v[i]->drawImage();
	}
	for( int i=0; i<E; i++ )
	{
		double kk = pow(k,2.5);
		oxyz_t o;
		double len = e0[i].oz.distance();
		o.ox = e0[i].ox.mul(1-kk).add(e1[i].ox.mul(kk));
		o.oy = e0[i].oy.mul(1-kk).add(e1[i].oy.mul(kk));
		o.oz = e0[i].oz.mul(1-kk).add(e1[i].oz.mul(kk));
		o.o  = e0[i].o.mul(1-k).add(e1[i].o.mul(k));
		o = fixOxyz(o);
		o.ox = o.ox.mul(0.03);
		o.oy = o.oy.mul(0.03);
		o.oz = o.oz.mul(len*(1-k)+0.6*k);
		e[i]->oxyz = o;
		e[i]->drawImage();
	}
	restoreImageProps();
}

Platonic3D::Platonic3D( double sc ) : Object3D()
{
	setSize( 5 );
	setCenter( {0,0,4} );

	glNewList( frameList,GL_COMPILE );
	glEndList();

	glNewList( imageList,GL_COMPILE );
	glEndList();

	for( int i=0; i<V; i++ )
	{
		v0[i] = {ve[i]*sc,ve[i+V]*sc,ve[i+2*V]*sc};
		v1[i] = {(i%10-(V/2-1)/2.0)/4.0,0,0.3+0.3*int(i/10)};
		v[i] = new Sphere3D( 0.1 );
		v[i]->setColor( {1,0.5,0} );
		v[i]->setShininess( 50 );
		v[i]->setCenter( v1[i] );
	}

	for( int i=0; i<E; i++ )
	{
		int ex1 = ed[2*i]-1;
		int ex2 = ed[2*i+1]-1;
		vect_t v1 = {ve[ex1],ve[ex1+V],ve[ex1+2*V]};
		vect_t v2 = {ve[ex2],ve[ex2+V],ve[ex2+2*V]};
		v1 = v1.mul( sc );
		v2 = v2.mul( sc );

		oxyz_t oxyz;
		oxyz.oz = v2.sub( v1 );
		oxyz.rebuild();
		oxyz.o = v1;
		oxyz.o.w = 1;
		oxyz.ox = oxyz.ox.mul( 0.03 );
		oxyz.oy = oxyz.oy.mul( 0.03 );
		oxyz.oz = oxyz.oz.mul( v2.distance( v1 ) );

		e[i] = new Cylinder3D( 1,1,false );
		e1[i] = e[i]->oxyz;
		e[i]->setColor( {0,0,0.5} );
		e[i]->setShininess( 50 );
		e[i]->oxyz = oxyz;
		e0[i]=oxyz;

		e1[i].o = {(i%10-(E/3-1)/2.0)/6.0,0.2*int(i/10)-0.2,-0.5};
	}
}


Platonic3D *p;

void drawScene()
{
	p->drawImage();
}


void drawFrames()
{
}

double kT = 0;
double kMax = 0;
void toggleK()
{
	kT = t;
	kMax = 1-kMax;
}


int main()
{
	/// Open window
	if( !openWindow3D( "Dodecahedron" ) ) return 1;

	/// Add buttons
	addButton( new Button2D( "buttons/button_next.png",GLFW_KEY_SPACE,toggleK ));
	addButton( new exitButton2D );

	viewZ = 3;
	viewBeta = 0.1;
	viewDistance = 15;
	viewAlpha = -M_PI/2;

	p = new Platonic3D( 0.5 );
	o0.oxyz = p->oxyz;
	o1.oxyz = p->oxyz;

	/// Main loop
	isRunning();
	double t0=t;
	while( isRunning() )
	{
		double dt = 30*( t-t0 );
		t0=t;

		viewAlpha += 0.003*sin( t );

		if( t-kT<ANIMATION_TIME )
		{
			double kk = 0.5+0.5*cos( ( ( t-kT )/ANIMATION_TIME )*M_PI/2 );
			k = k*kk+kMax*( 1-kk );
		}

		o0.rotateLocal( dt, {1,1,0} );
		o0.rotateLocal( 1.5*dt, {0,1,1} );
		o0.rotateLocal( 1.2*dt, {1,0,1} );

		oxyz_t o;
		o.ox = o1.oxyz.ox.mul(k).add(o0.oxyz.ox.mul(1-k));
		o.oy = o1.oxyz.oy.mul(k).add(o0.oxyz.oy.mul(1-k));
		o.oz = o1.oxyz.oz.mul(k).add(o0.oxyz.oz.mul(1-k));
		o.o = o1.oxyz.o.mul(k).add(o0.oxyz.o.mul(1-k));
		o = fixOxyz(o);
		p->oxyz = o;
	}
	return 0;
}

