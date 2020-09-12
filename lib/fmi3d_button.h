#ifndef FMI3D_BUTTON_H
#define FMI3D_BUTTON_H

/// Project:	FMI 3D (2013)
/// File:		fmi3d_button.h
///	About:		Header file for the 2D textured transparent button
///
/// History:
///		2013.05.07/PB	Declaration of Button2D
///		2013.05.08/PB	Interactive buttons
///		2013.05.12/PB	Classes for exitButton2D,groungButton2D,lightButton2D
///		2013.06.02/PB	Button2D::state()


#include <string>

#include "GL/glfw.h"
#include "fmi3d_ground.h"

using namespace std;

typedef void(*action_t)();

/// Predefined buttons
#define BUTTON_EXIT		0b0000000000000001
#define BUTTON_LIGHT	0b0000000000000010
#define BUTTON_GROUND	0b0000000000000100


/// ===========================================================================
/// Button2D
/// ===========================================================================

class Button2D
{
	public:
		int	key;
		action_t action;
		bool selected;
		Button2D(string imageName, int key, action_t action, int state=0, int totalStates=0);
		void drawImage();
		void select();
		void deselect();
		void setState(int state);
	private:
		string keyText;
		GLuint imageList;
		GLuint texButton;
		int state;
		int totalStates;
};

class exitButton2D: public Button2D
{
	public:
		exitButton2D():Button2D("buttons/button_exit.png",GLFW_KEY_ESC,glfwCloseWindow) {}
};

class lightButton2D: public Button2D
{
	public:
		lightButton2D():Button2D("buttons/button_light.png",'L',changeBrightness) {};
};

class groundButton2D: public Button2D
{
	public:
		groundButton2D():Button2D("buttons/button_ground.png",'G',changeGround) {};
};

extern Button2D* selectedButton;
extern void drawButtons();
extern bool onButtonKeyPressed( int key );
extern bool onButtonMousePressed( int button );
extern void buttonMouseOver(int x, int y);
extern Button2D* addButton(string imageName, int key, action_t action);
extern Button2D* addButton(Button2D* button);


#endif
