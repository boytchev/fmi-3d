
/// Project:	FMI 3D (2013)
/// File:		24451 Subdivided figure.cpp
/// Topic:		24 Surfaces
/// Slide:		07
///	About:		Figure made with subdivision
///
/// History:
///		2013.06.01/PB	Initial implementation
///     2013.08.13/PB   Change button image


#include "fmi3d.h"

//#include <iostream>

int level=2;
int mode=1;	/// 0=wireframe 1=fill+wireframe 2=fill
Object3D* turtle;

#define SCALE 0.2
#define HOME	turtle->identity()
#define FD(h)	turtle->translateLocal({h*SCALE,0,0})
#define BK(h)	turtle->translateLocal({-h*SCALE,0,0})
#define SL(h)	turtle->translateLocal({0,h*SCALE,0})
#define SR(h)	turtle->translateLocal({0,-h*SCALE,0})
#define SU(h)	turtle->translateLocal({0,0,h*SCALE})
#define SD(h)	turtle->translateLocal({0,0,-h*SCALE})
#define LT(h)	turtle->rotateLocal(h,{0,0,1})
#define RT(h)	turtle->rotateLocal(-h,{0,0,1})
#define UP(h)	turtle->rotateLocal(-h,{0,1,0})
#define DN(h)	turtle->rotateLocal(h,{0,1,0})
#define LR(h)	turtle->rotateLocal(-h,{1,0,0})
#define RR(h)	turtle->rotateLocal(h,{1,0,0})

#define N 82
Subdiv3D* subd[N];

#define M 84
vect_t P[M];

//Object3D* O[4];

vect_t fl( double gof, double gol )
{
	vect_t p = turtle->oxyz.o;
	FD( gof );
	SL( gol );
	vect_t res = turtle->oxyz.o;
	turtle->oxyz.o = p;
	return res;
}

void fla( int i, double gof1, double gof2, double gol )
{
	P[i+0]=fl( gof1,gol );
	P[i+1]=fl( gof1,-gol );
	P[i+2]=fl( gof2,-gol );
	P[i+3]=fl( gof2,gol );
}

void flb( int i, double gof1, double gof2, double gol )
{
	P[i+0]=fl( gof1,gol );
	P[i+1]=fl( gof2,gol );
	P[i+2]=fl( gof2,-gol );
	P[i+3]=fl( gof1,-gol );
}

void flc( int i, double gof1, double gof2, double gol )
{
	P[i+0]=fl( gof1,gol );
	P[i+1]=fl( gof2,gol );
}

void skeleton()
{
	double Ls[]= {0,8,4,3,10,8,5,4,4,3,4,7,7,0,0,2,8,2};

	double q = t/2;
	double qq=1;
	double SP1 = qq*60*sin( 3*q );
	double SP2 = qq*20*cos( 4*q );
	double SP3 = qq*80*sin( 5*q );
	double SP4 = qq*30*cos( 2*q );
	double SP5 = qq*30*cos( 1*q );

	double SP6 = qq*30*sin( 4*q );
	double SP7 = 30+qq*40*sin( 1*q );
	double SP8 = 40+qq*40*sin( 2*q );
	double SP9 = qq*40*sin( 3*q );
	double SP10 = qq*40*sin( 6*q );

	double SP6x = qq*30*cos( 4*q );
	double SP7x = 30+qq*40*sin( 1*q );
	double SP8x = 40+qq*40*cos( 2*q );
	double SP9x = qq*40*sin( 3*q );
	double SP10x = qq*40*cos( 6*q );

	double SPL11 = -45-qq*25*sin( 3*q );
	double SPL12 = 20+qq*30*cos( 2*q );
	double SPL13 = 0;

	double SPR11 = -45-qq*25*cos( 2*q );
	double SPR12 = 20+qq*30*cos( 3*q );
	double SPR13 = 0;

	double SPL14 = 25+qq*25*cos( 5*q );
	double SPR14 = 25+qq*25*sin( 4*q );


	HOME;
	SU( 26 );
	LT( 200*sin( t/4 ) );
	UP( 200*sin( t/3 ) );
	RR( 200*cos( t/2 ) );

	// now at waist
	oxyz_t waist=turtle->oxyz;
	fla( 0,-2,3,-4 );

	SU( Ls[1] );
	LT( SP1 );
	LR( SP2 );
	oxyz_t neck=turtle->oxyz;
	fla( 4,-4,4,-8 );

	SU( Ls[2] );
	fla( 8,-2,2,-3 );

	SU( Ls[9] );
	LT( SP3 );
	LR( SP4 );
	DN( SP5 );
	fla( 12,-1,1,-1 );

	SU( Ls[8] );
	fla( 16,-5,5,-5 );

	SU( Ls[7] );
	fla( 20,-1,1,-1 );

	subd[0]->setVertices( P[20], {P[16]}, P[21], {P[17]}, P[22], {P[18]}, P[23], {P[19]} );

	subd[1]->setVertices( P[23], {P[20]}, P[22], {P[21]}, P[18], {P[17],P[13],P[14]}, P[19], {P[15],P[12],P[16]} );
	subd[2]->setVertices( P[21], {P[20]}, P[17], {P[16],P[12],P[13]}, P[18], {P[14],P[15],P[19]}, P[22], {P[23]} );
	subd[3]->setVertices( P[23], {P[22]}, P[19], {P[18],P[14],P[15]}, P[16], {P[12],P[13],P[17]}, P[20], {P[21]} );
	subd[4]->setVertices( P[20], {P[23]}, P[16], {P[19],P[15],P[12]}, P[17], {P[13],P[14],P[18]}, P[21], {P[22]} );

	subd[5]->setVertices( P[18], {P[22],P[21],P[17]}, P[14], {P[13],P[9],P[10]}, P[15], {P[11],P[8],P[12]}, P[19], {P[16],P[20],P[23]} );
	subd[6]->setVertices( P[17], {P[21],P[20],P[16]}, P[13], {P[12],P[8],P[9]}, P[14], {P[10],P[11],P[15]}, P[18], {P[19],P[23],P[22]} );
	subd[7]->setVertices( P[19], {P[23],P[22],P[18]}, P[15], {P[14],P[10],P[11]}, P[12], {P[8],P[9],P[13]}, P[16], {P[17],P[21],P[20]} );
	subd[8]->setVertices( P[16], {P[20],P[23],P[19]}, P[12], {P[15],P[11],P[8]}, P[13], {P[9],P[10],P[14]}, P[17], {P[18],P[22],P[21]} );

	/// start left hand
	turtle->oxyz = neck;
	LT( 90 );
	UP( 20 );
	FD( Ls[3] );
	DN( 20 );

	UP( SP6x );
	RT( SP7x );

	FD( Ls[4] );
	RT( SP8x );

	UP( 90 ); //LR LT
	flb( 24,2,-2,2 );

	SD( Ls[5] );
	RR( SP9x );
	RT( SP10x );
	flb( 28,0.5,-0.5,0.5 );

	SD( Ls[6] );
	flb( 32,3,-3,1 );

	subd[9]->setVertices( P[32], {P[28]},P[33], {P[29]},P[34], {P[30]},P[35], {P[31]} );
	subd[10]->setVertices( P[32], {P[33]},P[35], {P[34]},P[31], {P[30],P[26],P[27]},P[28], {P[24],P[25],P[29]} );
	subd[11]->setVertices( P[35], {P[32]},P[34], {P[33]},P[30], {P[29],P[25],P[26]},P[31], {P[27],P[24],P[28]} );
	subd[12]->setVertices( P[34], {P[35]},P[33], {P[32]},P[29], {P[28],P[24],P[25]},P[30], {P[26],P[27],P[31]} );
	subd[13]->setVertices( P[33], {P[34]},P[32], {P[35]},P[28], {P[31],P[27],P[24]},P[29], {P[25],P[26],P[30]} );

	subd[14]->setVertices( P[28], {P[29],P[33],P[32]},P[31], {P[35],P[34],P[30]},P[27], {P[26],P[6],P[10]},P[24], {P[9],P[5],P[25]} );
	subd[15]->setVertices( P[31], {P[28],P[32],P[35]},P[30], {P[34],P[33],P[29]},P[26], {P[25],P[5],P[6]},P[27], {P[10],P[9],P[24]} );
	subd[16]->setVertices( P[30], {P[31],P[35],P[34]},P[29], {P[33],P[32],P[28]},P[25], {P[24],P[9],P[5]},P[26], {P[6],P[10],P[27]} );
	subd[17]->setVertices( P[29], {P[30],P[34],P[33]},P[28], {P[32],P[35],P[31]},P[24], {P[27],P[10],P[9]},P[25], {P[5],P[6],P[26]} );

	// start right hand
	turtle->oxyz = neck;
	RT( 90 );
	UP( 20 );
	FD( Ls[3] );
	DN( 20 );

	UP( SP6 );
	LT( SP7 );

	FD( Ls[4] );
	LT( SP8 );

	UP( 90 ); //
	flb( 36,2,-2,-2 );

	SD( Ls[5] );
	LR( SP9 );
	LT( SP10 );
	flb( 40,0.5,-0.5,-0.5 );
	SD( Ls[6] );
	flb( 44,3,-3,-1 );

	subd[18]->setVertices( P[47], {P[43]},P[46], {P[42]},P[45], {P[41]},P[44], {P[40]} );
	subd[19]->setVertices( P[40], {P[41],P[37],P[36]},P[43], {P[39],P[38],P[42]},P[47], {P[46]},P[44], {P[45]} );
	subd[20]->setVertices( P[43], {P[40],P[36],P[39]},P[42], {P[38],P[37],P[41]},P[46], {P[45]},P[47], {P[44]} );
	subd[21]->setVertices( P[42], {P[43],P[39],P[38]},P[41], {P[37],P[36],P[40]},P[45], {P[44]},P[46], {P[47]} );
	subd[22]->setVertices( P[41], {P[42],P[38],P[37]},P[40], {P[36],P[39],P[43]},P[44], {P[47]},P[45], {P[46]} );

	subd[23]->setVertices( P[36], {P[37],P[4],P[8]},P[39], {P[11],P[7],P[38]},P[43], {P[42],P[46],P[47]},P[40], {P[44],P[45],P[41]} );
	subd[24]->setVertices( P[39], {P[36],P[8],P[11]},P[38], {P[7],P[4],P[37]},P[42], {P[41],P[45],P[46]},P[43], {P[47],P[44],P[40]} );
	subd[25]->setVertices( P[38], {P[39],P[11],P[7]},P[37], {P[4],P[8],P[36]},P[41], {P[40],P[44],P[45]},P[42], {P[46],P[47],P[43]} );
	subd[26]->setVertices( P[37], {P[38],P[7],P[4]},P[36], {P[8],P[11],P[39]},P[40], {P[43],P[47],P[44]},P[41], {P[45],P[46],P[42]} );


	subd[27]->setVertices( P[14], {P[18],P[17],P[13]},P[10], {P[9],P[24],P[27],P[26],P[6]},P[11], {P[7],P[38],P[39],P[36],P[8]},P[15], {P[12],P[16],P[19]} );
	subd[28]->setVertices( P[13], {P[17],P[16],P[12]},P[9], {P[8],P[4],P[5],P[25],P[24]},P[10], {P[27],P[26],P[6],P[7],P[11]},P[14], {P[15],P[19],P[18]} );
	subd[29]->setVertices( P[15], {P[19],P[18],P[14]},P[11], {P[10],P[6],P[7],P[38],P[39]},P[8], {P[36],P[37],P[4],P[5],P[9]},P[12], {P[13],P[17],P[16]} );
	subd[30]->setVertices( P[12], {P[16],P[19],P[15]},P[8], {P[11],P[39],P[36],P[37],P[4]},P[9], {P[5],P[25],P[24],P[27],P[10]},P[13], {P[14],P[18],P[17]} );

	subd[31]->setVertices( P[8], {P[4],P[5],P[9],P[13],P[12]},P[11], {P[15],P[14],P[10],P[6],P[7]},P[39], {P[38],P[42],P[43]},P[36], {P[40],P[41],P[37]} );
	subd[32]->setVertices( P[11], {P[8],P[12],P[15],P[14],P[10]},P[7], {P[6],P[2],P[3],P[0],P[4]},P[38], {P[37],P[41],P[42]},P[39], {P[43],P[40],P[36]} );
	subd[33]->setVertices( P[7], {P[11],P[10],P[6],P[2],P[3]},P[4], {P[0],P[1],P[5],P[9],P[8]},P[37], {P[36],P[40],P[41]},P[38], {P[42],P[43],P[39]} );
	subd[34]->setVertices( P[4], {P[7],P[3],P[0],P[1],P[5]},P[8], {P[9],P[13],P[12],P[15],P[11]},P[36], {P[39],P[43],P[40]},P[37], {P[41],P[42],P[38]} );

	subd[35]->setVertices( P[24], {P[25],P[29],P[28]},P[27], {P[31],P[30],P[26]},P[10], {P[6],P[7],P[11],P[15],P[14]},P[9], {P[13],P[12],P[8],P[4],P[5]} );
	subd[36]->setVertices( P[27], {P[24],P[28],P[31]},P[26], {P[30],P[29],P[25]},P[6], {P[5],P[1],P[2],P[3],P[7]},P[10], {P[11],P[15],P[14],P[13],P[9]} );
	subd[37]->setVertices( P[26], {P[27],P[31],P[30]},P[25], {P[29],P[28],P[24]},P[5], {P[9],P[8],P[4],P[0],P[1]},P[6], {P[2],P[3],P[7],P[11],P[10]} );
	subd[38]->setVertices( P[25], {P[26],P[30],P[29]},P[24], {P[28],P[31],P[27]},P[9], {P[10],P[14],P[13],P[12],P[8]},P[5], {P[4],P[0],P[1],P[2],P[6]} );

	subd[39]->setVertices( P[10], {P[14],P[13],P[9],P[24],P[27]},P[6], {P[26],P[25],P[5],P[1],P[2]},P[7], {P[3],P[0],P[4],P[37],P[38]},P[11], {P[39],P[36],P[8],P[12],P[15]} );
	subd[40]->setVertices( P[8], {P[12],P[15],P[11],P[39],P[36]},P[4], {P[37],P[38],P[7],P[3],P[0]},P[5], {P[1],P[2],P[6],P[26],P[25]},P[9], {P[24],P[27],P[10],P[14],P[13]} );


	// do the left leg
	turtle->oxyz = waist;
	RR( 45 );
	SD( Ls[10] );
	flc( 60,5,-5,-3 );
	RR( -45 );
	LR( SPL11 );
	UP( SPL12 );
	LT( SPL13 );

	SD( Ls[11] );
	LT( 90 );
	DN( SPL14 );
	flb( 48,3,-3,-3 );

	SD( Ls[12] );
	flb( 52,1,-1,-1 );

	SD( 2*Ls[15] );
	flb( 56,2,-2,-2 );

	SU( Ls[15] );
	UP( 90 );
	SD( Ls[16] );
	fla( 62,1,-1,5 );

	subd[41]->setVertices( P[62], {P[55]},P[65], {P[59]},P[64], {P[56]},P[63], {P[52]} );
	subd[42]->setVertices( P[55], {P[52],P[48],P[51],P[50],P[54]},P[59], {P[58],P[57],P[56]},P[65], {P[64]},P[62], {P[63]} );
	subd[43]->setVertices( P[59], {P[55],P[54],P[58]},P[56], {P[57],P[53],P[52]},P[64], {P[63]},P[65], {P[62]} );
	subd[44]->setVertices( P[56], {P[59],P[58],P[57]},P[52], {P[53],P[49],P[48],P[51],P[55]},P[63], {P[62]},P[64], {P[65]} );
	subd[45]->setVertices( P[52], {P[56],P[57],P[53],P[49],P[48]},P[55], {P[51],P[50],P[54],P[58],P[59]},P[62], {P[65]},P[63], {P[64]} );
	subd[46]->setVertices( P[52], {P[48],P[51],P[55],P[62],P[63]},P[56], {P[64],P[65],P[59]},P[57], {P[58]},P[53], {P[54],P[50],P[49]} );
	subd[47]->setVertices( P[56], {P[52],P[63],P[64]},P[59], {P[65],P[62],P[55]},P[58], {P[54]},P[57], {P[53]} );
	subd[48]->setVertices( P[59], {P[56],P[64],P[65]},P[55], {P[62],P[63],P[52],P[48],P[51]},P[54], {P[50],P[49],P[53]},P[58], {P[57]} );
	subd[49]->setVertices( P[53], {P[49],P[48],P[52]},P[57], {P[56]},P[58], {P[59]},P[54], {P[55],P[51],P[50]} );
	subd[50]->setVertices( P[48], {P[2],P[1],P[51]},P[52], {P[55],P[62],P[63],P[64],P[56]},P[53], {P[57],P[58],P[54]},P[49], {P[50],P[61],P[60]} );
	subd[51]->setVertices( P[49], {P[60],P[2],P[48]},P[53], {P[52],P[56],P[57]},P[54], {P[58],P[59],P[55]},P[50], {P[51],P[1],P[61]} );
	subd[52]->setVertices( P[50], {P[61],P[60],P[49]},P[54], {P[53],P[57],P[58]},P[55], {P[59],P[65],P[62],P[63],P[52]},P[51], {P[48],P[2],P[1]} );
	subd[53]->setVertices( P[51], {P[1],P[61],P[50]},P[55], {P[54],P[58],P[59],P[65],P[62]},P[52], {P[63],P[64],P[56],P[57],P[53]},P[48], {P[49],P[60],P[2]} );

	// do the right leg
	turtle->oxyz=waist;
	LR( 45 );
	SD( Ls[10] );
	flc( 66,5,-5,3 );
	LR( -45 );
	RR( SPR11 );
	UP( SPR12 );
	RT( SPR13 );

	SD( Ls[11] );
	RT( 90 );
	DN( SPR14 );
	flb( 68,-3,3,3 );

	SD( Ls[12] );
	flb( 72,1,-1,1 );

	SD( 2*Ls[15] );
	flb( 76,2,-2,2 );
	SU( Ls[15] );
	UP( 90 );
	SD( Ls[16] );
	fla( 80,1,-1,-5 );

	subd[54]->setVertices( P[6], {P[10],P[27],P[26],P[25],P[5]},P[2], {P[1],P[51],P[48],P[49],P[60]},P[3], {P[66],P[68],P[69],P[70],P[0]},P[7], {P[4],P[37],P[38],P[39],P[11]} );
	subd[55]->setVertices( P[5], {P[25],P[24],P[9],P[8],P[4]},P[1], {P[0],P[67],P[61],P[50],P[51]},P[2], {P[48],P[49],P[60],P[66],P[3]},P[6], {P[7],P[11],P[10],P[27],P[26]} );
	subd[56]->setVertices( P[4], {P[8],P[36],P[37],P[38],P[7]},P[0], {P[3],P[69],P[70],P[71],P[67]},P[1], {P[61],P[50],P[51],P[48],P[2]},P[5], {P[6],P[26],P[25],P[24],P[9]} );
	subd[57]->setVertices( P[7], {P[38],P[39],P[11],P[10],P[6]},P[3], {P[2],P[60],P[66],P[68],P[69]},P[0], {P[70],P[71],P[67],P[61],P[1]},P[4], {P[5],P[9],P[8],P[36],P[37]} );
	subd[58]->setVertices( P[2], {P[6],P[5],P[1],P[51],P[48]},P[60], {P[49],P[50],P[61]},P[66], {P[67],P[71],P[68]},P[3], {P[69],P[70],P[0],P[4],P[7]} );
	subd[59]->setVertices( P[0], {P[4],P[7],P[3],P[69],P[70]},P[67], {P[71],P[68],P[66]},P[61], {P[60],P[49],P[50]},P[1], {P[51],P[48],P[2],P[6],P[5]} );
	subd[60]->setVertices( P[60], {P[2],P[48],P[49]},P[61], {P[50],P[51],P[1]},P[67], {P[0],P[70],P[71]},P[66], {P[68],P[69],P[3]} );

	subd[61]->setVertices( P[66], {P[60],P[61],P[67]},P[68], {P[71],P[74],P[73]},P[69], {P[72],P[75],P[70]},P[3], {P[0],P[4],P[7],P[6],P[2]} );
	subd[62]->setVertices( P[3], {P[7],P[6],P[2],P[60],P[66]},P[69], {P[68],P[73],P[72]},P[70], {P[75],P[74],P[71]},P[0], {P[67],P[61],P[1],P[5],P[4]} );
	subd[63]->setVertices( P[0], {P[1],P[5],P[4],P[7],P[3]},P[70], {P[69],P[72],P[75]},P[71], {P[74],P[73],P[68]},P[67], {P[66],P[60],P[61]} );
	subd[64]->setVertices( P[67], {P[61],P[1],P[0]},P[71], {P[70],P[75],P[74]},P[68], {P[73],P[72],P[69]},P[66], {P[3],P[2],P[60]} );
	subd[65]->setVertices( P[2], {P[3],P[7],P[6],P[5],P[1]},P[48], {P[51],P[55],P[52]},P[49], {P[53],P[54],P[50]},P[60], {P[61],P[67],P[66]} );
	subd[66]->setVertices( P[1], {P[5],P[4],P[0],P[67],P[61]},P[51], {P[50],P[54],P[55]},P[48], {P[52],P[53],P[49]},P[2], {P[60],P[66],P[3],P[7],P[6]} );
	subd[67]->setVertices( P[61], {P[67],P[66],P[60]},P[50], {P[49],P[53],P[54]},P[51], {P[55],P[52],P[48]},P[1], {P[2],P[6],P[5],P[4],P[0]} );
	subd[68]->setVertices( P[60], {P[66],P[3],P[2]},P[49], {P[48],P[52],P[53]},P[50], {P[54],P[55],P[51]},P[61], {P[1],P[0],P[67]} );

	subd[69]->setVertices( P[68], {P[66],P[67],P[71]},P[73], {P[74],P[78],P[77]},P[72], {P[76],P[82],P[81],P[80],P[75]},P[69], {P[70],P[0],P[3]} );
	subd[70]->setVertices( P[69], {P[3],P[66],P[68]},P[72], {P[73],P[77],P[76],P[82],P[81]},P[75], {P[80],P[83],P[79],P[78],P[74]},P[70], {P[71],P[67],P[0]} );
	subd[71]->setVertices( P[70], {P[0],P[3],P[69]},P[75], {P[72],P[81],P[80],P[83],P[79]},P[74], {P[78],P[77],P[73]},P[71], {P[68],P[66],P[67]} );
	subd[72]->setVertices( P[71], {P[67],P[0],P[70]},P[74], {P[75],P[79],P[78]},P[73], {P[77],P[76],P[72]},P[68], {P[69],P[3],P[66]} );

	subd[73]->setVertices( P[82], {P[76]},P[83], {P[79]},P[80], {P[75]},P[81], {P[72]} );
	subd[74]->setVertices( P[75], {P[79],P[78],P[74],P[71],P[70]},P[72], {P[69],P[68],P[73],P[77],P[76]},P[81], {P[82]},P[80], {P[83]} );
	subd[75]->setVertices( P[72], {P[75],P[70],P[69],P[68],P[73]},P[76], {P[77],P[78],P[79]},P[82], {P[83]},P[81], {P[80]} );
	subd[76]->setVertices( P[76], {P[72],P[73],P[77]},P[79], {P[78],P[74],P[75]},P[83], {P[80]},P[82], {P[81]} );
	subd[77]->setVertices( P[79], {P[76],P[77],P[78]},P[75], {P[74],P[71],P[70],P[69],P[72]},P[80], {P[81]},P[83], {P[82]} );
	subd[78]->setVertices( P[73], {P[68],P[71],P[74]},P[77], {P[78]},P[76], {P[79],P[83],P[82]},P[72], {P[81],P[80],P[75],P[70],P[69]} );
	subd[79]->setVertices( P[77], {P[73]},P[78], {P[74]},P[79], {P[75],P[80],P[83]},P[76], {P[82],P[81],P[72]} );
	subd[80]->setVertices( P[78], {P[77]},P[74], {P[73],P[68],P[71]},P[75], {P[70],P[69],P[72],P[81],P[80]},P[79], {P[83],P[82],P[76]} );
	subd[81]->setVertices( P[74], {P[71],P[70],P[75]},P[78], {P[79]},P[77], {P[76]},P[73], {P[72],P[69],P[68]} );

}


void drawScene()
{
	double lw=5.0-viewDistance/5.0-level/2.0;
	glLineWidth( lw>0.5?lw:0.5 );

	glLightModeli( GL_LIGHT_MODEL_TWO_SIDE,inReflection?GL_FALSE:GL_TRUE );

	for( int i=0; i<N; i++ )
		subd[i]->drawImage();
	glLightModeli( GL_LIGHT_MODEL_TWO_SIDE,GL_FALSE );
}


void drawFrames()
{
	if( mode==1 )
		for( int i=0; i<N; i++ )
			subd[i]->drawFrame();
}

Button2D* butNL;
void nextLevel()
{
	level = ( level+1 )%4;
	butNL->setState( level );
	for( int i=0; i<N; i++ )
	{
		subd[i]->setLevel( level );
	}
}

Button2D* butWF;
void showWireframe()
{
	mode=( mode+1 )%3;
	butWF->setState( mode );
	for( int i=0; i<N; i++ )
	{
		switch( mode )
		{
			case 0: subd[i]->setColor( {1,0,0,0.5} ); break;
			case 1: subd[i]->setColor( {1,0.8,0.5} ); break;
			case 2:	subd[i]->setColor( {random( 0.5,1 ),random( 0.5,1 ),random( 0.5,1 )} );
		}

		subd[i]->setPolygonMode( mode==0?GL_LINE:GL_FILL );
	}
}


int main()
{
	/// Open window
	if( !openWindow3D( "A subdivided figure" ) ) return 1;
	changeBrightness();
	changeGround();
	changeGround();
	changeGround();

	viewZ = 5;
	viewBeta = 0;
	viewDistance = 15;

	/// Add buttons
	butWF = new Button2D( "buttons/button_wireframe.png",'W',showWireframe,mode,3 );
	butNL = new Button2D( "buttons/button_n.png",GLFW_KEY_SPACE,nextLevel,level,4 );
	addButton( new exitButton2D );

	turtle = new Object3D();

	for( int i=0; i<N; i++ )
	{
		subd[i] = new Subdiv3D();
		subd[i]->setColor( {random( 0.5,1 ),random( 0.5,1 ),random( 0.5,1 )} );
		subd[i]->setLevel( level );
		subd[i]->setPolygonMode( GL_FILL );
	}
	skeleton();
	showWireframe();


	/// Main loop
	while( isRunning() )
	{
		skeleton();
	}

	return 0;
}

