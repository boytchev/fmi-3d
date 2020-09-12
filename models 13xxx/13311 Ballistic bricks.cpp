
/// Project:	FMI 3D (2013)
/// File:		13311 Ballistic bricks.cpp
/// Topic:		13 Physics
/// Slide:		39
///	About:		Bricks get thrown into a sandbox.
///
/// History:
///		2013.08.24/KK	Initial implementation
///		2013.08.24/PB	Finetuning
///     2013.09.11/PB   Reduced number of bricks


#include "fmi3d.h"

#define MAX_BRICKS 30

vect_t gravity = {0, 0, -0.002};
const vect_t scale = {1, 1.5, 0.5};
GLuint sandTex,woodTex;
double dt;

struct Brick
{
	enum State
	{
		idle,
		preparing,
		flying,
		sinking
	}       state;

	Cube3D* brick;
	vect_t  velocity;
	vect_t  position;
	vect_t  rotation;
	double  angle;

	Brick()
	{
		brick = new Cube3D( 0.5 );
		brick->setColor( {random(0.4,0.8), random(0.1,0.3), 0} );
		brick->setTexture( sandTex );
		brick->setTextureSize( 0.2 );
		state = idle;
	}
//	~Brick()
//	{
//		if( brick )
//			delete brick;
//		brick = NULL;
//	}

	void    Throw()
	{
		state = preparing;
	}

	void    Update()
	{
		switch( state )
		{
			case idle:
				{
					// wait for it.
					break;;//
				}
			case preparing:
				{
					position = {0, 0, 0};
					double a = random(0.3,M_PI/2-0.3);
					double b = M_PI/2-random(0.1,0.8);
					double r = 0.14;//random(0.03,0.05);
					velocity = {r*cos(a)*cos(b), r*sin(a)*cos(b), r*sin(b)};
					rotation = {random( -10,10 ), random( -10,10 ), random( -10,10 )};
					angle = 0;
					state = flying;
					break;
				}
			case flying:
				{
					brick->identity();
					brick->scaleGlobal( scale );
					brick->rotateGlobal( angle*dt,rotation );
					brick->translateGlobal( position );

					position = position.add( velocity.mul(dt) );
					velocity = velocity.add( gravity.mul(dt) );
					angle += 5;

					if( position.z <= 0 )
						state = sinking;

					break;
				}
			case sinking:
				{
					brick->identity();
					brick->scaleGlobal( scale );
					brick->rotateGlobal( angle,rotation );
					brick->translateGlobal( position );

					position = position.add( gravity.mul(dt) );

					if( position.z <= -0.5 )
						state = preparing;

					break;
				}
		}
	}
};

struct Sandbox
{
	Cylinder3D* center;
	Cube3D*     wall1;
	Cube3D*     wall2;
	Cube3D*     ground;

	Sandbox()
	{
		center = new Cylinder3D( 1, 1.2 );
		center->setColor( {0.5, 0.5, 0.5} );

		wall1 = new Cube3D( 1 );
		wall1->scaleGlobal( {10,1,1} );
		wall1->translateGlobal( {5, 0, 0.5} );
		wall1->setTexture(woodTex);

		wall2 = new Cube3D( 1 );
		wall2->scaleGlobal( {10,1,1} );
		wall2->translateGlobal( {5, 0, 0.5} );
		wall2->rotateGlobal( 90,{0,0,1} );
		wall2->setTexture(woodTex);

		ground = new Cube3D( 1 );
		ground->scaleGlobal( {10,10,0.1} );
		ground->translateGlobal( {5, 5, 0} );
		ground->setLighting(false);
		ground->setTexture(sandTex);
		ground->setTextureSize(1);
	}
//	~Sandbox()
//	{
//		delete center;
//		delete wall1;
//		delete wall2;
//		delete ground;
//	}
	void drawImage()
	{
		center->drawImage();
		wall1->drawImage();
		wall2->drawImage();
		ground->drawImage();
	}
};

Brick*      bricks[MAX_BRICKS];
Sandbox*    sandbox;

void drawScene()
{
	sandbox->drawImage();

	for( int i = 0; i < MAX_BRICKS; i++ )
		bricks[i]->brick->drawImage();
}

void drawFrames()
{
	for( int i = 0; i < MAX_BRICKS; i++ )
		bricks[i]->brick->drawFrame();
}

int main()
{
	if( !openWindow3D( "Ballistic bricks" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();

	addButton( new exitButton2D() );

	sandTex = loadTexture("13311 Texture sand.jpg");
	woodTex = loadTexture("textures/Material_Wood.jpg");
	sandbox = new Sandbox();

	for( int i = 0; i < MAX_BRICKS; i++ )
		bricks[i] = new Brick();

	int time = 0;
	viewDistance = 15;
	viewX = 5;
	viewY = 5;
	viewBeta = 0.1;

	isRunning();
	double t0 = t;
	dt = 0;
	while( isRunning() )
	{
		dt = dt*0.9 + 0.1*(70*(t-t0));
		t0 = t;

		viewAlpha = M_PI/2*sin(t/5)+M_PI/4;

		for( int i = 0; i < MAX_BRICKS; i++ )
			bricks[i]->Update();

		for( int i = 0; i < MAX_BRICKS; i++ )
		{
			if( time == i * 30 )
				bricks[i]->Throw();
		}

		time++;
	}

//	delete sandbox;
//
//	for( int i = 0; i < MAX_BRICKS; i++ )
//		delete bricks[i];

	return 0;
}

