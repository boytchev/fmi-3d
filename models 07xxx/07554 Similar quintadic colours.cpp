
/// Project:	FMI 3D (2013)
/// File:		07554 Similar quintadic colours.cpp
/// Topic:		07 Colours
/// Slide:		55
///	About:		Five similar colours
///
/// History:
///		2013.07.01/PB	Initial implementation
///     2013.09.13/PB   Fixed handler's width


#include "fmi3d.h"

#define N 10
#define MAX_OBJECTS 6
Circle2D* obj[MAX_OBJECTS];
Cube3D* cbj[MAX_OBJECTS];

double coss( double x )
{
	x = x+36000;
	x = x-360*int( x/360 );
	int d = x/60;
	switch( d )
	{
		case 0: case 5: return 1;
		case 2: case 3: return 0;
		case 1: return 0.5+0.5*cos( ( x-60 )/60*M_PI );
		case 4: return 0.5-0.5*cos( ( x-240 )/60*M_PI );
	}
	return 0;
}

vect_t HSVtoRGBColor( double h, double s, double v )
{
	double c = v*s; // chroma
	double m = v-c;

	return
	{
		m+c*coss( h ),
		m+c*coss( h+120 ),
		m+c*coss( h+240 )
	};
}

void HSVtoRGB( double h, double s, double v )
{
	vect_t c = HSVtoRGBColor( h,s,v );

	glColor3f( c.x,c.y,c.z );
	glVertex3f( s*cos( h*M_PI/180.0 ),s*sin( h*M_PI/180.0 ),1 );
}

class ColorWheel2D : public Object3D
{
	public:
		ColorWheel2D( );
	private:
};

ColorWheel2D::ColorWheel2D( ) : Object3D()
{
	setSize( 5 );
	setLighting( false );

	glNewList( frameList, GL_COMPILE );
	glColor3f( 0,0,0 );
	glEnable( GL_LINE_SMOOTH );
	glBegin( GL_LINE_LOOP );
	for( double i=0; i<2*M_PI; i+=0.1 )
	{
		glVertex3f( cos( i ),sin( i ),1 );
	}
	glEnd();
	glEndList();

	glNewList( imageList, GL_COMPILE );
	glBegin( GL_QUADS );
	for( int j=0; j<N; j++ )
	{
		for( int i=0; i<360; i+=1 )
		{
			// top
			HSVtoRGB( i,double( j )/N,1 );
			HSVtoRGB( i+10,double( j )/N,1 );
			HSVtoRGB( i+10,double( j+1 )/N,1 );
			HSVtoRGB( i,double( j+1 )/N,1 );
		}
	}
	glEnd();
	glDepthMask( GL_FALSE );
	glLineWidth( 3 );
	glCallList( frameList );
	glLineWidth( 10 );
	glDepthMask( GL_TRUE );
	glEndList();
}

ColorWheel2D *cw;

void drawScene()
{
	cw->drawImage();

	vect_t o = obj[0]->getCenter();

	for( int i=1; i<MAX_OBJECTS; i++ )
	{
		cbj[i]->drawImage();
	}

	for( int i=0; i<MAX_OBJECTS; i++ )
	{
        glLineWidth( 1 );
        if( i )
            for(int j=0; j<10; j++)
            {
                obj[i]->setRadius(0.25+j/75.0);
                obj[i]->drawImage();
            }
        else
        {
            glTranslated(0,0,1);
            obj[0]->drawImage();
            glTranslated(0,0,-1);
        }

//		glTranslated( 0,0,-0.1 );
		glLineWidth( 10 );
		vect_t v = obj[i]->getCenter();
		double d = v.distance( o );
		double k = ( d-0.3 )/d;
		glBegin( GL_LINES );
		o.vertex();
		glVertex3f( v.x*k,v.y*k,v.z );
		glEnd();
		glTranslated( 0,0,+0.1 );
	}
}


void drawFrames()
{
	cw->drawFrame();
	for( int i=1; i<MAX_OBJECTS; i++ )
	{
		cbj[i]->drawFrame();
	}

}

void keys( int key )
{
	if( ( key==GLFW_KEY_LEFT )     || ( key==GLFW_KEY_KP_4 ) ) activeObject->iMove( -10,0 );
	if( ( key==GLFW_KEY_RIGHT )    || ( key==GLFW_KEY_KP_6 ) ) activeObject->iMove( +10,0 );
	if( ( key==GLFW_KEY_UP )       || ( key==GLFW_KEY_KP_8 ) ) activeObject->iMove( 0,-10 );
	if( ( key==GLFW_KEY_DOWN )     || ( key==GLFW_KEY_KP_2 ) ) activeObject->iMove( 0,+10 );
	if( ( key==GLFW_KEY_PAGEDOWN ) || ( key==GLFW_KEY_KP_3 ) ) activeObject->iScale( -10,0.1,10 );
	if( ( key==GLFW_KEY_PAGEUP )   || ( key==GLFW_KEY_KP_9 ) ) activeObject->iScale( 10,0.1,10 );
}

void mice( int dX, int dY )
{
	if( glfwGetMouseButton( GLFW_MOUSE_BUTTON_LEFT ) )  activeObject->iMove( dX,dY );
	if( glfwGetMouseButton( GLFW_MOUSE_BUTTON_RIGHT ) ) activeObject->iScale( -dY,0.1,10 );
}

void processObj( int i, double angle )
{
	double d = obj[i]->getCenter().distance( {0,0,10} );
	if( d>4.95 ) d=4.95; else if( d<0.2 ) d=0.2;
	obj[i]->setCenter( {d,0,10} );
	obj[i]->clearOrientation();
	obj[i]->rotateGlobal( angle, {0,0,1} );
	cbj[i]->setColor( HSVtoRGBColor( angle,d/4.95,1 ) );
}

int main()
{
	/// Open window
	if( !openWindow2D( "Similar quintadic colours" ) ) return 1;
	changeBrightness();
	changeGround();
	changeGround();

	addButton( new exitButton2D );
	viewX = 2;

	double delta = 60;

	cw = new ColorWheel2D();
	for( int i=0; i<MAX_OBJECTS; i++ )
	{
		obj[i] = new Circle2D( i?0.3:0.2 );
		obj[i]->setCenter( {0,0,10} );
		obj[i]->setColor( {0,0,0} );
		obj[i]->setPolygonMode( i?GL_LINE:GL_FILL );
		if( i )
		{
			obj[i]->setCenter( {random( 1,3 ),random( -2,0 ),10} );
			obj[i]->interactive();
			obj[i]->setMouseMove( mice );
			obj[i]->setKeyPress( keys );
		}
	}

	for( int i=1; i<MAX_OBJECTS; i++ )
	{
		cbj[i] = new Cube3D( 1 );
		cbj[i]->setCenter( {8,5.5-i,0} );
		cbj[i]->setScale( {4,1,1} );
	}

	/// Main loop with automatic animation
	while( isRunning() )
	{
		double angle1 = atan2( obj[3]->getCenter().y,obj[3]->getCenter().x )*180/M_PI;

		viewX = 2;
		viewY = 0;

		if( obj[1]==activeObject )
		{
			double angle2 = atan2( obj[1]->getCenter().y,obj[1]->getCenter().x )*180/M_PI;
			delta = angle2-angle1;
			delta = delta+36000;
			delta = delta-360*int( delta/360 );
			if( delta<20 ) delta=20;
			if( delta>90 ) delta=90;
		} else
		if( obj[2]==activeObject )
		{
			double angle2 = atan2( obj[2]->getCenter().y,obj[2]->getCenter().x )*180/M_PI;
			delta = 2*(angle2-angle1);
			delta = delta+36000;
			delta = delta-360*int( delta/360 );
			if( delta<20 ) delta=20;
			if( delta>90 ) delta=90;
		} else
		if( obj[5]==activeObject )
		{
			double angle2 = atan2( obj[5]->getCenter().y,obj[5]->getCenter().x )*180/M_PI;
			delta = angle1-angle2;
			delta = delta+36000;
			delta = delta-360*int( delta/360 );
			if( delta<20 ) delta=20;
			if( delta>90 ) delta=90;
		} else
		if( obj[4]==activeObject )
		{
			double angle2 = atan2( obj[4]->getCenter().y,obj[4]->getCenter().x )*180/M_PI;
			delta = 2*(angle1-angle2);
			delta = delta+36000;
			delta = delta-360*int( delta/360 );
			if( delta<20 ) delta=20;
			if( delta>90 ) delta=90;
		}

		processObj( 1,angle1+delta );
		processObj( 2,angle1+delta/2 );
		processObj( 3,angle1 );
		processObj( 4,angle1-delta/2 );
		processObj( 5,angle1-delta );
	}

	return 0;
}
