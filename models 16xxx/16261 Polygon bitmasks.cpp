
/// Project:	FMI 3D (2013)
/// File:		16261 Polygon bitmasks.cpp
/// Topic:		16 Clipping
/// Slide:		26
///	About:		Polygon bitmasks
///
/// History:
///		2013.08.21/PB	Initial implementation


#include "fmi3d.h"

#define N 6
#define M 21
Object3D *p[N];
Font3D *fnt;
Text3D *txt[N+M];

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
	glPolygonMode( GL_FRONT_AND_BACK,GL_LINE );
	glLineWidth( 1 );
	for( int i=0; i<N+M; i++ )
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
		vect_t b = p[(i+1)%N]->getCenter().sub( {0,0,0.5} );
		b.mul(100).sub(a.mul(99)).vertex();
		a.mul(100).sub(b.mul(99)).vertex();
	}
	glEnd();
	glDisable( GL_LINE_STIPPLE );

	// Draw polygon face
	glPolygonMode( GL_FRONT_AND_BACK,GL_FILL );
	glColor4f( 0.5,0.5,1,0.25 );
	glBegin( GL_POLYGON );
	for( int i=0; i<N; i++ ) p[i]->getCenter().vertex();
	glEnd();
}


void drawFrames()
{
	for( int i=0; i<N; i++ ) p[i]->drawFrame();
}


void generate()
{
	int k=1;
	for( int i=0; i<N; i++ )
	{
		double a=double( i )/N*2*M_PI;
		double r=2+i/2.0+k;
		k=1-k;
		p[i]->setCenter( {r*cos( a ), r*sin( a ), 1} );
	}
	for( int i=0; i<N; i++ )
	{
		double s = 1;
		int j = (i+1)%N;
		txt[i]->setCenter(p[i]->getCenter());
		txt[i]->oxyz.ox = p[j]->getCenter().sub(p[i]->getCenter());
		if( txt[i]->oxyz.ox.x<0 )
		{
			s = -2.5;
			txt[i]->setCenter(p[j]->getCenter());
			txt[i]->oxyz.ox = p[i]->getCenter().sub(p[j]->getCenter());
		}
		txt[i]->oxyz.oy = txt[i]->oxyz.oz.vmul(txt[i]->oxyz.ox);
		txt[i]->oxyz.ox = txt[i]->oxyz.ox.mul(1/txt[i]->oxyz.ox.distance());
		txt[i]->oxyz.oy = txt[i]->oxyz.oy.mul(1/txt[i]->oxyz.oy.distance());
		txt[i]->translateLocal({p[j]->getCenter().sub(p[i]->getCenter()).distance()/2-0.85,0.15*s,0});
	}
}


int main()
{
	/// Open window
	if( !openWindow2D( "Polygon bitmasks" ) ) return 1;
	noGround();

	/// Add buttons
	addButton( new exitButton2D );


	for( int i=0; i<N; i++ ) p[i] = point2D( 0,0 );
	fnt = new Font3D( "Verdana", true );
	for( int i=0; i<N+M; i++ )
	{
		wchar_t w[N+1];
		for(int j=0; j<N; j++) w[j]=(j==i)?'1':'0';
		w[N]=0;
		txt[i] = new Text3D( w,fnt );
		txt[i]->setLighting(false);
		if( i<N )
			txt[i]->setColor( {0.5,0.5,1} );
		else
			txt[i]->setColor( {1,0,0} );
		txt[i]->setScale( {0.5,0.4,0} );
	}

	generate();

	txt[N+ 0]->setCenter({ 3.0,-3.0}); txt[N+ 0]->setText(L"000001");
	txt[N+ 1]->setCenter({ 3.5,-5.0}); txt[N+ 1]->setText(L"000011");
	txt[N+ 2]->setCenter({ 7.5,-4.5}); txt[N+ 2]->setText(L"100011");
	txt[N+ 3]->setCenter({ 4.5,-1.0}); txt[N+ 3]->setText(L"100001");
	txt[N+ 4]->setCenter({9.7,34.5});  txt[N+ 4]->setText(L"111001");
	txt[N+ 5]->setCenter({ 2.0, 1.4}); txt[N+ 5]->setText(L"100000");
	txt[N+ 6]->setCenter({ 5.5, 2.5}); txt[N+ 6]->setText(L"110001");
	txt[N+ 7]->setCenter({ 1.0, 4.0}); txt[N+ 7]->setText(L"110000");
	txt[N+ 8]->setCenter({-1.7, 3.5}); txt[N+ 8]->setText(L"010000");
	txt[N+ 9]->setCenter({-2.5, 8.0}); txt[N+ 9]->setText(L"111000");
	txt[N+10]->setCenter({-4.0, 5.0}); txt[N+10]->setText(L"011000");
	txt[N+11]->setCenter({-8.0, 6.5}); txt[N+11]->setText(L"011100");
	txt[N+12]->setCenter({-4.1, 3.0}); txt[N+12]->setText(L"001000");
	txt[N+13]->setCenter({-7.0, 0.0}); txt[N+13]->setText(L"001100");
	txt[N+14]->setCenter({-4.8,-4.0}); txt[N+14]->setText(L"000100");
	txt[N+15]->setCenter({-8.5,-5.8}); txt[N+15]->setText(L"001110");
	txt[N+16]->setCenter({-5.0,-7.0}); txt[N+16]->setText(L"000110");
	txt[N+17]->setCenter({-1.0,-5.0}); txt[N+17]->setText(L"000010");
	txt[N+18]->setCenter({16.5,-3.5}); txt[N+18]->setText(L"110011");
	txt[N+19]->setCenter({-0.7,-20});  txt[N+19]->setText(L"000111");
	txt[N+20]->setCenter({-9.5,16});   txt[N+20]->setText(L"111100");


	/// Main loop
	while( isRunning() )
	{
	}
	return 0;
}

