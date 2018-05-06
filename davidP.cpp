//Author: David Porter
//Last Edit: 2018/18/04
//Textures and sprite generation/mapping

#include <iostream>     
#include <ctime>
#include "global.h"

void david_func()
{
	//strcpy(g.title.text,"This is a David Print");
	//g.title.text_color = 0x00ff0000;
}
/*
void Animation::sword_stab()
{
        static double tix = 0.0;
        static char * info_here = g.info.get_place();
        double startTime, endTime;
        startTime = current_time();

        Player* actor = (Player*)actors[0];
        static Vec orig_pos;
        static Vec orig_dir;
        if (frame == 0) {
                VecCopy(actor->rhand_pos, orig_pos);
                VecCopy(actor->rhand_dir, orig_dir);
        }
}
*/
void Texture::init(Image *img)
{
	Log("Dimensions: %d %d\n", img->width, img->height);

	h = img->height;
	w = img->width;

	glGenTextures(1, &tex);

	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
			img->width, img->height,
			0, GL_RGBA, GL_UNSIGNED_BYTE, img->data);
}

void Sprite::init(float x, float y, float w, float h, int n) {
	nframes=n;
	frame=0;
	float dx = spriteTex->w;
	float dy = spriteTex->h;
	
	VecMake(x/dx,y/dy,0,start);
	VecCopy(start,pos);
	this->w = w/dx;
	this->h = h/dy;
}

void initSpriteTextures()
{
	Image playerF = "./images/player_sprite_sheet.png";
	g.spriteTextures[SS_PLAYER].init(&playerF);

	g.sprites[SB_PLAYER_F].set_texture(&g.spriteTextures[SS_PLAYER]);
	g.sprites[SB_PLAYER_F].init(0,  0, 50, 75, 1);
	g.sprites[SB_PLAYER_B].set_texture(&g.spriteTextures[SS_PLAYER]);
    g.sprites[SB_PLAYER_B].init(0, 75, 50, 75, 1);
    g.sprites[SB_PLAYER_R].set_texture(&g.spriteTextures[SS_PLAYER]);
    g.sprites[SB_PLAYER_R].init(0,150, 50, 75, 1);
    g.sprites[SB_PLAYER_L].set_texture(&g.spriteTextures[SS_PLAYER]);
    g.sprites[SB_PLAYER_L].init(0,225, 50, 75, 1);
	
    Image img = "./images/tiles.png";
    g.spriteTextures[SS_TILES].init(&img);

    g.sprites[SB_TILE_STONE].set_texture(&g.spriteTextures[SS_TILES]);
	g.sprites[SB_TILE_STONE].init(0, 0, 50, 50, 1);
	g.sprites[SB_TILE_WOOD].set_texture(&g.spriteTextures[SS_TILES]);
    g.sprites[SB_TILE_WOOD].init(0, 50, 50, 50, 1);
	g.sprites[SB_TILE_GRASS].set_texture(&g.spriteTextures[SS_TILES]);
    g.sprites[SB_TILE_GRASS].init(0, 100, 50, 50, 1);
	g.sprites[SB_TILE_GRASS2].set_texture(&g.spriteTextures[SS_TILES]);
    g.sprites[SB_TILE_GRASS2].init(50, 100, 50, 50, 1);
    
    Image img2 = "./images/sword.png";
    g.spriteTextures[SS_SWORD].init(&img2);

    g.sprites[SB_ITEM_SWORD].set_texture(&g.spriteTextures[SS_SWORD]);
    //don't need if only one sprite on page
	//g.sprites[SB_ITEM_SWORD].init(0, 0, 100, 100, 1); 
	
	Image img3 = "./images/bow.png";
    g.spriteTextures[SS_BOW].init(&img3);

    g.sprites[SB_ITEM_BOW].set_texture(&g.spriteTextures[SS_BOW]);
	g.sprites[SB_ITEM_BOW].init(0, 0, 100, 100, 1);
	
	
	
	
}

void Character::swapSprites()
{
	int test = int(rot+360)%360;
	if(test < 45)
		sprt = &g.sprites[SB_PLAYER_B];
	else if(test < 135)
		sprt = &g.sprites[SB_PLAYER_L];
	else if(test < 225)
		sprt = &g.sprites[SB_PLAYER_F];
	else if(test < 315)
		sprt = &g.sprites[SB_PLAYER_R];
	else
		sprt = &g.sprites[SB_PLAYER_B];

}
    
void Object::drawSprite()
{

	glPushMatrix();
	glColor3f(color[0], color[1], color[2]);
	if(faking)
		glTranslatef(fake_pos[0], fake_pos[1], -fake_pos[1]);
	else
		glTranslatef(pos[0], pos[1], -pos[1]);
	glRotatef(fake_rot,0,0,1);
	/*
float cx = 0;
	float cy = 0;
	//radius
	float ra = pradius;
	int num_segments = 100;

	float theta = 2 * PI / float(num_segments); 
	float c = cosf(theta);//precalculate the sine and cosine
	float s = sinf(theta);
	float t;

	float x = ra;//we start at angle = 0 
	float y = 0; 
	glBindTexture(GL_TEXTURE_2D, g.spriteTextures[SI_PLAYER_FRONT]);
	
	glBegin(GL_POLYGON); 
	for(int ii = 0; ii < num_segments; ii++) 
	{ 
		glTexCoord2f(((x/pradius)+1)/2, ((1-(y/pradius))+1)/2);
		glVertex2f(x + cx, y + cy);//output vertex 

		//apply the rotation matrix
		t = x;
		x = c * x - s * y;
		y = s * t + c * y;
	} 
	glEnd();
	glBindTexture(GL_TEXTURE_2D,0);
	*/
	glBindTexture(GL_TEXTURE_2D, sprt->spriteTex->tex);
	glBegin(GL_POLYGON);
		glTexCoord2f(sprt->pos[0], sprt->pos[1]);
		glVertex3f(-scale[0],  scale[1], scale[2]);
		
		glTexCoord2f(sprt->pos[0]+sprt->w, sprt->pos[1]);
		glVertex3f( scale[0],  scale[1], scale[2]);
		
		glTexCoord2f(sprt->pos[0]+sprt->w, sprt->pos[1]+sprt->h);
		glVertex3f( scale[0], -scale[1], scale[2]);
		
		glTexCoord2f(sprt->pos[0], sprt->pos[1]+sprt->h);
		glVertex3f(-scale[0], -scale[1], scale[2]);
	glEnd();
	glBindTexture(GL_TEXTURE_2D,0);
	
	glPopMatrix();

/*
	if (g.state[S_DEBUG]) {
		glColor3f(0,0,1);
		glBegin(GL_LINE_LOOP);
		glVertex2f(hitbox.scale[0], hitbox.scale[1]);
		glVertex2f(-hitbox.scale[0], hitbox.scale[1]);
		glVertex2f(-hitbox.scale[0], -hitbox.scale[1]);
		glVertex2f(hitbox.scale[0], -hitbox.scale[1]);
		glEnd();
	}

	glRotatef(rot,0,0,1);

	//hand stuff
	glColor3f(0.6f, 0.6f, 0.6f);
	glTranslatef(rhand_pos[0], rhand_pos[1], 0.0);

	glRotatef(rhand_rot,0,0,1);
	
	glBegin(GL_POLYGON);
	glVertex2f(-3.0f, 0.0f);
	glVertex2f(-3.0f, 60.0f);
	glVertex2f(3.0f, 60.0f);
	glVertex2f(3.0f, 0.0f);
	glEnd();
	
	
	glPopMatrix();

	if (g.state[S_DEBUG]) {
		for(int i=0; i<nattacks; i++){
			glColor3f(1,1,0);
			glBegin(GL_LINE_LOOP);
			glVertex2f(attacks[i].pos[0]+attacks[i].scale[0], 
				attacks[i].pos[1]+attacks[i].scale[1]);
			glVertex2f(attacks[i].pos[0]-attacks[i].scale[0], 
				attacks[i].pos[1]+attacks[i].scale[1]);
			glVertex2f(attacks[i].pos[0]-attacks[i].scale[0], 
				attacks[i].pos[1]-attacks[i].scale[1]);
			glVertex2f(attacks[i].pos[0]+attacks[i].scale[0], 
				attacks[i].pos[1]-attacks[i].scale[1]);
			glEnd();
		}
	}
	*/
}

void Character::draw()
{
	//lab7 profiling
	static double tix = 0.0;
	static char* info_here = g.info.get_place();
	double startTime, endTime;
	startTime = current_time();

	
/*	//starting coordinates
	float cx = 0;
	float cy = 0;
	//radius
	float ra = pradius;
	int num_segments = 100;

	float theta = 2 * PI / float(num_segments); 
	float c = cosf(theta);//precalculate the sine and cosine
	float s = sinf(theta);
	float t;

	float x = ra;//we start at angle = 0 
	float y = 0; 

	glBegin(GL_POLYGON); 
	for(int ii = 0; ii < num_segments; ii++) 
	{ 
		glVertex2f(x + cx, y + cy);//output vertex 

		//apply the rotation matrix
		t = x;
		x = c * x - s * y;
		y = s * t + c * y;
	} 
	glEnd();
*/
	
	if (g.state[S_DEBUG]) {
		glPushMatrix();
		glTranslatef(hitbox.pos[0], hitbox.pos[1], 0.0);
		glColor3f(0,0,1);
		glBegin(GL_LINE_LOOP);
		glVertex2f(hitbox.scale[0], hitbox.scale[1]);
		glVertex2f(-hitbox.scale[0], hitbox.scale[1]);
		glVertex2f(-hitbox.scale[0], -hitbox.scale[1]);
		glVertex2f(hitbox.scale[0], -hitbox.scale[1]);
		glEnd();
		glPopMatrix();
	
	}
	
	/*
	//player direction
	Vec up, upz, cross;
	VecMake(0, 1, 0, up);
	VecMake(0, 0, 1, upz);
	float angl = acos(VecDot(dir, up));	
	VecCross(up, dir, cross);
	if (VecDot(upz, cross)<0)
		angl = -angl;	
	angl = angl*180/PI;
	*/
	//glPushMatrix();
	//glTranslatef(pos[0], pos[1], 0.0);
	
	//glRotatef(rot,0,0,1);
	
	/*
	//head triangle 
	glPushMatrix();
	glTranslatef(0, pradius+1, 0.0);
	glBegin(GL_POLYGON);
	glVertex2f(-10.0f, 0.0f);
	glVertex2f(0.0f, 10.0f);
	glVertex2f(10.0f, 0.0f);
	glEnd();
	glPopMatrix();
	*/

	//hand stuff
	//glColor3f(0.6f, 0.6f, 0.6f);
	//glTranslatef(weapon.pos[0], weapon.pos[1], 0.0);
	
	/*
	angl = acos(VecDot(rhand_dir, up));	
	VecCross(up, rhand_dir, cross);
	if (VecDot(upz, cross)<0)
		angl = -angl;	
	angl = angl*180/PI;
	*/
	//glRotatef(weapon.rot,0,0,1);
	
	/*
	glBegin(GL_POLYGON);
	glVertex2f(-3.0f, 0.0f);
	glVertex2f(-3.0f, 60.0f);
	glVertex2f(3.0f, 60.0f);
	glVertex2f(3.0f, 0.0f);
	glEnd();
	*/
	/*
	Sprite* sprt;
	sprt = &g.sprites[SB_ITEM_SWORD];
	glBindTexture(GL_TEXTURE_2D, g.spriteTextures[SS_SWORD].tex);
	glBegin(GL_QUADS);
		//top-left
		glTexCoord2f(sprt->pos[0], sprt->pos[1]);
		glVertex3f(-10,  60, 0);

		//top-right
		glTexCoord2f(sprt->pos[0]+sprt->w, sprt->pos[1]);
		glVertex3f( 10,  60, 0);

		//bottom-right
		glTexCoord2f(sprt->pos[0]+sprt->w, sprt->pos[1]+sprt->h);
		glVertex3f( 10, 0, 0);

		//bottom-left
		glTexCoord2f(sprt->pos[0], sprt->pos[1]+sprt->h);
		glVertex3f(-10, 0, 0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D,0);
	
	glPopMatrix();
	*/
	if (g.state[S_DEBUG]) {
		for(int i=0; i<weapon.nattacks; i++){
			glColor3f(1,1,0);
			glBegin(GL_LINE_LOOP);
			glVertex2f(weapon.attacks[i].pos[0]+weapon.attacks[i].scale[0], 
				weapon.attacks[i].pos[1]+weapon.attacks[i].scale[1]);
			glVertex2f(weapon.attacks[i].pos[0]-weapon.attacks[i].scale[0], 
				weapon.attacks[i].pos[1]+weapon.attacks[i].scale[1]);
			glVertex2f(weapon.attacks[i].pos[0]-weapon.attacks[i].scale[0], 
				weapon.attacks[i].pos[1]-weapon.attacks[i].scale[1]);
			glVertex2f(weapon.attacks[i].pos[0]+weapon.attacks[i].scale[0], 
				weapon.attacks[i].pos[1]-weapon.attacks[i].scale[1]);
			glEnd();
		}
	}
	//glDisable(GL_DEPTH_TEST);
	//draw player
	int temp = int(rot+360)%360;
	if (temp < 90 || temp > 270) {
		weapon.drawSprite();
		drawSprite();
	} else {
		drawSprite();
		weapon.drawSprite();
	}
	//glEnable(GL_DEPTH_TEST);
	//return time spent
	endTime = current_time();
	tix += endTime - startTime;
	sprintf(info_here, "Character Draw Function: %f", tix);
}

void Enemy::draw(){
	Character::draw();
		
	if (g.state[S_DEBUG]) {
		glPushMatrix();
		glTranslatef(pos[0], pos[1], 0.0);
		glColor3f(1,0,1);
		glBegin(GL_LINE_LOOP);
		glVertex2f(0,0);
	
		float num_segments=50;
		for(int ii = 0; ii < num_segments; ii++) 
		{
			float lerp = ii/(num_segments-1);
			float ang = -v_fov/2 + v_fov*lerp;
			glVertex2f(v_dist * -sin((rot+ang)*PI/180), 
				v_dist * cos((rot+ang)*PI/180));
		}
		
		glEnd();
		
		num_segments=100;
		glBegin(GL_LINE_LOOP);
		for(int ii = 0; ii < num_segments; ii++) 
		{
			float lerp = ii/(num_segments-1);
			float ang = 360*lerp;
			glVertex2f(v_close * -sin((rot+ang)*PI/180), 
				v_close * cos((rot+ang)*PI/180));
		}
		glEnd();
		glPopMatrix();
	}
}

/*
void Animation::my_anim(){
	if (frame==0){
		cout << "hello world" << endl;
	}
}
*/










