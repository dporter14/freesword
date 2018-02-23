#ifndef _DEFS_H
#define _DEFS_H

#include <iostream>

typedef double Flt;
typedef double Vec[3];
typedef Flt	Matrix[4][4];

#define VecMake(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecNegate(a)	(a)[0]=(-(a)[0]); (a)[1]=(-(a)[1]); (a)[2]=(-(a)[2]);
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecLen(a)	sqrtf((a)[0]*(a)[0]+(a)[1]*(a)[1]+(a)[2]*(a)[2])
#define VecLen2(a)	((a)[0]*(a)[0]+(a)[1]*(a)[1]+(a)[2]*(a)[2])
#define VecCopy(a,b)	(b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2];
#define VecAdd(a,b,c) (c)[0]=(a)[0]+(b)[0]; (c)[1]=(a)[1]+(b)[1]; (c)[2]=(a)[2]+(b)[2]
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; (c)[1]=(a)[1]-(b)[1]; (c)[2]=(a)[2]-(b)[2]
#define VecS(A,a,b) (b)[0]=(A)*(a)[0]; (b)[1]=(A)*(a)[1]; (b)[2]=(A)*(a)[2]
#define VecAddS(A,a,b,c) (c)[0]=(A)*(a)[0]+(b)[0]; (c)[1]=(A)*(a)[1]+(b)[1]; (c)[2]=(A)*(a)[2]+(b)[2]
#define VecCross(a,b,c) (c)[0]=(a)[1]*(b)[2]-(a)[2]*(b)[1]; (c)[1]=(a)[2]*(b)[0]-(a)[0]*(b)[2]; (c)[2]=(a)[0]*(b)[1]-(a)[1]*(b)[0]
#define VecZero(v) (v)[0]=0.0;(v)[1]=0.0;v[2]=0.0
#define ABS(a) (((a)<0)?(-(a)):(a))
#define SGN(a) (((a)<0)?(-1):(1))
#define SGND(a) (((a)<0.0)?(-1.0):(1.0))

/*typedef struct t_rect {
	int left;
	int top;
	int right;
	int bot;
	int width;
	int height;
	int center;
	int centerx;
	int centery;
} Rect;*/

typedef struct Mouse {
	int x,y;
	int lastx,lasty;
	int lbuttondown;
} Mouse;

typedef struct Screen2 {
	int x_res, y_res;
	int screen_x_res, screen_y_res;
	float gl_perspective_viewangle;
	Flt xcenter, ycenter;
	Flt fy_res, fx_res;
} Screen2;


class Character {
	public:
		char* sprite_file;
		Vec pos;
		Character(){}
		~Character(){}
	private:
};

class Player : Character {
	public:
		int pos[2];
		int vel[2];
		int dir; //movement direction; 0/1/2/3 : up/left/down/right
		int orientation[2]; //direction player is facing; [0] = x [1] = y, x=-1:left y=1:up, etc.
		int pointer[2]; //placeholder for player's face
		int status; //0 alive 1 dead
	private:
};

class Enemy : Character {
	public:
	private:
};


/*//////////////////////////////////////////////////
 	MASON FUNCTIONS			############
*///////////////////////////////////////////////////
class Menu {

	public:
		Menu();
	private:
		double m_height, m_width;
		std::string m_buttonTitle;		
};
class mainMenu : public Menu {
	
	public:

	private:
	
};

void mason_func();

/////// END MASON FUNCTIONS ////////////////////////

/*//////////////////////////////////////////////////
	David Functions
*///////////////////////////////////////////////////

void print_david();

/////// End David Functions/////////////////////////

void movePlayer(Player *player);
void setPlayerOrientation(Player *player);

#endif

