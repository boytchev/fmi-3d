
/// Project:	FMI 3D (2013)
/// File:		28101 Distance shading.cpp
/// Topic:		28 Light
/// Slide:		11
///	About:		Distance shading
///
/// History:
///		2013.07.06/PB	Initial implementation


#include "fmi3d.h"


#define N 17
Cube3D* s[N][ N];

bool shading = false;

void drawScene()
{
	if( shading )
	{
		GLfloat fc[4]= {0,0,0,1};
		glFogfv( GL_FOG_COLOR,fc );
		glFogf( GL_FOG_START,viewDistance-10 );
		glFogf( GL_FOG_END,viewDistance+5 );
	}

	for( int i=0; i<N; i++ )
		for( int j=0; j<N; j++ )
			s[i][j]->drawImage();

	if( shading )
	{
		GLfloat fc[4]= {1,1,1,1};
		glFogfv( GL_FOG_COLOR,fc );
		glFogf( GL_FOG_START,viewDistance+20 );
		glFogf( GL_FOG_END,viewDistance*3+20 );
	}
}


void drawFrames()
{
}


void changeShading()
{
	shading = !shading;
}

int main()
{
	/// Open window
	if( !openWindow3D( "Distance shading" ) ) return 1;

	/// Add buttons
	addButton( new Button2D("buttons/button_light.png",GLFW_KEY_SPACE,changeShading));
	addButton( new exitButton2D );

	viewZ = 4;
	viewDistance = 22;

	for( int i=0; i<N; i++ )
		for( int j=0; j<N; j++ )
		{
			s[i][j] = new Cube3D( 5 );
			double z = random( 1,10 );
			s[i][j]->setScale( {1,1,z} );
			s[i][j]->setCenter( {( i-( N-1 )/2.0 ),( j-( N-1 )/2.0 ),z/2} );
			s[i][j]->setColor( {0.5,0.5,1} );
		}


	/// Main loop
	while( isRunning() )
	{
	}
	return 0;
}

