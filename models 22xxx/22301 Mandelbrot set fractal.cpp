
/// Project:	FMI 3D (2013)
/// File:		22301 Mandelbrot set fractal.cpp
/// Topic:		22 Fractals
/// Slide:		30
///	About:		Mandelbrot set fractal
///
/// History:
///		2013.05.25/PB	Initial implementation


#include "fmi3d.h"
#include "stdio.h"
#include "string.h"

//#include <iostream>

#define N 900
#define M 450

int maxDepth;
double colDepth;
Button2D* rB;
int K=0;

/// Pixel
class MandelbrotSet2D : public Object3D
{
	public:
		MandelbrotSet2D( );
		void generate( double cx, double cy, double scale );
		void drawImage();
		vect_t calcColor( double x, double y );
		vect_t col( int i );
		double sca;
		double cx;
		double cy;
	private:
		vect_t P[N][M];
};

MandelbrotSet2D::MandelbrotSet2D( ) : Object3D()
{
	maxDepth = 100;
	colDepth = 10;
	generate( -0.6,0,100 );
}

vect_t MandelbrotSet2D::calcColor( double x,double y )
{
	double az=0;
	double bz=0;

	for( int i=0; i<maxDepth; i++ )
	{
		double aa=az*az;
		double bb=bz*bz;
		if( aa+bb>4 ) return col( i );
		double az1=aa-bb+x;
		bz=2*az*bz+y;
		az=az1;
	}
	return {0,0,0};
}

vect_t MandelbrotSet2D::col( int i )
{
	double x = i/10.0;//sqrt(double(i));
	double r = 0.5+0.5*cos( x/colDepth*M_PI/2 );
	double g = 0.5+0.5*cos( 1.41*x/colDepth*M_PI/2 );
	double b = 0.5+0.5*sin( 2*x/colDepth*M_PI/2 );
	return {r,g,b};
}

void MandelbrotSet2D::generate( double cx, double cy, double scale )
{
	sca = scale;
	this->cx = cx;
	this->cy = cy;
	for( int x=0; x<N; x++ )
	{
		char res[100];
		sprintf( res,"Mandelbrot set fractal - rendering %.1f%%",100.0*x/N );
		glfwSetWindowTitle(res);
		for( int y=0; y<M; y++ )
			P[x][y]=calcColor( ( x-N/2.0 )/scale+cx, ( y-M/2.0 )/scale+cy );
	}
	glfwSetWindowTitle("Mandelbrot set fractal");
}

void MandelbrotSet2D::drawImage()
{
	//prepareImageProps();
	glPointSize( 0.5 );
	glDisable( GL_POINT_SMOOTH );
	glBegin( GL_POINTS );
	glDisable( GL_LIGHTING );
	for( int x=0; x<N; x++ )
		for( int y=0; y<M; y++ )
		{
			glColor3d( P[x][y].x,P[x][y].y,P[x][y].z );
			glVertex3d( x-N/2,y-M/2,1 );
		}
	glEnd();
	//restoreImageProps();
}

MandelbrotSet2D* mandel;
Font2D* font;
Text2D* text;

void drawScene()
{
	text->drawImage();
	glDisable( GL_LIGHTING );
	glDisable( GL_TEXTURE_2D );
	double s = 1.0/( 1000/viewDistance );
	glScaled( s,s,s );
	mandel->drawImage();
}


void drawFrames()
{
}

void reset()
{
	maxDepth = 30;
	colDepth = 5;
	mandel->generate( -0.6,0,100 );
	text->setText( L"1:10^2" );
	text->setCenter( {-6/8.0,-5,1} );
	K=0;
	rB->setState(K);
}

void GLFWCALL onMyMouseMove( int x, int y )
{
	buttonMouseOver( x,y );
}

void GLFWCALL onMyMousePressed( int button, int action )
{
	/// Scene buttons
	if( action==GLFW_PRESS )
		if( onButtonMousePressed( button ) )
			return; /// exit if button is processed

	/// Zoom fractal
	if( action==GLFW_PRESS )
	{
		/// Zoom in
		if( ( button==GLFW_MOUSE_BUTTON_LEFT ) && ( ( mandel->sca )<100000000000000 ) )
		{
			int xi,yi;
			double x,y;
			glfwGetMousePos( &xi,&yi );
			x = xi-windowWidth/2.0;
			y = windowHeight/2.0-yi;
			x = ( x/mandel->sca ) + ( mandel->cx );
			y = ( y/mandel->sca ) + ( mandel->cy );

			maxDepth = 2*1.5*maxDepth;
			colDepth = 1.15*colDepth;
			K++;
			rB->setState(K);
			mandel->generate( x,y,10*( mandel->sca ) );
		}
		/// Zoom out
		if( ( button==GLFW_MOUSE_BUTTON_RIGHT ) && ( ( mandel->sca )>100 ) )
		{
			maxDepth = maxDepth/2/1.5;
			colDepth = colDepth/1.15;
			mandel->generate( mandel->cx,mandel->cy,0.1*( mandel->sca ) );
			K--;
			rB->setState(K);
		}
		char res8[100];
		wchar_t res[100];
		sprintf( res8,"1:10^%d",int( log10( mandel->sca ) ) );
		int len=strlen( res8 );
		for( int i=0; i<len+1; i++ ) res[i]=res8[i];
		text->setText( res );
		text->setCenter( {-len/8.0,-5,1} );

	}
}

void GLFWCALL onMyKeyPressed( int key, int action )
{
	/// Scene buttons
	if( action==GLFW_PRESS )
		if( onButtonKeyPressed( key ) )
			return; /// exit if key is processed
}

int main()
{
	/// Open window
	if( !openWindow2D( "Mandelbrot fractal set" ) ) return 1;
	noGround();

	/// Add buttons
	rB = new Button2D( "buttons/button_toggle.png",GLFW_KEY_SPACE,reset,0,13 );
	addButton( new exitButton2D );

	glfwSetMousePosCallback( onMyMouseMove );
	glfwSetMouseButtonCallback( onMyMousePressed );
	glfwSetKeyCallback( onMyKeyPressed );

	/// Create pixels
	mandel = new MandelbrotSet2D( );
	font = new Font2D( "Consolas",24 );
	text = new Text2D( L"Scale",font );
	text->setCenter( {0,-5,10} );
	text->setColor( {0,0,0.4} );

	reset();

	/// Main loop
	while( isRunning() )
	{
	}

	return 0;
}

