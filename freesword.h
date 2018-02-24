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

/////// END Mason FUNCTIONS ////////////////////////

/*//////////////////////////////////////////////////
	David Functions
*///////////////////////////////////////////////////

void david_func();

/////// End David Functions/////////////////////////

/*//////////////////////////////////////////////////
 	JACOB FUNCTIONS			############
*///////////////////////////////////////////////////

void movePlayer(Player *player);
void setPlayerOrientation(Player *player);

void jacob_func();

/////// End Jacob Functions/////////////////////////

/*//////////////////////////////////////////////////
 	TAYLOR FUNCTIONS			############
*///////////////////////////////////////////////////

void taylor_func();

/////// End Taylor Functions/////////////////////////

#endif

