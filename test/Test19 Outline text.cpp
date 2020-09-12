
/// Project:	FMI 3D (2013)
/// File:		Test19 Outline text.cpp
///	About:		Text with outline font
///				Showing 3D text using outline font.
///
/// History:
///		2013.05.22/PB	Initial implementation


#include "fmi3d.h"


Font3D* font;
Text3D* text1;
Text3D* text2;
Text3D* text3;
Text3D* text4;
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
	if( !openWindow3D( "Test 19: Outline text" ) ) return 1;

	addButton( new exitButton2D );

	font = new Font3D( "Times New Roman" );

	text1 = new Text3D( L"Abc",font );
	text1->setColor( {0,0,1} );
	text1->setScale({2,2,1});
	text1->rotateLocal( 90, {0,0,1} );
	text1->rotateLocal( 90, {1,0,0} );

	text2 = new Text3D( L"Абв",font );
	text2->setColor( {0,0,1} );
	text2->setScale({2,2,1});
	text2->rotateLocal( 90, {0,0,1} );
	text2->rotateLocal( 90, {1,0,0} );

	text3 = new Text3D( L"αβγ",font );
	text3->setColor( {0,0,1} );
	text3->setScale({2,2,1});
	text3->rotateLocal( 90, {0,0,1} );
	text3->rotateLocal( 90, {1,0,0} );

	text4 = new Text3D( L"bбβ",font );
	text4->setColor( {0,0,1} );
	text4->setScale({2,2,1});
	text4->rotateLocal( 90, {0,0,1} );
	text4->rotateLocal( 90, {1,0,0} );

	plate = new Cube3D( 1 );
	plate->setScale( {10,10,1} );
	plate->setColor( {1,0,0} );

	while( isRunning() )
	{
		text1->setCenter( {5,5,1+fabs( 5*sin( 2*t ) )} );
		text2->setCenter( {5,-5,1+fabs( 5*sin( 2*t ) )} );
		text3->setCenter( {-5,5,1+fabs( 5*sin( 2*t ) )} );
		text4->setCenter( {-5,-5,1+fabs( 5*sin( 2*t ) )} );
		plate->setCenter( {0,0,fabs( 5*sin( 2*t ) )} );
	}
	return 0;
}

