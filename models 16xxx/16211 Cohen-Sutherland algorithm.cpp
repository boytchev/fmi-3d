
/// Project:	FMI 3D (2013)
/// File:		16211 Cohen-Sutherland algorithm.cpp
/// Topic:		16 Clipping
/// Slide:		21
///	About:		Cohen-Sutherland algorithm
///
/// History:
///		2013.08.21/PB	Initial implementation


#include "fmi3d.h"

#define N 20
Object3D *p1,*p2;
int minX=-5, maxX=5, minY=-3, maxY=3;
Font3D *fnt1;
Font3D *fnt2;
Text3D *txt[N];
int tag1,tag2;
vect_t col;
int show = 0;

void drawScene()
{
	// Draw interactive point
	p1->drawImage();
	p2->drawImage();

	glEnable( GL_LINE_SMOOTH );
	glDisable( GL_LIGHTING );
	glDisable( GL_TEXTURE_2D );
	glLineWidth( 0.5 );
	glDepthMask( GL_FALSE );
	for( int i=0; i<N; i++ ) txt[i]->drawImage();
	glDepthMask( GL_TRUE );

	// Draw lines
	glEnable( GL_LINE_STIPPLE );
	glLineStipple( 5,0b1010101010101010 );
	glLineWidth( 1 );
	glColor3f( 0,0,0 );
	glBegin( GL_LINES );
	glVertex2d( minX,-10000 );
	glVertex2d( minX,+10000 );
	glVertex2d( maxX,-10000 );
	glVertex2d( maxX,+10000 );
	glVertex2d( -10000,minY );
	glVertex2d( +10000,minY );
	glVertex2d( -10000,maxY );
	glVertex2d( +10000,maxY );
	glEnd();
	glDisable( GL_LINE_STIPPLE );

	// Draw polygon edges
	glLineWidth( 3 );
	glColor3f( 0,0,0 );
	glBegin( GL_LINE_LOOP );
	glVertex3d( minX,minY,0.2 );
	glVertex3d( minX,maxY,0.2 );
	glVertex3d( maxX,maxY,0.2 );
	glVertex3d( maxX,minY,0.2 );
	glEnd();

	// Draw polygon face
	glPolygonMode( GL_FRONT_AND_BACK,GL_FILL );
	glColor4f( 0.5,0.5,1,0.25 );
	glBegin( GL_POLYGON );
	glVertex2d( minX,minY );
	glVertex2d( minX,maxY );
	glVertex2d( maxX,maxY );
	glVertex2d( maxX,minY );
	glEnd();

	// Draw segment
	glColor3f( col.x,col.y,col.z );
	glLineWidth( 10 );
	glBegin( GL_LINES );
	p1->getCenter().sub( {0,0,0.5} ).vertex();
	p2->getCenter().sub( {0,0,0.5} ).vertex();
	glEnd();
}


void drawFrames()
{
	p1->drawFrame();
	p2->drawFrame();
}


Button2D *sB;
void hideShow()
{
	show = (show+1)%3;
	sB->setState(show);
}


int main()
{
	/// Open window
	if( !openWindow2D( "Cohen-Sutherland algorithm" ) ) return 1;
	noGround();

	/// Add buttons
	sB = new Button2D( "buttons/button_show.png",GLFW_KEY_SPACE,hideShow,0,3 );
	addButton( new exitButton2D );

	p1 = interactivePoint2D( random( -8,8 ),random( -5,5 ) );
	p2 = interactivePoint2D( random( -8,8 ),random( -5,5 ) );
	p1->setLighting( false );
	p2->setLighting( false );

	fnt1 = new Font3D( "Verdana" );
	fnt2 = new Font3D( "Verdana",true );
	for( int i=0; i<8; i++ )
	{
		txt[i] = new Text3D( L"",fnt1 );
		txt[i]->setLighting( false );
		txt[i]->setColor( {0,0,0} );
		txt[i]->setScale( {0.4,0.3,0} );
		txt[i]->setPolygonMode( GL_LINE );
	}
	for( int i=8; i<N; i++ )
	{
		txt[i] = new Text3D( L"",fnt2 );
		txt[i]->setLighting( false );
		txt[i]->setColor( {0.5,0.5,1} );
		txt[i]->setScale( {1.6,1.2,0} );
		txt[i]->setPolygonMode( GL_FILL );
	}

	txt[0]->setText( L"---0" ); txt[0]->setCenter( {-0.2,minY+0.15} );
	txt[1]->setText( L"---1" ); txt[1]->setCenter( {-0.2,minY-0.3} );
	txt[2]->setText( L"--0-" ); txt[2]->setCenter( {-0.2,maxY-0.3} );
	txt[3]->setText( L"--1-" ); txt[3]->setCenter( {-0.2,maxY+0.15} );
	txt[4]->setText( L"-0--" ); txt[4]->setCenter( {minX+0.15,0.7} );
	txt[5]->setText( L"-1--" ); txt[5]->setCenter( {minX-0.8,0.7} );
	txt[6]->setText( L"1---" ); txt[6]->setCenter( {maxX+0.15,0.7} );
	txt[7]->setText( L"0---" ); txt[7]->setCenter( {maxX-0.8,0.7} );
	txt[ 8]->setText( L"0100" ); txt[ 8]->setCenter( {-7.6-1.7,-0.3,2} ); txt[ 8]->setTag( 0b0100 );
	txt[ 9]->setText( L"0000" ); txt[ 9]->setCenter( {    -1.7,-0.3,2} ); txt[ 9]->setTag( 0b0000 );
	txt[10]->setText( L"1000" ); txt[10]->setCenter( {+7.3-1.7,-0.3,2} ); txt[10]->setTag( 0b1000 );
	txt[11]->setText( L"0110" ); txt[11]->setCenter( {-7.6-1.7,+4.3,2} ); txt[11]->setTag( 0b0110 );
	txt[12]->setText( L"0010" ); txt[12]->setCenter( {    -1.7,+4.3,2} ); txt[12]->setTag( 0b0010 );
	txt[13]->setText( L"1010" ); txt[13]->setCenter( {+7.3-1.7,+4.3,2} ); txt[13]->setTag( 0b1010 );
	txt[14]->setText( L"0101" ); txt[14]->setCenter( {-7.6-1.7,-5.0,2} ); txt[14]->setTag( 0b0101 );
	txt[15]->setText( L"0001" ); txt[15]->setCenter( {    -1.7,-5.0,2} ); txt[15]->setTag( 0b0001 );
	txt[16]->setText( L"1001" ); txt[16]->setCenter( {+7.3-1.7,-5.0,2} ); txt[16]->setTag( 0b1001 );
	txt[17]->setText( L"****" ); txt[17]->setCenter( {0,0,2} );
	txt[18]->setText( L"****" ); txt[18]->setCenter( {0,0,2} ); txt[18]->setScale( {1,1,0} );
	txt[19]->setText( L"*******" ); txt[19]->setCenter( {0,0,2} ); txt[19]->setScale( {0.5,0.5,0} );

	/// Main loop
	while( isRunning() )
	{
		double x1 = p1->getCenter().x;
		double x2 = p2->getCenter().x;
		double y1 = p1->getCenter().y;
		double y2 = p2->getCenter().y;
		bool midx1 = ( minX<=x1 && x1<=maxX );
		bool midx2 = ( minX<=x2 && x2<=maxX );
		bool midy1 = ( minY<=y1 && y1<=maxY );
		bool midy2 = ( minY<=y2 && y2<=maxY );
		bool minx1 = ( x1<minX );
		bool minx2 = ( x2<minX );
		bool miny1 = ( y1<minY );
		bool miny2 = ( y2<minY );
		bool maxx1 = ( x1>maxX );
		bool maxx2 = ( x2>maxX );
		bool maxy1 = ( y1>maxY );
		bool maxy2 = ( y2>maxY );
		txt[ 8]->setIsVisible( (minx1&&midy1) || (minx2&&midy2) );
		txt[ 9]->setIsVisible( (midx1&&midy1) || (midx2&&midy2) );
		txt[10]->setIsVisible( (maxx1&&midy1) || (maxx2&&midy2) );
		txt[11]->setIsVisible( (minx1&&maxy1) || (minx2&&maxy2) );
		txt[12]->setIsVisible( (midx1&&maxy1) || (midx2&&maxy2) );
		txt[13]->setIsVisible( (maxx1&&maxy1) || (maxx2&&maxy2) );
		txt[14]->setIsVisible( (minx1&&miny1) || (minx2&&miny2) );
		txt[15]->setIsVisible( (midx1&&miny1) || (midx2&&miny2) );
		txt[16]->setIsVisible( (maxx1&&miny1) || (maxx2&&miny2) );

		tag1=0; for( int i=8; i<=16; i++ ) if( txt[i]->getIsVisible() ) {tag1=txt[i]->getTag(); break;}
		tag2=0; for( int i=16; i>=8; i-- ) if( txt[i]->getIsVisible() ) {tag2=txt[i]->getTag(); break;}
		for( int i=8; i<N; i++ ) if( txt[i]->getIsVisible() ) txt[i]->setIsVisible(show!=1);
		for( int i=0; i<8; i++ ) txt[i]->setIsVisible(show!=1);

		if( ( tag1|tag2 ) == 0 )
			{ col= {0,0,0}; txt[18]->setText(L"видима"); txt[18]->setTag(17); }
		else if( ( tag1&tag2 ) != 0 )
			{ col= {0.5,0.5,0.5}; txt[18]->setText(L"невидима"); txt[18]->setTag(23); }
		else
			{ col= {1,0,0}; txt[18]->setText(L"отсичане"); txt[18]->setTag(21); }
		p1->setColor( col );
		p2->setColor( col );
		txt[19]->setColor( col );
		txt[18]->setColor( col );
		txt[17]->setColor( col );
		txt[17]->setCenter( p1->getCenter() );
		txt[17]->oxyz.ox = p2->getCenter().sub(p1->getCenter());
		if( txt[17]->oxyz.ox.x<0 )
		{
			txt[17]->setCenter( p2->getCenter() );
			txt[17]->oxyz.ox = p1->getCenter().sub(p2->getCenter());
		}
		txt[17]->oxyz.oy = txt[17]->oxyz.oz.vmul(txt[17]->oxyz.ox);
		txt[17]->oxyz.ox = txt[17]->oxyz.ox.mul(1/txt[17]->oxyz.ox.distance());
		txt[17]->oxyz.oy = txt[17]->oxyz.oy.mul(1/txt[17]->oxyz.oy.distance());
		txt[18]->oxyz = txt[17]->oxyz;
		txt[19]->oxyz = txt[17]->oxyz;
		txt[17]->translateLocal({p2->getCenter().sub(p1->getCenter()).distance()/2-1.85,0.25,0});
		txt[18]->translateLocal({p2->getCenter().sub(p1->getCenter()).distance()/2-txt[18]->getTag()/10.0,-0.75,0});
		txt[19]->translateLocal({p2->getCenter().sub(p1->getCenter()).distance()/2-0.6,-1.35,0});
		{
			wchar_t w[5] = {'0','0','0','0','\0'};
			for(int i=0,tag= (tag1&tag2) ; i<4; tag/=2,i++)
			{
				w[3-i]='0'+(tag%2);
			}
			txt[17]->setText(w);
		}
		{
			wchar_t w[5] = {'0','0','0','0','\0'};
			for(int i=0,tag= (tag1|tag2)^(tag1&tag2) ; i<4; tag/=2,i++)
			{
				w[3-i]='0'+(tag%2);
			}
			txt[19]->setText(w);
		}
		txt[17]->setIsVisible(show!=0);
		txt[18]->setIsVisible(show!=0);
		txt[19]->setIsVisible((tag1&tag2)==0 && (tag1|tag2)!=0 && show!=0);
	}
	return 0;
}

