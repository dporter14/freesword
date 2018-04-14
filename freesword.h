#ifndef _FREESWORD
#define _FREESWORD

#include "defs.h"

enum wep_type {W_SWORD};

class Weapon {
    wep_type type;
    Weapon(){
        type=W_SWORD;
    }
};

class Animation;

class Object {
	public:
		Animation* anim_handler; //		
};

enum anim_type {A_SWORD_SLASH, A_TEST};

class Animation {
	public:
		int frame, nframes;
		bool done;
		
		anim_type type;
		
		Object *actors[2];
		int nactors;
		
		void init();
		void play();
		void add_actor(Object* actor);
		void set_frames(int frames);
		void set_duration(float duration);
		
		
		Animation(){};
		~Animation(){};
		
		void sword_slash();
		void test();
		
		
};

class Character : public Object{
	public:
		//define color and radius of circle until we have sprite
		//char* sprite_file;
		Vec color;
		Flt pradius;
		
		Vec pos; // char's position
		Vec vel; // char's velocity
		Vec dir; // char's orientation
		Vec rhand_pos; //pos of right hand
		Vec rhand_dir; //orientation of right hadn
		bool status; //0 alive 1 dead
		
		//constructor
		Character(){
			anim_handler=NULL;
		} 
		~Character(){} //destructor
		void move();
		void lookAt(Flt x, Flt y);
		void setPosition(Flt x, Flt y);
		void setVelocity(Flt x, Flt y);
		void addVelocity(Flt x, Flt y);
<<<<<<< HEAD
		void draw();
=======
		double draw();  //change for lab7
>>>>>>> 98c1d1c7850d9b7ee45600bc2af50aadd1d25cc3
	private:

};

class Player : public Character {
    public:
        void init();
        Player(){}
        ~Player(){}
    private:
};

class Enemy : public Character {
    public:
        void attackPlayer();
    private:
};

class Wall {

    public:
        //wall width and height
        Flt width, height;
        //coordinates for center of wall
        Flt x, y;
<<<<<<< HEAD
        
        void draw();
};


=======
        Flt left, right, top, bot;      

        double draw(); //lab7
        void initWall(Flt, Flt, Flt, Flt);
};

class Door {

    public:
        Flt width, height;
        //center
        Flt x, y;
        Flt left, right, top, bot;
        
        bool isHoriz;
        bool isOpen;

        void draw();
        //function to open/close door
        void swing();
        void initDoor(Flt, Flt, Flt, Flt, bool);
};

class Level {
    public:
        Enemy enemies[100];
        Wall walls[100];
        Door doors[100];

        void buildLevel1();
    private:
};
>>>>>>> 98c1d1c7850d9b7ee45600bc2af50aadd1d25cc3

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
<<<<<<< HEAD

void DisplayTime1();
double DisplayTime2();
void TimeMain();

void mason_func();
//bool pause();
=======
class mainMenu : public Menu {

    public:

    private:

};

void mason_func();
bool pauseMenu();
>>>>>>> 98c1d1c7850d9b7ee45600bc2af50aadd1d25cc3

/////// END Mason FUNCTIONS ////////////////////////

/*//////////////////////////////////////////////////
<<<<<<< HEAD
  David Functions		####################
=======
  David Functions	                ############
>>>>>>> 98c1d1c7850d9b7ee45600bc2af50aadd1d25cc3
  *///////////////////////////////////////////////////

void david_func();

/////// End David Functions/////////////////////////

/*//////////////////////////////////////////////////
  JACOB FUNCTIONS			############
  *///////////////////////////////////////////////////

void jacob_func();
void initWalls();
<<<<<<< HEAD
=======
void interactDoor();
void collide(Door);
void buildLevel1();

>>>>>>> 98c1d1c7850d9b7ee45600bc2af50aadd1d25cc3
/////// End Jacob Functions/////////////////////////

/*//////////////////////////////////////////////////
  TAYLOR FUNCTIONS			############
  *///////////////////////////////////////////////////

void taylor_func();
void spawnEnemy(Flt x, Flt y);

/////// End Taylor Functions/////////////////////////

#endif

