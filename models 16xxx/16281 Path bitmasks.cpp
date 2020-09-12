
/// Project:	FMI 3D (2013)
/// File:		16281 Path bitmasks.cpp
/// Topic:		16 Clipping
/// Slide:		28
///	About:		Path bitmasks
///
/// History:
///		2013.08.21/PB	Initial implementation


#include "fmi3d.h"

#define N 6
#define M 21
#define K 100
Object3D *p[N];
Object3D *q;
Font3D *fnt;
Text3D *txt[N+1];
vect_t pnt[K];
int k=0,dk=0;

void drawScene()
{
	// Draw polygon vertices
	for( int i=0; i<N; i++ )
	{
		p[i]->drawImage();
	}

	glEnable( GL_LINE_SMOOTH );
	glDisable( GL_LIGHTING );
	glDisable( GL_TEXTURE_2D );
	glLineWidth( 1 );
	for( int i=0; i<N+1; i++ )
	{
		txt[i]->drawImage();
	}

	// Draw polygon edges
	glLineWidth( 5 );
	glColor3f( 0,0,0 );
	glBegin( GL_LINE_LOOP );
	for( int i=0; i<N; i++ )
	{
		p[i]->getCenter().sub( {0,0,0.5} ).vertex();
	}
	glEnd();

	// Draw polygon lines
	glEnable( GL_LINE_STIPPLE );
	glLineStipple( 5,0b1010101010101010 );
	glLineWidth( 1 );
	glColor3f( 0.5,0.5,1 );
	glBegin( GL_LINES );
	for( int i=0; i<N; i++ )
	{
		vect_t a = p[i]->getCenter().sub( {0,0,0.5} );
		vect_t b = p[( i+1 )%N]->getCenter().sub( {0,0,0.5} );
		b.mul( 100 ).sub( a.mul( 99 ) ).vertex();
		a.mul( 100 ).sub( b.mul( 99 ) ).vertex();
	}
	glEnd();
	glDisable( GL_LINE_STIPPLE );

	// Draw path
	glDisable( GL_DEPTH_TEST );
	glLineWidth( 6 );
	glColor3f( 0,0,0 );
	glBegin( GL_LINE_STRIP );
	for( int i=1; i<=K; i++ ) pnt[( k+i )%K].vertex();
	glEnd();
	glLineWidth( 4 );
	glColor3f( 1,1,0 );
	glBegin( GL_LINES );
	for( int i=1+dk; i<=K; i++ ) pnt[( k+i )%K].vertex();
	glEnd();
	glEnable( GL_DEPTH_TEST );

	q->drawImage();

	// Draw polygon face
	glPolygonMode( GL_FRONT_AND_BACK,GL_FILL );
	glColor4f( 0.5,0.5,1,0.25 );
	glBegin( GL_POLYGON );
	for( int i=0; i<N; i++ ) p[i]->getCenter().vertex();
	glEnd();
}


void drawFrames()
{
	//cs->drawFrame();
	q->drawFrame();
	for( int i=0; i<N; i++ ) p[i]->drawFrame();
}

double area( Object3D *a, Object3D *b, Object3D *c )
{
	vect_t A = a->getCenter();
	vect_t B = b->getCenter();
	vect_t C = c->getCenter();

	return ( A.x*( B.y-C.y )+B.x*( C.y-A.y )+C.x*( A.y-B.y ) )/2;
}

void generate()
{
	for( int i=0; i<N; i++ )
	{
		double a=double( i )/N*2*M_PI;
		double r=random( 2.5,5 );
		p[i]->setCenter( {r*cos( a ), r*sin( a ), 0.1} );
	}
	for( int i=0; i<N; i++ )
	{
		double s = 1;
		int j = ( i+1 )%N;
		txt[i]->setCenter( p[i]->getCenter() );
		txt[i]->oxyz.ox = p[j]->getCenter().sub( p[i]->getCenter() );
		if( txt[i]->oxyz.ox.x<0 )
		{
			s = -2.5;
			txt[i]->setCenter( p[j]->getCenter() );
			txt[i]->oxyz.ox = p[i]->getCenter().sub( p[j]->getCenter() );
		}
		txt[i]->oxyz.oy = txt[i]->oxyz.oz.vmul( txt[i]->oxyz.ox );
		txt[i]->oxyz.ox = txt[i]->oxyz.ox.mul( 1/txt[i]->oxyz.ox.distance() );
		txt[i]->oxyz.oy = txt[i]->oxyz.oy.mul( 1/txt[i]->oxyz.oy.distance() );
		txt[i]->translateLocal( {p[j]->getCenter().sub( p[i]->getCenter() ).distance()/2-0.85,0.15*s,0} );
	}
	k=0; dk=0;
	for( int i=0; i<K; i++ ) pnt[i]=q->getCenter().sub( {0,0,0.5} );
}


int main()
{
	/// Open window
	if( !openWindow2D( "Path bitmasks" ) ) return 1;
	noGround();

	/// Add buttons
	addButton( new Button2D( "buttons/button_random.png",GLFW_KEY_SPACE,generate ) );
	addButton( new exitButton2D );
	//cs = new CoordSys2D(-20,20,-20,20,true,10);
	//cs->setCenter({0,0,3});

	q = interactivePoint2D( 0,0 );
	q->setColor( {1,0,0} );

	for( int i=0; i<N; i++ )
	{
		p[i] = point2D( 0,0 );
		p[i]->setSize( 0.2 );
	}
	fnt = new Font3D( "Verdana", true );
	for( int i=0; i<N+1; i++ )
	{
		wchar_t w[N+1];
		for( int j=0; j<N; j++ ) w[j]=( j==i )?'1':'0';
		w[N]=0;
		txt[i] = new Text3D( w,fnt );
		txt[i]->setLighting( false );
		if( i<N )
		{
			txt[i]->setColor( {0.5,0.5,1} );
			txt[i]->setScale( {0.5,0.4,0} );
		}
		else
		{
			txt[i]->setColor( {1,0,0} );
			txt[i]->setScale( {1.0,0.8,0} );
		}
	}

	generate();

	/// Main loop
	while( isRunning() )
	{
		wchar_t w[N+1];
		for( int i=0; i<N; i++ )
		{
			double a = area( q,p[i],p[( i+1 )%N] );
			w[i] = ( a<0 )?'1':'0';
		}
		w[N]=0;
		txt[N]->setText( w );
		txt[N]->setCenter( q->getCenter().add( {-1.75,0.4,3} ) );

		if( q->getCenter().distance( pnt[k] )>0.3 )
		{
			k = ( k+1 )%K;
			pnt[k] = q->getCenter().sub( {0,0,0.2} );
			dk=1-dk;
		}
	}
	return 0;
}

