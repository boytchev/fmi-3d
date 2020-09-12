
/// Project:	FMI 3D (2013)
/// File:		24441 Hole through a hole of a hole.cpp
/// Topic:		24 Surfaces
/// Slide:		07
///	About:		Topology problem: hole through a hole of a hole
///
/// History:
///		2013.06.01/PB	Initial implementation
///     2013.09.13/PB   Changed button image, removed reflection


#include "fmi3d.h"

//#include <iostream>

int level=3;
int mode=1;	/// 0=wireframe 1=fill+wireframe 2=fill
double viewClip = 9;

#define ANIMATION_TIME 1.5
#define N 52
Subdiv3D* subd[N];
vect_t a1 = { 12 ,  -3 ,  2 };
vect_t a2 = { 12 ,  3 ,  2 };
vect_t a3 = { 12 ,  3 ,  -2 };
vect_t a4 = { 12 ,  -3 ,  -2 };
vect_t b1 = { 6 ,  0 ,  1 };
vect_t b2 = { 6 ,  0 ,  1 };
vect_t b3 = { 6 ,  0 ,  -1 };
vect_t b4 = { 6 ,  0 ,  -1 };
vect_t c1 = { 2 ,  -1 ,  1.5 };
vect_t c2 = { 2 ,  1 ,  1.5 };
vect_t c3 = { 2 ,  1 ,  -1.5 };
vect_t c4 = { 2 ,  -1 ,  -1.5 };
vect_t d1 = { 0 ,  1.5 ,  1 };
vect_t d2 = { 0 ,  4.5 ,  1 };
vect_t d3 = { 0 ,  4.5 ,  -1 };
vect_t d4 = { 0 ,  1.5 ,  -1 };
vect_t e1 = { 0 ,  -4.5 ,  1 };
vect_t e2 = { 0 ,  -1.5 ,  1 };
vect_t e3 = { 0 ,  -1.5 ,  -1 };
vect_t e4 = { 0 ,  -4.5 ,  -1 };
vect_t f1 = { -2 ,  -1 ,  1.5 };
vect_t f2 = { -2 ,  1 ,  1.5 };
vect_t f3 = { -2 ,  1 ,  -1.5 };
vect_t f4 = { -2 ,  -1 ,  -1.5 };
vect_t g1 = { -6 ,  0 ,  1 };
vect_t g2 = { -6 ,  0 ,  1 };
vect_t g3 = { -6 ,  0 ,  -1 };
vect_t g4 = { -6 ,  0 ,  -1 };
vect_t h1 = { -12 ,  -3 ,  2 };
vect_t h2 = { -12 ,  3 ,  2 };
vect_t h3 = { -12 ,  3 ,  -2 };
vect_t h4 = { -12 ,  -3 ,  -2 };
vect_t i1 = { 12 ,  -10 ,  10 };
vect_t i2 = { 12 ,  10 ,  10 };
vect_t i3 = { 12 ,  10 ,  -10 };
vect_t i4 = { 12 ,  -10 ,  -10 };
vect_t j1 = { -12 ,  -10 ,  10 };
vect_t j2 = { -12 ,  10 ,  10 };
vect_t j3 = { -12 ,  10 ,  -10 };
vect_t j4 = { -12 ,  -10 ,  -10 };
vect_t k1 = { 0.5 ,  -0.5 ,  10 };
vect_t k2 = { 0.5 ,  0.5 ,  10 };
vect_t k3 = { -0.5 ,  0.5 ,  10 };
vect_t k4 = { -0.5 ,  -0.5 ,  10 };
vect_t l1 = { 0.5 ,  -0.5 ,  -10 };
vect_t l2 = { 0.5 ,  0.5 ,  -10 };
vect_t l3 = { -0.5 ,  0.5 ,  -10 };
vect_t l4 = { -0.5 ,  -0.5 ,  -10 };


void drawScene()
{
    if( inReflection && level>4 ) return;

	double cp[4]= {0,0,-1,viewClip};

	double lw=6.0-viewDistance/5.0-level/2.0;
	glLineWidth( lw>0.5?lw:0.5 );

	glLightModeli( GL_LIGHT_MODEL_TWO_SIDE,inReflection?GL_FALSE:GL_TRUE );

	glEnable( GL_CLIP_PLANE1 );
	glClipPlane( GL_CLIP_PLANE1,cp );
	for( int i=0; i<N; i++ )
		subd[i]->drawImage();
	glDisable( GL_CLIP_PLANE1 );
	glLightModeli( GL_LIGHT_MODEL_TWO_SIDE,GL_FALSE );

}


void drawFrames()
{
    if( inReflection ) return;
    glDisable(GL_LINE_SMOOTH);
    glLineWidth(1);
	double cp[4]= {0,0,-1,viewClip};

	glEnable( GL_CLIP_PLANE1 );
	glClipPlane( GL_CLIP_PLANE1,cp );
	if( mode==1 )
		for( int i=0; i<N; i++ )
		{
		    subd[i]->setColor({0,0,0,0.5});
		    subd[i]->setPolygonMode(GL_LINE);
		    subd[i]->drawImage();
		    subd[i]->setPolygonMode(GL_FILL);
		    subd[i]->setColor({1,0.5,0});
		}
	glDisable( GL_CLIP_PLANE1 );
}


double showT;
double showClip = 9;
void showIntersection()
{
	showT = t;
	showClip = 4.5+9-showClip;
}

Button2D* butNL;
void nextLevel()
{
	level = ( level+1 )%6;
	butNL->setState(level);
	for( int i=0; i<N; i++ )
	{
		subd[i]->setLevel( level );
	}
}

Button2D* butWF;
void showWireframe()
{
	mode=(mode+1)%3;
	butWF->setState(mode);
	for( int i=0; i<N; i++ )
	{
		if( mode==0 )
			subd[i]->setColor( {0,0,0,0.5} );
		else
			subd[i]->setColor( {1,0.5,0} );
		subd[i]->setPolygonMode( mode==0?GL_LINE:GL_FILL );
	}
}


int main()
{
	/// Open window
	if( !openWindow3D( "A hole through a hole of a hole" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();

	showT = -ANIMATION_TIME;
	viewZ = 4;
	viewDistance=17;

	/// Add buttons
	butWF = new Button2D( "buttons/button_wireframe.png",'W',showWireframe,mode,3 );
	addButton( new  Button2D( "buttons/button_cut.png",'S',showIntersection ) );
	butNL = new  Button2D( "buttons/button_n.png",GLFW_KEY_SPACE,nextLevel,level,6 );
	addButton( new exitButton2D );

	for( int i=0; i<N; i++ )
	{
		subd[i] = new Subdiv3D();
		subd[i]->setColor( {1,0.5,0} );
		subd[i]->setShininess( 100 );
		subd[i]->setLevel( level );
		subd[i]->setPolygonMode( GL_FILL );
		subd[i]->setCenter( {0,0,4.5} );
		subd[i]->setSize( 0.4 );
	}

	subd[1]->setVertices( b2, {c2,d1,d2,d3,b3},a2, {a3,i3,i2},a1, {i1,i4,a4},b1, {b4,e4,e1,e2,c1} );
	subd[2]->setVertices( b3, {d3,d4,c3,c4,b4},a3, {a4,i4,i3},a2, {i2,i1,a1},b2, {b1,c1,c2,d1,d2} );
	subd[3]->setVertices( b4, {c4,e3,e4,e1,b1},a4, {a1,i1,i4},a3, {i3,i2,a2},b3, {b2,d2,d3,d4,c3} );
	subd[4]->setVertices( b1, {e1,e2,c1,c2,b2},a1, {a2,i2,i1},a4, {i4,i3,a3},b4, {b3,c3,c4,e3,e4} );
	subd[5]->setVertices( c2, {c3,d4,d1},b2, {d2,d3,b3,a3,a2},b1, {a1,a4,b4,e4,e1},c1, {e2,e3,c4} );
	subd[6]->setVertices( c4, {c1,e2,e3},b4, {e4,e1,b1,a1,a4},b3, {a3,a2,b2,d2,d3},c3, {d4,d1,c2} );
	subd[7]->setVertices( d2, {g2,g3,d3},b2, {b3,a3,a2,a1,b1},c2, {c1,c4,c3},d1, {d4,f3,f2} );
	subd[8]->setVertices( d3, {g3,f3,d4},b3, {c3,c4,b4,a4,a3},b2, {a2,a1,b1,c1,c2},d2, {d1,f2,g2} );
	subd[9]->setVertices( d4, {f3,f2,d1},c3, {c2,c1,c4},b3, {b4,a4,a3,a2,b2},d3, {d2,g2,g3} );
	subd[10]->setVertices( d1, {f2,g2,d2},c2, {b2,b1,c1},c3, {c4,b4,b3},d4, {d3,g3,f3} );
	subd[11]->setVertices( e2, {f1,f4,e3},c1, {c4,c3,c2},b1, {b2,a2,a1,a4,b4},e1, {e4,g4,g1} );
	subd[12]->setVertices( e3, {f4,g4,e4},c4, {b4,b3,c3},c1, {c2,b2,b1},e2, {e1,g1,f1} );
	subd[13]->setVertices( e4, {g4,g1,e1},b4, {b1,a1,a4,a3,b3},c4, {c3,c2,c1},e3, {e2,f1,f4} );
	subd[14]->setVertices( e1, {g1,f1,e2},b1, {c1,c2,b2,a2,a1},b4, {a4,a3,b3,c3,c4},e4, {e3,f4,g4} );
	subd[15]->setVertices( c3, {b3,d3,d4},c2, {d1,d2,b2},c1, {b1,e1,e2},c4, {e3,e4,b4} );
	subd[16]->setVertices( f4, {g4,e4,e3},f1, {e2,e1,g1},f2, {g2,d2,d1},f3, {d4,d3,g3} );
	subd[17]->setVertices( f1, {f2,f3,f4},e2, {e3,c4,c1},e1, {b1,b4,e4},g1, {g4,h4,h1,h2,g2} );
	subd[18]->setVertices( f4, {f3,g3,g4},e3, {e4,b4,c4},e2, {c1,b1,e1},f1, {g1,g2,f2} );
	subd[19]->setVertices( g4, {g3,h3,h4,h1,g1},e4, {e1,b1,b4},e3, {c4,c1,e2},f4, {f1,f2,f3} );
	subd[20]->setVertices( g1, {h1,h2,g2,f2,f1},e1, {e2,c1,b1},e4, {b4,c4,e3},g4, {f4,f3,g3,h3,h4} );
	subd[21]->setVertices( g2, {g1,h1,h2,h3,g3},d2, {d3,b3,b2},d1, {c2,c3,d4},f2, {f3,f4,f1} );
	subd[22]->setVertices( g3, {h3,h4,g4,f4,f3},d3, {d4,c3,b3},d2, {b2,c2,d1},g2, {f2,f1,g1,h1,h2} );
	subd[23]->setVertices( f3, {f4,f1,f2},d4, {d1,c2,c3},d3, {b3,b2,d2},g3, {g2,h2,h3,h4,g4} );
	subd[24]->setVertices( f2, {f1,g1,g2},d1, {d2,b2,c2},d4, {c3,b3,d3},f3, {g3,g4,f4} );
	subd[25]->setVertices( g2, {h2,h3,g3,d3,d2},f2, {d1,d4,f3},f1, {f4,e3,e2},g1, {e1,e4,g4,h4,h1} );
	subd[26]->setVertices( g4, {h4,h1,g1,e1,e4},f4, {e3,e2,f1},f3, {f2,d1,d4},g3, {d3,d2,g2,h2,h3} );
	subd[27]->setVertices( h2, {j2,j3,h3},g2, {g3,d3,d2,d1,f2},g1, {f1,e2,e1,e4,g4},h1, {h4,j4,j1} );
	subd[28]->setVertices( h3, {j3,j4,h4},g3, {g4,f4,f3,d4,d3},g2, {d2,d1,f2,f1,g1},h2, {h1,j1,j2} );
	subd[29]->setVertices( h4, {j4,j1,h1},g4, {g1,e1,e4,e3,f4},g3, {f3,d4,d3,d2,g2},h3, {h2,j2,j3} );
	subd[30]->setVertices( h1, {j1,j2,h2},g1, {g2,f2,f1,e2,e1},g4, {e4,e3,f4,f3,g3},h4, {h3,j3,j4} );

	subd[31]->setVertices( j2, {k3,k2,i2,i3,j3},h2, {h3,g3,g2},h1, {g1,g4,h4},j1, {j4,i4,i1,k1,k4} );
	subd[32]->setVertices( j3, {i3,l2,l3,l4,j4},h3, {h4,g4,g3},h2, {g2,g1,h1},j2, {j1,k4,k3,k2,i2} );
	subd[33]->setVertices( j4, {l4,l1,i4,i1,j1},h4, {h1,g1,g4},h3, {g3,g2,h2},j3, {j2,i2,i3,l2,l3} );
	subd[34]->setVertices( j1, {i1,k1,k4,k3,j2},h1, {h2,g2,g1},h4, {g4,g3,h3},j4, {j3,l3,l4,l1,i4} );
	subd[35]->setVertices( i1, {k1,k4,j1,j4,i4},a1, {a4,b4,b1},a2, {b2,b3,a3},i2, {i3,j3,j2,k3,k2} );
	subd[36]->setVertices( i2, {j2,k3,k2,k1,i1},a2, {a1,b1,b2},a3, {b3,b4,a4},i3, {i4,l1,l2,l3,j3} );
	subd[37]->setVertices( i3, {l2,l3,j3,j2,i2},a3, {a2,b2,b3},a4, {b4,b1,a1},i4, {i1,j1,j4,l4,l1} );
	subd[38]->setVertices( i4, {j4,l4,l1,l2,i3},a4, {a3,b3,b4},a1, {b1,b2,a2},i1, {i2,k2,k1,k4,j1} );
	subd[39]->setVertices( i2, {k2,k1,i1,a1,a2},i3, {a3,a4,i4,l1,l2},j3, {l3,l4,j4,h4,h3},j2, {h2,h1,j1,k4,k3} );
	subd[40]->setVertices( i4, {l1,l2,i3,a3,a4},i1, {a1,a2,i2,k2,k1},j1, {k4,k3,j2,h2,h1},j4, {h4,h3,j3,l3,l4} );

	subd[41]->setVertices( k1, {l1,l4,k4},i1, {j1,j4,i4,a4,a1},i2, {a2,a3,i3,j3,j2},k2, {k3,l3,l2} );
	subd[42]->setVertices( k4, {l4,l3,k3},j1, {j2,h2,h1,h4,j4},i1, {i4,a4,a1,a2,i2},k1, {k2,l2,l1} );
	subd[43]->setVertices( k3, {l3,l2,k2},j2, {i2,i3,j3,h3,h2},j1, {h1,h4,j4,i4,i1},k4, {k1,l1,l4} );
	subd[44]->setVertices( k2, {l2,l1,k1},i2, {i1,a1,a2,a3,i3},j2, {j3,h3,h2,h1,j1},k3, {k4,l4,l3} );
	subd[45]->setVertices( l4, {j4,j3,l3},k4, {k3,j2,j1},k1, {i1,i2,k2},l1, {l2,i3,i4} );
	subd[46]->setVertices( l3, {j3,i3,l2},k3, {k2,i2,j2},k4, {j1,i1,k1},l4, {l1,i4,j4} );
	subd[47]->setVertices( l2, {i3,i4,l1},k2, {k1,i1,i2},k3, {j2,j1,k4},l3, {l4,j4,j3} );
	subd[48]->setVertices( l1, {i4,j4,l4},k1, {k4,j1,i1},k2, {i2,j2,k3},l2, {l3,j3,i3} );
	subd[49]->setVertices( i4, {a4,a1,i1,j1,j4},l1, {l4,k4,k1},l2, {k2,k3,l3},i3, {j3,j2,i2,a2,a3} );
	subd[50]->setVertices( i3, {i2,a2,a3,a4,i4},l2, {l1,k1,k2},l3, {k3,k4,l4},j3, {j4,h4,h3,h2,j2} );
	subd[51]->setVertices( j3, {h3,h2,j2,i2,i3},l3, {l2,k2,k3},l4, {k4,k1,l1},j4, {i4,i1,j1,h1,h4} );
	subd[0]->setVertices( j4, {j1,h1,h4,h3,j3},l4, {l3,k3,k4},l1, {k1,k2,l2},i4, {i3,a3,a4,a1,i1} );

	/// Main loop
	while( isRunning() )
	{
		if( t-showT<ANIMATION_TIME )
		{
			double k = cos( ( ( t-showT )/ANIMATION_TIME )*M_PI/2 );

			viewClip  = viewClip*k+showClip*( 1-k );
		}
	}

	return 0;
}

