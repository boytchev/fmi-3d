
/// Project:	FMI 3D (2013)
/// File:		11101 Quadropus
/// Topic:		11 Animation
/// Slide:		10
///	About:		Animated quadropus
///
/// History:
///		2013.07.30/PB	Initial implementation
///     2013.09.10/PB   Fixed text position
///     2013.09.13/PB   Improved speed


#include "fmi3d.h"
#include "stdio.h"
#include "string.h"

float floorRadius  = 7;
float floorWidth   = 2;
int   columnCount  = 10;
float columnRadius = 0.3;
float columnHeight = 5;

int modes = 3;
int mode = 0;
double delays[6] = {0, 1/10.0, 1/4.0};
double t0,ct;
int c0;

Font2D* font;
Text2D* text;

GLuint idFloor=10, idWall;

void drawFloor()
{
	glPushMatrix();
	glScalef( floorRadius,floorRadius,floorWidth );
	glCallList( idUnitCylinder3D[QUALITY_MEDIUM] );
	glScaled( 1,1,-1 );
	glCallList( idUnitCircle2D[QUALITY_MEDIUM] );
	glScaled( 1,1,-1 );
	glTranslated( 0,0,1 );
	glCallList( idUnitCircle2D[QUALITY_MEDIUM] );
	glColor3f( 1,1,1 );
	glPopMatrix();
}

void drawWall()
{
	glPushMatrix();
	glScalef( floorRadius-1,floorRadius-1,columnHeight );
	glCallList( idUnitCylinder3D[QUALITY_MEDIUM] );
	glPopMatrix();

	for( int i=0; i<columnCount; i++ )
	{
		glPushMatrix();
		glRotatef( i*360/columnCount,0,0,1 );
		glTranslatef( floorRadius-0.5,0,0 );
		glScalef( columnRadius,columnRadius,columnHeight );
		glCallList( idUnitCylinder3D[QUALITY_LOWER] );
		glPopMatrix();
	}
	glTranslatef( 0,0,columnHeight );
}

void drawTower( float z, int floors, float k, float ofs )
{
	glPushMatrix();
	glTranslatef( 0,0,z );

	glCallList( idFloor );
	for( int i=0; i<floors; i++ )
	{
		float f = ( ( float )i )/floors*0.95+0.05;
		glColor3f( f,f,f*0.9+0.1 );
		glCallList( idWall );
		glCallList( idFloor );
		glScalef( k,k,k );
		glRotatef( 10*sin( 2*t0+i/10.0+ofs ),1,i/10.0,0 );
		glRotatef( 10*sin( 0.8*t0-ofs ),0,1,cos( i/10+ofs ) );
	}

	glPopMatrix();
}

void drawScene()
{
	if( t-t0>delays[mode] )
	{
		t0=t;
		c0++;
	}
	glDisable( GL_TEXTURE_2D );
	glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, 10 );
	glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, whiteSpecular );
	glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, noEmission );

	glScaled( 0.8,0.8,0.8 );
	glRotated( 30*t0,1,0,1 );
	glRotated( 47*t0,0,1,-1 );

	glPushMatrix();
	glColor3f( 0.7,0.7,0.7 );
	glScaled( 10,10,10 );
	glCallList( idUnitSphere3D[QUALITY_HIGH] );
	glColor3f( 1,1,1 );
	glPopMatrix();

	drawTower( 7,50,0.9+0.05*sin( t0 ),0 );
	for( int i = 0; i<3; i++ )
	{
		glPushMatrix();
		glRotatef( 120*i,0,0,1 );
		glRotatef( 120-10,0,1,0 );
		drawTower( 7,50,0.9+0.05*cos( t0+128*i*M_PI/180 ),0 );
		glPopMatrix();
	}

	glDisable( GL_DEPTH_TEST );
	glDepthMask( GL_FALSE );

	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
	glOrtho( -windowWidth/2, windowWidth/2, -windowHeight/2, +windowHeight/2, -2000, 2000 );

	/// Draw buttons
	glMatrixMode( GL_MODELVIEW );

	glPushMatrix();
//	gluLookAt(	windowWidth/17.5,-windowHeight/17,100,
//				windowWidth/17.5,-windowHeight/17,0,
//				0,1,0 );
    glLoadIdentity();
    glTranslated(-windowWidth/2+84,windowHeight/2-64,0);
	text->drawImage();
	glPopMatrix();

	/// Restore projection
	glMatrixMode( GL_PROJECTION );
	glPopMatrix(); //projection
	glMatrixMode( GL_MODELVIEW );

	glEnable( GL_LIGHTING );
	glDepthMask( GL_TRUE );
	glEnable( GL_DEPTH_TEST );
}

void drawFrames()
{
}

Button2D * fB;
void changeDelay()
{
	mode = ( mode+1 )%modes;
	fB->setState( mode );
	c0 = 0;
	ct = t;
}

void updateFps()
{
	char res8[100];
	wchar_t res[100];
	if( t-ct>2 )
	{
		sprintf( res8,"fps=%.1f",c0/( t-ct ) );
		int len=strlen( res8 );
		for( int i=0; i<len+1; i++ ) res[i]=res8[i];
		text->setText( res );
	}
	else
		text->setText( L"" );
}

int main()
{
	useCustomFogColor = true;
	customFogColor[0] = 0.3;
	customFogColor[1] = 0.3;
	customFogColor[2] = 0.6;
	customFogColor[3] = 1;

	if( !openWindow3D( "Quadropus" ) ) return 1;
	changeBrightness();
	noGround();

	fB = new Button2D( "buttons/button_time.png",GLFW_KEY_SPACE,changeDelay,mode,modes );
	addButton( new exitButton2D() );

	font = new Font2D( "Consolas",48,true );
	text = new Text2D( L"",font );
	text->setCenter( {0,0,0} );
	text->setColor( {0.75,0.75,1} );

	idFloor = ++UID;
	glNewList( idFloor,GL_COMPILE );
	drawFloor();
	glEndList();

	idWall = ++UID;
	glNewList( idWall,GL_COMPILE );
	drawWall();
	glEndList();

	viewDistance = 100;

	t0 = t;
	c0 = 0;
	ct = t;
	while( isRunning() )
	{
		updateFps();
	}
	return 0;
}

