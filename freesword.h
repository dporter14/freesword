#ifndef _FREESWORD
#define _FREESWORD

#include "defs.h"

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
int david_func();
int taylor_func();
int mason_func();
int jacob_func();

void movePlayer(Player *player);
void setPlayerOrientation(Player *player);

#endif

