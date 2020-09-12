
/// Project:	FMI 3D (2013)
/// File:		fmi3d_button.cpp
///	About:		Module for 2D buttons
///
/// History:
///		2013.05.07/PB	Implementation of Button2D
///		2013.05.08/PB	Interactive buttons
///		2013.05.16/PB	Fixed drawButtons(), added addButton()
///		2013.06.02/PB	Button2D::state()
///     2013.09.15/PB   Buttons are always on top


#include "GL/glfw.h"
#include <list>
//#include <iostream>

#include "fmi3d_lib.h"
#include "fmi3d_primitives.h"
#include "fmi3d_button.h"

#define BUTTON_SIZE	64	/// Visual button size in pixels
#define BUTTON_GAP 4	/// Gap between buttons
#define BUTTON_STATE_SIZE 0.12	/// Size of state boxes
#define BUTTON_STATE_GAP 0.16	/// Offset between state boxes
#define BUTTONS_IN_ROW 6		/// Number of button is a vetical row

#define SELECTED_COLOR	1,1,1,1
#define DEFAULT_COLOR	1,1,1,0.5
#define STATE_SELECTED_COLOR	0.5,0.5,1,0.5
#define STATE_DEFAULT_COLOR		1,1,1,0.2
#define STATE_DARK_COLOR		0,0,0,0.2
#define BUTTON_TEXT_COLOR		0,0,0,0.7

list<Button2D*> buttons; /// list of all created buttons
Button2D* selectedButton = NULL;

Button2D::Button2D( string imageName, int key, action_t action, int state, int totalStates )
{
//    cout<<"2D="<<systemFont2D->getFontList()<<endl;
//    cout<<"3D="<<systemFont3D->getFontList()<<endl<<endl;

	this->key = key;
	this->action = action;
	this->selected = false;
	this->state = state;
	this->totalStates = totalStates;

	texButton = -1;
	imageList = ++UID;

	texButton = loadTexture( imageName );

	/// Set key text
	if( key<128 ) keyText=char( key );
	if( key==GLFW_KEY_SPACE ) keyText="Space";
	if( key==GLFW_KEY_ESC ) keyText="Esc";


	/// Create a display list for the button
	glNewList( imageList,GL_COMPILE );
	{
		glBindTexture( GL_TEXTURE_2D,texButton );

		/// Draw button
		glBegin( GL_QUADS );
		{
			glTexCoord2d( 0,0 );
			glVertex3f( 0,1,1 );
			glTexCoord2d( 0,1 );
			glVertex3f( 0,0,1 );
			glTexCoord2d( 1,1 );
			glVertex3f( 1,0,1 );
			glTexCoord2d( 1,0 );
			glVertex3f( 1,1,1 );
		}
		glEnd();

		glPushMatrix();
		glPushAttrib( GL_LIST_BASE | GL_LINE_BIT | GL_POLYGON_BIT | GL_ENABLE_BIT );
		glColor4f( BUTTON_TEXT_COLOR );
		glDisable( GL_TEXTURE_2D );
		glDisable( GL_LIGHTING );
		glPolygonMode( GL_FRONT_AND_BACK,GL_LINE );
		glEnable( GL_LINE_SMOOTH );
		glLineWidth( 0.1 );
		glDepthMask( GL_FALSE );
		glRasterPos3d( 0.5-keyText.length()/2.0*0.095,0.08,2 );
		glListBase( systemFont2D->getFontList() );
		glCallLists( keyText.length(), GL_UNSIGNED_BYTE, keyText.c_str() );
		glDepthMask( GL_TRUE );
		glPopAttrib();
		glPopMatrix();
	}
	glEndList();

	buttons.push_back( this );
}

void Button2D::drawImage()
{
	/// Draw main image of the button
	glCallList( imageList );

	/// Draw state
	if( totalStates==0 ) return;
	//glColor3f(1,0,0);
	glPushAttrib( GL_CURRENT_BIT|GL_ENABLE_BIT|GL_LIGHTING_BIT|GL_LINE_BIT|GL_TEXTURE_BIT|GL_POLYGON_BIT );
	glDisable( GL_TEXTURE_2D );
	glDisable( GL_LIGHTING );
	glDisable( GL_LINE_SMOOTH );
	glPushMatrix();
	glTranslated( 1.02,0.04,0 );
	for( int i=0; i<totalStates; i++ )
	{
		glPolygonMode( GL_FRONT_AND_BACK,GL_LINE );
		glColor4f( STATE_DARK_COLOR );
		glLineWidth( 1 );
		glBegin( GL_QUADS );
		{
			glVertex3f( 0,BUTTON_STATE_SIZE,1 );
			glVertex3f( 0,0,1 );
			glVertex3f( BUTTON_STATE_SIZE,0,1 );
			glVertex3f( BUTTON_STATE_SIZE,BUTTON_STATE_SIZE,1 );
		}
		glEnd();
		glLineWidth( 1 );
		if( i==state )
		{
			glPolygonMode( GL_FRONT_AND_BACK,GL_FILL );
			glColor4f( STATE_SELECTED_COLOR );
		}
		else
		{
			glPolygonMode( GL_FRONT_AND_BACK,GL_FILL );
			glColor4f( STATE_DEFAULT_COLOR );
		}
		glBegin( GL_QUADS );
		{
			glVertex3f( 0,BUTTON_STATE_SIZE,2 );
			glVertex3f( 0,0,2 );
			glVertex3f( BUTTON_STATE_SIZE,0,2 );
			glVertex3f( BUTTON_STATE_SIZE,BUTTON_STATE_SIZE,2 );
		}
		glEnd();
		glTranslated( 0,BUTTON_STATE_GAP,0 );
		if( i%BUTTONS_IN_ROW==(BUTTONS_IN_ROW-1) )
		{
			glTranslated( BUTTON_STATE_GAP,-BUTTONS_IN_ROW*BUTTON_STATE_GAP,0 );
		}
	}
	glPopMatrix();
	glPopAttrib();
}

void drawButtons()
{
	/// Setup scene for button drawing
	glPushAttrib( GL_CURRENT_BIT | GL_ENABLE_BIT );
	glDisable(GL_DEPTH_TEST);
	glDisable( GL_LIGHTING );
	glDisable( GL_FOG );
	glEnable( GL_TEXTURE_2D );
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

	/// Change projection
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
	glOrtho( -windowWidth/2, windowWidth/2, -windowHeight/2, +windowHeight/2, -2000, 2000 );

	/// Draw buttons
	glMatrixMode( GL_MODELVIEW );

	glPushMatrix();

	int vPos = BUTTON_GAP;
	for( list<Button2D*>::iterator button=buttons.begin(); button!=buttons.end(); ++button )
	{
		if( ( *button )->selected )
			glColor4f( SELECTED_COLOR );
		else
			glColor4f( DEFAULT_COLOR );
		glLoadIdentity();
		glTranslated( -windowWidth/2+BUTTON_GAP,windowHeight/2-BUTTON_SIZE-vPos,1900 );
		glScaled( BUTTON_SIZE,BUTTON_SIZE,1 );
		( *button )->drawImage();
		vPos+=BUTTON_SIZE+BUTTON_GAP;
	}

	glPopMatrix();

	/// Restore projection
	glMatrixMode( GL_PROJECTION );
	glPopMatrix(); //projection
	glMatrixMode( GL_MODELVIEW );

	/// Restore scene
	glPopAttrib();
}

/// Select a button
void Button2D::select()
{
	if( selectedButton ) selectedButton->deselect();
	selectedButton = this;
	selected = true;
}

/// Deselect a button
void Button2D::deselect()
{
	selected = false;
}

/// Set state info
void Button2D::setState( int state )
{
	this->state = state;
}


/// If the mouse is over a button and no mouse button is pressed,
/// than show that the button could be clicked on
void buttonMouseOver( int x, int y )
{
	int vPos = BUTTON_GAP;

	/// No mouse button must be pressed
	if( glfwGetMouseButton( GLFW_MOUSE_BUTTON_LEFT ) ) goto nothingSelected;
	if( glfwGetMouseButton( GLFW_MOUSE_BUTTON_RIGHT ) ) goto nothingSelected;
	if( glfwGetMouseButton( GLFW_MOUSE_BUTTON_MIDDLE ) ) goto nothingSelected;

	/// Test horizontal limits
	if( x<BUTTON_GAP ) goto nothingSelected;
	if( x>BUTTON_GAP+BUTTON_SIZE ) goto nothingSelected;

	for( list<Button2D*>::iterator button=buttons.begin(); button!=buttons.end(); ++button )
	{
		if( y>=vPos && y<vPos+BUTTON_SIZE )
		{
			( *button )->select();
			return;
		}
		vPos+=BUTTON_SIZE+BUTTON_GAP;
	}

nothingSelected:
	if( selectedButton ) selectedButton->deselect();
	selectedButton = NULL;
}

/// If a button key is pressed, then activate its action and return true.
/// If key is not recognized, return false.
bool onButtonKeyPressed( int key )
{
	for( list<Button2D*>::iterator button=buttons.begin(); button!=buttons.end(); ++button )
	{
		if( ( ( *button )->key == key ) && ( *button )->action )
		{
			( *button )->action();
			return true;
		};
	}
	return false;
}

/// If the mouse is pressed over a button, then activate its action and return
/// true. Otherwise return false.
bool onButtonMousePressed( int button )
{
	/// Exit if no selected button
	if( !selectedButton ) return false;

	if( button==GLFW_MOUSE_BUTTON_LEFT )
	{
		if( selectedButton->action ) selectedButton->action();
		return true;
	}

	return false;
}


/// Add nameless button
Button2D* addButton( string imageName, int key, action_t action )
{
	return new Button2D( imageName,key,action );
}
Button2D*  addButton( Button2D* __attribute__( ( unused ) )button )
{
	return button;
}
