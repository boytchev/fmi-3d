#ifndef FMI3D_GROUND_H
#define FMI3D_GROUND_H

/// Project:	FMI 3D (2013)
/// File:		fmi3d_ground.h
///	About:		Header file for the 3D textured ground
///
/// History:	2013.04.27/PB	Declaration of Ground3D
///				2013.05.03/PB	Ground texture can be changed


#include "GL/glfw.h"



/// ===========================================================================
/// Ground of the virtual 3D world. Composed of GROUND_COUNT x GROUND_COUNT
/// squares plates each of size GROUND_SIZE x GROUND_SIZE, decorated by
/// a texture. The ground is at level Z.
/// ===========================================================================

#define GROUND_SIZE  50.0  /// Size of a ground plate
#define GROUND_COUNT 20    /// Number of ground plates along X and Y


/// Ground texture (marble and tiles are reflective)
enum ground_t
{
	GROUND_INVISIBLE,
	GROUND_MARBLE,
	GROUND_TILES,
	GROUND_ROCK,
	GROUND_METAL
};


class Ground3D
{
	public:
		Ground3D(ground_t groundStyle);
		void drawImage();
		bool isReflective();
		bool isVisible();
		void changeGround();
		void noGround();
	private:
		ground_t groundStyle;
		bool reflective;
		GLuint imageList;
		GLuint patchList;
		GLuint texGround;
		void buildGroundPatch();
};


extern Ground3D* ground;	/// Global ground instance
extern void changeGround();
extern void noGround();

#endif
