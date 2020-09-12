
/// Project:	FMI 3D (2013)
/// File:		24141 Constructing Bezier surface.cpp
/// Topic:		24 Surfaces
/// Slide:		07
///	About:		Step-by-step constructiopn of a Bezier surface
///
/// History:
///		2013.05.29/PB	Initial implementation
///     2013.09.13/PB   Improved speed


#include "fmi3d.h"

#include "stdio.h"
#include "string.h"

//#include <iostream>


#define N 3
Object3D* P[N+1][N+1];	/// control points
GLfloat   S[N+1][N+1];
GLfloat   O[N+1][N+1];
Font3D*   font;
Text3D*	  T[N+1][N+1];

int step=0;
double k1=0;

double fact[N+1];		/// factorials
double nk[N+1][N+1];	/// (n k)

void setFactoriels()
{
	fact[0] = 1;
	for( int i=1; i<=N; i++ ) fact[i]=i*fact[i-1];
	for( int i=0; i<=N; i++ )
		for( int k=0; k<=i; k++ )
			nk[i][k] = fact[i]/fact[k]/fact[i-k];
}

double B( int n, int i, double u )
{
	return nk[n][i]*pow( u,i )*pow( 1-u,n-i );
}

/// Calculate point on a Bezier surface
vect_t Bp( double u, double v )
{
	vect_t r = {0,0,0};
	for( int i=0; i<=N; i++ )
		for( int j=0; j<=N; j++ )
		{
			double k=B( N,i,u )*B( N,j,v );
			r = r.add( P[i][j]->getCenter().mul( k ) );
		}
	return r;
}

/// Estimate normal vector to a point on a Bezier surface
vect_t Np( double u, double v )
{
	double e=0.001;
	vect_t dU = Bp( u-e,v ).sub( Bp( u+e,v ) );
	vect_t dV = Bp( u,v-e ).sub( Bp( u,v+e ) );
	return dU.vmul( dV );
}

void drawControlPoints()
{
	for( int i=0; i<=N; i++ )
		for( int j=0; j<=N; j++ )
		{
			P[i][j]->drawImage();
			if( step<3 ) T[i][j]->drawImage();
		}

	glDepthMask( GL_FALSE );
	glLineWidth( 0.5 );
	if( step<3 )
		for( int i=0; i<=N; i++ )
			for( int j=0; j<=N; j++ )
			{
				//T[i][j]->setPolygonMode( GL_LINE );
				T[i][j]->drawImage();
				//T[i][j]->setPolygonMode( GL_FILL );
			}
	glDepthMask( GL_TRUE );
}

void drawStraightSegments()
{
	glColor3f( 0,0,1 );
	glLineWidth( 3 );
	glDisable( GL_LIGHTING );
	glEnable( GL_LINE_SMOOTH );

	glBegin( GL_LINES );
	for( int i=0; i<=N; i++ )
		for( int j=0; j<=N; j++ )
		{
			if( i+1<=N )
			{
				P[i][j]->getCenter().vertex();
				P[i+1][j]->getCenter().vertex();
			}
			if( j+1<=N )
			{
				P[i][j]->getCenter().vertex();
				P[i][j+1]->getCenter().vertex();
			}
		}
	glEnd();
}

void drawBezierContours(vect_t col={0,0,1})
{
	glColor3f( col.x, col.y, col.z );
	glLineWidth( 3 );
	glDisable( GL_LIGHTING );
	glEnable( GL_LINE_SMOOTH );

	glDepthMask( GL_FALSE );

	double u;
	double v;
	double dS=0.05;

	glBegin( GL_LINE_STRIP );
	for( v=0; v<1; v=v+dS ) Bp( 0,v ).vertex();
	Bp( 0,1 ).vertex();
	glEnd();

	glBegin( GL_LINE_STRIP );
	for( v=0; v<1; v=v+dS ) Bp( 1,v ).vertex();
	Bp( 1,1 ).vertex();
	glEnd();

	glBegin( GL_LINE_STRIP );
	for( u=0; u<1; u=u+dS ) Bp( u,0 ).vertex();
	Bp( 1,0 ).vertex();
	glEnd();

	glBegin( GL_LINE_STRIP );
	for( u=0; u<1; u=u+dS ) Bp( u,1 ).vertex();
	Bp( 1,1 ).vertex();
	glEnd();

	glDepthMask( GL_TRUE );
}


#define M 15
void drawBezierPatches( bool normals )
{
	glColor3f( 0,0,1 );
	glEnable( GL_LIGHTING );

	glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, 3 );
	glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, whiteSpecular );
	glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, noEmission );

	vect_t Bp1[M+1][M+1];
	vect_t Np1[M+1][M+1];
	for( int i=0; i<=M; i++ )
	{
		for( int j=0; j<=M; j++ )
		{
			Bp1[i][j] = Bp( i/double( M ),j/double( M ) );
			Np1[i][j] = Np( i/double( M ),j/double( M ) );
		}
	}

	glBegin( GL_QUADS );
	for( int i=0; i<M; i++ )
	{
		for( int j=0; j<M; j++ )
		{
			vect_t norm;

			norm = Np1[i][j];
			glNormal3d( norm.x, norm.y, norm.z );
			Bp1[i][j].vertex();

			if( normals ) norm = Np1[i+1][j];
			glNormal3d( norm.x, norm.y, norm.z );
			Bp1[i+1][j].vertex();

			if( normals ) norm = Np1[i+1][j+1];
			glNormal3d( norm.x, norm.y, norm.z );
			Bp1[i+1][j+1].vertex();

			if( normals ) norm = Np1[i][j+1];
			glNormal3d( norm.x, norm.y, norm.z );
			Bp1[i][j+1].vertex();

		}
	}
	glEnd();

	glMaterialf( GL_FRONT_AND_BACK,GL_SHININESS,0.0 );
	glMaterialfv( GL_FRONT_AND_BACK,GL_SPECULAR, noSpecular );
	glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, defaultEmission );

}


void drawBezierIsolines()
{
	glColor3f( 0,0,1 );
	glLineWidth( 1 );
	glDisable( GL_LIGHTING );
	glEnable( GL_LINE_SMOOTH );

	glDepthMask( GL_FALSE );

	double u;
	double v;
	double dS=0.02;
	double dM=0.1;

	for( u=dM; u<1-dM/2; u=u+dM )
	{
		glBegin( GL_LINE_STRIP );
		for( v=0; v<1; v=v+dS ) Bp( u,v ).vertex();
		Bp( u,1 ).vertex();
		glEnd();
	}

	for( v=dM; v<1-dM/2; v=v+dM )
	{
		glBegin( GL_LINE_STRIP );
		for( u=0; u<1; u=u+dS ) Bp( u,v ).vertex();
		Bp( 1,v ).vertex();
		glEnd();
	}

	glDepthMask( GL_TRUE );
}

void drawScene()
{
	if( step<4 ) drawControlPoints();
	if( step<=2 ) drawStraightSegments();
	if( step==2 ) drawBezierContours({1,0,0});
	if( step==3 ) drawBezierContours();
	if( step==3 ) drawBezierIsolines();
	if( step==4 ) drawBezierPatches( false );
	if( step==5 ) drawBezierPatches( true );
}


void drawFrames()
{
	if( step<=2 ) drawStraightSegments();
	if( step==2 ) drawBezierContours({1,0,0});
	if( step==3 ) drawBezierContours();
	if( step==3 ) drawBezierIsolines();
}


double f( int i, int j )
{
	return sin( t*S[i][j]+O[i][j] );
}

Button2D* cS;
void changeStep()
{
	step = ( step+1 )%6;
	cS->setState(step);
	double mark = ((step>=2)&&(step<=3))?1:0;

	P[0][0]->setColor( {mark,0.5*mark,0} );
	P[0][N]->setColor( {mark,0.5*mark,0} );
	P[N][0]->setColor( {mark,0.5*mark,0} );
	P[N][N]->setColor( {mark,0.5*mark,0} );

	P[0][0]->setSize( 0.3+0.05*mark );
	P[0][N]->setSize( 0.3+0.05*mark );
	P[N][0]->setSize( 0.3+0.05*mark );
	P[N][N]->setSize( 0.3+0.05*mark );
}


int main()
{
	/// Open window
	if( !openWindow3D( "Construction Bezier surface" ) ) return 1;
	changeGround();
	changeGround();
	changeGround();

	/// Add buttons
	cS = new Button2D( "buttons/button_next.png",GLFW_KEY_SPACE,changeStep,step,6 );
	addButton( new exitButton2D );

	/// Create control points
	for( int i=0; i<=N; i++ )
		for( int j=0; j<=N; j++ )
		{
			P[i][j] = new Sphere3D( 0.3,QUALITY_LOW );
			P[i][j]->setColor( {0,0,0} );
			P[i][j]->setShininess( 10 );

			T[i][j] = new Text3D( L"", systemFont3D );
			T[i][j]->setColor( {1,0,0} );
			T[i][j]->setSize( 0.4 );

			char res8[100];
			wchar_t res[100];
			sprintf( res8,"%d,%d",i,j );
			int len=strlen( res8 );
			for( int k=0; k<len+1; k++ ) res[k]=res8[k];
			T[i][j]->setText( res );

			S[i][j] = random( 2,4 );
			O[i][j] = random( 0,2*M_PI );
		}

	setFactoriels();

	/// Main loop
	double t0=t;
	while( isRunning() )
	{
		viewAlpha += ( t-t0 )/10;
		t0 = t;

		if( step>0 )
			k1 = 1*0.05+k1*0.95;
		else
			k1 = 0*0.05+k1*0.95;

		for( int i=0; i<=N; i++ )
			for( int j=0; j<=N; j++ )
			{
				double k = 1;
				if( ( i>0 ) && ( i<N ) ) k=3;
				if( ( j>0 ) && ( j<N ) ) k=3;
				P[i][j]->setCenter( {5*( i-( N )/2.0 ),5*( j-( N )/2.0 ),4+k1*k*f( i,j )} );
			}

		for( int i=0; i<=N; i++ )
			for( int j=0; j<=N; j++ )
			{
				vect_t p = P[i][j]->getCenter();
				T[i][j]->identity();
				T[i][j]->rotateLocal( 90-viewBeta*180/M_PI, {1,0,0} );
				T[i][j]->rotateGlobal( 90+viewAlpha*180/M_PI, {0,0,1} );
				T[i][j]->setCenter( {p.x,p.y,p.z+0.4} );
			}
	}
	return 0;
}

