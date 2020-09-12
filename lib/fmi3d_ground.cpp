
/// Project:	FMI 3D (2013)
/// File:		fmi3d_ground.cpp
///	About:		Module for 3D ground
///
/// History:
///		2013.04.27/PB	Implementation of Ground3D
///		2013.05.03/PB	Ground texture can be changed
///		2013.05.07/PB	All textures converted from TGA to PNG
///		2013.05.08/PB	changeGround() made top-level
///		2013.05.09/PB	Fixed fog usage in 2D mode
///		2013.05.10/PB	All textures converted to JPG
///		2013.05.16/PB	Added isVisible() and noGround()


#include "GL/glfw.h"

#include "fmi3d_ground.h"
#include "fmi3d_lib.h"
#include "fmi3d_primitives.h"


Ground3D* ground;

Ground3D::Ground3D( ground_t groundStyle )
{
	texGround = -1;
	patchList = ++UID;
	imageList = ++UID;
	this->groundStyle = groundStyle;
	buildGroundPatch();

	/// Create a display list for all plates.
	glNewList( imageList,GL_COMPILE );
	{
		glPushAttrib( GL_CURRENT_BIT | GL_ENABLE_BIT );
		glColor4f( 1,1,1,0.8 );
		glDisable( GL_LIGHTING );
		glEnable( GL_TEXTURE_2D );
		glBindTexture( GL_TEXTURE_2D,texGround );
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL );

		glPushMatrix();
		glScalef( GROUND_SIZE,GROUND_SIZE,0 );

		glNormal3f( 0,0,1 );

		for( int i=0; i<GROUND_COUNT; i++ )
		{
			for( int j=0; j<GROUND_COUNT; j++ )
			{
				glPushMatrix();
				glTranslatef( i-GROUND_COUNT/2,j-GROUND_COUNT/2,0 );
				glCallList( patchList );
				glPopMatrix();
			}
		}

		glPopMatrix();
		glPopAttrib();
	}
	glEndList();
}

void Ground3D::drawImage()
{
	if( groundStyle==GROUND_INVISIBLE )
		return;

	if( window3D )
		glEnable( GL_FOG );
	else
		glDisable( GL_FOG );
	glCallList( imageList );
}

bool Ground3D::isReflective()
{
	return reflective;
}

bool Ground3D::isVisible()
{
	return groundStyle!=GROUND_INVISIBLE;
}

/// Prepare a ground patch display list
void Ground3D::buildGroundPatch( )
{
	double groundScale = 1;

	switch( groundStyle )
	{
		case GROUND_INVISIBLE:
			break;
		case GROUND_MARBLE:
			texGround = loadTexture( "Textures/Ground_Marble.jpg" );
			reflective = true;
			groundScale = 3;
			break;
		case GROUND_TILES:
			texGround = loadTexture( "Textures/Ground_Tiles.jpg" );
			reflective = true;
			groundScale = 3;
			break;
		case GROUND_ROCK:
			texGround = loadTexture( "Textures/Ground_Rock.jpg" );
			reflective = false;
			groundScale = 3;
			break;
		case GROUND_METAL:
			texGround = loadTexture( "Textures/Ground_Metal.jpg" );
			reflective = false;
			groundScale = 21;
			break;
	}

	/// Create a display list for a single plate
	if( glIsList( patchList ) ) glDeleteLists( patchList,1 );
	glNewList( patchList,GL_COMPILE );
	{
		glBegin( GL_QUADS );
		glTexCoord2f( 0, 0 );
		glVertex2f( 0, 0 );
		glTexCoord2f( 0, groundScale );
		glVertex2f( 0, 1 );
		glTexCoord2f( groundScale, groundScale );
		glVertex2f( 1, 1 );
		glTexCoord2f( groundScale, 0 );
		glVertex2f( 1, 0 );
		glEnd();
	}
	glEndList();
}

/// Change the texture of the ground
void Ground3D::changeGround( )
{
	glDeleteTextures( 1,&texGround );

	switch( groundStyle )
	{
		case GROUND_INVISIBLE:
			break;
		case GROUND_MARBLE:
			groundStyle = GROUND_TILES;
			break;
		case GROUND_TILES:
			groundStyle = GROUND_ROCK;
			break;
		case GROUND_ROCK:
			groundStyle = GROUND_METAL;
			break;
		case GROUND_METAL:
			groundStyle = GROUND_MARBLE;
			break;
	}

	buildGroundPatch();
}

/// Remove the texture of the ground
void Ground3D::noGround( )
{
	glDeleteTextures( 1,&texGround );
	groundStyle = GROUND_INVISIBLE;
}

void changeGround( )
{
	ground->changeGround();
}

void noGround( )
{
	ground->noGround();
}
