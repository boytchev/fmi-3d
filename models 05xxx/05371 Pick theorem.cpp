/// Project:	FMI 3D (2013)
/// File:	    05371 Pick theorem.cpp
/// Topic:		05 Lines and polygons
/// Slide:		37
///	About:		Pick theorem ilustration
///
/// History:
///		2013.08.xx/VH	Initial implementation
///		2013.08.12/VH	Interface fixes
///		2013.08.13/PB	Finetuning


#include "fmi3d.h"
#include <complex>
#include <algorithm>
#include "string.h"

#define N 10

vector<Object3D*> pts;
CoordSys2D* cs;
Text2D *inside_field , *border_field;
Text2D *area_field;
Font2D* font;
Button2D *start;

#define sz(a) ((int)(a).size())
#define pb(a) push_back(a)
#define all(a) (a).begin() , (a).end()

// Primitives
bool operator==( const vect_t& f , const vect_t& s )
{
	return f.x == s.x && f.y == s.y;
}
#define EPS 1E-9
double sgn( double w )
{
	return fabs( w ) < EPS ? 0 : ( w>0 ? 1 : -1 );
}
double vp( vect_t f , vect_t s )
{
	return f.x*s.y - f.y*s.x;
}
bool intersect( vect_t lo1 , vect_t hi1 ,
				vect_t lo2 , vect_t hi2 )
{
	if( sgn( vp( hi1.sub( lo1 ) , lo2.sub( lo1 ) ) ) *
			sgn( vp( hi1.sub( lo1 ) , hi2.sub( lo1 ) ) ) >= 0 )return false;
	if( sgn( vp( hi2.sub( lo2 ) , lo1.sub( lo2 ) ) ) *
			sgn( vp( hi2.sub( lo2 ) , hi1.sub( lo2 ) ) ) >= 0 )return false;
	return true;
}
// Polygon tests
vector<vect_t> poly;
#define IN 0
#define OUT 1
#define MIDDLE 2
bool is_between( vect_t lo , vect_t hi , vect_t mid )
{
	return min( lo.x , hi.x )<=mid.x+EPS && mid.x-EPS<=max( lo.x , hi.x )&&
		   min( lo.y , hi.y )<=mid.y+EPS && mid.y-EPS<=max( lo.y , hi.y );
}
int is_inside( vect_t w )
{
	for( int i=0; i<sz( poly ); ++i )
	{
		vect_t lo = poly[i] , hi = poly[( i+1 )%sz( poly )];
		double SIN = fabs( vp( hi.sub( lo ) , w.sub( lo ) ) );
		SIN /= lo.distance( hi ) * lo.distance( w );
		if( SIN < 1E-2 )
			if( is_between( lo , hi , w ) )
				return MIDDLE;
	}
	int in = 0;
	for( int i=0; i<sz( poly ); ++i )
	{
		vect_t lo = poly[i] , hi = poly[( i+1 )%sz( poly )];
		if( lo.y > hi.y )swap( lo , hi );
		if( fabs( lo.y - hi.y ) < EPS )continue;
		if( lo.y+EPS < w.y && hi.y > w.y-EPS )
			if( sgn( vp( lo.sub( w ) , hi.sub( w ) ) ) > 0 )
				++in;
	}
	return ( in&1 ) ? IN : OUT;
}
// Drawing
bool is_ok( vector<vect_t> w );
void drawScene()
{
	glDisable( GL_LIGHTING );
	cs->drawImage();
	for( int i=0; i<sz( pts ); ++i )
		pts[i]->drawImage();

	vector<int> type;
	int idx = 0;
	for( int x=-12; x<=12; ++x )
		for( int y=-8; y<=8; ++y )
			type.pb( is_inside( {( double )x , ( double )y} ) );

	int inside = count( all( type ) , IN );
	int border = count( all( type ) , MIDDLE );
	vect_t text_color = {0, 0, 0};
	if( !is_ok( poly ) )text_color = {1 , 0 , 0};

	char res8[100];
	wchar_t res[100];

	sprintf( res8,"a=%d",inside );
	for( unsigned i=0; i<strlen( res8 )+1; i++ ) res[i]=res8[i];
	inside_field->setText( res );
	inside_field->setColor( text_color );
	inside_field->drawImage();

	sprintf( res8,"b=%d",border );
	for( unsigned i=0; i<strlen( res8 )+1; i++ ) res[i]=res8[i];
	border_field->setText( res );
	border_field->setColor( text_color );
	border_field->drawImage();

	sprintf( res8,"S=%d+%d/2-1=%.1f",inside,border,inside + border/2.0 - 1 );
	for( unsigned i=0; i<strlen( res8 )+1; i++ ) res[i]=res8[i];
	area_field->setText( res );
	area_field->setColor( text_color );
	area_field->drawImage();

	glDisable( GL_LIGHTING );
	glDepthMask( GL_FALSE );

	glEnable( GL_LINE_SMOOTH );
	glLineWidth( 5 );
	glColor3f( 0,0,0 );
	glBegin( GL_LINE_LOOP );
	for( int i=0; i<sz( pts ); ++i )
	{
		vect_t lo = pts[i]->getCenter();
		glVertex2f( lo.x , lo.y );
	}
	glEnd();

	glPointSize( 10 );
	glBegin( GL_POINTS );
	glColor3f( 1 , 0 , 0 );
	for( int x=-12; x<=12; ++x )
		for( int y=-8; y<=8; ++y )
			if( type[idx++] == IN )
				glVertex2f( x , y );
	glColor3f( 0 , 0.75 , 0 );
	idx = 0;
	for( int x=-12; x<=12; ++x )
		for( int y=-8; y<=8; ++y )
			if( type[idx++] == MIDDLE )
				glVertex2f( x , y );
	glEnd();
	glDepthMask( GL_TRUE );
}

void drawFrames()
{
	for( int i=0; i<sz( pts ); ++i )
		pts[i]->drawFrame();
}

// Creating the polygon
vect_t fix_point( vect_t cur )
{
	vect_t best = {floor( cur.x ) , floor( cur.y )},
		   nxt;
	double dx = 0.5 , dy = 0.5;
	if( cur.x < 0 )dx *= -1; if( cur.y < 0 )dy *= -1;
	for( int sx=-2; sx<=2; ++sx )
		for( int sy=-2; sy<=2; ++sy )
		{
			nxt = {floor( cur.x + dx ) + sx , floor( cur.y + dy ) + sy};
			if( nxt.distance( cur ) < best.distance( cur ) )
				best = nxt;
		}
	return best;
}
bool is_ok( vector<vect_t> w )
{
	w.pb( w[0] );
	for( int i = 1; i<sz( w ); ++i )
		for( int j=i+1; j<sz( w ); ++j )
			if( intersect( w[i-1] , w[i] , w[j-1] , w[j] ) )
				return false;
	return true;
}
void create_figure()
{
	vector<vect_t>& use = poly;
	pts.clear() , use.clear();
	use.push_back( {double(irandom(-5,-3)),double(irandom(-4,-2))} );
	use.push_back( {double(irandom(-5,-3)),double(irandom(+3,+5))} );
	use.push_back( {double(irandom(+3,+5)),double(irandom(+3,+5))} );
	use.push_back( {double(irandom(+3,+5)),double(irandom(-4,-2))} );
	for( int step=0; step<N-4; ++step )
	{
		vector<vect_t> neo = use;
		int pos = rand() % sz( use );
		for( int i=0; i<2; ++i )
		{
			int npos = rand() % sz( use );
			if( use[npos].distance( use[( npos+1 )%sz( use )] ) >
					use[pos].distance( use[( pos+1 )%sz( use )] ) )
				pos = npos;
		}
		vect_t lo = use[pos] , hi = use[( pos+1 )%sz( use )];
		complex<double> mid( hi.x - lo.x , hi.y - lo.y );
		double angle = random( -0.65 , 0.65 );
		mid *= polar( 0.6 , angle );
		vect_t middle = fix_point( {mid.real() + lo.x ,
									mid.imag() + lo.y
								   } );
		if( find( all( neo ) , middle ) != neo.end() )continue;
		neo.insert( neo.begin() + pos+1 , middle );
		if( is_ok( neo ) )use = neo;
	}
	for( int i=0; i<sz( use ); ++i )
		pts.pb( interactivePoint2D( use[i].x , use[i].y ) );
}
void update_poly()
{
	poly.clear();
	for( int i=0; i<sz( pts ); ++i )
	{
		vect_t cur = pts[i]->getCenter();
		vect_t fcur = fix_point( cur );
		poly.pb( fcur );
	}
	for( int i=0; i<sz( poly ); ++i )
		pts[i]->setCenter( poly[i] );
}

int main()
{
	if( !openWindow2D( "Pick theorem" ) )
		return 1;

	noGround();
	viewDistance = 30;
	glEnable( GL_POINT_SMOOTH );

	start = new Button2D( "buttons/button_random.png" , 'R' , create_figure );
	addButton( new exitButton2D() );

	font = new Font2D( "Verdana" , 20 , true , false );

	cs = new CoordSys2D( -12,12,-7,8,true,1,0.5,L"XY" );

	create_figure();

	inside_field = new Text2D( L"" , font );
	inside_field->setCenter( {-5 , -7.8} );

	border_field = new Text2D( L"", font );
	border_field->setCenter( {-3 , -7.8} );

	area_field = new Text2D( L"", font );
	area_field->setCenter( {-1 , -7.8} );

	while( isRunning() )
	{
		if( !glfwGetMouseButton( GLFW_MOUSE_BUTTON_LEFT ) )
			update_poly();
		else
		{
			poly.clear();
			for( int i=0; i<sz( pts ); ++i )
				poly.pb( pts[i]->getCenter() );
		}
		viewX = viewY = 0;
	}
	return 0;
}

