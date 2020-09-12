
/// Project:	FMI 3D (2013)
/// File:		07322 RGB to grayscale.cpp
/// Topic:		07 Colours
/// Slide:		32
///	About:		Converting RGB to several grayscales
///
/// History:
///		2013.06.28/PB	Initial implementation
///     2013.09.13/PB   Changed arrow sizes


#include "fmi3d.h"

//#include <iostream>

#define N 5
#define ANIMATION_TIME 2

int mode = 0;
bool showVectors = false;

double min( vect_t p )
{
	double a = p.x;
	if( a>p.y ) a=p.y;
	if( a>p.z ) a=p.z;
	return a;
}
double max( vect_t p )
{
	double a = p.x;
	if( a<p.y ) a=p.y;
	if( a<p.z ) a=p.z;
	return a;
}
vect_t RGBtoGrayscale( vect_t p )
{
	double c = 0;
	switch( mode )
	{
		case 0: c = ( p.x+p.y+p.z )/3; break;	//average
		case 1: c = 0.2126*p.x+0.7152*p.y+0.0722*p.z; break; // luminosity
		case 2: c = ( min( p )+max( p ) )/2; // lightness
	}
	return {c,c,c};
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
	vect_t c = RGBtoGrayscale( p );
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
	vect_t c = RGBtoGrayscale( p );
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

ColorCube3D *q[2][3];
Vector3D *v[4];
Text3D *tv[4];

void drawScene()
{
	glPushMatrix();
	glTranslated( 0,0,6 );
	glRotated( 30*sin( t/5 ),1,0,0 );
	glTranslated( -3,-3,-3 );
	for( int i=0; i<2; i++ )
		q[i][mode]->drawImage();

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

Button2D *nM;
void nextMode()
{
	mode = ( mode+1 )%3;
	nM->setState( mode );
}

int main()
{
	useCustomFogColor = true;

	/// Open window
	if( !openWindow3D( "RGB to grayscale" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();

	addButton( new Button2D( "buttons/button_show.png",'S',showHide ) );
	nM = new Button2D( "buttons/button_next.png",'G',nextMode,mode,3 );
	addButton( new Button2D( "buttons/button_cut.png",GLFW_KEY_SPACE,splitUnsplit ) );
	addButton( new exitButton2D );

	for( mode=2; mode>=0; mode-- )
		for( int i=0; i<2; i++ )
			q[i][mode] = new ColorCube3D( i );
	mode = 0;

	for( int i=0; i<4; i++ )
	{
		v[i] = new Vector3D( {1,0,0},0.4 );
		v[i]->setColor( {0,0,0} );
		v[i]->setLength( 5.7 );
		tv[i] = new Text3D( L"R",systemFont3D );
		tv[i]->rotateGlobal( 90, {1,0,0} );
		tv[i]->rotateGlobal( 135,{0,0,1} );
	}
	v[1]->setDirection({0,1,0});
	v[2]->setDirection({0,0,1});
	v[3]->setDirection({0,0,1});
	tv[0]->setText(L"R");
	tv[0]->setColor( {1,0,0} );
	tv[1]->setText(L"G");
	tv[1]->setColor( {0,1,0} );
	tv[2]->setText(L"B");
	tv[2]->setColor( {0,0,1} );
	tv[3]->setText(L"B");
	tv[3]->setColor( {0,0,1} );

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

		q[0][mode]->identity();
		q[0][mode]->translateLocal( {-h,0,0} );

		q[1][mode]->identity();
		q[1][mode]->translateLocal( {h,0,0} );

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
