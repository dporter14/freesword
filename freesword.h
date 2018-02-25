#ifndef _FREESWORD
#define _FREESWORD

#include "defs.h"

class Character {
	public:
		char* sprite_file;
		Vec pos; // char's position
		Vec vel; // char's velocity
		Vec dir; // char's orientation
		bool status; //0 alive 1 dead
		
		Character(){} //constructor
		~Character(){} //destructor
		void move();
		void lookAt(Flt x, Flt y);
		void setPosition(Flt x, Flt y);
		void setVelocity(Flt x, Flt y);
		void addVelocity(Flt x, Flt y);
	private:
};

class Player : public Character {
	public:
		Vec pointer; //placeholder for player's face
		Flt pradius;	
		void lookAt(Flt x, Flt y);
		void init();
		Player(){}
		~Player(){}
	private:
};

class Enemy : public Character {
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

