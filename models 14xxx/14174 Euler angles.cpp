
/// Project:	FMI 3D (2013)
/// File:		14174 Euler angles.cpp
/// Topic:		14 Orientation
/// Slide:		17
///	About:		Euler angles
///
/// History:
///		2013.08.10/PB	Initial implementation
///     2013.09.13/PB   Increased sizes of arrays


#include "fmi3d.h"

#define S 2.7
#define N (int(400*S))

Sphere3D *s;
CoordSys3D *cs,*cse;
vect_t p[3][N+1];
vect_t l[6];
vect_t col[3];
bool show = true;

Object3D *q;
int n = 0;

void drawScene()
{
	glDisable( GL_TEXTURE_2D );
	cs->drawImage();
	cse->drawImage();

	glEnable( GL_DEPTH_TEST );
	glDepthMask( GL_TRUE );

	s->setPolygonMode( GL_FILL );
	s->setColor( {0.75,0.75,1} );
	glEnable( GL_CULL_FACE );
	glCullFace( GL_FRONT );
	s->drawImage();
	glCullFace( GL_BACK );
	s->drawImage();
	glDisable( GL_CULL_FACE );

	glEnable( GL_LINE_SMOOTH );
	s->setPolygonMode( GL_LINE );
	s->setColor( {0,0,0} );
	s->setScale( {4.51,4.51,4.51} );
	s->drawImage();
	s->setSize( 4.5 );

	if( show )
	{
		glDepthMask( GL_FALSE );
		glLineWidth( 2 );
		glColor3f( 1,0,0 );
		for( int j=0; j<3; j++ )
		{
			glColor3f( col[j].x,col[j].y,col[j].z );
			glBegin( GL_LINE_STRIP );
			for( int i=0; i<n; i++ ) p[j][i].vertex();
			glEnd();
		}

		glPolygonMode( GL_FRONT_AND_BACK,GL_FILL );
		for( int j=0; j<3; j++ )
		{
			glColor4f( col[j].x,col[j].y,col[j].z,0.2 );
			glBegin( GL_TRIANGLE_FAN );
			glVertex3f( 0,0,5 );
			for( int i=0; i<n; i++ )
			{
				p[j][i].vertex();
			}
			glEnd();
		}

		glLineWidth( 1 );
		glColor3f( 0.5,0.5,0.5 );
		glBegin( GL_LINES );
		for( int i=0; i<6; i++ )
		{
			int k=1;
			switch( i )
			{
				case 0: case 3: k=0; break;
				case 1: case 2: case 4: k=1; break;
				case 5: k=2; break;
			}
			glColor3f( col[k].x,col[k].y,col[k].z );
			glVertex3f( 0,0,5 );
			l[i].vertex();
		}
		glEnd();
	}
	glDepthMask( GL_TRUE );
}


void drawFrames()
{
	cs->drawFrame();
	cse->drawFrame();
}


void showHide()
{
	show = !show;
}


int main()
{
	/// Open window
	if( !openWindow3D( "Euler angles" ) ) return 1;

	/// Add buttons
	addButton( new Button2D("buttons/button_show.png",GLFW_KEY_SPACE,showHide));
	addButton( new exitButton2D );
	changeGround();
	changeGround();
	changeGround();

	viewAlpha = M_PI/4;
	viewBeta = M_PI/4;
	viewZ = 5;

	s = new Sphere3D( 4.5,QUALITY_LOW );
	s->setShadeModel( GL_FLAT );
	s->setScale( {5,5,-5} );
	s->setCenter( {0,0,5} );

	cs = new CoordSys3D( -5,8,-5,8,-5,6.2, false, 0, 0.3, L"XYZ" );
	cs->setAxesNamesColor( {0,0,0} );
	cs->setCenter( {0,0,5} );
	cse = new CoordSys3D( 0.2,6,0.2,7,0.2,6, false, 0, 0.5, L"XYZ" );
	cse->setColor( {0,0,0.9} );
	cse->setAxesNamesColor( {0,0,1} );
	cse->setCenter( {0,0,5} );

	col[0] = {1,0,0};
	col[1] = {1,0.8,0};
	col[2] = {0,0.8,0};

	q = new Object3D();
	for( int i=0; i<6; i++ ) l[i]= {0,0,0};

	/// Main loop
	isRunning();
	double t1=0,t0=t,a=-1,aold=-1;
	while( isRunning() )
	{
		t1+=( t-t0 );
		t0=t;

		aold=a;
		a = 92-92*cos( t1/S );
		cse->clearOrientation();
		s->clearOrientation();
		q->identity();
		q->setCenter( {0,0,5} );

		if( a<= 60 )
		{
			cse->rotateLocal( a, {0,0,1} );
			s->rotateLocal( a, {0,0,1} );
			q->rotateLocal( a, {0,0,1} );
		}
		else if( a<= 124 )
		{
			cse->rotateLocal( 60, {0,0,1} );
			cse->rotateLocal( a-60, {1,0,0} );
			s->rotateLocal( 60, {0,0,1} );
			s->rotateLocal( a-60, {1,0,0} );
			q->rotateLocal( 60, {0,0,1} );
			q->rotateLocal( a-60, {1,0,0} );
		}
		else
		{
			cse->rotateLocal( 60, {0,0,1} );
			cse->rotateLocal( 64, {1,0,0} );
			cse->rotateLocal( a-124, {0,0,1} );
			s->rotateLocal( 60, {0,0,1} );
			s->rotateLocal( 64, {1,0,0} );
			s->rotateLocal( a-124, {0,0,1} );
			q->rotateLocal( 60, {0,0,1} );
			q->rotateLocal( 64, {1,0,0} );
			q->rotateLocal( a-124, {0,0,1} );
		}

		if( t1/S<M_PI )
		{
			q->translateLocal( {6.75,0,0} );
			p[0][n]=q->getCenter();
			q->translateLocal( {-6.75,7.75,0} );
			p[1][n]=q->getCenter();
			q->translateLocal( {0,-7.75,6.75} );
			p[2][n]=q->getCenter();

			if( aold<60 && 60<a )
			{
				l[0] = p[0][n-1];
				l[1] = p[1][n-1];
			}
			if( aold<124.5 && 124.5<a )
			{
				l[2] = p[1][n-1];
			}
			if( true )
			{
				l[3] = p[0][n-1];
				l[4] = p[1][n-1];
				l[5] = p[2][n-1];
			}
			n++;
		}
	}
	return 0;
}

