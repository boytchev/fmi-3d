
/// Project:	FMI 3D (2013)
/// File:		13151 Cosine ease
/// Topic:		13 Physics
/// Slide:		15
///	About:		Cosine ease-in and ease-out
///
/// History:
///		2013.07.28/JB	Initial implementation
///		2013.08.15/PB	Finetuning

#include "fmi3d.h"

Sphere3D* point;
Circle2D* circ;

double r=5;

void drawScene()
{
    point->drawImage();

	glEnable(GL_LINE_WIDTH);
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(2);
    circ->drawImage();
}

void drawFrames()
{
}

int main()
{
	if( !openWindow3D( "Cosine ease" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();
	//noGround();

	addButton( new exitButton2D() );

    circ = new Circle2D(r, QUALITY_HIGH );
    circ->setColor( {0,0,0} );
    circ->setShininess(100);
    circ->setCenter({0,0,2});
    circ->setPolygonMode( GL_LINE );

    point = new Sphere3D(0.8, QUALITY_HIGH);
	point->setColor( {1,0,0} );
	point->setCenter( {r,0,2} );
    point->setShininess(50);

	isRunning();
	double animT = random(3,5);
	double timeT = t;
	double speed = random(35,150);
	double last=0, ofs = 0, dir=1;
	while( isRunning() )
	{
		if( t-timeT<animT )
		{
			double k = 1-cos( ( ( t-timeT )/animT )*M_PI );
			point->identity();
			point->setCenter( {r,0,2} );
			point->rotateGlobal(ofs+speed*k,{0,0,1});
			last = speed*k;
		}
		else if( t-timeT>animT+0.5 )
		{
			ofs += last;
			animT = random(3,10);
			timeT = t;
			dir = -dir;
			speed = random(50,300)*dir;
		}
	}
	return 0;
}

