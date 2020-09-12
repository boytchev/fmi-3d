#ifndef FMI3D_CAMERA_H
#define FMI3D_CAMERA_H

/// Project:	FMI 3D (2013)
/// File:		fmi3d_camera.h
///	About:		Header file for the module for 3D camera navigation
///
/// History:
///		2013.04.28/PB	File created
///		2013.05.03/PB	Support for 2D mode
///		2013.05.19/PB	Min/max of viewDistance
///		2013.05.20/PB	Support for anaglyphic stereo


#define LEFT_EYE -1
#define MIDDLE_EYE 0
#define RIGHT_EYE 1

extern void setViewMatrix(int eye);
extern void setProjectionMatrix(int eye);


/// Exports of camera 3D navigational functions
extern void cameraPanXY3D( double dX, double dY );
extern void cameraPanXZ3D( double dX, double dY );
extern void cameraRot3D( double dX, double dY );
extern void cameraZoom3D( double dX, double dY );
extern void cameraNavigation3D();

/// Exports of camera 2D navigational functions
extern void cameraPanXY2D( double dX, double dY );
extern void cameraZoom2D( double dX, double dY );
extern void cameraNavigation2D();

extern double viewDistance;
extern double minViewDistance;
extern double maxViewDistance;
extern double viewAlpha;
extern double viewBeta;
extern double viewX;
extern double viewY;
extern double viewZ;
extern double minViewBeta;
extern double maxViewBeta;

#endif
