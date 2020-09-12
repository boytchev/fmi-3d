
/// Project:	FMI 3D (2013)
/// File:		10071 Antibiotic pills
/// Topic:		10 Composite objects
/// Slide:		07
///	About:		Antibiotic pills as composite objects
///
/// History:
///		2013.08.16/PB	Initial implementation


#include "fmi3d.h"


class Pill3D : public Object3D
{
	public:
		Pill3D( double radius, double length );
		void drawImage();
		void randomize();
	private:
		Sphere3D *s1,*s2;
		Cylinder3D *c1,*c2;
};

Pill3D::Pill3D( double radius, double length ) : Object3D()
{
	double k = 1;//.05;
	double m = 1;

	setShininess( 50 );
	setSize(3);

	s1 = new Sphere3D( radius );
	s1->setCenter( {length/2,0,0} );

	s2 = new Sphere3D( radius*k );
	s2->setCenter( {-length/2,0,0} );

	c1 = new Cylinder3D( radius*m, length/2, false );
	c1->setCenter( {0,0,0} );
	c1->rotateLocal(90,{0,1,0});

	c2 = new Cylinder3D( radius*k*m, length/2, false );
	c2->setCenter( {0,0,0} );
	c2->rotateLocal(-90,{0,1,0});

	randomize();

	/// Create empty display frame list of the pawn
	glNewList( frameList,GL_COMPILE );
	glEndList();

	/// Create display list of the pawn
	glNewList( imageList,GL_COMPILE );
	glEndList();
}

void Pill3D::drawImage()
{
	if( !isVisible ) return;
	prepareImageProps();

	s1->drawImage();
	s2->drawImage();
	c1->drawImage();
	c2->drawImage();

	restoreImageProps();
}

void Pill3D::randomize( )
{
	vect_t col1 = {random(0,1),random(0,1),random(0,1)};
	vect_t col2 = {random(0,1),random(0,1),random(0,1)};

	s1->setColor(col1);
	s2->setColor(col2);
	c1->setColor(col1);
	c2->setColor(col2);
}


#define ANIMATION_TIME 2
#define N 30
Pill3D *p[N];

void drawScene()
{
	for( int i=0; i<N; i++ )
		p[i]->drawImage();
}


void drawFrames()
{
}


void randomize()
{
	for( int i=0; i<N; i++)
	{
		bool ok=false;
		while( !ok )
		{
			p[i]->setCenter( {random(-10,10),random(-10,10),0.58} );

			ok = true;
			for(int j=0; j<i; j++) if( p[i]->getCenter().distance(p[j]->getCenter()) < 2 ) ok=false;
		}
		p[i]->rotateLocal(random(0,360),{0,0,1});
		p[i]->randomize();
	}
}

int main()
{
	if( !openWindow3D( "Antibiotic pills" ) ) return 1;

	addButton( new Button2D("buttons/button_random.png",GLFW_KEY_SPACE,randomize));
	addButton( new exitButton2D() );

	viewZ = -1;

	for( int i=0; i<N; i++)
	{
		p[i] = new Pill3D( 0.2, 0.5);
	}
	randomize();

	double t0=t;
	while( isRunning() )
	{
		viewAlpha += ( t-t0 )/20;
		t0 = t;
	}
	return 0;
}

