
/// Project:	FMI 3D (2013)
/// File:		22261 Fern leaf.cpp
/// Topic:		22 Fractals
/// Slide:		26
///	About:		A fractal fern leaf
///
/// History:
///		2013.05.25/PB	Initial implementation


#include "fmi3d.h"

//#include <iostream>


#define DEPTH 5
#define LENGTH 14
#define SIZE 0.04
#define N 14800 /*adjusted depending on LENGTH, DEPTH and SIZE*/
int style=0; // 0=flat fern; 1=3d fern; 2=tree


/// Fern3D
class Fern3D : public Object3D
{
	public:
		Fern3D( );
		void generateFern( double alpha, double beta, double gamma );
		void drawFern( double size, int steps );
	private:
		int n;
		double alpha;
		double beta;
		double gamma;
		vect_t p[N];
		vect_t q[N];
		unsigned char d[N];
		Object3D* turtle;
};


Fern3D::Fern3D( )
{
	turtle = new Object3D();
	turtle->identity();

	turtle->translateLocal( {0,0,2} );
	turtle->translateLocal( {1,0,0} );
	generateFern( 0,0,0 );
}

void Fern3D::generateFern( double alpha, double beta, double gamma )
{
	this->alpha=alpha;
	this->beta=beta;
	this->gamma=gamma;
	n=0;
	turtle->identity();
	drawFern( 5,0 );
	glDeleteLists( imageList,1 );
	glNewList( imageList,GL_COMPILE );
	{
		glColor3f( 0,1,0 );
		glLineWidth( 1 );
		glDisable( GL_LIGHTING );
		glDepthMask( GL_FALSE );
		glEnable( GL_LINE_SMOOTH );
		for( int i=0; i<n; i++ )
		{
			double lw=5-d[i]/2.0;
			glLineWidth( ( lw>0.5 )?lw:0.5 );
			glColor3f( 0,d[i]/25.0,0 );
			glBegin( GL_LINES );
			p[i].vertex();
			q[i].vertex();
			glEnd();
		}
		glDepthMask( GL_TRUE );
	}
	glEndList();
	frameList = imageList;
}

void Fern3D::drawFern( double size, int steps )
{
	if( n>=N ) return;
	if( steps>DEPTH ) return;
	if( ( size<SIZE ) ) return;

	turtle->rotateLocal( -beta/((style==2)?5:2), {0,1,0} );
	for( int i=0; i<LENGTH; i++ )
	{
		if( ( n<N ) && ( size>SIZE ) )
		{
			d[n] = i+5*steps;
			p[n] = turtle->oxyz.o;
			turtle->translateLocal( {0,0,size*0.5} );
			q[n] = turtle->oxyz.o;
			n++;
		}

		if( i<LENGTH-2 )
		{
			turtle->rotateLocal( random(0.5,1)*gamma, {0,0,1} );

			oxyz_t o = turtle->oxyz;
			turtle->rotateLocal( 50, {1,0,0} );
			drawFern( size*0.4,steps+1 );
			turtle->oxyz = o;
			turtle->rotateLocal( -50, {1,0,0} );
			drawFern( size*0.4,steps+1 );
			turtle->oxyz = o;
		}
		turtle->rotateLocal( alpha, {1,0,0} );
		turtle->rotateLocal( beta, {0,1,0} );
		size = size*0.8;
	}


}


Fern3D* fern;

void drawScene()
{
	fern->drawImage();
}


void drawFrames()
{
	fern->drawFrame();
}


void randomize()
{
	switch( style )
	{
		case 0: fern->generateFern(random(-20,20),0,0); break;
		case 1: fern->generateFern(random(-20,20),random(-40,40),0); break;
		case 2: fern->generateFern(random(-20,20),random(-30,30),random(0,180)); break;
	}
}

Button2D* fS;
void toggle()
{
	style = (style+1)%3;
	fS->setState(style);
	randomize();
}


int main()
{
	/// Open window
	if( !openWindow3D( "Fern leaf" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();

	/// Add buttons
	addButton( new Button2D("buttons/button_random.png",'R',randomize) );
	fS = new Button2D("buttons/button_next.png",GLFW_KEY_SPACE,toggle,style,3);
	addButton( new exitButton2D );

	fern = new Fern3D();
	viewZ = 5;
	viewDistance = 15;

	/// Main loop
	double t0=t;
	while( isRunning() )
	{
		viewAlpha+=(t-t0)/10.0;
		t0=t;
	}

	return 0;
}

