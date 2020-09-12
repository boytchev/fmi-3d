
/// Project:	FMI 3D (2013)
/// File:		29142 Stereo patterns.cpp
/// Topic:		29 Stereo graphics
/// Slide:		14
///	About:		Stereo perception due to patterns
///
/// History:
///		2013.07.19/PB	Initial implementation


#include "fmi3d.h"

void drawScene()
{
}


void drawFrames()
{
}


int main()
{
	/// Open window
	if( !openWindow3D( "Stereo patterns" ) ) return 1;
	changeBrightness();
	changeGround();
	changeGround();
	changeGround();
	changeGround();

	/// Add buttons
	addButton( new Button2D("buttons/button_ground.png",GLFW_KEY_SPACE,changeGround));
	addButton( new exitButton2D );

	/// Main loop
	while( isRunning() )
	{
	}
	return 0;
}

