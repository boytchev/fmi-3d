
/// Project:	FMI 3D (2013)
/// File:		15181 Platonic solids.cpp
/// Topic:		15 Convex hulls
/// Slide:		18
///	About:		Platonic solids
///
/// History:
///		2013.08.18/PB	Initial implementation


#include "fmi3d.h"
#include <vector>

#define ANIMATION_TIME 1
#define N 5

int mode = 0;
int mode2 = -1;
int modes = 5;

class Platonic3D : public Object3D
{
	public:
		Platonic3D( double sc, vector<double>v, vector<int>e );
	private:
};

oxyz_t fixOxyz( oxyz_t oxyz )
{
	oxyz.oz = oxyz.ox.vmul( oxyz.oy );
	oxyz.oy = oxyz.oz.vmul( oxyz.ox );

	oxyz.ox = oxyz.ox.mul( 1/oxyz.ox.distance() );
	oxyz.oy = oxyz.oy.mul( 1/oxyz.oy.distance() );
	oxyz.oz = oxyz.oz.mul( 1/oxyz.oz.distance() );
	oxyz.o.w = 1;
	return oxyz;
}

Platonic3D::Platonic3D( double sc, vector<double>v, vector<int>e ) : Object3D()
{
	setShininess( 50 );
	setSize( 5 );
	setCenter( {0,0,4} );

	glNewList( frameList,GL_COMPILE );
	glEndList();

	int n = v.size()/3;
	glNewList( imageList,GL_COMPILE );
	glColor3f( 1,0.5,0 );
	for( int i=0; i<n; i++ )
	{
		glPushMatrix();
		glTranslated( v[i]*sc,v[i+n]*sc,v[i+2*n]*sc );
		glScaled( 0.1,0.1,0.1 );
		glCallList( idUnitSphere3D[QUALITY_MEDIUM] );
		glPopMatrix();
	}

	int m = e.size();
	glColor3f( 0,0,0.4 );
	for( int i=0; i<m-1; i+=2 )
	{
		int e1 = e[i]-1;
		int e2 = e[i+1]-1;
		vect_t v1 = {v[e1],v[e1+n],v[e1+2*n]};
		vect_t v2 = {v[e2],v[e2+n],v[e2+2*n]};
		v1 = v1.mul( sc );
		v2 = v2.mul( sc );

		oxyz_t oxyz;
		oxyz.oz = v2.sub( v1 );
		oxyz.rebuild();
		oxyz.o = v1;
		oxyz.o.w = 1;
		oxyz.ox = oxyz.ox.mul( 0.03 );
		oxyz.oy = oxyz.oy.mul( 0.03 );
		oxyz.oz = oxyz.oz.mul( v2.distance( v1 ) );


		glPushMatrix();
		glMultMatrixd( ( GLdouble* )&oxyz );
		glCallList( idUnitCylinder3D[QUALITY_LOW] );
		glPopMatrix();
	}
	glEndList();
}


Platonic3D *p[N];
Font3D *fnt;
Text3D *txt;

void drawScene()
{
	txt->drawImage();
	p[mode]->drawImage();
	if( mode2>=0 ) p[mode2]->drawImage();
}


void drawFrames()
{
}

double modeT = -10;
Button2D *mB;
void changeMode()
{
	mode2 = (mode+1)%modes;
	modeT = t;
}

int main()
{
	/// Open window
	if( !openWindow3D( "Platonic solids" ) ) return 1;

	/// Add buttons
	mB = new Button2D( "buttons/button_next.png",GLFW_KEY_SPACE,changeMode,mode,modes );
	addButton( new exitButton2D );

	viewZ = 4;
	viewBeta = 0.1;
	viewDistance = 15;
	viewAlpha = -M_PI/2;

	fnt = new Font3D("Arial",true,false,0.1);
	txt = new Text3D(L"      Тетраедър",fnt);
	txt->setColor({0.25,0.25,1});
	txt->setScale({2,2,2});
	txt->setCenter({-8,0,8});
	txt->rotateLocal(90,{1,0,0});

	p[0] = new Platonic3D( 1,
	{
		0.0, -0.5, 0.5, 0.0, 0.5774, -0.2887, -0.2887, 0.0,	-0.2041, -0.2041,
		-0.2041, 0.6124
	}, {1, 2, 1, 3, 1, 4, 2, 3, 2, 4, 3, 4} );

	p[1] = new Platonic3D( 0.7,
	{
		0.5, -0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, 0.5, -0.5, -0.5, 0.5,
		0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, 0.5, 0.5, 0.5, 0.5
	}, {1, 2, 2, 3, 3, 4, 4, 1, 5, 6, 6, 7, 7, 8, 8, 5, 1, 5, 2, 6, 3, 7, 4, 8} );

	p[2] = new Platonic3D( 0.9,
	{
		0.5, -0.5, -0.5, 0.5, 0.0, 0.0, 0.5, 0.5, -0.5, -0.5, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.7071,  -0.7071
	}, {6, 1, 6, 2, 6, 3, 6, 4, 5, 1, 5, 2, 5, 3, 5, 4, 1, 2, 2, 3, 3, 4, 4, 1} );

	p[3] = new Platonic3D( 0.5,
	{
		0.0, 0.8090, 0.5, -0.5, -0.8090, 0.0, 1.3095, 0.8093, -0.8093, -1.3095,
		0.8093, 1.3095, 0.0, -1.3095, -0.8093, 0.5, 0.8090, 0.0, -0.8090, -0.5,
		0.8507, 0.2629, -0.6882, -0.6882, 0.2629, 1.3769, 0.4255, -1.1139,
		-1.1139, 0.4255, 1.1139, -0.4255, -1.3769, -0.4255, 1.1139, 0.6882,
		-0.2629, -0.8507, -0.2629, 0.6882, 1.1135, 1.1135, 1.1135, 1.1135,
		1.1135, 0.2065, 0.2065, 0.2065, 0.2065, 0.2065, -0.2065, -0.2065,
		-0.2065, -0.2065, -0.2065, -1.1135, -1.1135, -1.1135, -1.1135, -1.1135
	},
	{
		17, 18, 18, 19, 19, 20, 20, 16, 16, 17, 17, 12, 12, 8, 8, 13, 13, 18,
		19, 14, 14, 9, 9, 13, 1, 2, 2, 3, 3, 4, 4, 5, 5, 1, 1, 6, 6, 15, 15, 10,
		10, 5, 20, 15, 10, 14, 9, 4, 3, 8, 2, 7, 7, 11, 7, 12, 11, 6, 11, 16
	} );

	p[4] = new Platonic3D( 0.75,
	{
		0.0, 0.0, 0.8090, 0.5, -0.5, -0.809, 0.5, 0.8090, 0.0, -0.8090, -0.5,
		0.0, 0.0, 0.8507, 0.2629, -0.6882, -0.6882, 0.2629, 0.6882, -0.2626,
		-0.8507, -0.2629, 0.6882, 0.0, 0.9512, 0.4253, 0.4253, 0.4253, 0.4253,
		0.4253, -0.4253, -0.4253, -0.4253, -0.4253, -0.4253, -0.9512
	},
	{
		12, 10, 10, 11, 11, 12, 8, 3, 3, 4, 4, 8, 6, 2, 2, 11, 11, 6, 2, 1, 1,
		6, 4, 1, 1, 3, 2, 7, 7, 3, 12, 7, 9, 10, 9, 8, 9, 5, 12, 9, 12, 8, 1, 5,
		5, 4, 5, 6, 4, 9, 2, 3, 6, 10, 7, 8, 7, 11, 10, 5
	} );

	/// Main loop
	double t0=t;
	while( isRunning() )
	{
		double dt = 30*( t-t0 );
		t0=t;

		viewAlpha += 0.003*sin(t);

		if( mode2>=0 )
		{
			if( t-modeT>ANIMATION_TIME/2.0 )
			{
				switch( mode2 )
				{
				case 0: txt->setText(L"      Тетраедър"); break;
				case 1: txt->setText(L"   Куб (Хексаедър)"); break;
				case 2: txt->setText(L"       Октаедър"); break;
				case 3: txt->setText(L"     Додекаедър"); break;
				case 4: txt->setText(L"      Икосаедър"); break;
				}
			}

			if( t-modeT<ANIMATION_TIME )
			{
				double k = 0.5+0.5*cos( ( ( t-modeT )/ANIMATION_TIME )*M_PI );
				p[mode]->oxyz.o.z = -6+10*k;
				k = 0.5-0.5*cos( ( ( t-modeT )/ANIMATION_TIME )*M_PI );
				p[mode2]->oxyz.o.z = -6+10*k;
			}
			else
			{
				mode = mode2;
				mB->setState(mode);
				mode2 = -1;
			}
		}

		for( int i=0; i<N; i++ )
		{
			p[i]->rotateLocal( dt, {1,1,0} );
			p[i]->rotateLocal( 0.5*dt, {0,1,1} );
			p[i]->rotateLocal( 2*dt, {0,0,1} );
		}
	}
	return 0;
}

