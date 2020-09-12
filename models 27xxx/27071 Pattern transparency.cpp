
/// Project:	FMI 3D (2013)
/// File:		27071 Pattern transparency.cpp
/// Topic:		27 Textures
/// Slide:		07
///	About:		A pattern-based transparency
///
/// History:
///		2013.07.23/PB	Initial implementation
///     2013.09.13/PB   Changed button image


#include "fmi3d.h"

#define N 5
Sphere3D *s[N];
unsigned oddo[32];
unsigned even[32];
unsigned full[32];
int mode = 0;

void drawScene()
{
	glEnable(GL_FOG);
	glFogf(GL_FOG_START,viewDistance+25);
	glFogf(GL_FOG_END,viewDistance+50);
	s[0]->drawImage();
	glDisable(GL_FOG);

	glEnable( GL_POLYGON_STIPPLE );
	glPolygonStipple( ( GLubyte* )full );
	switch( mode )
	{
		case 0:
			s[1]->drawImage();
			break;
		case 1:
			glPolygonStipple( ( GLubyte* )oddo );
			s[1]->drawImage();
			glPolygonStipple( ( GLubyte* )full );
			break;
		case 2:
			s[1]->drawImage();
			glPolygonStipple( ( GLubyte* )oddo );
			break;
		case 3:
			glPolygonStipple( ( GLubyte* )oddo );
			s[1]->drawImage();
			break;
		case 4:
			glPolygonStipple( ( GLubyte* )oddo );
			s[1]->drawImage();
			glPolygonStipple( ( GLubyte* )even );
			break;
	}
	s[2]->drawImage();
	s[3]->drawImage();
	s[4]->drawImage();
	glDisable( GL_POLYGON_STIPPLE );
}


void drawFrames()
{
}


Button2D *mB;
int modes = 5;
void changeMode()
{
	mode = ( mode+1 )%modes;
	mB->setState( mode );
}


int main()
{
	/// Open window
	if( !openWindow3D( "Pattern transparency" ) ) return 1;
	noGround();
	minViewBeta = -0.95;
	maxViewBeta = +0.95;
	viewZ = 0;

	/// Add buttons
	mB = new Button2D( "buttons/button_xray.png",GLFW_KEY_SPACE,changeMode,mode,modes );
	addButton( new exitButton2D );

	for( int i=0; i<N; i++ ) s[i] = new Sphere3D( 1,( i==1 )?QUALITY_LOW:QUALITY_HIGH );

	s[0]->setScale( {40,40,400} );
	s[0]->setTexture( loadTexture( "textures/Material_Wood.jpg" ) );
	s[0]->setTextureScale( {20,100,1,1} );
	s[0]->setLighting( false );

	s[1]->setSize( 4 );
	s[1]->setColor( {0,0,0.4} );
	s[1]->setShininess( 5 );
	s[1]->setShadeModel( GL_FLAT );

	s[2]->setScale( {7,1,1} );
	s[2]->setColor( {1,0.5,0} );
	s[2]->setShininess( 5 );

	s[3]->setScale( {1,7,1} );
	s[3]->setColor( {1,0.5,0} );
	s[3]->setShininess( 5 );

	s[4]->setScale( {1,1,7} );
	s[4]->setColor( {1,0.5,0} );
	s[4]->setShininess( 5 );

	for( int i=0; i<32; i++ )
	{
		oddo[i]=( i%2 )?0xaaaaaaaa:0x55555555;
		even[i]=( i%2 )?0x55555555:0xaaaaaaaa;
		full[i]=0xffffffff;
	}

	/// Main loop
	double t0 = t;
	double sp = 0;
	while( isRunning() )
	{
		sp += 20*( t-t0 );
		t0 = t;

		for( int i=1; i<N; i++ )
		{
			s[i]->clearOrientation();
			s[i]->rotateGlobal( sp, {2,0,1} );
			s[i]->rotateGlobal( sp, {0,1,sin( t )} );
		}
	}
	return 0;
}

