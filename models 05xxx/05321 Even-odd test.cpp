
/// Project:	FMI 3D (2013)
/// File:	    05321 Even-odd test.cpp
/// Topic:		05 Lines and polygons
/// Slide:		32
///	About:		Check if a point is inside a polygon by
///             even-odd test
///
/// History:
///		2013.07.xx/VH	Initial implementation
///		2013.08.02/VH	Fixed intersection
///		2013.08.05/PB	Finetuning
///     2013.09.13/PB   Added intersection count label


#include "fmi3d.h"
#include <algorithm>

// vector operations
#define EPS 1E-9
vect_t empty = {-1E9 , -1E9},
	   zero = {0,0};
bool operator!=( const vect_t& f , const vect_t& s )
{
	return f.x != s.x || f.y != s.y;
}
int sgn( double w )
{
	return fabs( w ) < EPS ? 0 : ( w > 0 ? 1 : -1 );
}
double vp( vect_t f , vect_t s )
{
	return f.x * s.y - f.y * s.x;
}
double sp( vect_t f , vect_t s )
{
	return f.x * s.x + f.y * s.y;
}
double det( double a , double b , double c , double d )
{
	return a * d - b * c;
}
bool intersect_1d( double lo1 , double hi1 , double lo2 , double hi2 )
{
	if( lo1 > hi1 )swap( lo1 , hi1 );
	if( lo2 > hi2 )swap( lo2 , hi2 );
	return max( lo1 , lo2 )-EPS < min( hi1 , hi2 )+EPS;
}
double len( vect_t w )
{
	return w.distance( zero );
}
pair<vect_t , vect_t> intersect( vect_t c1 , vect_t d1 ,
								 vect_t c2 , vect_t d2 )
{
	if( !( d1 != zero ) || !( d2 != zero ) ||
			abs( vp( d1 , d2 ) )/( len( d1 )*len( d2 ) ) < 1E-2 ) // on one line
	{
		vect_t e1 = c1.add( d1 ) , e2 = c2.add( d2 );
		if( abs( vp( d1 , e2.sub( c1 ) ) )/( len( d1 )*len( e2.sub( c1 ) ) ) > 1E-2 )
			return make_pair( empty , empty );
		if( !intersect_1d( c1.x , e1.x , c2.x , e2.x ) ||
				!intersect_1d( c1.y , e1.y , c2.y , e2.y ) )
			return make_pair( empty , empty );
		vect_t lo = c1 , hi = e1;
		if( sgn( sp( d1 , d2 ) ) < 0 )swap( c2 , e2 );
		if( sgn( sp( d1 , c2.sub( c1 ) ) ) > 0 )lo = c2;
		if( sgn( sp( c1.sub( e1 ) , e2.sub( e1 ) ) ) > 0 )hi = e2;
		return make_pair( lo , hi );
	}
	double zm = det( d1.x , -d2.x , d1.y , -d2.y );
	double ch = det( c2.sub( c1 ).x , -d2.x , c2.sub( c1 ).y , -d2.y );
	double koef = ch / zm;
	if( koef > 1+EPS || koef < -EPS )return make_pair( empty , empty );
	vect_t ans = c1.add( d1.mul( koef ) );
	if( sgn( sp( d2 , ans.sub( c2 ) ) ) < 0 )return make_pair( empty , empty );
	if( sgn( sp( {-d2.x , -d2.y} , ans.sub( c2.add( d2 ) ) ) ) < 0 )
		return make_pair( empty , empty );
	return make_pair( ans , ans );
}

#define POINTS 9
Object3D* pts[POINTS];
Font2D* fnt;
Text2D* txt;
CoordSys2D* cs;
Object3D *center , *direction;
vect_t c , d , id;

void print_sides()
{
	glColor3f( 0,0,1 );
	for( int i=0; i<POINTS; ++i )
	{
		vect_t lo = pts[i]->getCenter();
		vect_t hi = pts[( i+1 )%POINTS]->getCenter();
		if( lo.distance( hi ) < EPS ) continue;
		glVertex3f( lo.x , lo.y, -1 );
		glVertex3f( hi.x , hi.y, -1 );
	}
}
void drawScene()
{
	glDisable( GL_LIGHTING );
	cs->drawImage();
	for( int i=0; i<POINTS; ++i )
		pts[i]->drawImage();
	center->drawImage();
	direction->drawImage();

	glLineWidth( 3 );
	glColor3f( 1,0,0 );
	glBegin( GL_LINES );
	for( int i=0; i<POINTS; ++i )
	{
		vect_t lo = pts[i]->getCenter();
		vect_t hi = pts[( i+1 )%POINTS]->getCenter();
		pair<vect_t , vect_t> ip = intersect( lo , hi.sub( lo ) , c , id.sub( c ) );
		if( ip.first != empty && ip.first != ip.second )
		{
			glVertex2f( ip.first.x , ip.first.y );
			glVertex2f( ip.second.x , ip.second.y );
		}
	}
	glEnd();

	glLineWidth( 4 );
	glBegin( GL_LINES );
	if( d.distance( c ) > EPS )
	{
		glColor3f( 1,0.5,0.5 );
		glVertex2f( c.x , c.y );
		glVertex2f( id.x , id.y );
	}
	print_sides();
	glEnd();

	glDisable(GL_DEPTH_TEST);
	glPointSize( 12 );
	glColor3f( 1,0,0 );
	int k=0;
    glBegin( GL_POINTS );
	for( int i=0; i<POINTS; ++i )
	{
		vect_t lo = pts[i]->getCenter();
		vect_t hi = pts[( i+1 )%POINTS]->getCenter();
		pair<vect_t , vect_t> ip = intersect( lo , hi.sub( lo ) , c , id.sub( c ) );
		if( ip.first != empty && !( ip.first != ip.second ) )
		{
			glVertex2f( ip.first.x , ip.first.y );
			k++;
		}
	}
    glEnd();

    wchar_t a[3];
    if(k<10 )
        {a[0]=k+'0'; a[1]='\0';}
    else
        {a[0]=k/10+'0'; a[1]=k%10+'0'; a[2]='\0';}
    txt->setCenter(center->getCenter().add({-0.1,-0.1,1}));
    txt->setText(a);
    txt->drawImage();

	glEnable(GL_DEPTH_TEST);
}

void drawFrames()
{
	for( int i=0; i<POINTS; ++i )
		pts[i]->drawFrame();
	center->drawFrame();
	direction->drawFrame();
}

void randomize()
{
	for( int i=0; i<POINTS; ++i )
	{
		double x = 10 * cos( i * 2*M_PI / POINTS + random( 0 , 1 ) );
		double y = 6 * sin( i * 2*M_PI / POINTS + random( 0 , 1 ) );
		pts[i]->setCenter({x,y,0});
	}
}

int main()
{
	if( !openWindow2D( "Even-odd test" ) )
		return 1;

	glEnable( GL_POINT_SMOOTH );

	noGround();
	viewDistance = 30;

	addButton( new Button2D("buttons/button_random.png",'R',randomize));
	addButton( new exitButton2D() );

	cs = new CoordSys2D( -12,12,-8,8,false,1,0.5,L"XY" );
	center = interactivePoint2D( random( 0,2 ) , random( 0,2 ) );
	direction = interactivePoint2D( random( 0,2 ) , random( 0,2 ) );

    fnt = new Font2D("Consolas",16,true,false);
    txt = new Text2D(L"",fnt);
    txt->setColor({1,1,1});

	for( int i=0; i<POINTS; ++i )
	{
		pts[i] = interactivePoint2D( 0,0 );
	}
	randomize();

	while( isRunning() )
	{
		viewX = viewY = 0;
		c = center->getCenter();
		d = direction->getCenter();
		id = c.add( d.sub( c ).mul( 50 / d.distance( c ) ) );
	}
	return 0;
}

