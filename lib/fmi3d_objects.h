#ifndef FMI3D_OBJECTS_H
#define FMI3D_OBJECTS_H

/// Project:	FMI 3D (2013)
/// File:		fmi3d_objects.h
///	About:		Header file for the module for 3D objects
///
/// History:
///		2013.04.27/PB	Declaration of Object3D and Cube3D
///		2013.05.01/PB	Vector and matrix manipulation
///		2013.05.05/PB	Paper plane, move size parameter to Object3D, cylinder
///		2013.05.06/PB	Circle, cone, sphere
///		2013.05.09/PB	Pawn
///		2013.05.10/PB	Color picking, drag and drop
///		2013.05.12/PB	Active color and full object manipulation
///		2013.05.16/PB	interactivePoint2D()
///		2013.05.17/PB	point2D(),point3D()
///		2013.05.18/PB	Visibility of objects, CoordSys2D
///		2013.05.22/PB	Font2D, Font3D, Text2D, Text3D, inReflection, CoordSys3D
///		2013.05.23/PB	Vector3D
///		2013.05.29/PB	Bezier3D
///		2013.05.31/PB	Object3D::setLighting()
///		2013.06.01/PB	Subdiv3D, Object3D::getTag(),setTag()
///		2013.07.23/PB	Object3D::ShadeModel()


#include <string>
#include <vector>
#include "GL/glfw.h"

#include "fmi3d_primitives.h"

using namespace std;

class Object3D;
typedef void(*mouseMove_t)(int,int);
typedef void(*keyPress_t)(int);
extern void selectNextObject();

/// Environment
extern GLfloat defaultEmission[4];
extern GLfloat noEmission[4];
extern GLfloat whiteSpecular[4];
extern GLfloat noSpecular[4];

struct oxyz_t;

/// General 4D vector-point (in homogeneous coordinates).
/// Used to manage positions, axes and coordinate systems.
struct vect_t
{
	public:
		GLdouble x;
		GLdouble y;
		GLdouble z;
		GLdouble w;
		vect_t v3d( double x, double y, double z );
		vect_t p3d( double x, double y, double z );
		vect_t s3d( double alpha, double beta, double r );
		double distance( vect_t p );
		double distance( );
		vect_t mul( double k );
		double smul( vect_t p );
		vect_t vmul( vect_t p );
		vect_t mmul( oxyz_t m );
		vect_t add( vect_t p );
		vect_t sub( vect_t p );
		void   swap( );
		void vertex();
};


/// A fully-defined orthogonal cartesian coordinate system.
/// Used to represent orientation and rotation in 3D space.
struct oxyz_t
{
	public:
		vect_t ox;    /// unit vector along X axis
		vect_t oy;    /// unit vector along Y axis
		vect_t oz;    /// unit vector along Z axis
		vect_t o;     /// coordinates of the origin
		void identity();			/// loads identity matrix
		void translate( vect_t v );	/// loads translation matrix
		void scale( vect_t v );		/// loads scale matrix
		void rotateY( double alpha );	/// loads rotation around Y matrix
		void rotateZ( double alpha );	/// loads rotation around Z matrix
		oxyz_t multiply( oxyz_t b );	/// multiplies with a matrix
		void rebuild();	/// rebuild ox and oy from oz azis
};


/// Matrix wrapper of a coordinate system and methods for its manipulation.
/// Used as a parent for graphical objects.
class Object3D
{
	public:
		Object3D();

		void identity();
		void clearOrientation();

		void rotateGlobal( double angle, vect_t axis );
		void translateGlobal( vect_t direction );
		void scaleGlobal( vect_t factor );

		void rotateLocal( double angle, vect_t axis );
		void translateLocal( vect_t direction );
		void scaleLocal( vect_t factor );

		virtual void drawImage();
		virtual void drawFrame();

		vect_t getCenter();
		void setCenter( vect_t center );
		void setColor( vect_t color );
		vect_t getColor();
		void setActiveColor( vect_t color );
		void setShininess( double shininess );
		vect_t getScale();
		void setScale( vect_t scale );
		void setTexture( GLuint texture );
		void setTextureSize( double size );
		void setTextureScale( vect_t scale );
		void setPolygonMode( GLuint polygonMode );
		void setLighting( bool lighting );
		void setShadeModel( int mode );

		void setSize( double size );
		double getSize();

		void hasNoColor( );
		void setIsActive(bool active);
		void setIsVisible( bool visible );
		bool getIsVisible();
		void interactive( );
		void setMouseMove(mouseMove_t mouseMoveFunction);
		void mouseEvent(int dX, int dY);
		void setKeyPress(keyPress_t keyPressFunction);
		void keyEvent(int key);
		void iMove(double dX, double dY);
		void iScale(double dS, double min, double max);

		int getTag();
		void setTag(int tag);
		void debugDumpOxyz( string header );
		GLuint imageList;
		oxyz_t oxyz;
	protected:
		int tag;
		vect_t color;
		vect_t activeColor;
		vect_t scale;
		GLuint frameList;
		GLuint texture;
		vect_t textureScale;
		GLuint polygonMode;
		bool   lighting;
		bool   hasColor;
		bool   isActive;
		bool   isVisible;
		double shininess;
		int    shadeModel;
		void drawList( GLuint imageList );
		void prepareImageProps();
		void restoreImageProps();
		void prepareFrameProps();
		void restoreFrameProps();
		mouseMove_t mouseMoveFunction;
		keyPress_t keyPressFunction;
};

extern Object3D* findObject(GLuint imageList);

/// Cube
class Cube3D : public Object3D
{
	public:
		Cube3D( double size );
};


/// Paper plane
class PaperPlane3D : public Object3D
{
	public:
		PaperPlane3D( double size );
};


/// Cylinder
class Cylinder3D : public Object3D
{
	private:
		GLuint baseList;
	public:
		Cylinder3D( double radius, double height, bool bases=true, quality_t quality=QUALITY_MEDIUM );
		void drawImage();
		void setHeight( double height );
		double getHeight();
		void setRadius( double radius );
		double getRadius();
};


/// Circle
class Circle2D : public Object3D
{
	public:
		Circle2D( double radius, quality_t quality=QUALITY_MEDIUM );
		void setRadius( double radius );
		double getRadius();
};


/// Cone
class Cone3D : public Object3D
{
	private:
		GLuint baseList;
	public:
		Cone3D( double radius, double height, bool base=true, quality_t quality=QUALITY_MEDIUM );
		void drawImage();
		void setHeight( double height );
		double getHeight();
		void setRadius( double radius );
		double getRadius();
};


/// Sphere
class Sphere3D : public Object3D
{
	public:
		Sphere3D( double radius, quality_t quality=QUALITY_MEDIUM );
		void setRadius( double radius );
		double getRadius();
};


/// Pawn
class Pawn3D : public Object3D
{
	public:
		Pawn3D( double size );
		void drawImage();
		void setOffset(double offset);
	private:
		double		offset;
		Sphere3D*	head;
		Cone3D*		body;
		Sphere3D*	neck;
		Sphere3D*	waist;
		Sphere3D*	bottom;
		Circle2D*	feet;
};
class WhitePawn3D : public Pawn3D
{
	public:
		WhitePawn3D( double size );
};
class BlackPawn3D : public Pawn3D
{
	public:
		BlackPawn3D( double size );
};


/// 2D Coordinate system
class CoordSys2D : public Object3D
{
	public:
		CoordSys2D( double minX, double maxX, double minY, double maxY, bool grid, double subunit=10, double arrowSize=0.2, wstring axesNames=L"" );
		void setAxesNamesColor(vect_t color);
		void drawImage();
		void drawFrame();
	private:
		vect_t axesNamesColor;
		GLuint axesList;
		GLuint gridList;
		GLuint subgridList;
		GLuint arrowsList;
		GLuint namesList;
		void draw();
};


/// 3D Coordinate system
class CoordSys3D : public Object3D
{
	public:
		CoordSys3D( double minX, double maxX, double minY, double maxY, double minZ, double maxZ, bool grid, double subunit=10, double arrowSize=0.2, wstring axesNames=L"" );
		void setAxesNamesColor(vect_t color);
		void drawImage();
		void drawFrame();
	private:
		vect_t axesNamesColor;
		GLuint axesList;
		GLuint gridList;
		GLuint subgridList;
		GLuint arrowsList;
		GLuint namesList;
		void draw();
};


/// Font2D (i.e. Bitmap font)
class Font2D
{
	public:
		Font2D( string faceName, int sizePixels=32, bool bold=false, bool italic=false );
		GLuint getFontList();
	private:
		GLuint fontList;
};


/// Font3D (i.e. Outline font)
class Font3D
{
	public:
		Font3D( string faceName, bool bold=false, bool italic=false, double depth=0.3 );
		GLuint getFontList();
	private:
		GLuint fontList;
};


/// Text2D
class Text2D : public Object3D
{
	public:
		Text2D( wstring text, Font2D* font );
		void setText(wstring text);
		void drawImage();
		void drawFrame();
	private:
		wstring text;
		Font2D* font;
};


/// Text3D
class Text3D : public Object3D
{
	public:
		Text3D( wstring text, Font3D* font );
		void setText(wstring text);
		void drawImage();
		void drawFrame();
	private:
		wstring text;
		Font3D* font;
};




/// 3D vector
class Vector3D : public Object3D
{
	public:
		Vector3D( vect_t direction, double arrowSize=0.2 );
		Vector3D( Object3D* direction, double arrowSize=0.2 );
		void setDirection(vect_t direction);
		void setDirection(Object3D* target);
		void setLength(double length);
		void drawImage();
		void drawFrame();
	private:
		double length;
		vect_t fromPos;
		vect_t toPos;
		void draw();
};


/// 3D NURBS-based Bezier surface
#define MAX_NURBS_N 100
class Bezier3D : public Object3D
{
	public:
		Bezier3D( int n );
		vect_t getControlPoint( int i, int j );
		void setControlPoint( int i, int j, vect_t p );
		void setTolerance( double tolerance );
		void drawImage();
	private:
		int n;
		GLUnurbsObj* nurbs;
		GLfloat P[MAX_NURBS_N][MAX_NURBS_N][3];	/// Control points
		GLfloat T[MAX_NURBS_N][MAX_NURBS_N][3];	/// Texture points
		GLfloat K[2*MAX_NURBS_N];				/// Knots
};


/// 3D Subdiv-based NURBS surface
class Subdiv3D : public Object3D
{
	public:
		Subdiv3D( );
		void setLevel( int level );
		void setVertices( vect_t Va, vector<vect_t> Sa,
						  vect_t Vb, vector<vect_t> Sb,
						  vect_t Vc, vector<vect_t> Sc,
						  vect_t Vd, vector<vect_t> Sd );
		void setVertices( Object3D* Va, vector<Object3D*> Sa,
						  Object3D* Vb, vector<Object3D*> Sb,
						  Object3D* Vc, vector<Object3D*> Sc,
						  Object3D* Vd, vector<Object3D*> Sd );
		void drawImage();
	private:
		vect_t Va; vector<vect_t> Sa;
		vect_t Vb; vector<vect_t> Sb;
		vect_t Vc; vector<vect_t> Sc;
		vect_t Vd; vector<vect_t> Sd;

		vect_t vertex( vect_t a, vect_t b, vect_t c, vect_t d, vector<vect_t>Sa, vect_t Sb, vect_t Sd );
		vect_t face( vect_t b1, vect_t b2, vect_t b3, vect_t b4 );
		vect_t edge( vect_t a1, vect_t a2, vect_t b1, vect_t b2, vect_t b3, vect_t b4 );
		vect_t norm( vector<vect_t> Vd, vect_t d, vect_t a, vect_t c );
		void subnubs( vect_t Va, vector<vect_t> Sa,
					  vect_t Vb, vector<vect_t> Sb,
					  vect_t Vc, vector<vect_t> Sc,
					  vect_t Vd, vector<vect_t> Sd,
					  vect_t NewVa,
					  vect_t NewVb,
					  vect_t NewVc,
					  vect_t NewVd,
					  int levels );
		void draw_nubs( vect_t Va, vector<vect_t> Sa,
						vect_t Vb, vector<vect_t> Sb,
						vect_t Vc, vector<vect_t> Sc,
						vect_t Vd, vector<vect_t> Sd,
						int levels );
		int level;
		bool dirty;
};


extern Font3D* systemFont3D;
extern Font2D* systemFont2D;
extern Object3D* interactivePoint2D( double x, double y );
extern Object3D* point2D( double x, double y );
extern Object3D* point3D( double x, double y, double z );
extern void drawDottedLine(int count, Object3D* object[]);
extern void drawLine( int count, vect_t object[] );

#endif
