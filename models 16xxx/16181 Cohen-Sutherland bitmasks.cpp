
/// Project:	FMI 3D (2013)
/// File:		16181 Cohen-Sutherland bitmasks.cpp
/// Topic:		16 Clipping
/// Slide:		18
///	About:		Cohen-Sutherland bitmasks
///
/// History:
///		2013.08.21/PB	Initial implementation


#include "fmi3d.h"

#define N 17
int minX=-5, maxX=5, minY=-3, maxY=3;
Font3D *fnt1;
Font3D *fnt2;
Text3D *txt[N];
bool show = false;

void drawScene()
{
	glEnable( GL_LINE_SMOOTH );
	glDisable( GL_LIGHTING );
	glDisable( GL_TEXTURE_2D );
	glLineWidth( 0.5 );
	glDepthMask( GL_FALSE );
	for( int i=0; i<N; i++ ) txt[i]->drawImage();
	glDepthMask( GL_TRUE );

	// Draw lines
	glEnable( GL_LINE_STIPPLE );
	glLineStipple( 5,0b1010101010101010 );
	glLineWidth( 1 );
	glColor3f( 0,0,0 );
	glBegin( GL_LINES );
	glVertex2d( minX,-10000 );
	glVertex2d( minX,+10000 );
	glVertex2d( maxX,-10000 );
	glVertex2d( maxX,+10000 );
	glVertex2d( -10000,minY );
	glVertex2d( +10000,minY );
	glVertex2d( -10000,maxY );
	glVertex2d( +10000,maxY );
	glEnd();
	glDisable( GL_LINE_STIPPLE );

	// Draw polygon edges
	glLineWidth( 3 );
	glColor3f( 0,0,0 );
	glBegin( GL_LINE_LOOP );
	glVertex3d( minX,minY,0.2 );
	glVertex3d( minX,maxY,0.2 );
	glVertex3d( maxX,maxY,0.2 );
	glVertex3d( maxX,minY,0.2 );
	glEnd();

	// Draw polygon face
	glPolygonMode( GL_FRONT_AND_BACK,GL_FILL );
	glColor4f( 0.5,0.5,1,0.25 );
	glBegin( GL_POLYGON );
	glVertex2d( minX,minY );
	glVertex2d( minX,maxY );
	glVertex2d( maxX,maxY );
	glVertex2d( maxX,minY );
	glEnd();
}


void drawFrames()
{
}


void hideShow()
{
	show = !show;
}

int main()
{
	/// Open window
	if( !openWindow2D( "Cohen-Sutherland bitmasks" ) ) return 1;
	noGround();

	/// Add buttons
	addButton( new Button2D( "buttons/button_show.png",GLFW_KEY_SPACE,hideShow ) );
	addButton( new exitButton2D );

	fnt1 = new Font3D( "Verdana" );
	fnt2 = new Font3D( "Verdana",true );
	for( int i=0; i<8; i++ )
	{
		txt[i] = new Text3D( L"",fnt1 );
		txt[i]->setLighting(false);
		txt[i]->setColor( {0,0,0} );
		txt[i]->setScale( {0.4,0.3,0} );
		txt[i]->setPolygonMode( GL_LINE );
	}
	for( int i=8; i<N; i++ )
	{
		txt[i] = new Text3D( L"",fnt2 );
		txt[i]->setLighting(false);
		txt[i]->setColor( {0.5,0.5,1} );
		txt[i]->setScale( {1.6,1.2,0} );
		txt[i]->setPolygonMode( GL_FILL );
	}

	txt[0]->setText( L"---0" ); txt[0]->setCenter( {-0.2,minY+0.15} );
	txt[1]->setText( L"---1" ); txt[1]->setCenter( {-0.2,minY-0.3} );
	txt[2]->setText( L"--0-" ); txt[2]->setCenter( {-0.2,maxY-0.3} );
	txt[3]->setText( L"--1-" ); txt[3]->setCenter( {-0.2,maxY+0.15} );
	txt[4]->setText( L"-0--" ); txt[4]->setCenter( {minX+0.15,0.7} );
	txt[5]->setText( L"-1--" ); txt[5]->setCenter( {minX-0.8,0.7} );
	txt[6]->setText( L"1---" ); txt[6]->setCenter( {maxX+0.15,0.7} );
	txt[7]->setText( L"0---" ); txt[7]->setCenter( {maxX-0.8,0.7} );
	txt[ 8]->setText( L"0100" ); txt[ 8]->setCenter( {-7.6-1.7,-0.3,2} );
	txt[ 9]->setText( L"0000" ); txt[ 9]->setCenter( {    -1.7,-0.3,2} );
	txt[10]->setText( L"1000" ); txt[10]->setCenter( {+7.3-1.7,-0.3,2} );
	txt[11]->setText( L"0110" ); txt[11]->setCenter( {-7.6-1.7,+4.3,2} );
	txt[12]->setText( L"0010" ); txt[12]->setCenter( {    -1.7,+4.3,2} );
	txt[13]->setText( L"1010" ); txt[13]->setCenter( {+7.3-1.7,+4.3,2} );
	txt[14]->setText( L"0101" ); txt[14]->setCenter( {-7.6-1.7,-5.0,2} );
	txt[15]->setText( L"0001" ); txt[15]->setCenter( {    -1.7,-5.0,2} );
	txt[16]->setText( L"1001" ); txt[16]->setCenter( {+7.3-1.7,-5.0,2} );

	/// Main loop
	while( isRunning() )
	{
		for( int i=8; i<N; i++ ) txt[i]->setIsVisible(show);
	}
	return 0;
}

