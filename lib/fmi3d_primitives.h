#ifndef FMI3D_PRIMITIVES_H
#define FMI3D_PRIMITIVES_H

/// Project:	FMI 3D (2013)
/// File:		fmi3d_primitives.h
///	About:		Header file for the module for graphical primitives
///
/// History:
///		2013.04.27/PB	Initial version
///		2013.05.05/PB	Paper plane, cylinder
///		2013.05.06/PB	Cone, circle, sphere
///		2013.05.23/PB	Arrow


#include <GL/glfw.h>

/// Quality of round objects - number of approximating fragments
enum quality_t {QUALITY_HIGHER, QUALITY_HIGH, QUALITY_MEDIUM, QUALITY_LOW, QUALITY_LOWER, MAX_QUALITY};
extern int quality[MAX_QUALITY];


extern int UID;						/// Global unique identifier
extern void createPrimitives();		/// Create display lists for graphical primitives
extern GLuint idUnitCube3D;			/// Precompiled display list for unit cube
extern GLuint idUnitPaperPlane3D;	/// Precompiled display list for unit paper plane
extern GLuint idUnitCylinder3D[MAX_QUALITY];/// Precompiled display list for unit cylinder
extern GLuint idUnitCone3D[MAX_QUALITY];	/// Precompiled display list for unit cone
extern GLuint idUnitSphere3D[MAX_QUALITY];	/// Precompiled display list for unit sphere
extern GLuint idUnitCircle2D[MAX_QUALITY];	/// Precompiled display list for unit circle
extern GLuint idUnitArrow3D;		/// Precompiled display list for unit arrow


#endif
