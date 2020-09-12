#ifndef FMI3D_LIB_H
#define FMI3D_LIB_H

/// Project:	FMI 3D (2013)
/// File:		fmi3d_lib.h
///	About:		Header file for the library
///
/// History:
///		2013.04.26/PB	Initial version
///		2013.04.27/PB	Added declaration of Object3D
///		2013.05.03/PB	Support for 2D mode
///		2013.05.07/PB	Loading textures from PNG files (inc. transparencies)
///		2013.05.08/PB	Interactive buttons
///		2013.05.10/PB	getPixelColor(), getPixelColori(), objectAtPoint()
///		2013.06.09/PB	Support for quad-buffer stereo


#include <cstdlib>
#include <math.h>
#include <string>
#include <GL/glfw.h>

#include "fmi3d_objects.h"

using namespace std;


/// Sometimes M_PI is missing
#define M_PI 3.14159265358979323846


/// Useful functions
extern double random(double a, double b);
extern int irandom(int a, int b);
extern GLuint noTexture;
extern GLuint loadTexture(string textureName);
extern vect_t getPixelColor( int x, int y, int glBuffer);
extern GLuint getPixelColori( int x, int y, int glBuffer );
extern Object3D* objectAtPoint( int x, int y );
extern vect_t closestPointToSegment(Object3D* A, Object3D* P1, Object3D* P2);

/// Window management
extern int windowHeight;
extern int windowWidth;
extern bool openWindow3D(string caption);
extern bool openWindow2D(string caption);
extern bool isRunning();
extern bool inShadow;
extern bool inReflection;
extern bool window3D;
extern void setBrightness();
extern void changeBrightness();
extern bool inColorPicking;
extern bool inAnagliphicStereo;
extern bool inMono;
extern bool inQuadBufferStereo;

/// Drawing functions (must be used-defines in the main file)
extern void drawScene();
extern void drawFrames();
extern bool useCustomFogColor;
extern GLfloat customFogColor[4];

/// Time, fps and frames
extern double t;
extern double fps;
extern int frame;

/// Interactivity
extern bool inDrag;	/// Currently in drag mode
extern Object3D* activeObject;	/// Currently selected object for dragging
extern void defaultKeyPress( int key );
extern void defaultMouseMove( int dX, int dY );

/// Environment
extern GLfloat defaultEmission[4];
extern GLfloat noEmission[4];
extern GLfloat whiteSpecular[4];
extern GLfloat noSpecular[4];

#endif
