
/// Project:	FMI 3D (2013)
/// File:		fmi3d_lib.cpp
///	About:		Module for graphical primitives
///
/// History:
///		2013.04.26/PB	Initial version
///		2013.04.27/PB	Implementation of Object3D, transformations
///						and view point management
///		2013.04.28/PB	Refactored. Code moved to camera module.
///		2013.05.01/PB	Fixed FPS measurement. Added random().
///		2013.05.03/PB	Keyboard navigation, toggle day/night mode,
///						support for 2D mode
///		2013.05.05/PB	Fix fog in 3D mode
///		2013.05.06/PB	Emission light
///		2013.05.07/PB	Loading textures from PNG files (inc. transparencies)
///		2013.05.08/PB	Interactive buttons
///		2013.05.10/PB	getPixelColor(), getPixelColori(), objectAtPoint()
///		2013.05.10/PB	Loading textures from PNG/JPG files
///		2013.05.12/PB	Full object interactivity
///		2013.05.16/PB	Default event handlers, closestPointToSegment()
///		2013.06.09/PB	Support for quad-buffer stereo
///		2013.09.30/PB	Current working folder is set as needed


#include <Windows.h>
#include <Winbase.h>
#include <unistd.h>

#include "fmi3d_lib.h"
#include "fmi3d_primitives.h"
#include "fmi3d_ground.h"
#include "fmi3d_camera.h"
#include "fmi3d_button.h"

#include "stb_image.h"

#include <time.h>
#include <stdio.h>
//#include <iostream> // cout

/// Global variables
int Bright = true;		/// Bright or dark mode
int swapInterval = 1;	/// Keep it 0 for faster graphics
GLuint noTexture;		/// Global no-texture texture
bool inShadow;			/// Draw normal or shadowed
bool inReflection;		/// Draw normal or reflected
bool inColorPicking;	/// Draw color-picking images
bool inDrag = false;	/// Currently in drag mode
Object3D* activeObject;	/// Currently selected object for dragging

bool inAnagliphicStereo = false;
bool inMono = true;
bool inQuadBufferStereo = false;

/// Time management variables
double t=0;   			/// current time in seconds (since program start)
double fps=40; 			/// current fps (approximated)
int frame = 0;			/// current frame
int lastFrame = 0;
double startTime, lastTime;

/// Environment
GLfloat defaultEmission[4] =	{0.1, 0.1, 0.1, 1};
GLfloat noEmission[4] =			{0,0,0,0};
GLfloat whiteSpecular[4] =		{1,1,1,1};
GLfloat noSpecular[4] =			{0,0,0,0};

bool useCustomFogColor = false;
GLfloat customFogColor[4] = {0.5,0.8,1};

/// Properties of the graphical window
string windowCaption;
int windowHeight;
int windowWidth;
bool window3D;


/// ===========================================================================
/// Useful functions
/// ===========================================================================

/// Random floating point number
double random( double a, double b )
{
	return ( rand()/( float )RAND_MAX )*( b-a )+a;
}


/// Random integer number
int irandom( int a, int b )
{
	return rand()%( b-a+1 )+a;
}


/// Loads a texture from a TGA/PNG/JPG file.
/// Returns the texture's ID.
GLuint loadTexture( string textureName )
{
	int width, height, bitdepth;

	GLuint texture;

	glEnable( GL_TEXTURE_2D );
	glGenTextures( 1,&texture );
	glBindTexture( GL_TEXTURE_2D,texture );

	glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT );

	bitdepth = 8;
	unsigned char *data = stbi_load( textureName.c_str(), &width, &height, &bitdepth, 4 );
	if( !data ) data = stbi_load( ( "../"+textureName ).c_str(), &width, &height, &bitdepth, 4 );
	if( !data ) return false;

	gluBuild2DMipmaps( GL_TEXTURE_2D,
					   4,
					   width,
					   height,
					   GL_RGBA,
					   GL_UNSIGNED_BYTE,
					   data );
	glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR );
	glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR );
	glTexEnvf( GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE );

	stbi_image_free( data );

	return texture;
}


/// ===========================================================================
/// Window management
/// ===========================================================================

/// Set scene brightnes
void setBrightness()
{
	GLfloat fogDark[4] = {0,0,0,1};
	GLfloat fogBright[4] = {1,1,1,1};

	if( inColorPicking )
	{
		glClearColor( 0,0,0,0.5 );
	}
	else
	{
		glFogi( GL_FOG_MODE, GL_LINEAR );
		if( useCustomFogColor )
		{
			glClearColor( customFogColor[0],customFogColor[1],customFogColor[2],1 );
			glFogfv( GL_FOG_COLOR, customFogColor );
		}
		else if( Bright )
		{
			glClearColor( 1,1,1,1 );
			glFogfv( GL_FOG_COLOR, fogBright );
		}
		else
		{
			glClearColor( 0,0,0,1 );
			glFogfv( GL_FOG_COLOR, fogDark );
		}
	}
}
void changeBrightness()
{
	Bright = !Bright;
	setBrightness();
}


/// Handler for window resize, adjust OpenGL view port
void GLFWCALL onWindowResize( int width, int height )
{
	glfwGetWindowSize( &width, &height );
	height = height > 0 ? height : 1;

	windowHeight = height;
	windowWidth = width;

	glViewport( 0, 0, width, height );

}


/// Helper function to simplify check of mouse-key combinations
bool mouseInput( int mouseKey, int mouseButton )
{
	bool key,button;

	if( mouseKey==GLFW_KEY_UNKNOWN )
		key = !glfwGetKey( GLFW_KEY_LCTRL );
	else
		key = glfwGetKey( mouseKey );
	button=glfwGetMouseButton( mouseButton );

	return key && button;
}

/// Capture and process mouse motion in 3D
int oldX, oldY, oldT;
void GLFWCALL onMouseMove3D( int x, int y )
{
	if( activeObject )
	{
		/// Mouse motion while button is pressed sent to object
		if( mouseInput( GLFW_KEY_UNKNOWN,GLFW_MOUSE_BUTTON_LEFT ) ) activeObject->mouseEvent( x-oldX,y-oldY );
		if( mouseInput( GLFW_KEY_UNKNOWN,GLFW_MOUSE_BUTTON_RIGHT ) ) activeObject->mouseEvent( x-oldX,y-oldY );
	}
	else
	{
		/// Change button appearance if mouse is over it
		if( !inDrag ) buttonMouseOver( x,y );

		/// Rotate with left mouse button, zoom with right mouse button
		if( mouseInput( GLFW_KEY_UNKNOWN,GLFW_MOUSE_BUTTON_LEFT ) ) cameraRot3D( x-oldX,y-oldY );
		if( mouseInput( GLFW_KEY_UNKNOWN,GLFW_MOUSE_BUTTON_RIGHT ) ) cameraZoom3D( x-oldX,y-oldY );

		/// Pan with Control and left/right mouse buttons
		if( mouseInput( GLFW_KEY_LCTRL,GLFW_MOUSE_BUTTON_LEFT ) ) cameraPanXY3D( x-oldX,y-oldY );
		if( mouseInput( GLFW_KEY_LCTRL,GLFW_MOUSE_BUTTON_RIGHT ) ) cameraPanXZ3D( x-oldX,y-oldY );
	}
	oldX = x;
	oldY = y;
	oldT = t;
}
void GLFWCALL onMouseMove2D( int x, int y )
{
	//cout <<"dx="<<(x-oldX)/(t-oldT)<<" pixels/seconf"<<endl;
	if( activeObject )
	{
		/// Mouse motion while button is pressed sent to object
		if( mouseInput( GLFW_KEY_UNKNOWN,GLFW_MOUSE_BUTTON_LEFT ) ) activeObject->mouseEvent( x-oldX,y-oldY );
		if( mouseInput( GLFW_KEY_UNKNOWN,GLFW_MOUSE_BUTTON_RIGHT ) ) activeObject->mouseEvent( x-oldX,y-oldY );
	}
	else
	{
		/// Change button appearance if mouse is over it
		if( !inDrag ) buttonMouseOver( x,y );

		/// Pan with left mouse button
		if( mouseInput( GLFW_KEY_UNKNOWN,GLFW_MOUSE_BUTTON_LEFT ) ) cameraPanXY2D( x-oldX,y-oldY );
		if( mouseInput( GLFW_KEY_UNKNOWN,GLFW_MOUSE_BUTTON_RIGHT ) ) cameraZoom2D( x-oldX,y-oldY );
	}
	oldX = x;
	oldY = y;
	oldT = t;
}

void GLFWCALL onMousePressed( int button, int action )
{
	/// Scene buttons
	if( action==GLFW_PRESS )
		if( onButtonMousePressed( button ) )
			return; /// exit if button is processed

	/// Enter in drag mode
	if( action==GLFW_PRESS )
	{
		inDrag = true;
		int x,y;
		glfwGetMousePos( &x,&y );
		if( activeObject ) activeObject->setIsActive( false );
		activeObject = objectAtPoint( x,y );
		if( activeObject ) activeObject->setIsActive( true );
	}
	else
	{
		inDrag = false;
		if( activeObject ) activeObject->setIsActive( false );
		activeObject = NULL;
	}
}

/// Capture and process keyboard motion in 3D
double oldTL = 0;
double oldTR = 0;
double oldTU = 0;
double oldTD = 0;
double oldTPU = 0;
double oldTPD = 0;
double newT;
/// Helper function to simplify check of keyboard input
void keyboardInput( int checkKey, int key, int action, double *oldT, void( *navigation3D )( double,double ), double k1, double k2 )
{
	if( key==checkKey && action==GLFW_PRESS )   *oldT=t;
	if( key==checkKey && action==GLFW_RELEASE ) *oldT=0;
	if( newT!=*oldT )
	{
		if( *oldT!=0 && glfwGetKey( checkKey ) )
		{
			navigation3D( k1*( newT-*oldT ),k2*( newT-*oldT ) );
			*oldT = newT;
		}
	}
}

void GLFWCALL onKeyPressed3D( int key, int action )
{
	double k = 1000;
	newT = t;

	/// Scene buttons
	if( action==GLFW_PRESS )
		if( onButtonKeyPressed( key ) )
			return; /// exit if key is processed

	/// Interactive objects
	if( ( action==GLFW_PRESS ) && ( key==GLFW_KEY_TAB ) )
	{
		if( glfwGetKey( GLFW_KEY_LCTRL ) )
		{
			if( activeObject )
			{
				activeObject->setIsActive( false );
				activeObject=NULL;
			}
		}
		else
			selectNextObject();
		return;
	}

	if( ( action==GLFW_PRESS ) && ( activeObject ) )
	{
		activeObject->keyEvent( key );
		return;
	}

	/// Navigation 3D
	if( glfwGetKey( GLFW_KEY_LCTRL ) )
	{
		/// Normal arrows
		keyboardInput( GLFW_KEY_LEFT,key,action,&oldTL,cameraPanXY3D,-k/2,0 );
		keyboardInput( GLFW_KEY_RIGHT,key,action,&oldTR,cameraPanXY3D,k/2,0 );
		keyboardInput( GLFW_KEY_UP,key,action,&oldTU,cameraPanXY3D,0,-k/2 );
		keyboardInput( GLFW_KEY_DOWN,key,action,&oldTD,cameraPanXY3D,0,k/2 );
		keyboardInput( GLFW_KEY_PAGEUP,key,action,&oldTPU,cameraPanXZ3D,0,-k/2 );
		keyboardInput( GLFW_KEY_PAGEDOWN,key,action,&oldTPD,cameraPanXZ3D,0,k/2 );
		/// Keypad arrows
		keyboardInput( GLFW_KEY_KP_4,key,action,&oldTL,cameraPanXY3D,-k,0 );
		keyboardInput( GLFW_KEY_KP_6,key,action,&oldTR,cameraPanXY3D,k,0 );
		keyboardInput( GLFW_KEY_KP_8,key,action,&oldTU,cameraPanXY3D,0,-k );
		keyboardInput( GLFW_KEY_KP_2,key,action,&oldTD,cameraPanXY3D,0,k );
		keyboardInput( GLFW_KEY_KP_9,key,action,&oldTPU,cameraPanXZ3D,0,-k );
		keyboardInput( GLFW_KEY_KP_3,key,action,&oldTPD,cameraPanXZ3D,0,k );
	}
	else
	{
		/// Normal arrows
		keyboardInput( GLFW_KEY_LEFT,key,action,&oldTL,cameraRot3D,-k,0 );
		keyboardInput( GLFW_KEY_RIGHT,key,action,&oldTR,cameraRot3D,k,0 );
		keyboardInput( GLFW_KEY_UP,key,action,&oldTU,cameraRot3D,0,-k );
		keyboardInput( GLFW_KEY_DOWN,key,action,&oldTD,cameraRot3D,0,k );
		keyboardInput( GLFW_KEY_PAGEUP,key,action,&oldTPU,cameraZoom3D,0,-k );
		keyboardInput( GLFW_KEY_PAGEDOWN,key,action,&oldTPD,cameraZoom3D,0,k );
		/// Keypad arrows
		keyboardInput( GLFW_KEY_KP_4,key,action,&oldTL,cameraRot3D,-k,0 );
		keyboardInput( GLFW_KEY_KP_6,key,action,&oldTR,cameraRot3D,k,0 );
		keyboardInput( GLFW_KEY_KP_8,key,action,&oldTU,cameraRot3D,0,-k );
		keyboardInput( GLFW_KEY_KP_2,key,action,&oldTD,cameraRot3D,0,k );
		keyboardInput( GLFW_KEY_KP_9,key,action,&oldTPU,cameraZoom3D,0,-k );
		keyboardInput( GLFW_KEY_KP_3,key,action,&oldTPD,cameraZoom3D,0,k );
	}
}
void GLFWCALL onKeyPressed2D( int key, int action )
{
	double k = 300;
	newT = t;

	/// Scene buttons
	if( action==GLFW_PRESS )
		if( onButtonKeyPressed( key ) )
			return; /// exit if key is processed


	/// Interactive objects
	if( ( action==GLFW_PRESS ) && ( key==GLFW_KEY_TAB ) )
	{
		if( glfwGetKey( GLFW_KEY_LCTRL ) )
		{
			if( activeObject )
			{
				activeObject->setIsActive( false );
				activeObject=NULL;
			}
		}
		else
			selectNextObject();
		return;
	}

	if( ( action==GLFW_PRESS ) && ( activeObject ) )
	{
		activeObject->keyEvent( key );
		return;
	}

	/// Navigation 2D
	/// Normal arrows
	keyboardInput( GLFW_KEY_LEFT,key,action,&oldTL,cameraPanXY2D,-k,0 );
	keyboardInput( GLFW_KEY_RIGHT,key,action,&oldTR,cameraPanXY2D,k,0 );
	keyboardInput( GLFW_KEY_UP,key,action,&oldTU,cameraPanXY2D,0,-k );
	keyboardInput( GLFW_KEY_DOWN,key,action,&oldTD,cameraPanXY2D,0,k );
	keyboardInput( GLFW_KEY_PAGEUP,key,action,&oldTPU,cameraZoom2D,0,-k );
	keyboardInput( GLFW_KEY_PAGEDOWN,key,action,&oldTPD,cameraZoom2D,0,k );
	/// Keypad arrows
	keyboardInput( GLFW_KEY_KP_4,key,action,&oldTL,cameraPanXY2D,-k,0 );
	keyboardInput( GLFW_KEY_KP_6,key,action,&oldTR,cameraPanXY2D,k,0 );
	keyboardInput( GLFW_KEY_KP_8,key,action,&oldTU,cameraPanXY2D,0,-k );
	keyboardInput( GLFW_KEY_KP_2,key,action,&oldTD,cameraPanXY2D,0,k );
	keyboardInput( GLFW_KEY_KP_9,key,action,&oldTPU,cameraZoom2D,0,-k );
	keyboardInput( GLFW_KEY_KP_3,key,action,&oldTPD,cameraZoom2D,0,k );
}


/// Initializes OpenGL, GLFW and the virtual scene
bool openWindow3D( string caption )
{
#define NAME_SIZE 1024
	char name[NAME_SIZE];
	GetModuleFileName( NULL,name,NAME_SIZE );
	for( int i=strlen( name ); i>0; i-- )
		if( name[i]=='\\' )
		{
			name[i]=0;
			break;
		}
	chdir( name );

	//cout << name << endl;

	/// -----------------------------------------------------------------------
	/// GLFW-related initialization (graphics window and its properties)
	/// -----------------------------------------------------------------------
	{
		/// Initialize GLFW
		glfwInit();

		GLFWvidmode desktop;
		glfwGetDesktopMode( &desktop );

		/// Open window, center it within the desktop and set window's properties
		/// First try with quad-buffer, if fails, try with normal
		glfwOpenWindowHint( GLFW_STEREO,GL_TRUE );
		if( glfwOpenWindow( 1000,600,/*desktop.Width, desktop.Height,*/ 0, 0, 0, 0, 16, 0, GLFW_WINDOW ) )
		{
			inQuadBufferStereo = true;
			inMono = false;
		}
		else
		{
			inQuadBufferStereo = false;
			inMono = true;
			glfwOpenWindowHint( GLFW_STEREO,GL_FALSE );
			if( !glfwOpenWindow( 1000,600,/*desktop.Width, desktop.Height,*/ 0, 0, 0, 0, 16, 0, GLFW_WINDOW ) ) return false;
		}

		glfwGetDesktopMode( &desktop );
		glfwGetWindowSize( &windowWidth, &windowHeight );
		glfwSetWindowPos( ( desktop.Width-windowWidth )/2, ( desktop.Height-windowHeight )/4 );
		glfwEnable( GLFW_MOUSE_CURSOR );
		glfwSetWindowTitle( caption.c_str() );
		windowCaption = caption;
		glfwSwapInterval( swapInterval );
		glfwEnable( GLFW_KEY_REPEAT );

		/// Hook resize events
		onWindowResize( windowWidth,windowHeight );
		glfwSetWindowSizeCallback( onWindowResize );

		/// Hook mouse & kbd events
		glfwSetMousePosCallback( onMouseMove3D );
		glfwGetMousePos( &oldX,&oldY );
		glfwSetMouseButtonCallback( onMousePressed );
		glfwSetKeyCallback( onKeyPressed3D );
	}

	/// -----------------------------------------------------------------------
	/// OpenGL-related initialization
	/// -----------------------------------------------------------------------
	{
		glEnable( GL_DEPTH_TEST );
		glEnable( GL_LIGHTING );
		glEnable( GL_LIGHT0 );
		glEnable( GL_COLOR_MATERIAL );
		glEnable( GL_NORMALIZE );
		glEnable( GL_BLEND );
		glBlendFunc( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA );

		glShadeModel( GL_SMOOTH );
		glColor3f( 1,1,1 );

//		glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, defaultEmission );

		setBrightness();
	}

	/// -----------------------------------------------------------------------
	/// FMI3D-related initialization
	/// -----------------------------------------------------------------------
	{
		UID = 0;
		window3D = true;
		ground = new Ground3D( GROUND_TILES );
		systemFont3D = new Font3D( "Consolas", false, false, 0.2 );
		systemFont2D = new Font2D( "Verdana", 12, false, false );
		startTime = glfwGetTime();
		createPrimitives();
		noTexture = ++UID;
		srand( time( NULL ) );
	}

	return true;
}

/// Initializes OpenGL, GLFW and the virtual scene
bool openWindow2D( string caption )
{
	bool res = openWindow3D( caption );

	/// Hook mouse & kbd events
	glfwSetMousePosCallback( onMouseMove2D );
	glfwGetMousePos( &oldX,&oldY );
	glfwSetMouseButtonCallback( onMousePressed );
	glfwSetKeyCallback( onKeyPressed2D );
	window3D = false;
	return res;
}


/// Render the whole scene
void renderScene()
{
	double cp[4]= {0,0,1,0};

	/// Draw the visible part of all objects
	inShadow = false;
	inReflection = false;

	/// Ground clipping is active only in 3D mode
	if( window3D && ground->isVisible() )
	{
		glEnable( GL_CLIP_PLANE0 );
		glClipPlane( GL_CLIP_PLANE0,cp );
	}

//	drawScene(); // fixed 2013.06.01

	if( !inColorPicking )
	{
		if( ground->isVisible() )
		{
			if( window3D )
			{
				/// Draw the reflection or shadows of all objects
				if( ground->isReflective() )
				{
					inReflection = true;
					glScalef( 1,1,-1 );
					glClipPlane( GL_CLIP_PLANE0,cp );
					drawScene();
					glScalef( 1,1,-1 );
					inReflection = false;
				}
				else
				{
					inShadow = true;
					glPushMatrix();
					glTranslatef( 0,0,-0.05 );
					glScalef( 1,1,-0.01 );
					glClipPlane( GL_CLIP_PLANE0,cp );
					drawScene();
					glPopMatrix();
					inShadow = false;
				}
			}

			glDisable( GL_CLIP_PLANE0 );

			/// Draw ground
			ground->drawImage();
		}
		drawScene();				// fixed 2013.06.01

		/// Draw buttons
		drawButtons();

		/// Draw the visible part of all frames
		if( ground->isVisible() )
		{
			glEnable( GL_CLIP_PLANE0 );
			glClipPlane( GL_CLIP_PLANE0,cp );
		}
		drawFrames();
	}
	else							// fixed 2013.06.01
	{
		// fixed 2013.06.01
		/// In color  picking		// fixed 2013.06.01
		drawScene();				// fixed 2013.06.01
	}

	//if( window3D )
	//{
	glDisable( GL_CLIP_PLANE0 );
	//}
}


/// Process a single step from the animation and swap buffers
bool isRunning()
{
	if( inAnagliphicStereo )
	{
		glDrawBuffer( GL_BACK );
		glClear( GL_COLOR_BUFFER_BIT+GL_DEPTH_BUFFER_BIT );

		setProjectionMatrix( LEFT_EYE );
		setViewMatrix( LEFT_EYE );
		glColorMask( true, false, false, false );
		renderScene();

		glClear( GL_DEPTH_BUFFER_BIT ) ;

		setProjectionMatrix( RIGHT_EYE );
		setViewMatrix( RIGHT_EYE );
		glColorMask( false, true, true, false );
		renderScene();
		glColorMask( true, true, true, true );
	}

	if( inQuadBufferStereo )
	{
		glDrawBuffer( GL_BACK_LEFT );
		glClear( GL_COLOR_BUFFER_BIT+GL_DEPTH_BUFFER_BIT );

		setProjectionMatrix( LEFT_EYE );
		setViewMatrix( LEFT_EYE );
		renderScene();

		glDrawBuffer( GL_BACK_RIGHT );
		glClear( GL_COLOR_BUFFER_BIT+GL_DEPTH_BUFFER_BIT ) ;

		setProjectionMatrix( RIGHT_EYE );
		setViewMatrix( RIGHT_EYE );
		renderScene();
	}

	if( inMono )
	{
		glDrawBuffer( GL_BACK );
		glClear( GL_COLOR_BUFFER_BIT+GL_DEPTH_BUFFER_BIT );

		setProjectionMatrix( MIDDLE_EYE );
		setViewMatrix( MIDDLE_EYE );
		renderScene();
	}

	/// Measure and update fps
	{
		frame++;

		t = glfwGetTime();
		fps = ( frame-lastFrame )/( t-lastTime );
//		cout << "dT=" << fps<< "\n";

//		char buf[100];
//		snprintf( buf,99,"%s [%0.0f fps]",windowCaption.c_str(),fps );
//		glfwSetWindowTitle( buf );

		lastFrame = frame;
		lastTime = t;
	}

	glfwSwapBuffers();

	if( window3D )
	{
		onKeyPressed3D( 0,0 );
		cameraNavigation3D();
	}
	else
	{
		onKeyPressed2D( 0,0 );
		cameraNavigation2D();
	}

	return( !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED ) );
}


/// Get the pixel colour at window coordinates (X,Y)
GLuint getPixelColori( int x, int y, int glBuffer )
{
	GLuint pixel;

	glReadBuffer( glBuffer );
	glReadPixels( x, windowHeight-y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &pixel );

	return pixel;
}
vect_t getPixelColor( int x, int y, int glBuffer )
{
	uint8_t pixel[4];
	vect_t col;

	glReadBuffer( glBuffer );
	glReadPixels( x, windowHeight-y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel );

	col.x=pixel[0]/255.0;
	col.y=pixel[1]/255.0;
	col.z=pixel[2]/255.0;
	col.w=pixel[3]/255.0;

	return col;
}

/// Find object at given pixel coordinates. Return NULL if no object is there.
Object3D* objectAtPoint( int x, int y )
{
	if( ( x<0 ) || ( x>=windowWidth ) || ( y<0 ) || ( y>=windowHeight ) ) return NULL;

	inColorPicking = true;
	setBrightness();
	glDepthMask( GL_TRUE );
	glDrawBuffer( GL_BACK );
	glClear( GL_COLOR_BUFFER_BIT+GL_DEPTH_BUFFER_BIT );
	drawScene();
//float c[4];
//glGetFloatv(GL_CURRENT_COLOR,&c[0]);
//cout<<"current afterdr color = "<<c[0]<<","<<c[1]<<","<<c[2]<<","<<c[3]<<","<<endl;

	GLuint imageList = getPixelColori( x,y,GL_BACK );
//cout << "color="<<hex<<imageList<<" obj="<<findObject( imageList )<<endl;
//	for(int i=0;i<100;i++) glfwSwapBuffers();

	inColorPicking = false;
	setBrightness();
	glClear( GL_COLOR_BUFFER_BIT+GL_DEPTH_BUFFER_BIT );

	return findObject( imageList );
}

/// Default key event handler for moving object in OXY
void defaultKeyPress( int key )
{
	if( ( key==GLFW_KEY_LEFT )     || ( key==GLFW_KEY_KP_4 ) ) activeObject->iMove( -10,0 );
	if( ( key==GLFW_KEY_RIGHT )    || ( key==GLFW_KEY_KP_6 ) ) activeObject->iMove( +10,0 );
	if( ( key==GLFW_KEY_UP )       || ( key==GLFW_KEY_KP_8 ) ) activeObject->iMove( 0,-10 );
	if( ( key==GLFW_KEY_DOWN )     || ( key==GLFW_KEY_KP_2 ) ) activeObject->iMove( 0,+10 );
}

/// Default mouse event handler for moving object in OXY
void defaultMouseMove( int dX, int dY )
{
	if( glfwGetMouseButton( GLFW_MOUSE_BUTTON_LEFT ) )  activeObject->iMove( dX,dY );
}


/// Return a point from segment P1-P2, which is closest to point A
vect_t closestPointToSegment( Object3D* A, Object3D* P1, Object3D* P2 )
{
	vect_t a = A->getCenter();
	vect_t p1 = P1->getCenter();
	vect_t p2 = P2->getCenter();

	double len = ( p1.x-p2.x )*( p1.x-p2.x )+( p1.y-p2.y )*( p1.y-p2.y );
	double t = ( ( a.x-p1.x )*( p2.x-p1.x )+( a.y-p1.y )*( p2.y-p1.y ) )/len;
	if( t<0.1 ) t=0.1;
	if( t>0.9 ) t=0.9;

	return p1.add( p2.sub( p1 ).mul( t ) );
}
