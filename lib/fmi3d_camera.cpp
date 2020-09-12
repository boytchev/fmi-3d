
/// Project:	FMI 3D (2013)
/// File:		fmi3d_camera.cpp
///	About:		Module for 3D camera navigation
///
/// History:	2013.04.28/PB	Initial implementation of rotation and panning
///				2013.04.30/PB	Fixed decay factors for better inertia
///				2013.05.03/PB	Support for 2D mode
///				2013.05.11/PB	Improved inertia
///				2013.05.19/PB	Min/max of viewDistance
///		2013.05.20/PB	Support for anaglyphic stereo

#include <math.h>
#include "GL/glfw.h"
#include "fmi3d_lib.h"
#include "fmi3d_camera.h"
#include "fmi3d_ground.h"
//#include <iostream>

/// Camera position in spherical coordinates in respect to the focal point
double viewAlpha	= 0.4;	// radians
double viewBeta		= 0.4;	// radians
double viewDistance = 20;
double minViewDistance  = 5;
double maxViewDistance  = 100;
double minViewBeta		= 0.06;
double maxViewBeta		= M_PI/2-0.06;

double dViewAlpha 	= 0;	// inertia
double dViewBeta 	= 0;
double dViewDistance= 1;


/// Cartesian coordinates of the focal point
double viewX 	= 0;
double viewY 	= 0;
double viewZ 	= 2;

double dViewX 	= 0;	// inertia
double dViewY 	= 0;
double dViewZ 	= 0;

#define FIELD_OF_VIEW	45.0
#define EYE_SEPARATION	0.2
#define STEREO_DEPTH 	2.0
#define NEAR_CLIPPING	2.0
#define FAR_CLIPPING	1000.0

void applyFrustum( double eye )
{
	float top, bottom, left, right;

	double FOV = FIELD_OF_VIEW * M_PI / 180;
	top     = NEAR_CLIPPING * tan( FOV/2 );
	bottom  = -top;

	float a = tan( FOV/2 ) * viewDistance * double( windowWidth )/double( windowHeight )/STEREO_DEPTH;

	left    = -( a - eye ) * NEAR_CLIPPING/viewDistance*STEREO_DEPTH;
	right   = ( a + eye ) * NEAR_CLIPPING/viewDistance*STEREO_DEPTH;

	// Set the Projection Matrix
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glFrustum( left, right, bottom, top, NEAR_CLIPPING, FAR_CLIPPING );

	// Displace the world to left or right
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glTranslatef( eye, 0.0f, 0.0f );
}

/// Set view-model matrix of the view point. In anaglyphic mode
/// assume, that the matrix is selected and set to identity
void setViewMatrix(int eye)
{
	if( !inAnagliphicStereo )
	{
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
	}

	if( window3D )
	{
		double e=eye;//10*sin(t);
		gluLookAt(
			viewX+viewDistance*cos(viewAlpha)*cos(viewBeta)-e*EYE_SEPARATION/2*sin(viewAlpha),
			viewY+viewDistance*sin(viewAlpha)*cos(viewBeta)+e*EYE_SEPARATION/2*cos(viewAlpha),
			viewZ+viewDistance*sin(viewBeta),
			viewX-e*EYE_SEPARATION/2*sin(viewAlpha),viewY+e*EYE_SEPARATION/2*cos(viewAlpha),viewZ,
			0,0,1 );
	}
	else
	{
		glScalef( 1000/viewDistance,1000/viewDistance,1000/viewDistance );
		gluLookAt(
			viewX,viewY,viewZ+viewDistance,
			viewX,viewY,viewZ,
			0,1,0 );
	}
}

/// Set projection matrix. In stereo mode projections are different for left
/// and right eyes.
void setProjectionMatrix( int eye )
{
	switch( eye )
	{
		case LEFT_EYE:
			applyFrustum( EYE_SEPARATION/2.0 );
			break;
		case RIGHT_EYE:
			applyFrustum( -EYE_SEPARATION/2.0 );
			break;
		case MIDDLE_EYE:
			if( window3D )
				applyFrustum( 0 );
			else
			{
				glMatrixMode( GL_PROJECTION );
				glLoadIdentity();
				glOrtho( -windowWidth/2.0, windowWidth/2.0, -windowHeight/2.0, +windowHeight/2.0, -10*FAR_CLIPPING, 10*FAR_CLIPPING );
				glMatrixMode( GL_MODELVIEW );
			}
			break;
	}
}

/// Camera panning for mouse position change of dX and dY pixels
void cameraPanXY3D( double dX, double dY )
{
	double dx=viewDistance/500*dX;
	double dy=viewDistance/500*dY;

	/// Adjust panning in view coordinates
	dViewX = -dy*cos( viewAlpha )+dx*sin( viewAlpha );
	dViewY = -dy*sin( viewAlpha )-dx*cos( viewAlpha );
}
void cameraPanXY2D( double dX, double dY )
{
	/// Adjust panning in view coordinates
	dViewX = -viewDistance/1000*dX;
	dViewY = viewDistance/1000*dY;
}

void cameraPanXZ3D( double dX, double dY )
{
	double dx=viewDistance/500*dX;
	double dy=0;

	/// Adjust panning in view coordinates
	dViewX = -dy*cos( viewAlpha )+dx*sin( viewAlpha );
	dViewY = -dy*sin( viewAlpha )-dx*cos( viewAlpha );
	dViewZ=viewDistance/500*dY;
}


/// Camera rotating in 3D for mouse position change of dX and dY pixels
void cameraRot3D( double dX, double dY )
{
	/// Adjust rotating to be either horizontal, or vertical (but not both)
	if( fabs( dX )>fabs( dY ) ) dY=0;
	if( fabs( dX )<fabs( dY ) ) dX=0;

	dViewAlpha = -dX/500;
	dViewBeta = +dY/500;
}


/// Camera distance/zoom for mouse position change of dY pixels (dX is unused)
void cameraZoom3D( double dX, double dY )
{
	dViewDistance = expl( dY/750 );
}
void cameraZoom2D( double dX, double dY )
{
	dViewDistance = expl( dY/750 );
}

/// Adjust camera position matrix and manage inertia camera motion inertia
void cameraNavigation3D()
{
	/// Adjust sensitivity.
	double senseDecay = 1-12/fps;
	if( senseDecay<0.1 ) senseDecay=0.1;
	if( senseDecay>0.99 ) senseDecay=0.99;

	/// Adjust focal point position
	viewX += dViewX;
	viewY += dViewY;
	viewZ += dViewZ;
	if( ground->isVisible())
		{if( viewZ<0 ) viewZ=0;}
	else
		{if( viewZ<-100 ) viewZ=-100;}
	if( viewZ>100 ) viewZ=100;

	/// Adjust camera position
	viewAlpha += dViewAlpha;
	viewBeta  += dViewBeta;
	if( viewBeta<minViewBeta ) viewBeta=minViewBeta;
	if( viewBeta>maxViewBeta ) viewBeta=maxViewBeta;

	viewDistance *= dViewDistance;
	if( viewDistance<minViewDistance ) viewDistance=minViewDistance;
	if( viewDistance>maxViewDistance ) viewDistance=maxViewDistance;

	/// Inertia - decays all navigational speeds
	dViewX*=senseDecay;
	dViewY*=senseDecay;
	dViewZ*=senseDecay;
	dViewAlpha*=senseDecay;
	dViewBeta*=senseDecay;
	dViewDistance=dViewDistance*senseDecay+1*( 1-senseDecay );

	/// Adjust fog (otherwise background dissapears quickly)
	glFogf( GL_FOG_START,viewDistance+20 );
	glFogf( GL_FOG_END,viewDistance*3+20 );
}

/// Adjust camera position matrix and manage inertia camera motion inertia
void cameraNavigation2D()
{
	/// Adjust sensitivity.
	double senseDecay = 1-12/fps;
	if( senseDecay<0.1 ) senseDecay=0.1;
	if( senseDecay>0.99 ) senseDecay=0.99;

	viewAlpha = -90;
	viewBeta = 90;
	viewZ = 0;

	/// Adjust focal point position
	viewX += dViewX;
	viewY += dViewY;
	viewZ += dViewZ;
	if( viewZ<0 ) viewZ=0;
	if( viewZ>100 ) viewZ=100;

	viewDistance *= dViewDistance;
	if( viewDistance<minViewDistance ) viewDistance=minViewDistance;
	if( viewDistance>maxViewDistance ) viewDistance=maxViewDistance;

	/// Inertia - decays all navigational speeds
	dViewX*=senseDecay;
	dViewY*=senseDecay;
	dViewZ*=senseDecay;
	dViewDistance=dViewDistance*senseDecay+1*( 1-senseDecay );
}

