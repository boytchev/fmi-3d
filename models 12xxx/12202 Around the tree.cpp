
/// Project:	FMI 3D (2013)
/// File:		12202 Around the tree
/// Topic:		12 Trajectories
/// Slide:		20
///	About:		Circular motion around a circle
///
/// History:
///		2013.08.23/JB	Initial implementation
///     2013.09.01/PB   Rewritten
///     2013.09.13/PB   Changed colours


#include "fmi3d.h"

#define ANIMATION_TIME 15

Sphere3D *boy,*girl,*fly1;
Cone3D *tree[3];
Font3D *fnt;
Text3D *boy1,*boy2;
Text3D *girl1,*girl2;

void drawScene()
{
	for(int i=0; i<3; i++) tree[i]->drawImage();
	boy1->drawImage();
	boy2->drawImage();
	girl1->drawImage();
	girl2->drawImage();
	fly1->drawImage();
}


void drawFrames()
{
}


double startT = -5*ANIMATION_TIME;
void start()
{
	startT = t;
}

int main()
{
	if( !openWindow3D( "Around the tree" ) ) return 1;
	changeGround();

	addButton( new Button2D("buttons/button_next.png",GLFW_KEY_SPACE,start) );
	addButton( new exitButton2D() );

	viewDistance = 20*2;
	viewZ = 4;

	GLuint treeTex = loadTexture("12202 Texture wood.jpg");
	for(int i=0; i<3; i++)
	{
		tree[i] = new Cone3D( 8+0.5*i, 60-5*i, false );
		tree[i]->setTexture(treeTex);
		tree[i]->setTextureScale({5,5,5,10.0-3*i});
		tree[i]->setCenter({0,0,0});
	}

	boy = new Sphere3D( 1 );
	boy->setColor({0,0,1});
	boy->setShininess(50);

	fnt = new Font3D("Arial");

	boy1 = new Text3D(L"Δ",fnt);
	boy1->setScale({9,9,4});
	boy1->setColor({0,0,1});
	boy2 = new Text3D(L"°",fnt);
	boy2->setScale({11,11,2});
	boy2->setColor({0,0,1});

	girl1 = new Text3D(L"Δ",fnt);
	girl1->setScale({9,9,4});
	girl1->setColor({1,0,0});
	girl2 = new Text3D(L"°",fnt);
	girl2->setScale({11,11,2});
	girl2->setColor({1,0,0});

	fly1 = new Sphere3D(0.4,QUALITY_MEDIUM);
	fly1->setColor({0,0,0});
	fly1->setShininess(50);

	isRunning();
	double boyA = 0;
	double girlA = 0;
	double flyA = 0;
	while( isRunning() )
	{
		double k;

		if( t-startT<ANIMATION_TIME )
		{
			k = 0.5+0.5*cos( ( ( t-startT )/ANIMATION_TIME )*M_PI );
		}
		else if( t-startT<2*ANIMATION_TIME )
		{
			k = 0;
		}
		else if( t-startT<2*ANIMATION_TIME+0.5 )
		{
			k = 0.5-0.5*cos( ( ( t-startT-2*ANIMATION_TIME )/0.5 )*M_PI );
		}
		else k=1;
		boyA = 170*k;
		girlA = -170*k;

		boy1->identity();
		boy1->translateLocal({0.25,14,6});
		boy1->rotateLocal(-90,{1,0,0});
		boy1->rotateLocal(90,{0,1,0});
		boy1->rotateGlobal(boyA,{0,0,1});

		boy2->identity();
		boy2->translateLocal({0,5.5,9.4});
		boy2->rotateLocal(90,{0,1,0});
		boy2->rotateGlobal(boyA,{0,0,1});

		girl1->identity();
		girl1->translateLocal({0.3+1,8.8,0.2});
		girl1->rotateLocal(90,{1,0,0});
		girl1->rotateLocal(90,{0,1,0});
		girl1->rotateGlobal(girlA,{0,0,1});

		girl2->identity();
		girl2->translateLocal({1,5.5,9.4});
		girl2->rotateLocal(90,{0,1,0});
		girl2->rotateGlobal(girlA,{0,0,1});

		double j = 0.5+0.5*sin(2*(t-startT)/(0.5+(boyA-girlA)/360) );
		flyA = j*boyA+(1-j)*(girlA);
		if( k==1 ) flyA=0;
		fly1->identity();
		fly1->translateLocal({0,11.4,7.4});
		fly1->rotateGlobal(flyA-1,{0,0,1});
		if( k==1 )
		{
			fly1->setCenter(fly1->getCenter().add({1*sin(23*t),1*cos(27*t),1*sin(21*t)}));
		}
	}
	return 0;
}

