
/// Project:	FMI 3D (2013)
/// File:		08101 Distances.cpp
/// Topic:		08 Rasterisation
/// Slide:		10
///	About:		Euclidean, Manhattan and Maximal distance
///
/// History:
///		2013.08.15/PB	Initial implementation


#include "fmi3d.h"

CoordSys2D *cs;
Object3D *p, *q;
Font2D* font;
Text2D* text;

int mode = 0;
int modes = 3;

double min( double x, double y ) {return ( x<y )?x:y; }
double max( double x, double y ) {return ( x>y )?x:y; }

void drawScene()
{
	cs->drawImage();

	glDepthMask( GL_FALSE );
	glDisable( GL_LIGHTING );
	glDisable( GL_TEXTURE_2D );

	vect_t pp = p->getCenter();
	vect_t qq = q->getCenter();
	double m = 0;

	glEnable( GL_LINE_SMOOTH );
	glLineWidth( 10 );

	if( mode==0 )
	{
		text->setText( L"Евклидово" );
		glColor3f( 1,0,0 );
		glBegin( GL_LINE_STRIP );
		pp.vertex();
		qq.vertex();
		glEnd();

		glPolygonMode( GL_FRONT_AND_BACK,GL_FILL );
		glColor4f( 1,0,0,0.3 );
		vect_t m = pp.add( qq ).mul( 0.5 );
		double r = pp.sub( qq ).distance()/2;
		glBegin( GL_TRIANGLE_FAN );
		m.vertex();
		for( int i=0; i<=50; i++ )
		{
			double a = i/50.0*M_PI*2;
			glVertex2d( m.x+r*cos( a ),m.y+r*sin( a ) );
		}
		glEnd();
	}

	if( mode==1 )
	{
		text->setText( L"Старазагорско" );
		glColor3f( 1,0,0 );
		glBegin( GL_LINE_STRIP );
		pp.vertex();
		glVertex2d( pp.x,qq.y );
		qq.vertex();
		glEnd();

		glEnable( GL_POINT_SMOOTH );
		glPointSize( 7 );
		glColor3f( 0.5,0.5,1 );
		glBegin( GL_POINTS );
		glVertex2d( p->getCenter().x,q->getCenter().y );
		glEnd();

		if( fabs( pp.x-qq.x )>0.5 && fabs( pp.y-qq.y )>0.5 )
		{
			glPolygonMode( GL_FRONT_AND_BACK,GL_FILL );
			glColor4f( 1,0,0,0.3 );
			glBegin( GL_QUADS );
			glVertex2f( min( pp.x,qq.x )+0.2,min( pp.y,qq.y )+0.2 );
			glVertex2f( min( pp.x,qq.x )+0.2,max( pp.y,qq.y )-0.2 );
			glVertex2f( max( pp.x,qq.x )-0.2,max( pp.y,qq.y )-0.2 );
			glVertex2f( max( pp.x,qq.x )-0.2,min( pp.y,qq.y )+0.2 );
			glEnd();

		}
	}
	if( mode==2 )
	{
		text->setText( L"Максимално" );
		glColor3f( 1,0,0 );
		glBegin( GL_LINE_STRIP );
		pp.vertex();
		if( fabs( pp.x-qq.x )>fabs( pp.y-qq.y ) )
		{
			m = fabs( pp.x-qq.x );
			glVertex2d( qq.x,pp.y );
		}
		else
		{
			m = fabs( pp.y-qq.y );
			glVertex2d( pp.x,qq.y );
		}
		glEnd();

		glPolygonMode( GL_FRONT_AND_BACK,GL_FILL );
		glColor4f( 1,0,0,0.3 );
		glBegin( GL_QUADS );
		vect_t n = {min( pp.x,qq.x ),min( pp.y,qq.y )};
		n.vertex();
		n.add( {m,0,0} ).vertex();
		n.add( {m,m,0} ).vertex();
		n.add( {0,m,0} ).vertex();
		glEnd();
	}

	p->drawImage();
	q->drawImage();
	text->drawImage();
	glDepthMask( GL_TRUE );
}


void drawFrames()
{
	p->drawFrame();
	q->drawFrame();
	cs->drawFrame();
}


Button2D *mB;
void nextMode()
{
	mode = ( mode+1 )%modes;
	mB->setState( mode );
}

int main()
{
	/// Open window
	if( !openWindow2D( "Distances" ) ) return 1;
	noGround();

	/// Add buttons
	mB = new Button2D( "buttons/button_next.png", GLFW_KEY_SPACE, nextMode, mode, modes );
	addButton( new exitButton2D );

	cs = new CoordSys2D( -8,8,-5,5,true,5,0.7,L"XY" );
	font = new Font2D( "Verdana",32,true );
	text = new Text2D( L"",font );
	text->setCenter( {-8,5.3,0} );
	text->setColor( {0.5,0.5,1} );

	p = interactivePoint2D( 5,3 );
	q = interactivePoint2D( -3,-1 );

	/// Main loop
	while( isRunning() )
	{
	}
	return 0;
}

