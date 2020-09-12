
/// Project:	FMI 3D (2013)
/// File:		Test18 Bitmap text.cpp
///	About:		Text with bitmap font
///				Showing 2D text using bitmap font.
///
/// History:
///		2013.05.22/PB	Initial implementation


#include "fmi3d.h"


Font2D* font;
Text2D* text1;
Text2D* text2;
Text2D* text3;
Text2D* text4;
Object3D* plate;

void drawScene()
{
	text1->drawImage();
	text2->drawImage();
	text3->drawImage();
	text4->drawImage();
	plate->drawImage();
}

void drawFrames()
{
	plate->drawFrame();
}



int main()
{
	if( !openWindow3D( "Test 18: Bitmap text" ) ) return 1;

	addButton( new exitButton2D );

	font = new Font2D("Times New Roman",64,false,false);

	text1 = new Text2D(L"Abc",font);
	text1->setColor({0,0,0});

	text2 = new Text2D(L"Абв",font);
	text2->setColor({0,0,0});

	text3 = new Text2D(L"αβγ",font);
	text3->setColor({0,0,0});

	text4 = new Text2D(L"bбβ",font);
	text4->setColor({0,0,0});

	plate = new Cube3D(1);
	plate->setScale({10,10,1});
	plate->setColor({1,0,0});

	while( isRunning() )
	{
		text1->setCenter({5,5,1+fabs(5*sin(2*t))});
		text2->setCenter({5,-5,1+fabs(5*sin(2*t))});
		text3->setCenter({-5,5,1+fabs(5*sin(2*t))});
		text4->setCenter({-5,-5,1+fabs(5*sin(2*t))});
		plate->setCenter({0,0,fabs(5*sin(2*t))});
	}
	return 0;
}

