
/// Project:	FMI 3D (2013)
/// File:		07432 YUV cube.cpp
/// Topic:		07 Colours
/// Slide:		43
///	About:		YUV cube
///
/// History:
///		2013.06.28/PB	Initial implementation
///     2013.09.13/PB   Changed arrow sizes


#include "fmi3d.h"

//#include <iostream>

#define N 5
#define ANIMATION_TIME 2
bool showVectors = false;

vect_t YUVtoRGB( vect_t p )
{
	double v = p.x;
	double u = p.y;
	double y = p.z;
	u -= 0.5;
	v -= 0.5;
	return {y+v/0.877,y-0.359*u-0.581*v,y+u/0.492};
}


class Quad3D : public Object3D
{
	public:
		Quad3D( vect_t p1, vect_t p2, vect_t p3, vect_t p4 );
	private:
		void P( vect_t p );
		void recurse( vect_t p1, vect_t p2, vect_t p3, vect_t p4, int level );
};

Quad3D::Quad3D( vect_t p1, vect_t p2, vect_t p3, vect_t p4 ) : Object3D()
{
	setLighting( false );

	/// Create display frame list of the cube
	glNewList( frameList,GL_COMPILE );
	glEndList();

	/// Create display list of the cube
	glNewList( imageList,GL_COMPILE );
	glBegin( GL_QUADS );
	recurse( p1,p2,p3,p4,N );
	glEnd();
	glEndList();
}

void Quad3D::recurse( vect_t p1, vect_t p2, vect_t p3, vect_t p4, int level )
{
	if( level )
	{
		vect_t p12 = p1.add( p2 ).mul( 0.5 );
		vect_t p23 = p2.add( p3 ).mul( 0.5 );
		vect_t p34 = p3.add( p4 ).mul( 0.5 );
		vect_t p41 = p4.add( p1 ).mul( 0.5 );
		vect_t pmm = p12.add( p34 ).mul( 0.5 );
		level--;
		recurse( p1,p12,pmm,p41,level );
		recurse( p2,p23,pmm,p12,level );
		recurse( p3,p34,pmm,p23,level );
		recurse( p4,p41,pmm,p34,level );
	}
	else
	{
		P( p1 );
		P( p2 );
		P( p3 );
		P( p4 );
	}
}


void Quad3D::P( vect_t p )
{
	vect_t c = YUVtoRGB( p );
	glColor3f( c.x,c.y,c.z );
	p.vertex();
}

class Triad3D : public Object3D
{
	public:
		Triad3D( vect_t p1, vect_t p2, vect_t p3 );
	private:
		void P( vect_t p );
		void recurse( vect_t p1, vect_t p2, vect_t p3, int level );
};

Triad3D::Triad3D( vect_t p1, vect_t p2, vect_t p3 ) : Object3D()
{
	setLighting( false );

	/// Create display frame list of the cube
	glNewList( frameList,GL_COMPILE );
	glEndList();

	/// Create display list of the cube
	glNewList( imageList,GL_COMPILE );
	glBegin( GL_TRIANGLES );
	recurse( p1,p2,p3,N );
	glEnd();
	glEndList();
}

void Triad3D::recurse( vect_t p1, vect_t p2, vect_t p3, int level )
{
	if( level )
	{
		vect_t p12 = p1.add( p2 ).mul( 0.5 );
		vect_t p23 = p2.add( p3 ).mul( 0.5 );
		vect_t p31 = p3.add( p1 ).mul( 0.5 );
		level--;
		recurse( p1,p12,p31,level );
		recurse( p2,p23,p12,level );
		recurse( p3,p31,p23,level );
		recurse( p12,p23,p31,level );
	}
	else
	{
		P( p1 );
		P( p2 );
		P( p3 );
	}
}

void Triad3D::P( vect_t p )
{
	vect_t c = YUVtoRGB( p );
	glColor3f( c.x,c.y,c.z );
	p.vertex();
}

class ColorCube3D : public Object3D
{
	public:
		ColorCube3D( int part );
		void drawImage( );
	private:
		Quad3D*  q[3];
		Triad3D* r[2];
};

ColorCube3D::ColorCube3D( int part ) : Object3D()
{
	switch( part )
	{
		case 0:
			q[0] = new Quad3D( {0,0,0}, {0,0,1}, {0,1,1}, {0,1,0} );
			q[1] = new Quad3D( {0,1,0}, {0,1,1}, {1,1,1}, {1,1,0} );
			q[2] = new Quad3D( {0,0,0}, {0,0,1}, {1,1,1}, {1,1,0} );

			r[0] = new Triad3D( {0,0,1}, {0,1,1}, {1,1,1} );
			r[1] = new Triad3D( {0,0,0}, {0,1,0}, {1,1,0} );
			break;
		case 1:
			q[0] = new Quad3D( {0,0,0}, {0,0,1}, {1,0,1}, {1,0,0} );
			q[1] = new Quad3D( {1,0,0}, {1,0,1}, {1,1,1}, {1,1,0} );
			q[2] = new Quad3D( {0,0,0}, {0,0,1}, {1,1,1}, {1,1,0} );

			r[0] = new Triad3D( {0,0,1}, {1,0,1}, {1,1,1} );
			r[1] = new Triad3D( {0,0,0}, {1,0,0}, {1,1,0} );
			break;
	}

	setSize( 6 );
}

void ColorCube3D::drawImage()
{
	if( !isVisible ) return;
	prepareImageProps();
	for( int i=0; i<3; i++ ) q[i]->drawImage();
	for( int i=0; i<2; i++ ) r[i]->drawImage();
	restoreImageProps();
}

ColorCube3D *q[2];
Vector3D *v[4];
Text3D *tv[4];

void drawScene()
{
	glPushMatrix();
	glTranslated( 0,0,6 );
	glRotated( 30*sin( t/5 ),1,0,0 );
	glTranslated( -3,-3,-3 );
	for( int i=0; i<2; i++ )
		q[i]->drawImage();

	if( showVectors )
		for( int i=0; i<4; i++ )
		{
			v[i]->drawImage();
			tv[i]->drawImage();
		}
	glPopMatrix();
}


void drawFrames()
{
	if( !showVectors ) return;
	glPushMatrix();
	glTranslated( 0,0,6 );
	glRotated( 30*sin( t/5 ),1,0,0 );
	glTranslated( -3,-3,-3 );
	for( int i=0; i<4; i++ )
	{
		v[i]->drawFrame();
	}
	glPopMatrix();
}

double splitT;
double splitH = 0;
void splitUnsplit()
{
	splitT = t;
	splitH = 4-splitH;
}

void showHide()
{
	showVectors = !showVectors;
}

int main()
{
	/// Open window
	if( !openWindow3D( "YUV cube" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();

	addButton( new Button2D( "buttons/button_show.png",'S',showHide ) );
	addButton( new Button2D( "buttons/button_cut.png",GLFW_KEY_SPACE,splitUnsplit ) );
	addButton( new exitButton2D );

	for( int i=0; i<2; i++ )
		q[i] = new ColorCube3D( i );

	for( int i=0; i<4; i++ )
	{
		v[i] = new Vector3D( {1,0,0},0.4 );
		v[i]->setColor( {0,0,0} );
		v[i]->setLength( 5.7 );
		tv[i] = new Text3D( L"R",systemFont3D );
		tv[i]->rotateGlobal( 90, {1,0,0} );
		tv[i]->rotateGlobal( 135,{0,0,1} );
		tv[i]->setColor( {0,0,1} );
	}
	v[1]->setDirection({0,1,0});
	v[2]->setDirection({0,0,1});
	v[3]->setDirection({0,0,1});
	tv[0]->setText(L"V");
	tv[1]->setText(L"U");
	tv[2]->setText(L"Y");
	tv[3]->setText(L"Y");

	viewZ = 4.5;
	minViewBeta = 0;
	double h=0;

	/// Main loop with automatic animation
	while( isRunning() )
	{
		double k = 1;
		if( t-splitT<ANIMATION_TIME )
		{
			k = cos( ( ( t-splitT )/ANIMATION_TIME )*M_PI/2 );
			h = h*k+splitH*( 1-k );
		}

		q[0]->identity();
		q[0]->translateLocal( {-h,0,0} );

		q[1]->identity();
		q[1]->translateLocal( {h,0,0} );

		v[0]->setCenter({0+h,-0.3,-0.3});
		v[1]->setCenter({-0.3-h,0,-0.3});
		v[2]->setCenter({-0.3-h,-0.3,0});
		v[3]->setCenter({-0.3+h,-0.3,0});

		tv[0]->setCenter({0+h+6.6,-0.4,-0.6});
		tv[1]->setCenter({-0.3-h,0+6.2,-0.6});
		tv[2]->setCenter({-h,-0.4,0+6.2});
		tv[3]->setCenter({+h,-0.4,0+6.2});
	}

	return 0;
}
