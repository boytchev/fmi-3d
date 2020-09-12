
/// Project:	FMI 3D (2013)
/// File:		fmi3d_primitives.cpp
///	About:		Module for graphical primitives
///
/// History:
///		2013.04.27/PB	Initial version
///		2013.05.05/PB	Paper plane, cylinder
///		2013.05.06/PB	Cone, circle, sphere
///		2013.05.23/PB	Arrow


#include <GL/glfw.h>
#include "fmi3d_lib.h"
#include "fmi3d_primitives.h"
#include "fmi3d_objects.h"

int UID = 0;				/// Unique identifier used to manage display lists
GLuint idUnitCube3D;		/// Precompiled display list for unit cube
GLuint idUnitPaperPlane3D;	/// Precompiled display list for unit paper plane
GLuint idUnitCylinder3D[MAX_QUALITY];	/// Precompiled display list for unit cylinder
GLuint idUnitCone3D[MAX_QUALITY];		/// Precompiled display list for unit cone
GLuint idUnitSphere3D[MAX_QUALITY];		/// Precompiled display list for unit sphere
GLuint idUnitCircle2D[MAX_QUALITY];		/// Precompiled display list for unit circle
GLuint idUnitArrow3D;		/// Precompiled display list for unit cube

int quality[MAX_QUALITY] = {128,64,32,16,8};

/// Generate diaply list for unit cube
GLuint unitCube3D()
{
#define a 0.5
	glNewList( ++UID,GL_COMPILE );

	glBegin( GL_POLYGON ); /// -X
	{
		glNormal3d( -1, 0, 0 );
		glTexCoord2d( 0,0 );
		glVertex3d( -a, -a, -a );
		glTexCoord2d( 0,1 );
		glVertex3d( -a, -a, +a );
		glTexCoord2d( 1,1 );
		glVertex3d( -a, +a, +a );
		glTexCoord2d( 1,0 );
		glVertex3d( -a, +a, -a );
	}
	glEnd();

	glBegin( GL_POLYGON ); /// +X
	{
		glNormal3d( +1, 0, 0 );
		glTexCoord2d( 0,0 );
		glVertex3d( +a, -a, -a );
		glTexCoord2d( 0,1 );
		glVertex3d( +a, -a, +a );
		glTexCoord2d( 1,1 );
		glVertex3d( +a, +a, +a );
		glTexCoord2d( 1,0 );
		glVertex3d( +a, +a, -a );
	}
	glEnd();

	glBegin( GL_POLYGON ); /// -Y
	{
		glNormal3d( 0, -1, 0 );
		glTexCoord2d( 0,0 );
		glVertex3d( -a, -a, -a );
		glTexCoord2d( 0,1 );
		glVertex3d( -a, -a, +a );
		glTexCoord2d( 1,1 );
		glVertex3d( +a, -a, +a );
		glTexCoord2d( 1,0 );
		glVertex3d( +a, -a, -a );
	}
	glEnd();

	glBegin( GL_POLYGON ); /// +Y
	{
		glNormal3d( 0, +1, 0 );
		glTexCoord2d( 0,0 );
		glVertex3d( -a, +a, -a );
		glTexCoord2d( 0,1 );
		glVertex3d( -a, +a, +a );
		glTexCoord2d( 1,1 );
		glVertex3d( +a, +a, +a );
		glTexCoord2d( 1,0 );
		glVertex3d( +a, +a, -a );
	}
	glEnd();

	glBegin( GL_POLYGON ); /// -Z
	{
		glNormal3d( 0, 0, -1 );
		glTexCoord2d( 0,0 );
		glVertex3d( -a, -a, -a );
		glTexCoord2d( 0,1 );
		glVertex3d( -a, +a, -a );
		glTexCoord2d( 1,1 );
		glVertex3d( +a, +a, -a );
		glTexCoord2d( 1,0 );
		glVertex3d( +a, -a, -a );
	}
	glEnd();

	glBegin( GL_POLYGON ); /// +Z
	{
		glNormal3d( 0, 0, +1 );
		glTexCoord2d( 0,0 );
		glVertex3d( -a, -a, +a );
		glTexCoord2d( 0,1 );
		glVertex3d( -a, +a, +a );
		glTexCoord2d( 1,1 );
		glVertex3d( +a, +a, +a );
		glTexCoord2d( 1,0 );
		glVertex3d( +a, -a, +a );
	}
	glEnd();
	glEndList();

	return UID;
}


/// Generate diaply list for paper plane. The center is at the back bottom
/// middle point. Forward is along X direction.
GLuint unitPaperPlane3D()
{
	glNewList( ++UID,GL_COMPILE );
	glLightModeli( GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE );
	glBegin( GL_TRIANGLES );

	{
		/// Right wing
		glNormal3d( 0, 0, 1 );
		glTexCoord2d( 0.5,1 );
		glVertex3d( 1, 0, 0 );

		glTexCoord2d( 0.25,0 );
		glVertex3d( 0, -0.1, 0.5 );

		glTexCoord2d( 0,0 );
		glVertex3d( 0, -0.5, 0.5 );
	}

	{
		/// Left wing
		glNormal3d( 0, 0, 1 );
		glTexCoord2d( 0.5,1 );
		glVertex3d( 1, 0, 0 );

		glTexCoord2d( 1,0 );
		glVertex3d( 0, 0.5, 0.5 );

		glTexCoord2d( 0.75,0 );
		glVertex3d( 0, 0.1, 0.5 );
	}

	{
		/// Right body
		glNormal3d( 0, 1, 0.5 );
		glTexCoord2d( 0.5,1 );
		glVertex3d( 1, 0, 0 );

		glTexCoord2d( 0.5,0 );
		glVertex3d( 0, 0, 0 );

		glTexCoord2d( 0,0 );
		glVertex3d( 0, -0.1, 0.5 );
	}

	{
		/// Left body
		glNormal3d( 0, 1, -0.5 );
		glTexCoord2d( 0.5,1 );
		glVertex3d( 1, 0, 0 );

		glTexCoord2d( 0.5,0 );
		glVertex3d( 0, 0, 0 );

		glTexCoord2d( 0,0 );
		glVertex3d( 0, 0.1, 0.5 );
	}

	glEnd();

	glLightModeli( GL_LIGHT_MODEL_TWO_SIDE,GL_FALSE );
	glEndList();

	return UID;
}

/// Generate diaply list for unit cylinder
GLuint unitCylinder3D( int precision )
{
	double dalpha = 2*M_PI/precision;
	double dprec = 1.0/precision;
	double dx = cos( dalpha );
	double dy = sin( dalpha );

	double x1 = 1;
	double y1 = 0;

	glNewList( ++UID,GL_COMPILE );

	glBegin( GL_QUAD_STRIP ); /// side
	glNormal3f( x1, y1, 0 );
	glTexCoord2d( -dprec, 0 );
	glVertex3f( x1, y1, 0 );
	glTexCoord2d( -dprec, 1 );
	glVertex3f( x1, y1, 1 );

	for( int i=0; i<precision; i++ )
	{
		double x2 = x1*dx-y1*dy;
		double y2 = y1*dx+x1*dy;

		glNormal3f( x2, y2, 0 );
		glTexCoord2d( i*dprec, 0 );
		glVertex3f( x2, y2, 0 );
		glTexCoord2d( i*dprec, 1 );
		glVertex3f( x2, y2, 1 );

		x1 = x2;
		y1 = y2;
	}

	glEnd();
	glEndList();
	return UID;
}

/// Generate diaply list for unit cone
GLuint unitCone3D( int precision )
{
	double dalpha = 2*M_PI/precision;
	double dprec = 1.0/precision;
	double dx = cos( dalpha );
	double dy = sin( dalpha );

	double x1 = 1;
	double y1 = 0;

	glNewList( ++UID,GL_COMPILE );

	glBegin( GL_QUAD_STRIP ); /// side
	glNormal3f( x1, y1, 0 );
	glTexCoord2d( -dprec, 0 );
	glVertex3f( x1, y1, 0 );
	glNormal3f( 0, 0, 1 );
	glTexCoord2d( -dprec, 1 );
	glVertex3f( 0, 0, 1 );

	for( int i=0; i<precision; i++ )
	{
		double x2 = x1*dx-y1*dy;
		double y2 = y1*dx+x1*dy;

		glNormal3f( x2, y2, 0 );
		glTexCoord2d( i*dprec, 0 );
		glVertex3f( x2, y2, 0 );
		glNormal3f( 0, 0, 1 );
		glTexCoord2d( i*dprec, 1 );
		glVertex3f( 0, 0, 1 );

		x1 = x2;
		y1 = y2;
	}

	glEnd();
	glEndList();
	return UID;
}

/// Generate diaply list for unit circle facing down
GLuint unitCircle2D( int precision )
{
	double dalpha = 2*M_PI/precision;
	double dx = cos( dalpha );
	double dy = sin( dalpha );

	double x1 = 1;
	double y1 = 0;

	glNewList( ++UID, GL_COMPILE );

	glNormal3f( 0,0,1 );
	glBegin( GL_POLYGON );
	for( int i=0; i<precision; i++ )
	{
		double x2 = x1*dx-y1*dy;
		double y2 = y1*dx+x1*dy;

		glTexCoord2d( x1, y1 );
		glVertex3f( x1, y1, 0 );

		x1 = x2;
		y1 = y2;
	}
	glEnd();
	glEndList();

	return UID;
}


/// Generate diaply list for unit sphere
GLuint unitSphere3D( int precision )
{
	double alpha;
	double dalpha = 2.0*M_PI/precision;
	double beta = M_PI/2;
	double dbeta = 1.0*M_PI/precision;
	double tx;
	double ty;
	double dprec = 1.0/precision;

	glNewList( ++UID,GL_COMPILE );

	ty = 1;
	for( int j=0; j<precision; j++, beta-=dbeta )
	{
		alpha = 0;
		tx = 0;
		for( int i=0; i<precision; i++, alpha+=dalpha )
		{
			vect_t p;
			glBegin( GL_QUADS );

			p = p.s3d( alpha,beta,1 );
			glTexCoord2d(tx,ty);
			glNormal3f( p.x,p.y,p.z );
			glVertex3f( p.x,p.y,p.z );

			p = p.s3d( alpha+dalpha,beta,1 );
			glTexCoord2d(tx+dprec,ty);
			glNormal3f( p.x,p.y,p.z );
			glVertex3f( p.x,p.y,p.z );

			p = p.s3d( alpha+dalpha,beta-dbeta,1 );
			glTexCoord2d(tx+dprec,ty-dprec);
			glNormal3f( p.x,p.y,p.z );
			glVertex3f( p.x,p.y,p.z );

			p = p.s3d( alpha,beta-dbeta,1 );
			glTexCoord2d(tx,ty-dprec);
			glNormal3f( p.x,p.y,p.z );
			glVertex3f( p.x,p.y,p.z );

			glEnd( );

			tx+=dprec;
		}
		ty-=dprec;
	}

	glEndList();
	return UID;
}


/// Generate diaply list for unit arrow
GLuint unitArrow3D( int precision )
{
	double dalpha = 2*M_PI/precision;
	double dx = cos( dalpha );
	double dy = sin( dalpha );

	double x1 = 1;
	double y1 = 0;

	glNewList( ++UID,GL_COMPILE );

	glBegin( GL_TRIANGLE_FAN ); /// side
	glVertex3f( 0, 0, 1 );

	for( int i=0; i<=precision; i++ )
	{
		glVertex3f( x1, y1, 0 );

		double x2 = x1*dx-y1*dy;
		double y2 = y1*dx+x1*dy;
		x1 = x2;
		y1 = y2;
	}

	glEnd();
	glEndList();
	return UID;
}


/// Create display lists for all supported graphical primitives
void createPrimitives()
{
	idUnitCube3D		= unitCube3D();
	idUnitPaperPlane3D	= unitPaperPlane3D();
	idUnitArrow3D		= unitArrow3D(8);

	for( int i=0; i<MAX_QUALITY; i++ )
	{
		idUnitCircle2D[i]	= unitCircle2D( quality[i] );
		idUnitCylinder3D[i]	= unitCylinder3D( quality[i] );
		idUnitCone3D[i]		= unitCone3D( quality[i] );
		idUnitSphere3D[i]	= unitSphere3D( quality[i] );
	}
}
