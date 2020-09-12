/// Project:	FMI 3D (2013)
/// File:	    05301 point_inside_polygon(winding_number).cpp
/// Topic:		05 Lines and polygons
/// Slide:		30
///	About:		Check if a point inside a polygon by winding number test
///
/// History:
///		2013.07.30/VH	Initial implementation
///		2013.07.31/PB	Finetuning
///     2013.09.13/PB   Dotted lines moved behind the polygon


#include "fmi3d.h"

// vector operations
#define EPS 1E-9
int sgn( double w )
{
	return fabs( w ) < EPS ? 0 : ( w > 0 ? 1 : -1 );
}
double vp( vect_t f , vect_t s )
{
	return f.x * s.y - f.y * s.x;
}

#define POINTS 9
Object3D* pts[POINTS];
CoordSys2D* cs;
Object3D* center;

bool draw_side_lines = false;
void side_lines()
{
	draw_side_lines ^= true;
}

void print_sides( bool infinite = false )
{
	vect_t c = center->getCenter();
	glColor3f( 1,0,0 );
	for( int i=0; i<POINTS; ++i )
	{
		vect_t lo = pts[i]->getCenter();
		vect_t hi = pts[( i+1 )%POINTS]->getCenter();
		if( lo.distance( hi ) < EPS )continue;
		if( infinite )
		{
			vect_t dir = hi.sub( lo ).mul( 50/lo.distance( hi ) );
			lo = lo.sub( dir ) , hi = hi.add( dir );
		}
		if( sgn( vp( lo.sub( c ) , hi.sub( c ) ) ) >= 0 )
			glVertex2f( lo.x , lo.y ),
						glVertex2f( hi.x , hi.y );
	}
	glColor3f( 0,0,1 );
	for( int i=0; i<POINTS; ++i )
	{
		vect_t lo = pts[i]->getCenter();
		vect_t hi = pts[( i+1 )%POINTS]->getCenter();
		if( lo.distance( hi ) < EPS )continue;
		if( infinite )
		{
			vect_t dir = hi.sub( lo ).mul( 50/lo.distance( hi ) );
			lo = lo.sub( dir ) , hi = hi.add( dir );
		}
		if( sgn( vp( lo.sub( c ) , hi.sub( c ) ) ) < 0 )
			glVertex2f( lo.x , lo.y ),
						glVertex2f( hi.x , hi.y );
	}
}
void drawScene()
{
	glDisable( GL_LIGHTING );
	cs->drawImage();
	for( int i=0; i<POINTS; ++i )
		pts[i]->drawImage();
	center->drawImage();

	glTranslated( 0,0,-2 );
	if( draw_side_lines )
	{
		glPushAttrib( GL_ENABLE_BIT );
		glLineWidth( 0.7 );
		glEnable( GL_LINE_STIPPLE );
		glLineStipple( 1 , 0xCCCC );
		glBegin( GL_LINES );
		print_sides( true );
		glEnd();
		glPopAttrib();
	}

	glTranslated( 0,0,+1 );
	glLineWidth( 3 );
	glBegin( GL_LINES );
	print_sides();
	glEnd();
}

void drawFrames()
{
	for( int i=0; i<POINTS; ++i )
		pts[i]->drawFrame();
	center->drawFrame();
}

void randomize()
{
	for( int i=0; i<POINTS; ++i )
	{
		double x = 10 * cos( i * 2*M_PI / POINTS + random( -0.2 , 0.2 ) );
		double y = 6 * sin( i * 2*M_PI / POINTS + random( -0.2 , 0.2 ) );
		pts[i]->setCenter( {x,y,0} );
	}
}

int main()
{
	if( !openWindow2D( "Winding number test" ) )
		return 1;

	noGround();
	viewDistance = 30;

	addButton( new Button2D( "buttons/button_show.png", GLFW_KEY_SPACE, side_lines ) );
	addButton( new Button2D( "buttons/button_random.png", 'R', randomize ) );
	addButton( new exitButton2D() );

	cs = new CoordSys2D( -12,12,-8,8,false,1,0.5,L"XY" );
	center = interactivePoint2D( random( 0,2 ) , random( 0,2 ) );

	for( int i=0; i<POINTS; ++i )
	{
		double x = 10 * cos( i * 2*M_PI / POINTS + random( 0 , 1 ) );
		double y = 6 * sin( i * 2*M_PI / POINTS + random( 0 , 1 ) );
		pts[i] = interactivePoint2D( x,y );
	}
	while( isRunning() )
		viewX = viewY = 0;

	return 0;
}

