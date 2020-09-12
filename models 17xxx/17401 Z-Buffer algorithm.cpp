
/// Project:	FMI 3D (2013)
/// File:		17401 Z-Buffer algorithm.cpp
/// Topic:		17 Cutting
/// Slide:		40
///	About:		Z-Buffer algorithm
///
/// History:
///		2013.08.09/PB	Initial implementation
///     2013.09.13/PB   Cubes replaced by squares


#include "fmi3d.h"

#define ANIMATION_TIME 2

class Square3D : public Object3D
{
	public:
		Square3D( double size );
};

/// Create an object with default orientation
Square3D::Square3D( double size ) : Object3D()
{
	setSize( size );

#define a 0.5

	/// Create display frame list of the cube
	glNewList( frameList,GL_COMPILE );
	glBegin( GL_POLYGON ); /// +Z
	{
		glNormal3d( 0, 0, +1 );
		glVertex2d( -a, -a );
		glVertex2d( -a, +a );
		glVertex2d( +a, +a );
		glVertex2d( +a, -a );
	}
	glEnd();
	glEndList();
#undef a
	/// Create display list of the cube
	glNewList( imageList,GL_COMPILE );
	glCallList( frameList );
	glEndList();
}

#define N 2
#define S 3
#define M 21
vect_t p[N][S];
vect_t col[N];
Square3D *c[N][M][M];
int showC = 0;
double h=0;

void drawScene()
{
	if( showC>0 )
		for( int n=0; n<N; n++ )
		{
			glPushMatrix();
			glTranslated( 0,h*( n-( N-1 )/2.0 ),0 );
			for( int x=0; x<M; x++ )
				for( int y=0; y<M; y++ )
				{

				c[n][x][y]->drawImage();
				}
			glPopMatrix();
		}

	if( showC<1 )
	{

		glEnable( GL_DEPTH_TEST );

		glDisable( GL_LIGHTING );
		glDisable( GL_TEXTURE_2D );
		glPolygonMode( GL_FRONT_AND_BACK,GL_FILL );
		glEnable( GL_POLYGON_OFFSET_FILL );
		glPolygonOffset( 1,1 );
		if( showC==0 )
			for( int i=0; i<N; i++ )
			{
				glPushMatrix();
				glTranslated( 0,h*( i-( N-1 )/2.0 ),0 );
				if( inShadow )
					glColor3f(0,0,0);
				else
					glColor4f( col[i].x,col[i].y,col[i].z,1 );
				glBegin( GL_POLYGON );
				for( int j=0; j<S; j++ )
					glVertex3f( p[i][j].x,p[i][j].y,p[i][j].z );
				glEnd();
				glPopMatrix();
			}
		glDisable( GL_POLYGON_OFFSET_FILL );
		glEnable( GL_LINE_SMOOTH );
		glLineWidth( 2.5 );
		glPolygonMode( GL_FRONT_AND_BACK,GL_LINE );
		glColor4f( 0,0,0,1 );
		for( int i=0; i<N; i++ )
		{
			glPushMatrix();
			glTranslated( 0,h*( i-( N-1 )/2.0 ),0 );
			glBegin( GL_POLYGON );
			for( int j=0; j<S; j++ )
				glVertex3f( p[i][j].x,p[i][j].y,p[i][j].z );
			glEnd();
			glPopMatrix();
		}
	}
}


void drawFrames()
{
	if( showC>0 )
		for( int n=0; n<N; n++ )
		{
			glPushMatrix();
			glTranslated( 0,h*( n-( N-1 )/2.0 ),0 );
			for( int x=0; x<M; x++ )
				for( int y=0; y<M; y++ )
					c[n][x][y]->drawFrame();
			glPopMatrix();
		}
}

void raise( int i, vect_t a )
{
	int x = int( a.x*2+( M )/2.0 );
	int y = int( a.y*2+( M )/2.0 );
	//double z = ( a.z*2+1 )/2.0-0.5;
	double z = a.z;
	if( x>=0 && x<M && y>=0 && y<M )
	{
		//c[i][x][y]->setScale( {0.5,0.5,z} );
		c[i][x][y]->oxyz.o.z = z;
		c[i][x][y]->setColor( col[i] );
		//c[i][x][y]->setIsVisible( true );
	}
}

void triangle( int i, vect_t a, vect_t b, vect_t c, int n )
{
	if( n<1 ) return;
	vect_t ab = a.add( b ).mul( 0.5 );
	vect_t bc = b.add( c ).mul( 0.5 );
	vect_t ca = c.add( a ).mul( 0.5 );
	raise( i,ab );
	raise( i,bc );
	raise( i,ca );
	triangle( i,a,ab,ca,n-1 );
	triangle( i,b,bc,ab,n-1 );
	triangle( i,c,ca,bc,n-1 );
	triangle( i,ab,bc,ca,n-1 );
}

void randomize()
{
	for( int i=0; i<N; i++ )
	{
        col[i] = {random( 0,1 ),random( 0,1 ),random( 0,1 )};
		while( col[0].distance(col[1])<0.4 )
            col[i] = {random( 0,1 ),random( 0,1 ),random( 0,1 )};
		double x = random(-1,1);
		double y = random(-1,1);
		double o = random( 0,2*M_PI );
		for( int j=0; j<S; j++ )
		{
			double z = 1+2*j;
			double a = double( j )/3*M_PI*2+o;
			double r = random( 3,7 );
			p[i][j]= {x+r*cos( a ),y+r*sin( a ),z};
		}

		for( int x=0; x<M; x++ )
			for( int y=0; y<M; y++ )
			{
				c[i][x][y]->oxyz.o.z = 0.5;
				c[i][x][y]->setColor( {0.2,0.2,0.2} );
				//c[i][x][y]->setIsVisible(false);
			}
		triangle( i,p[i][0],p[i][1],p[i][2],5 );
	}
}

void changeShow()
{
	showC = ( showC+1 )%2;
}

double splitT;
double splitH = 0;
void splitUnsplit()
{
	splitT = t;
	splitH = 11-splitH;
}


int main()
{
	/// Open window
	if( !openWindow3D( "Z-Buffer algorithm" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();

	/// Add buttons
//	nB = new Button2D( "buttons/button_next.png",GLFW_KEY_SPACE,fall,0,N );
	addButton( new Button2D( "buttons/button_cut.png",GLFW_KEY_SPACE,splitUnsplit ) );
	addButton( new Button2D( "buttons/button_half.png",'S',changeShow) );
	addButton( new Button2D( "buttons/button_random.png",'R',randomize ) );
	addButton( new exitButton2D );

	viewZ = 3;
	viewBeta = 1;

	//GLuint tex = loadTexture("17401 Texture.jpg");

	for( int n=0; n<N; n++ )
		for( int x=0; x<M; x++ )
			for( int y=0; y<M; y++ )
			{
				c[n][x][y] = new Square3D( 0.5 );
				c[n][x][y]->setCenter( {( x-( M-1 )/2.0 )/2,( y-( M-1 )/2.0 )/2,1} );
				c[n][x][y]->setLighting( false );
			}

	randomize();


	/// Main loop
	while( isRunning() )
	{
		double k = 1;
		if( t-splitT<ANIMATION_TIME )
		{
			k = cos( ( ( t-splitT )/ANIMATION_TIME )*M_PI/2 );
			h = h*k+splitH*( 1-k );
		}
	}
	return 0;
}

