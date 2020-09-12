
/// Project:	FMI 3D (2013)
/// File:		fmi3d_objects.cpp
///	About:		Module for 3D objects
///
/// History:
///		2013.04.27/PB	Implementation of Object3D, coordinate system, transformations
///		2013.05.01/PB	Vector and matrix manipulation
///		2013.05.03/PB	Shadows when ground is non-reflective
///		2013.05.05/PB	Paper plane, move size parameter to Object3D, cylinder
///		2013.05.06/PB	Circle, cone, sphere
///		2013.05.09/PB	Pawn, fixed bug in cylinder and cone
///		2013.05.10/PB	Color picking, drag and drop
///		2013.05.12/PB	Active color and full object interactivity, interactivePoint()
///		2013.05.16/PB	point2D(),point3D()
///		2013.05.18/PB	Visibility of objects, CoordSys2D
///		2013.05.22/PB	Font2D, Font3D, Text2D, Text3D, inReflection, CoordSys3D
///		2013.05.23/PB	Vector3D
///		2013.05.29/PB	Bezier3D
///		2013.05.31/PB	Object3D::setLighting();
///		2013.06.01/PB	Subdiv3D, Object3D::getTag(),setTag()
///		2013.07.24/PB	Possible fix of bug in Bezier3D
///		2013.08.10/PB	CoordSys2D/3D can be drawn without coordinate marks if suunites==0


#include <list>

#include "math.h"
#include "windows.h"
#include "GL/glu.h"

#include "fmi3d_objects.h"
#include "fmi3d_primitives.h"
#include "fmi3d_lib.h"
#include "fmi3D_camera.h"

//#include <iostream>
//#include <iomanip>

using namespace std;


Font3D* systemFont3D;
Font2D* systemFont2D;

/// ===========================================================================
/// Vector and matrix management
/// ===========================================================================

/// Construct 3D vector
vect_t vect_t::v3d( double x, double y, double z )
{
	return {x,y,z,0};
}

/// Construct 3D point
vect_t vect_t::p3d( double x, double y, double z )
{
	return {x,y,z,1};
}

/// Construct 3D point from spherical coordinates (angles in radians)
vect_t vect_t::s3d( double alpha, double beta, double r )
{
	return {r*cos( alpha )*cos( beta ),r*sin( alpha )*cos( beta ),r*sin( beta ),1};
}

/// Distance to point
double vect_t::distance( vect_t p )
{
	return sqrt( ( p.x-x )*( p.x-x ) + ( p.y-y )*( p.y-y ) + ( p.z-z )*( p.z-z ) );
}
double vect_t::distance(  )
{
	return sqrt( x*x + y*y + z*z );
}

/// Addition with vector
vect_t vect_t::add( vect_t p )
{
	return {x+p.x,y+p.y,z+p.z,w+p.w};
}

/// Subtraction with vector
vect_t vect_t::sub( vect_t p )
{
	return {x-p.x,y-p.y,z-p.z,w-p.w};
}

/// Multiplication with scalar
vect_t vect_t::mul( double k )
{
	return {x*k,y*k,z*k,w*k};
}

/// Scalar multiplication with vector
double vect_t::smul( vect_t p )
{
	return x*p.x+y*p.y+z*p.z+w*p.w;
}

/// Vector multiplication with vector
vect_t vect_t::vmul( vect_t p )
{
	return {y*p.z-z*p.y,z*p.x-x*p.z,x*p.y-y*p.x};
}

/// Multiplication of point with matrix
vect_t vect_t::mmul( oxyz_t m )
{
	vect_t v = {x,y,z,w};
	vect_t r;
	r.x = v.smul( m.ox );
	r.y = v.smul( m.oy );
	r.z = v.smul( m.oz );
	r.w = v.smul( m.o );

	return {r.x/r.w, r.y/r.w, r.z/r.w, 1};
}

/// Swap (x,y) with (z,w)
void vect_t::swap( )
{
	double a;
	a=x; x=z; z=a;
	a=y; y=w; w=a;
}

/// Generate vertex
void vect_t::vertex()
{
	glVertex3d( x,y,z );
}

/// Initialize with the identity matrix
void oxyz_t::identity()
{
	ox = {1,0,0,0};
	oy = {0,1,0,0};
	oz = {0,0,1,0};
	o  = {0,0,0,1};
}

/// Initialize with translational matrix
void oxyz_t::translate( vect_t v )
{
	ox = {1,0,0,v.x};
	oy = {0,1,0,v.y};
	oz = {0,0,1,v.z};
	o  = {0,0,0,1};
}

/// Initialize with scale matrix
void oxyz_t::scale( vect_t v )
{
	ox = {v.x,0,0,0};
	oy = {0,v.y,0,0};
	oz = {0,0,v.z,0};
	o  = {0,0,0,1};
}

/// Initialize with rotation around Z matrix
void oxyz_t::rotateZ( double alpha )
{
	ox = {cos( alpha ),-sin( alpha ),0,0};
	oy = {sin( alpha ),cos( alpha ),0,0};
	oz = {0,0,1,0};
	o  = {0,0,0,1};
}

/// Initialize with rotation around Y matrix
void oxyz_t::rotateY( double alpha )
{
	ox = {cos( alpha ),0,sin( alpha ),0};
	oy = {0,1,0,0};
	oz = {-sin( alpha ),cos( alpha ),0,0};
	o  = {0,0,0,1};
}

/// Multiply with another matrix
oxyz_t oxyz_t::multiply( oxyz_t b )
{
	oxyz_t c;
	c.ox.x = (*this).ox.x*b.ox.x+(*this).ox.y*b.oy.x+(*this).ox.z*b.oz.x+(*this).ox.w*b.o.x;
	c.ox.y = (*this).ox.x*b.ox.y+(*this).ox.y*b.oy.y+(*this).ox.z*b.oz.y+(*this).ox.w*b.o.y;
	c.ox.z = (*this).ox.x*b.ox.z+(*this).ox.y*b.oy.z+(*this).ox.z*b.oz.z+(*this).ox.w*b.o.z;
	c.ox.w = (*this).ox.x*b.ox.w+(*this).ox.y*b.oy.w+(*this).ox.z*b.oz.w+(*this).ox.w*b.o.w;

	c.oy.x = (*this).oy.x*b.ox.x+(*this).oy.y*b.oy.x+(*this).oy.z*b.oz.x+(*this).oy.w*b.o.x;
	c.oy.y = (*this).oy.x*b.ox.y+(*this).oy.y*b.oy.y+(*this).oy.z*b.oz.y+(*this).oy.w*b.o.y;
	c.oy.z = (*this).oy.x*b.ox.z+(*this).oy.y*b.oy.z+(*this).oy.z*b.oz.z+(*this).oy.w*b.o.z;
	c.oy.w = (*this).oy.x*b.ox.w+(*this).oy.y*b.oy.w+(*this).oy.z*b.oz.w+(*this).oy.w*b.o.w;

	c.oz.x = (*this).oz.x*b.ox.x+(*this).oz.y*b.oy.x+(*this).oz.z*b.oz.x+(*this).oz.w*b.o.x;
	c.oz.y = (*this).oz.x*b.ox.y+(*this).oz.y*b.oy.y+(*this).oz.z*b.oz.y+(*this).oz.w*b.o.y;
	c.oz.z = (*this).oz.x*b.ox.z+(*this).oz.y*b.oy.z+(*this).oz.z*b.oz.z+(*this).oz.w*b.o.z;
	c.oz.w = (*this).oz.x*b.ox.w+(*this).oz.y*b.oy.w+(*this).oz.z*b.oz.w+(*this).oz.w*b.o.w;

	c.o.x = (*this).o.x*b.ox.x+(*this).o.y*b.oy.x+(*this).o.z*b.oz.x+(*this).o.w*b.o.x;
	c.o.y = (*this).o.x*b.ox.y+(*this).o.y*b.oy.y+(*this).o.z*b.oz.y+(*this).o.w*b.o.y;
	c.o.z = (*this).o.x*b.ox.z+(*this).o.y*b.oy.z+(*this).o.z*b.oz.z+(*this).o.w*b.o.z;
	c.o.w = (*this).o.x*b.ox.w+(*this).o.y*b.oy.w+(*this).o.z*b.oz.w+(*this).o.w*b.o.w;

	return c;
}

/// Rebuild OX and OY axis from OZ axis
void oxyz_t::rebuild()
{
	/// Ortogonal axes
	oy = oz.add({M_PI/2,M_PI/3,M_PI/4});
	ox = oy.vmul(oz);
	oy = oz.vmul(ox);

	/// Unit lengths
	ox = ox.mul(1/ox.distance());
	oy = oy.mul(1/oy.distance());
	oz = oz.mul(1/oz.distance());
}

/// ===========================================================================
/// A general 3D object class. Used as a parent of other 3D objects. Contains
/// commands to manage object position, orientation, size and visualization.
/// ===========================================================================

/// Create an object with default orientation
list<Object3D*>objects; /// list of all created objects
Object3D::Object3D()
{
	identity();
	imageList=++UID;
	if( glIsList( imageList ) ) glDeleteLists( imageList,1 );
	frameList=++UID;
	if( glIsList( frameList ) ) glDeleteLists( frameList,1 );

	setCenter( {0,0,0} );
	setScale( {1,1,1} );
	setColor( {1,1,1} );
	setActiveColor( {1,0,0} );
	setTexture( noTexture );
	setTextureSize( 1 );
	setPolygonMode( GL_FILL );
	setLighting( true );
	setShadeModel( GL_SMOOTH );
	setShininess( 0 );
	setMouseMove( NULL );
	setKeyPress( NULL );
	hasColor = true;
	isActive = false;
	isVisible = true;
	tag = 0;
}

#define IDENTITY		\
	glPushMatrix();		\
	glLoadIdentity();	\
	glGetDoublev(GL_MODELVIEW_MATRIX,(GLdouble*)&oxyz);	\
	glPopMatrix();

#define MATRIX_GLOBAL(cmd)		\
	glPushMatrix();		\
	glLoadIdentity();	\
	cmd					\
	glMultMatrixd((GLdouble*)&oxyz);	\
	glGetDoublev(GL_MODELVIEW_MATRIX,(GLdouble*)&oxyz);	\
	glPopMatrix();

#define MATRIX_LOCAL(cmd)		\
	glPushMatrix();		\
	glLoadMatrixd((GLdouble*)&oxyz);	\
	cmd					\
	glGetDoublev(GL_MODELVIEW_MATRIX,(GLdouble*)&oxyz);	\
	glPopMatrix();

/// Clear position and orientation matrix
void Object3D::identity()
{
	oxyz.identity();
}

/// Clear orientation submatrix
void Object3D::clearOrientation()
{
	//IDENTITY( {});
	oxyz.ox = {1,0,0,0};
	oxyz.oy = {0,1,0,0};
	oxyz.oz = {0,0,1,0};
}

/// Translate along a vector
void Object3D::translateGlobal( vect_t direction )
{
	MATRIX_GLOBAL( {glTranslated( direction.x,direction.y,direction.z );} );
}
void Object3D::translateLocal( vect_t direction )
{
	MATRIX_LOCAL( {glTranslated( direction.x,direction.y,direction.z );} );
}

/// Rotate around an axis
void Object3D::rotateGlobal( double angle, vect_t axis )
{
	MATRIX_GLOBAL( {glRotated( angle,axis.x,axis.y,axis.z );} );
}
void Object3D::rotateLocal( double angle, vect_t axis )
{
	MATRIX_LOCAL( {glRotated( angle,axis.x,axis.y,axis.z );} );
}

/// Scale by a vector-factor
void Object3D::scaleGlobal( vect_t factor )
{
	MATRIX_GLOBAL( {glScaled( factor.x,factor.y,factor.z );} );
}
void Object3D::scaleLocal( vect_t factor )
{
	MATRIX_LOCAL( {glScaled( factor.x,factor.y,factor.z );} );
}

/// Set position
void Object3D::setCenter( vect_t center )
{
	oxyz.o = center;
	oxyz.o.w = 1;
}

/// Get position
vect_t Object3D::getCenter()
{
	return( oxyz.o );
}

/// Set color
void Object3D::setColor( vect_t color )
{
	this->color = color;
}

/// Get color
vect_t Object3D::getColor( )
{
	return color;
}

/// Set active color
void Object3D::setActiveColor( vect_t color )
{
	this->activeColor = color;
}

/// Set shininess
void Object3D::setShininess( double shininess )
{
	this->shininess = shininess;
}

/// Set scale
void Object3D::setScale( vect_t scale )
{
	this->scale = scale;
}

/// Get scale
vect_t Object3D::getScale( )
{
	return scale;
}

/// Set tag
void Object3D::setTag( int tag )
{
	this->tag = tag;
}

/// Get tag
int Object3D::getTag( )
{
	return tag;
}


void Object3D::setPolygonMode( GLuint polygonMode )
{
	this->polygonMode = polygonMode;
}


void Object3D::setLighting ( bool lighting )
{
	this->lighting = lighting;
}


void Object3D::setShadeModel ( int mode )
{
	this->shadeModel = mode;
}


/// Set lack of color
void Object3D::hasNoColor( )
{
	hasColor = false;
}

/// Set activity status
void Object3D::setIsActive( bool active )
{
	isActive = active;
}

/// Set visibility status
void Object3D::setIsVisible( bool visible )
{
	isVisible = visible;
}

/// get visibility status
bool Object3D::getIsVisible( )
{
	return isVisible;
}

void Object3D::interactive()
{
	if( !findObject( imageList ) )
		objects.push_back( this );
	setMouseMove( defaultMouseMove );
	setKeyPress( defaultKeyPress );
}


void Object3D::setMouseMove( mouseMove_t mouseMoveFunction )
{
	this->mouseMoveFunction = mouseMoveFunction;
}


void Object3D::mouseEvent( int dX, int dY )
{
	if( mouseMoveFunction ) mouseMoveFunction( dX,dY );
}


void Object3D::setKeyPress( keyPress_t keyPressFunction )
{
	this->keyPressFunction = keyPressFunction;
}


void Object3D::keyEvent( int key )
{
	if( keyPressFunction ) keyPressFunction( key );
}

/// Helper function for interactive move
void Object3D::iMove( double dX, double dY )
{
	if( window3D )
	{
		double vx = dX*viewDistance/720;
		double vy = dY*viewDistance/720;

		oxyz.o.x += vy*cos( viewAlpha )-vx*sin( viewAlpha );
		oxyz.o.y += vy*sin( viewAlpha )+vx*cos( viewAlpha );
	}
	else
	{
		oxyz.o.x += dX*viewDistance/1000;
		oxyz.o.y += -dY*viewDistance/1000;
	}
}

/// Helper function for interactive scale
void Object3D::iScale( double dS, double min, double max )
{
	double p = expl( dS/100.0 );
	scale.x *= p;
	if( scale.x<min ) scale.x=min;
	if( scale.x>max ) scale.x=max;
	scale.y *= p;
	if( scale.y<min ) scale.y=min;
	if( scale.y>max ) scale.y=max;
	scale.z *= p;
	if( scale.z<min ) scale.z=min;
	if( scale.z>max ) scale.z=max;
}

/// Set size (i.e. uniform scale)
void Object3D::setSize( double size )
{
	setScale( {size,size,size} );
}


/// Get size (i.e. scale along Z)
double Object3D::getSize()
{
	return( scale.z );
}


/// Set texture
void Object3D::setTexture( GLuint texture )
{
	this->texture = texture;
}

/// Set texture size. Size 3 means 3 texture blocks fit into 1 unit
void Object3D::setTextureSize( double size )
{
	this->textureScale = {size,size,size,1};
}

void Object3D::setTextureScale( vect_t scale )
{
	this->textureScale = scale;
}

/// Prepare image properties
void Object3D::prepareImageProps()
{
	/// In color picking mode use color id and no effects
	if( inColorPicking )
	{
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		glDisable( GL_POLYGON_OFFSET_FILL );
		glDisable( GL_LIGHTING );
		glDisable( GL_TEXTURE_2D );
		glDisable( GL_FOG );
		glShadeModel( GL_FLAT );
		glBlendFunc( GL_ONE, GL_ZERO );
//		glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, noEmission );
//		glMaterialf( GL_FRONT_AND_BACK,GL_SHININESS,0.0 );
//		glMaterialfv( GL_FRONT_AND_BACK,GL_SPECULAR, noSpecular);
		glColor4ubv( ( GLubyte* )&imageList );
//cout << "draw object = "<<hex<<imageList<<endl;

		goto drawObject;
	}

	/// Set fill parameters
	glPolygonMode( GL_FRONT_AND_BACK, polygonMode );
	glEnable( GL_POLYGON_OFFSET_FILL );
	glPolygonOffset( 1,1 );

	/// Set object color and light
	if( inShadow )
	{
		glColor4f( 0,0,0,(color.w+1)/2 );
		glDisable( GL_LIGHTING );
	}
	else
	{
		if( hasColor )
		{
			if( isActive )
				glColor4f( activeColor.x,activeColor.y,activeColor.z,1 );
			else
				glColor4f( color.x,color.y,color.z,(color.w==0)?1:color.w );
		}

		if( lighting )
			glEnable( GL_LIGHTING);
		else
			glDisable(GL_LIGHTING);
	}
	glShadeModel( shadeModel );
	glEnable(GL_AUTO_NORMAL);

	/// Set object texture
	if( texture==noTexture )
	{
		glDisable( GL_TEXTURE_2D );
	}
	else
	{
		glEnable( GL_TEXTURE_2D );
		glBindTexture( GL_TEXTURE_2D,texture );

		glMatrixMode( GL_TEXTURE );
		glPushMatrix();
		glScaled( textureScale.x,textureScale.y,textureScale.z );
		glMatrixMode( GL_MODELVIEW );
	}

	/// Set shininess, remove emission
	if( shininess>=1 )
	{
		glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, shininess );
		glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, whiteSpecular );
		glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, noEmission );
	}

	if( polygonMode==GL_LINE )
	{
		glDisable(GL_LIGHTING);
		glDisable( GL_TEXTURE_2D );
		glDepthMask( GL_FALSE );
	}

drawObject:
	/// Set object position, orientation and size
	glPushMatrix();
	glMultMatrixd( ( GLdouble* )&oxyz );
	glScaled( scale.x, scale.y, scale.z );
}

void Object3D::restoreImageProps()
{
	/// In color picking mode use color id and no effects
	if( inColorPicking )
	{
		glEnable( GL_LIGHTING );
		glEnable( GL_TEXTURE_2D );
		glShadeModel( GL_SMOOTH );
		glBlendFunc( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA );
		glPolygonMode( GL_FRONT_AND_BACK,GL_FILL );
		goto exit;
	}

	/// Restore matrices
	if( texture!=noTexture )
	{
		glMatrixMode( GL_TEXTURE );
		glPopMatrix(); // texture
		glMatrixMode( GL_MODELVIEW );
	}

exit:

	/// Remove shininess, add emmision
	if( shininess>=1 )
	{
		glMaterialf( GL_FRONT_AND_BACK,GL_SHININESS,0.0 );
		glMaterialfv( GL_FRONT_AND_BACK,GL_SPECULAR, noSpecular );
		glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, defaultEmission );
	}

	if( polygonMode==GL_LINE )
		glDepthMask( GL_TRUE );

	glPopMatrix(); // modelview
}

void Object3D::drawImage()
{
	if( !isVisible ) return;
	drawList( imageList );
}

void Object3D::drawList( GLuint imageList )
{
	prepareImageProps( );
	glCallList( imageList );
	restoreImageProps( );
}

/// Prepare frame properties
void Object3D::prepareFrameProps()
{
	/// Set black color and disable light
	glColor3f( 0,0,0 );
	glDisable( GL_LIGHTING );

	/// Set antialiased lines
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glDisable( GL_POLYGON_OFFSET_FILL );
	glEnable( GL_LINE_SMOOTH );
	double lw=3.0-viewDistance/10.0;
	glLineWidth( lw>0.5?lw:0.5 );

	/// Set object position, orientation and size
	glPushMatrix();
	glMultMatrixd( ( GLdouble* )&oxyz );
	glScaled( scale.x, scale.y, scale.z );
}

void Object3D::restoreFrameProps()
{
	glPopMatrix(); // modelview
}

/// Draw object frame
void Object3D::drawFrame()
{
	if( !isVisible ) return;
	prepareFrameProps( );
	glCallList( frameList );
	restoreFrameProps( );
}

// Debug function - print local coordinate system to cout
void Object3D::debugDumpOxyz( string header )
{
#ifdef DEBUG
	cout << "Coordinate system: " << header << "\n";
	cout << "  center = ("<<setw( 6 )<<oxyz.o.x<<", "<<setw( 6 )<<oxyz.o.y<<", "<<setw( 6 )<<oxyz.o.z<<")\n";
	cout << "      ox = ("<<setw( 6 )<<oxyz.ox.x<<", "<<setw( 6 )<<oxyz.ox.y<<", "<<setw( 6 )<<oxyz.ox.z<<")\n";
	cout << "      oy = ("<<setw( 6 )<<oxyz.oy.x<<", "<<setw( 6 )<<oxyz.oy.y<<", "<<setw( 6 )<<oxyz.oy.z<<")\n";
	cout << "      oz = ("<<setw( 6 )<<oxyz.oz.x<<", "<<setw( 6 )<<oxyz.oz.y<<", "<<setw( 6 )<<oxyz.oz.z<<")\n\n";
#endif
}


/// ===========================================================================
/// A 3D cube class.
/// ===========================================================================

/// Create an object with default orientation
Cube3D::Cube3D( double size ) : Object3D()
{
	setSize( size );

	/// Create display frame list of the cube
	glNewList( frameList,GL_COMPILE );
	glCallList( idUnitCube3D );
	glEndList();

	/// Create display list of the cube
	glNewList( imageList,GL_COMPILE );
	glCallList( idUnitCube3D );
	glEndList();
}


/// ===========================================================================
/// A 3D paper plane.
/// ===========================================================================

/// Create an object with default orientation
PaperPlane3D::PaperPlane3D( double size ) : Object3D()
{
	setSize( size );

	/// Create display frame list of the paper plane
	glNewList( frameList,GL_COMPILE );
	glCallList( idUnitPaperPlane3D );
	glEndList();

	/// Create display list of the paper plane
	glNewList( imageList,GL_COMPILE );
	glCallList( idUnitPaperPlane3D );
	glEndList();
}


/// ===========================================================================
/// A 3D cylinder.
/// ===========================================================================

/// Create an object with default orientation
Cylinder3D::Cylinder3D( double radius, double height, bool bases, quality_t quality ) : Object3D()
{
	setScale( {radius,radius,height} );

	baseList = ++UID;

	/// Create display frame list of the cylinder
	glNewList( frameList,GL_COMPILE );
	glCallList( idUnitCircle2D[quality] );
	glTranslated( 0,0,1 );
	glCallList( idUnitCircle2D[quality] );
	glEndList();

	/// Create display list of the cylinder side
	glNewList( imageList,GL_COMPILE );
	glCallList( idUnitCylinder3D[quality] );
	glEndList();

	/// Create display list of the cylinder bases
	if( bases )
	{
		glNewList( baseList,GL_COMPILE );
		glScaled( 1,1,-1 );
		glCallList( idUnitCircle2D[quality] );
		glScaled( 1,1,-1 );
		glTranslated( 0,0,1 );
		glCallList( idUnitCircle2D[quality] );
		glEndList();
	}
	else baseList=0;
}

void Cylinder3D::drawImage()
{
	if( !isVisible ) return;
	if( baseList ) drawList( baseList );

	textureScale.swap();
	drawList( imageList );
	textureScale.swap();

}


void Cylinder3D::setRadius( double radius )
{
	scale.x = radius;
	scale.y = radius;
}

double Cylinder3D::getRadius()
{
	return( scale.x );
}

void Cylinder3D::setHeight( double height )
{
	scale.z = height;
}

double Cylinder3D::getHeight()
{
	return( scale.z );
}


/// ===========================================================================
/// A 2D circle.
/// ===========================================================================

/// Create an object with default orientation
Circle2D::Circle2D( double radius, quality_t quality ) : Object3D()
{
	setScale( {radius,radius,radius} );

	/// Create display frame list of the circle
	glNewList( frameList,GL_COMPILE );
	glCallList( idUnitCircle2D[quality] );
	glEndList();

	/// Create display list of the circle
	glNewList( imageList,GL_COMPILE );
	glCallList( idUnitCircle2D[quality] );
	glEndList();
}

void Circle2D::setRadius( double radius )
{
	scale.x = radius;
	scale.y = radius;
}

double Circle2D::getRadius()
{
	return( scale.x );
}


/// ===========================================================================
/// A 3D cone.
/// ===========================================================================

/// Create an object with default orientation
Cone3D::Cone3D( double radius, double height, bool base, quality_t quality ) : Object3D()
{
	setScale( {radius,radius,height} );

	baseList = ++UID;

	/// Create display frame list of the cone
	glNewList( frameList,GL_COMPILE );
	glCallList( idUnitCircle2D[quality] );
	glEndList();

	/// Create display list of the cone side
	glNewList( imageList,GL_COMPILE );
	glCallList( idUnitCone3D[quality] );
	glEndList();

	/// Create display list of the cone base
	if( base )
	{
		glNewList( baseList,GL_COMPILE );
		glScaled( 1,1,-1 );
		glCallList( idUnitCircle2D[quality] );
		glEndList();
	}
	else baseList=0;
}

void Cone3D::drawImage()
{
	if( !isVisible ) return;
	if( baseList ) drawList( baseList );

	textureScale.swap();
	drawList( imageList );
	textureScale.swap();
}


void Cone3D::setRadius( double radius )
{
	scale.x = radius;
	scale.y = radius;
}

double Cone3D::getRadius()
{
	return( scale.x );
}

void Cone3D::setHeight( double height )
{
	scale.z = height;
}

double Cone3D::getHeight()
{
	return( scale.z );
}


/// ===========================================================================
/// A 3D sphere.
/// ===========================================================================

/// Create an object with default orientation
Sphere3D::Sphere3D( double radius, quality_t quality ) : Object3D()
{
	setScale( {radius,radius,radius} );

	/// Create empty display frame list of the sphere
	glNewList( frameList,GL_COMPILE );
	glEndList();

	/// Create display list of the sphere
	glNewList( imageList,GL_COMPILE );
	glCallList( idUnitSphere3D[quality] );
	glEndList();
}

void Sphere3D::setRadius( double radius )
{
	scale.x = radius;
	scale.y = radius;
	scale.z = radius;
}

double Sphere3D::getRadius()
{
	return( scale.x );
}


/// ===========================================================================
/// A 3D pawn.
/// ===========================================================================

/// Create an object with default orientation
Pawn3D::Pawn3D( double size ) : Object3D()
{
	setSize( size );
	setShininess( 50 );
	setOffset( 0 );

	head  = new Sphere3D( 0.5 );
	head->setCenter( {0,0,3} );
	head->hasNoColor();

	body  = new Cone3D( 0.8,3,false );
	body->setCenter( {0,0,0.1} );
	body->hasNoColor();

	neck  = new Sphere3D( 0.6,QUALITY_LOW );
	neck->setCenter( {0,0,2.43} );
	neck->setScale( {0.5,0.5,0.12} );
	neck->hasNoColor();

	waist = new Sphere3D( 0.5 );
	waist->setCenter( {0,0,0.55} );
	waist->setScale( {0.86,0.86,0.3} );
	waist->hasNoColor();;

	bottom = new Sphere3D( 0.5 );
	bottom->setScale( {1.1,1.1,0.7} );
	bottom->hasNoColor();

	feet = new Circle2D( 1.1 );
	feet->hasNoColor();

	/// Create empty display frame list of the pawn
	glNewList( frameList,GL_COMPILE );
	glEndList();

	/// Create display list of the pawn
	glNewList( imageList,GL_COMPILE );
	glEndList();
}

void Pawn3D::drawImage()
{
	if( !isVisible ) return;
	prepareImageProps();

	glTranslatef( 0,-3*offset,0 );
	head->drawImage();

	glTranslatef( 0,0.6*offset,0 );
	neck->drawImage();
	glTranslatef( 0,0.5*offset,0 );
	body->drawImage();
	glTranslatef( 0,1.2*offset,0 );
	waist->drawImage();

	glTranslatef( 0,1.3*offset,0 );
	double cp[4]= {0,0,1,0};
	glEnable( GL_CLIP_PLANE0 );
	glClipPlane( GL_CLIP_PLANE0,cp );
	bottom->drawImage();
	glDisable( GL_CLIP_PLANE0 );

	glTranslatef( 0,1.3*offset,0 );
	glScaled( 1,1,-1 );
	feet->drawImage();
	glScaled( 1,1,-1 );

	restoreImageProps();
}

void Pawn3D::setOffset( double offset )
{
	this->offset = offset;
}

WhitePawn3D::WhitePawn3D( double size ) : Pawn3D( size )
{
	setColor( {0.9,0.9,0.9} );
}

BlackPawn3D::BlackPawn3D( double size ) : Pawn3D( size )
{
	setColor( {0.1,0.1,0.1} );
}




/// ===========================================================================
/// A 2D coordinate system.
/// ===========================================================================

/// Create an object with default orientation
CoordSys2D::CoordSys2D( double minX, double maxX, double minY, double maxY, bool grid, double subunits, double arrowSize, wstring axesNames ) : Object3D()
{
	double extension = arrowSize/2;
	double arrowLength = arrowSize;
	double arrowWidth = arrowSize/3;

	setColor( {0,0,0} );
	setAxesNamesColor( {0.5,0.5,1} );

	/// Create display frame list of the axis
	axesList = ++UID;
	glNewList( axesList,GL_COMPILE );
	{
		glBegin( GL_LINES );
		glVertex3f( minX-extension,0,0 );
		glVertex3f( maxX+extension,0,0 );
		glVertex3f( 0,minY-extension,0 );
		glVertex3f( 0,maxY+extension,0 );
		glEnd();
	}
	glEndList();

	/// Create display frame list of the arrows
	arrowsList = ++UID;
	glNewList( arrowsList,GL_COMPILE );
	{
		glBegin( GL_TRIANGLES ); // arrows
		glVertex3f( maxX+extension+arrowLength,0,0 );
		glVertex3f( maxX+extension,+arrowWidth,0 );
		glVertex3f( maxX+extension,-arrowWidth,0 );
		glVertex3f( 0,maxY+extension+arrowLength,0 );
		glVertex3f( -arrowWidth,maxY+extension,0 );
		glVertex3f( +arrowWidth,maxY+extension,0 );
		glEnd();
	}
	glEndList();

	/// Create display frame list of the grid
	gridList = ++UID;
	glNewList( gridList,GL_COMPILE );
	if( subunits )
	{
		glBegin( GL_LINES ); // axes
		for( int x=( int )minX; x<=( int )maxX; x++ )
		{
			glVertex3f( x,( grid?maxY:+arrowWidth ),0 );
			glVertex3f( x,( grid?minY:-arrowWidth ),0 );
		}
		for( int y=( int )minY; y<=( int )maxY; y++ )
		{
			glVertex3f( ( grid?maxX:+arrowWidth ),y,0 );
			glVertex3f( ( grid?minX:-arrowWidth ),y,0 );
		}
		glEnd();
	}
	glEndList();

	/// Create display frame list of the grid
	subgridList = ++UID;
	glNewList( subgridList,GL_COMPILE );
	if( subunits )
	{
		/// Draw subunit marks
		if( grid )
		{
			glEnable( GL_LINE_STIPPLE );
			glLineStipple( 1,0b1010101010101010 );
		}
		glBegin( GL_LINES ); // axes
		for( int x=( int )( subunits*minX ); x<=( int )( subunits*maxX ); x++ )
		{
			glVertex3f( x/subunits,( grid?maxY:+arrowWidth/2 ),0 );
			glVertex3f( x/subunits,( grid?minY:-arrowWidth/2 ),0 );
		}
		for( int y=( int )( subunits*minY ); y<=( int )( subunits*maxY ); y++ )
		{
			glVertex3f( ( grid?maxX:+arrowWidth/2 ),y/subunits,0 );
			glVertex3f( ( grid?minX:-arrowWidth/2 ),y/subunits,0 );
		}
		glEnd();
		if( grid )
		{
			glDisable( GL_LINE_STIPPLE );
		}
	}
	glEndList();

	/// Create display list of axes' names
	namesList = ++UID;
	glNewList( namesList,GL_COMPILE );
	if( axesNames.length() )
	{
		glPushAttrib( GL_LIST_BASE );
		glListBase( systemFont3D->getFontList() );

		/// Draw the name of axis X
		glPushMatrix();
		glRasterPos3d( 0,0,0 );
		glTranslated( maxX+extension,0,0 );
		glScaled( 1*arrowSize, 1*arrowSize, 1*arrowSize );
		glTranslated( 0,-1,0.1 );
		glCallLists( 1, GL_UNSIGNED_SHORT, axesNames.c_str() );
		glPopMatrix();

		/// Draw the name of axis Y
		glPushMatrix();
		glRasterPos3d( 0,0,0 );
		glTranslated( 0,maxY+extension,0 );
		glScaled( 1*arrowSize, 1*arrowSize, 1*arrowSize );
		glTranslated( -1,0,0.1 );
		glCallLists( 1, GL_UNSIGNED_SHORT, axesNames.c_str()+1 );
		glPopMatrix();

		glPopAttrib();
	}
	glEndList();

}

void CoordSys2D::setAxesNamesColor( vect_t color )
{
	axesNamesColor = color;
}

void CoordSys2D::draw( )
{
	double lw=3.0-viewDistance/10.0;
	if( lw<0.3 ) lw=0.3;

	glEnable( GL_LIGHTING );
	if( hasColor )
	{
		if( isActive )
			glColor4f( activeColor.x,activeColor.y,activeColor.z,1 );
		else
			glColor4f( color.x,color.y,color.z,1 );
	}

	glDisable( GL_LIGHTING );
	glCallList( arrowsList );
	glLineWidth( 3*lw );
	glDepthMask( GL_FALSE );
	glCallList( axesList );
	glLineWidth( lw );
	glCallList( gridList );
	glLineWidth( lw/2 );
	glCallList( subgridList );
	glDepthMask( GL_TRUE );

	glEnable( GL_LIGHTING );
	if( hasColor )
	{
		if( isActive )
			glColor4f( activeColor.x,activeColor.y,activeColor.z,1 );
		else
			glColor4f( axesNamesColor.x,axesNamesColor.y,axesNamesColor.z,1 );
	}
	glCallList( namesList );
}

void CoordSys2D::drawImage()
{
	if( !isVisible ) return;
	prepareImageProps( );
	draw( );
	restoreImageProps( );
}

void CoordSys2D::drawFrame()
{
	if( !isVisible ) return;
	prepareFrameProps();
	draw( );
	restoreFrameProps();
}




/// ===========================================================================
/// A 3D coordinate system.
/// ===========================================================================

/// Create an object with default orientation
CoordSys3D::CoordSys3D( double minX, double maxX, double minY, double maxY, double minZ, double maxZ, bool grid, double subunits, double arrowSize, wstring axesNames ) : Object3D()
{
	double extension = arrowSize/2;
	double arrowLength = arrowSize;
	double arrowWidth = arrowSize/3;

	setColor( {0,0,0} );
	setAxesNamesColor( {0.5,0.5,1} );

	/// Create display frame list of the axis
	axesList = ++UID;
	glNewList( axesList,GL_COMPILE );
	{
		glBegin( GL_LINES );
		glVertex3f( minX-extension,0,0 );
		glVertex3f( maxX+extension,0,0 );
		glVertex3f( 0,minY-extension,0 );
		glVertex3f( 0,maxY+extension,0 );
		glVertex3f( 0,0,minZ-extension );
		glVertex3f( 0,0,maxZ+extension );
		glEnd();
	}
	glEndList();

	/// Create display frame list of the arrows
	arrowsList = ++UID;
	glNewList( arrowsList,GL_COMPILE );
	{
		glPushMatrix();
		glTranslated( 0,0,maxZ+extension );
		glScaled( arrowWidth,arrowWidth,arrowLength );
		glCallList( idUnitArrow3D );
		glPopMatrix();

		glPushMatrix();
		glTranslated( maxX+extension,0,0 );
		glRotated( 90,0,1,0 );
		glScaled( arrowWidth,arrowWidth,arrowLength );
		glCallList( idUnitArrow3D );
		glPopMatrix();

		glPushMatrix();
		glTranslated( 0,maxY+extension,0 );
		glRotated( -90,1,0,0 );
		glScaled( arrowWidth,arrowWidth,arrowLength );
		glCallList( idUnitArrow3D );
		glPopMatrix();

	}
	glEndList();

	/// Create display frame list of the grid
	gridList = ++UID;
	glNewList( gridList,GL_COMPILE );
	if( subunits )
	{
		glBegin( GL_LINES ); // axes
		for( int x=( int )minX; x<=( int )maxX; x++ )
		{
			glVertex3f( x,( grid?maxY:+arrowWidth ),0 );
			glVertex3f( x,( grid?minY:-arrowWidth ),0 );
		}
		for( int y=( int )minY; y<=( int )maxY; y++ )
		{
			glVertex3f( ( grid?maxX:+arrowWidth ),y,0 );
			glVertex3f( ( grid?minX:-arrowWidth ),y,0 );
		}
		for( int z=( int )minZ; z<=( int )maxZ; z++ )
		{
			glVertex3f( ( maxX>0 )?+arrowWidth:0,0,z );
			glVertex3f( ( minX<0 )?-arrowWidth:0,0,z );
			glVertex3f( 0,( maxY>0 )?+arrowWidth:0,z );
			glVertex3f( 0,( minY<0 )?-arrowWidth:0,z );
		}
		glEnd();
	}
	glEndList();

	/// Create display frame list of the grid
	subgridList = ++UID;
	glNewList( subgridList,GL_COMPILE );
	if( subunits )
	{
		/// Draw subunit marks
		if( grid )
		{
			glEnable( GL_LINE_STIPPLE );
			glLineStipple( 1,0b1010101010101010 );
		}
		glBegin( GL_LINES ); // axes
		for( int x=( int )( subunits*minX ); x<=( int )( subunits*maxX ); x++ )
		{
			glVertex3f( x/subunits,( grid?maxY:+arrowWidth/2 ),0 );
			glVertex3f( x/subunits,( grid?minY:-arrowWidth/2 ),0 );
		}
		for( int y=( int )( subunits*minY ); y<=( int )( subunits*maxY ); y++ )
		{
			glVertex3f( ( grid?maxX:+arrowWidth/2 ),y/subunits,0 );
			glVertex3f( ( grid?minX:-arrowWidth/2 ),y/subunits,0 );
		}
		glEnd();
		if( grid )
		{
			glDisable( GL_LINE_STIPPLE );
		}
	}
	glEndList();

	/// Create display list of axes' names
	namesList = ++UID;
	glNewList( namesList,GL_COMPILE );
	if( axesNames.length() )
	{
		glPushAttrib( GL_LIST_BASE );
		glListBase( systemFont3D->getFontList() );

		/// Draw the name of axis X
		glPushMatrix();
		glRasterPos3d( 0,0,0 );
		glTranslated( maxX+extension+arrowLength,0,0 );
		glScaled( 1*arrowSize, 1*arrowSize, 1*arrowSize );
		glTranslated( 0.6,0.1,-0.3 );
		glRotated( 180,0,0,1 );
		glRotated( 90,1,0,0 );
		glCallLists( 1, GL_UNSIGNED_SHORT, axesNames.c_str() );
		glPopMatrix();

		/// Draw the name of axis Y
		glPushMatrix();
		glRasterPos3d( 0,0,0 );
		glTranslated( 0,maxY+extension+arrowLength,0 );
		glScaled( 1*arrowSize, 1*arrowSize, 1*arrowSize );
		glTranslated( 0.1,0.2,-0.3 );
		glRotated( 90,0,0,1 );
		glRotated( 90,1,0,0 );
		glCallLists( 1, GL_UNSIGNED_SHORT, axesNames.c_str()+1 );
		glPopMatrix();

		/// Draw the name of axis Z
		glPushMatrix();
		glRasterPos3d( 0,0,0 );
		glTranslated( 0,0,maxZ+extension+arrowLength );
		glScaled( 1*arrowSize, 1*arrowSize, 1*arrowSize );
		glTranslated( 0.2,-0.2,0.3 );
		glRotated( 135,0,0,1 );
		glRotated( 90,1,0,0 );
		glCallLists( 1, GL_UNSIGNED_SHORT, axesNames.c_str()+2 );
		glPopMatrix();

		glPopAttrib();
	}
	glEndList();

}

void CoordSys3D::setAxesNamesColor( vect_t color )
{
	axesNamesColor = color;
}

void CoordSys3D::draw( )
{
	double lw=3.0-viewDistance/10.0;
	if( lw<0.3 ) lw=0.3;

	glEnable( GL_LIGHTING );
	if( hasColor )
	{
		if( isActive )
			glColor4f( activeColor.x,activeColor.y,activeColor.z,1 );
		else
			glColor4f( color.x,color.y,color.z,1 );
	}

	glDisable( GL_LIGHTING );
	glCallList( arrowsList );
	glLineWidth( 3*lw );
	glDepthMask( GL_FALSE );
	glCallList( axesList );
	glLineWidth( lw );
	glCallList( gridList );
	glLineWidth( lw/2 );
	glCallList( subgridList );
	glDepthMask( GL_TRUE );

	glEnable( GL_LIGHTING );
	if( hasColor )
	{
		if( isActive )
			glColor4f( activeColor.x,activeColor.y,activeColor.z,1 );
		else
			glColor4f( axesNamesColor.x,axesNamesColor.y,axesNamesColor.z,1 );
	}
	glCallList( namesList );
}

void CoordSys3D::drawImage()
{
	if( !isVisible ) return;
	prepareImageProps( );
	draw( );
	restoreImageProps( );
}

void CoordSys3D::drawFrame()
{
	if( !isVisible ) return;
	prepareFrameProps();
	draw( );
	restoreFrameProps();
}


/// ===========================================================================
/// A 2D bitmap font
/// ===========================================================================
// 0x0000 - 0x024F : Basic Latin
// 0x0370 - 0x03FF : Greek (144)
// 0x0400 - 0x04FF : Cyrillic (256)
#define FONT2D_COUNT 0x500

Font2D::Font2D( string faceName, int sizePixels, bool bold, bool italic )
{
	HDC		hdc = wglGetCurrentDC();
	HGLRC	hglrc = wglGetCurrentContext();
	wglMakeCurrent( hdc, hglrc );

	fontList = ++UID;
	UID += FONT2D_COUNT;

	LOGFONT fontStyle;

	strncpy( fontStyle.lfFaceName,faceName.c_str(),LF_FACESIZE );
	fontStyle.lfCharSet=RUSSIAN_CHARSET;
	fontStyle.lfClipPrecision=CLIP_DEFAULT_PRECIS;
	fontStyle.lfEscapement=0;
	fontStyle.lfHeight=sizePixels;
	fontStyle.lfItalic=italic;
	fontStyle.lfOrientation=0;
	fontStyle.lfOutPrecision=OUT_DEFAULT_PRECIS;
	fontStyle.lfPitchAndFamily=DEFAULT_PITCH;
	fontStyle.lfQuality=DEFAULT_QUALITY;
	fontStyle.lfStrikeOut=0;
	fontStyle.lfUnderline=0;
	fontStyle.lfWeight=bold?FW_BOLD:FW_NORMAL;
	fontStyle.lfWidth=0;

	HGDIOBJ  myFont = CreateFontIndirect( &fontStyle );
	HGDIOBJ oldFont = SelectObject( hdc, myFont );

	int err = wglUseFontBitmapsW( hdc, 0, FONT2D_COUNT, fontList );
	if( !err ) err = wglUseFontBitmapsW( hdc, 0, FONT2D_COUNT, fontList );
	err = GetLastError();
	if( err )
	{
		LPVOID lpMsgBuf;
		FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER |
					   FORMAT_MESSAGE_FROM_SYSTEM |
					   FORMAT_MESSAGE_IGNORE_INSERTS,
					   NULL,
					   err,
					   MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ), // Default language
					   ( LPTSTR ) &lpMsgBuf,
					   0,
					   NULL );

		MessageBoxA( NULL, ( LPCTSTR )lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );
		LocalFree( lpMsgBuf );
	}

	SelectObject( hdc, oldFont );
	DeleteObject( myFont );
}

GLuint Font2D::getFontList()
{
	return fontList;
}






/// ===========================================================================
/// A 3D outline font
/// ===========================================================================
// 0x0000 - 0x024F : Basic Latin
// 0x0370 - 0x03FF : Greek (144)
// 0x0400 - 0x04FF : Cyrillic (256)
#define FONT3D_COUNT FONT2D_COUNT

Font3D::Font3D( string faceName, bool bold, bool italic, double depth )
{
	HDC		hdc = wglGetCurrentDC();
	HGLRC	hglrc = wglGetCurrentContext();
	wglMakeCurrent( hdc, hglrc );

	fontList = ++UID;
	UID += FONT3D_COUNT;

	LOGFONT fontStyle;

	strncpy( fontStyle.lfFaceName,faceName.c_str(),LF_FACESIZE );
	fontStyle.lfCharSet=RUSSIAN_CHARSET;
	fontStyle.lfClipPrecision=CLIP_DEFAULT_PRECIS;
	fontStyle.lfEscapement=0;
	fontStyle.lfHeight=0;
	fontStyle.lfItalic=italic;
	fontStyle.lfOrientation=0;
	fontStyle.lfOutPrecision=OUT_DEFAULT_PRECIS;
	fontStyle.lfPitchAndFamily=DEFAULT_PITCH;
	fontStyle.lfQuality=DEFAULT_QUALITY;
	fontStyle.lfStrikeOut=0;
	fontStyle.lfUnderline=0;
	fontStyle.lfWeight=bold?FW_BOLD:FW_NORMAL;
	fontStyle.lfWidth=0;

	HGDIOBJ  myFont = CreateFontIndirect( &fontStyle );
	HGDIOBJ oldFont = SelectObject( hdc, myFont );

	int err = wglUseFontOutlinesW( hdc, 0, FONT3D_COUNT, fontList, 0 ,depth, WGL_FONT_POLYGONS,NULL );
	err = GetLastError();
	if( err )
	{
		LPVOID lpMsgBuf;
		FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER |
					   FORMAT_MESSAGE_FROM_SYSTEM |
					   FORMAT_MESSAGE_IGNORE_INSERTS,
					   NULL,
					   err,
					   MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ), // Default language
					   ( LPTSTR ) &lpMsgBuf,
					   0,
					   NULL );

		MessageBoxA( NULL, ( LPCTSTR )lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );
		LocalFree( lpMsgBuf );
	}

	SelectObject( hdc, oldFont );
	DeleteObject( myFont );
}

GLuint Font3D::getFontList()
{
	return fontList;
}


/// ===========================================================================
/// A 2D bitmap text
/// ===========================================================================
Text2D::Text2D( wstring text, Font2D* font )
{
	this->text = text;
	this->font = font;
}

void Text2D::setText(wstring text)
{
	this->text = text;
}

void Text2D::drawImage()
{
	if( !isVisible ) return;
	if( inShadow ) return;
	if( inReflection ) return;

	glDisable( GL_LIGHTING );
	glDisable( GL_TEXTURE_2D );

	/// In color picking mode use color id and no effects
	if( inColorPicking )
	{
		glDisable( GL_FOG );
		glShadeModel( GL_FLAT );
		glBlendFunc( GL_ONE, GL_ZERO );
		glColor4ubv( ( GLubyte* )&imageList );

		goto drawObject;
	}

	/// Set object color and light
	if( hasColor )
	{
		if( isActive )
			glColor4f( activeColor.x,activeColor.y,activeColor.z,1 );
		else
			glColor4f( color.x,color.y,color.z,1 );
	}

drawObject:
	/// Set object position, orientation and size
	glPushMatrix();
	glMultMatrixd( ( GLdouble* )&oxyz );
	glRasterPos3d( 0,0,0 );

	glPushAttrib( GL_LIST_BASE );
	glListBase( font->getFontList() );
	glCallLists( text.length(), GL_UNSIGNED_SHORT, text.c_str() );
	glPopAttrib();

	glPopMatrix();

	/// In color picking mode use color id and no effects
	if( inColorPicking )
	{
		glEnable( GL_LIGHTING );
		glEnable( GL_TEXTURE_2D );
		glShadeModel( shadeModel );
		glBlendFunc( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA );
		glPolygonMode( GL_FRONT_AND_BACK,GL_FILL );
	}
}

void Text2D::drawFrame()
{
	// empty
}




/// ===========================================================================
/// A 3D outline text
/// ===========================================================================
Text3D::Text3D( wstring text, Font3D* font )
{
	this->text = text;
	this->font = font;
}

void Text3D::setText(wstring text)
{
	this->text = text;
}

void Text3D::drawImage()
{
	if( !isVisible ) return;

	prepareImageProps();

	glRasterPos3d( 0,0,0 );
	glPushAttrib( GL_LIST_BASE );
	glListBase( font->getFontList() );
	glCallLists( text.length(), GL_UNSIGNED_SHORT, text.c_str() );
	glPopAttrib();

	restoreImageProps();
}

void Text3D::drawFrame()
{
	if( !isVisible ) return;

	prepareFrameProps();

	glDisable( GL_LIGHTING );
	glDisable( GL_TEXTURE_2D );

	glRasterPos3d( 0,0,0 );
	glListBase( font->getFontList() );
	glCallLists( text.length(), GL_UNSIGNED_SHORT, text.c_str() );

	restoreFrameProps();
}




/// ===========================================================================
/// A 3D vector.
/// ===========================================================================

/// Create an object with default orientation
Vector3D::Vector3D( vect_t direction, double arrowSize ) : Object3D()
{
	double arrowLength = arrowSize;
	double arrowWidth = arrowSize/3;

	setColor( {0,0,0} );
	setDirection( direction );

	/// Create display frame list of the arrow
	glNewList( imageList,GL_COMPILE );
	{
		glPushMatrix();
		glScaled( arrowWidth,arrowWidth,arrowLength );
		glCallList( idUnitArrow3D );
		glPopMatrix();
	}
	glEndList();
}
Vector3D::Vector3D( Object3D* direction, double arrowSize )
	: Vector3D( direction->getCenter(),arrowSize )
{
}

/// Set direction of a vector
void Vector3D::setDirection( vect_t direction )
{
	oxyz.oz = direction;
	oxyz.rebuild();
};
void Vector3D::setDirection( Object3D* target )
{
	setDirection( ( target->getCenter() ).sub( getCenter() ) );
};

/// Set length of a vector
void Vector3D::setLength( double length )
{
	this->length = length;
};

void Vector3D::draw( )
{
	double lw=3.0-viewDistance/10.0;
	if( lw<0.3 ) lw=0.3;

	glEnable( GL_LIGHTING );
	if( hasColor )
	{
		if( isActive )
			glColor4f( activeColor.x,activeColor.y,activeColor.z,1 );
		else
			glColor4f( color.x,color.y,color.z,1 );
	}

	glDisable( GL_LIGHTING );
	//glCallList( arrowsList );
	glLineWidth( 3*lw );
	glDepthMask( GL_FALSE );

	/// Draw body
	glBegin( GL_LINES );
	glVertex3d( 0,0,0 );
	glVertex3d( 0,0,length );
	glEnd();

	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glTranslated(0,0,length);
	glCallList(imageList);

	glDepthMask( GL_TRUE );
	//cout << oxyz.ox.distance() << " "  << oxyz.oy.distance() << " "  << oxyz.oz.distance() << endl;
	//cout << oxyz.ox.smul(oxyz.oy) << " " << oxyz.oy.smul(oxyz.oz) << " "<< oxyz.oz.smul(oxyz.ox) << " "<< endl;
}

void Vector3D::drawImage()
{
	if( !isVisible ) return;
	prepareImageProps( );
	draw( );
	restoreImageProps( );
}

void Vector3D::drawFrame()
{
	if( !isVisible ) return;
	prepareFrameProps();
	draw( );
	restoreFrameProps();
}




/// ===========================================================================
/// A NURBS-based Bezier surface.
/// ===========================================================================

//void nurbsError(GLenum errorCode)
//{
//   cout << "nurbserror "<<errorCode<<" "<<gluErrorString(errorCode)<<endl;
//}

Bezier3D::Bezier3D( int n ) :Object3D()
{
	this->n = n;

	/// Create NURBS object
	nurbs = gluNewNurbsRenderer();

	/// Set texture points
	for( int i=0; i<n; i++ )
		for( int j=0; j<n; j++ )
		{
			T[i][j][0] = i/(n-1.0);
			T[i][j][1] = -j/(n-1.0);
			T[i][j][2] = 0;
		}

	/// Set knots
	for( int i=0; i<2*n; i++ )
		K[i] = (i<n)?0:1;

	gluNurbsProperty( nurbs, GLU_DISPLAY_MODE, GLU_FILL );
	gluNurbsProperty( nurbs, GLU_SAMPLING_TOLERANCE, 20.0 );
//    gluNurbsCallback( nurbs, GLU_ERROR,(_GLUfuncptr)nurbsError);
}

void Bezier3D::setTolerance( double tolerance )
{
	gluNurbsProperty( nurbs, GLU_SAMPLING_TOLERANCE, tolerance );
}

vect_t Bezier3D::getControlPoint(int i, int j)
{
	return {P[i][j][0],P[i][j][1],P[i][j][2]};
}

void Bezier3D::setControlPoint(int i, int j, vect_t p)
{
	P[i][j][0] = p.x;
	P[i][j][1] = p.y;
	P[i][j][2] = p.z;
}

void Bezier3D::drawImage()
{
	if( !isVisible ) return;
	prepareImageProps();
	gluBeginSurface( nurbs );
	gluNurbsSurface( nurbs,
					 2*n, K, 2*n, K,
					 MAX_NURBS_N * 3, 3, &T[0][0][0],
					 n, n, GL_MAP2_TEXTURE_COORD_2 );
	gluNurbsSurface( nurbs,
					 2*n, K, 2*n, K,
					 MAX_NURBS_N * 3, 3, &P[0][0][0],
					 n, n, GL_MAP2_VERTEX_3 );
	gluEndSurface( nurbs );

	restoreImageProps();
}




/// ===========================================================================
/// A NURBS-like subdivision surface.
/// ===========================================================================

Subdiv3D::Subdiv3D() : Object3D()
{
	level = 3;
	dirty = false;
}

void Subdiv3D::setVertices(	vect_t Va, vector<vect_t> Sa,
							vect_t Vb, vector<vect_t> Sb,
							vect_t Vc, vector<vect_t> Sc,
							vect_t Vd, vector<vect_t> Sd
						  )
{
	this->Va = Va;
	this->Sa = Sa;
	this->Vb = Vb;
	this->Sb = Sb;
	this->Vc = Vc;
	this->Sc = Sc;
	this->Vd = Vd;
	this->Sd = Sd;
	dirty = true;
}

void Subdiv3D::setVertices(	Object3D* Va, vector<Object3D*> Sa,
							Object3D* Vb, vector<Object3D*> Sb,
							Object3D* Vc, vector<Object3D*> Sc,
							Object3D* Vd, vector<Object3D*> Sd
						  )
{
	this->Va = Va->getCenter();
	this->Vb = Vb->getCenter();
	this->Vc = Vc->getCenter();
	this->Vd = Vd->getCenter();

	unsigned i;
	this->Sa.resize( Sa.size() ); for( i=0; i<Sa.size(); i++ ) this->Sa[i]=Sa[i]->getCenter();
	this->Sb.resize( Sb.size() ); for( i=0; i<Sb.size(); i++ ) this->Sb[i]=Sb[i]->getCenter();
	this->Sc.resize( Sc.size() ); for( i=0; i<Sc.size(); i++ ) this->Sc[i]=Sc[i]->getCenter();
	this->Sd.resize( Sd.size() ); for( i=0; i<Sd.size(); i++ ) this->Sd[i]=Sd[i]->getCenter();

	dirty = true;
}

void Subdiv3D::setLevel( int level )
{
	this->level = level;
	dirty = true;
}


vect_t Subdiv3D::vertex( vect_t a, vect_t b, vect_t c, vect_t d, vector<vect_t>Sa, vect_t Sb, vect_t Sd )
{
	int Na=Sa.size();
	int n,k,i;
	double f;
	vect_t result;

	n = ( ( Na-1 )/2 )+3;
	k = 4*n*n;
	result.x = ( k-7*n )*a.x + 6*b.x + c.x + 6*d.x + Sb.x + Sd.x;
	result.y = ( k-7*n )*a.y + 6*b.y + c.y + 6*d.y + Sb.y + Sd.y;
	result.z = ( k-7*n )*a.z + 6*b.z + c.z + 6*d.z + Sb.z + Sd.z;

	f = 6;
	for( i=0; i<Na; i++ )
	{
		result.x = result.x + f*Sa[i].x;
		result.y = result.y + f*Sa[i].y;
		result.z = result.z + f*Sa[i].z;
		f = 7-f;
	}

	result.x = result.x/k;
	result.y = result.y/k;
	result.z = result.z/k;
	return result;
}

vect_t Subdiv3D::face( vect_t b1, vect_t b2, vect_t b3, vect_t b4 )
{
	vect_t result;

	result.x = ( b1.x+b2.x+b3.x+b4.x )/4;
	result.y = ( b1.y+b2.y+b3.y+b4.y )/4;
	result.z = ( b1.z+b2.z+b3.z+b4.z )/4;
	return result;
} //face

vect_t Subdiv3D::edge( vect_t a1, vect_t a2, vect_t b1, vect_t b2, vect_t b3, vect_t b4 )
{
	vect_t result;
	result.x = ( 6*a1.x+6*a2.x+b1.x+b2.x+b3.x+b4.x )/16;
	result.y = ( 6*a1.y+6*a2.y+b1.y+b2.y+b3.y+b4.y )/16;
	result.z = ( 6*a1.z+6*a2.z+b1.z+b2.z+b3.z+b4.z )/16;
	return result;
} //edge


void Subdiv3D::subnubs( vect_t Va, vector<vect_t> Sa,
						vect_t Vb, vector<vect_t> Sb,
						vect_t Vc, vector<vect_t> Sc,
						vect_t Vd, vector<vect_t> Sd,
						vect_t NewVa,
						vect_t NewVb,
						vect_t NewVc,
						vect_t NewVd,
						int levels )
{
	int Na = Sa.size();
	int Nb = Sb.size();
	int Nc = Sc.size();
	int Nd = Sd.size();

	vector<vect_t> k;
	int Nk;
	vect_t k1,k2,k3,k4;

	if( Nd==1 )
	{ k1=Vc; k2=Sc[Nc-1]; }
	else
	{ k1=Sd[Nd-2]; k2=Sd[Nd-3];}

	if( Nb==1 )
	{ k3=Vc; k4=Sc[0]; }
	else
	{ k3=Sb[1]; k4=Sb[2]; }

	Nk=0;
	k.resize( Na );

	if( Na==1 )
	{
		Nk++; k[Nk-1]=edge( Va,Sa[0],Vd,Sd[Nd-1],Vb,Sb[0] );
	}

	if( Na==3 )
	{
		Nk++; k[Nk-1]=edge( Va,Sa[0],Vd,Sd[Nd-1],Sa[1],Sa[2] );
		Nk++; k[Nk-1]=face( Va,Sa[0],Sa[1],Sa[2] );
		Nk++; k[Nk-1]=edge( Va,Sa[2],Sa[0],Sa[1],Vb,Sb[0] );
	}

	if( Na==5 )
	{
		Nk++; k[Nk-1]=edge( Va,Sa[0],Vd,Sd[Nd-1],Sa[1],Sa[2] );
		Nk++; k[Nk-1]=face( Va,Sa[0],Sa[1],Sa[2] );
		Nk++; k[Nk-1]=edge( Va,Sa[2],Sa[0],Sa[1],Sa[3],Sa[4] );
		Nk++; k[Nk-1]=face( Va,Sa[2],Sa[3],Sa[4] );
		Nk++; k[Nk-1]=edge( Va,Sa[4],Sa[2],Sa[3],Vb,Sb[0] );
	}

	draw_nubs( NewVa,
			   k,

			   edge( Va,Vb,Vc,Vd,Sa[Na-1],Sb[0] ),
	{
		face( Va,Vb,Sa[Na-1],Sb[0] ),
		edge( Vb,Sb[0],Va,Sa[Na-1],k3,k4 ),
		NewVb
	},

	face( Va,Vb,Vc,Vd ),
	{
		edge( Vb,Vc,Va,Vd,Sb[Nb-1],Sc[0] ),
		NewVc,
		edge( Vc,Vd,Va,Vb,Sc[Nc-1],Sd[0] )
	},

	edge( Va,Vd,Vb,Vc,Sa[0],Sd[Nd-1] ),
	{
		NewVd,
		edge( Vd,Sd[Nd-1],Va,Sa[0],k1,k2 ),
		face( Vd,Va,Sd[Nd-1],Sa[0] )
	},

	levels
			 );


} //subnubs

vect_t Subdiv3D::norm( vector<vect_t> Vd, vect_t d, vect_t a, vect_t c )
{
//	  ...d(n-1)
//	d1
//	d0   d--a
//       |
//       c
//

	vect_t r= {0,0,0,0};
	vect_t n;
	int Nd=Vd.size();

	n = c.sub( d ).vmul( a.sub( d ) );
	n = n.mul( 1/n.distance() ); r = r.add( n );

	n = Vd[0].sub( d ).vmul( c.sub( d ) );
	n = n.mul( 1/n.distance() ); r = r.add( n );

	for( int i=2; i<Nd; i+=2 )
	{
		n = Vd[i].sub( d ).vmul( Vd[i-2].sub( d ) );
		n = n.mul( 1/n.distance() ); r = r.add( n );
	}

	n = a.sub( d ).vmul( Vd[Nd-1].sub( d ) );
	n = n.mul( 1/n.distance() ); r = r.add( n );

	r = r.mul( -1.0/r.distance() );

	return r;
}

void Subdiv3D::draw_nubs( vect_t Va, vector<vect_t> Sa,
						  vect_t Vb, vector<vect_t> Sb,
						  vect_t Vc, vector<vect_t> Sc,
						  vect_t Vd, vector<vect_t> Sd,
						  int levels )
{

	int Na = Sa.size();
	int Nb = Sb.size();
	int Nc = Sc.size();
	int Nd = Sd.size();

	if( levels<1 )
	{
		vect_t n;
		glBegin( GL_QUADS );
		glEdgeFlag( true );
		n=norm( Sa,Va,Vb,Vd );
		glNormal3f( n.x,n.y,n.z );
		glVertex3f( Va.x,Va.y,Va.z );

		n=norm( Sb,Vb,Vc,Va );
		glNormal3f( n.x,n.y,n.z );
		glVertex3f( Vb.x,Vb.y,Vb.z );

		n=norm( Sc,Vc,Vd,Vb );
		glNormal3f( n.x,n.y,n.z );
		glVertex3f( Vc.x,Vc.y,Vc.z );

		n=norm( Sd,Vd,Va,Vc );
		glNormal3f( n.x,n.y,n.z );
		glVertex3f( Vd.x,Vd.y,Vd.z );
		glEnd();
	}
	else
	{
		vect_t NewVa,NewVb,NewVc,NewVd;

		NewVa = vertex( Va,Vb,Vc,Vd, Sa, Sb[0], Sd[Nd-1] );
		NewVb = vertex( Vb,Vc,Vd,Va, Sb, Sc[0], Sa[Na-1] );
		NewVc = vertex( Vc,Vd,Va,Vb, Sc, Sd[0], Sb[Nb-1] );
		NewVd = vertex( Vd,Va,Vb,Vc, Sd, Sa[0], Sc[Nc-1] );

		subnubs( Va,Sa, Vb,Sb, Vc,Sc, Vd,Sd, NewVa, NewVb, NewVc, NewVd, levels-1 );
		subnubs( Vb,Sb, Vc,Sc, Vd,Sd, Va,Sa, NewVb, NewVc, NewVd, NewVa, levels-1 );
		subnubs( Vc,Sc, Vd,Sd, Va,Sa, Vb,Sb, NewVc, NewVd, NewVa, NewVb, levels-1 );
		subnubs( Vd,Sd, Va,Sa, Vb,Sb, Vc,Sc, NewVd, NewVa, NewVb, NewVc, levels-1 );
	}
}

void Subdiv3D::drawImage()
{
	if( !isVisible ) return;

	if( dirty )
	{
		dirty = false;
		glDeleteLists( imageList,1 );
		glNewList( imageList,GL_COMPILE );
		draw_nubs( Va,Sa,Vb,Sb,Vc,Sc,Vd,Sd,level );
		glEndList();

		frameList = imageList;
	}
	prepareImageProps();
	glCallList( imageList );
	restoreImageProps();
}







Object3D* findObject( GLuint imageList )
{
	imageList &= 0xffffff;
	for( list<Object3D*>::iterator object=objects.begin(); object!=objects.end(); ++object )
	{
		if( imageList == ( *object )->imageList ) return *object;
	}
	return NULL;
}

/// Select the next object to be active object
void selectNextObject()
{
	/// Exit if there are no interactive objects
	if( objects.size()==0 ) return;
	if( activeObject )
	{
		activeObject->setIsActive( false );
		for( list<Object3D*>::iterator object=objects.begin(); object!=objects.end(); ++object )
			if( activeObject == ( *object ) )
			{
				object++;
				if( object!=objects.end() )
				{
					activeObject = *object;
					activeObject->setIsActive( true );
					return;
				}
				break;
			}
	}

	/// select first
	activeObject = *objects.begin();
	activeObject->setIsActive( true );
}

/// Create interactive 2D point
Object3D* interactivePoint2D( double x, double y )
{
	Object3D* point = new Circle2D( 0.3, QUALITY_LOW );
	point->setCenter( {x,y,1} );
	point->setColor( {0,0,0} );
	point->interactive();
	return point;
}

/// Create 2D point
Object3D* point2D( double x, double y )
{
	Object3D* point = new Circle2D( 0.3, QUALITY_LOW );
	point->setCenter( {x,y,1} );
	point->setColor( {0,0,0} );
	return point;
}

/// Create 3D point
Object3D* point3D( double x, double y, double z )
{
	Object3D* point = new Sphere3D( 0.3, QUALITY_LOW );
	point->setCenter( {x,y,z} );
	point->setColor( {0.1,0.1,0.1} );
	point->setShininess( 20 );
	return point;
}

/// Draw dotted line strip connecting objects' centers
void drawDottedLine( int count, Object3D* object[] )
{
	glDepthMask( GL_FALSE );
	glColor3f( 1,0,0 );
	glLineWidth( 1 );
	glEnable( GL_LINE_STIPPLE );
	glLineStipple( 2,0b1010101010101010 );
	glBegin( GL_LINE_STRIP );
	for( int i=0; i<count; i++ ) object[i]->getCenter().vertex();
	glEnd();
	glDisable( GL_LINE_STIPPLE );
	glDepthMask( GL_TRUE );
}

/// Draw line strip connecting points
void drawLine( int count, vect_t object[] )
{
	glDepthMask( GL_FALSE );
	glBegin( GL_LINE_STRIP );
	for( int i=0; i<count; i++ ) object[i].vertex();
	glEnd();
	glDepthMask( GL_TRUE );
}
