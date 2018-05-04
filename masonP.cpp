// Author: Mason Pierce
// Draws Menu Buttons on pause and other overlay items.
// Last edited 4/21/2018

#include <iostream>
#include <ctime>
#include <cstdio>
#include "GL/glx.h"
#include "fonts.h"
#include "global.h"
#include <sys/timeb.h>

void mason_func()
{
	//strcpy(g.title.text,"Mason: Test test");
	//g.title.text_color = 0x00ff00ff;
}

Menu::Menu()
{
	nButtons = 0;
}
clickState Menu::getOver(float x, float y) {
    for(int loop = 0; loop < nButtons; loop++) {

        if(buttons[loop].isOver(x, y) ) { 
            return buttons[loop].state;
        }
    }
    return C_NONE;
}
void resumeGame() {
	g.state[S_PAUSED] = 0;
}
bool Button::isOver(float x, float y) {

    if (x >= r.left && x <= r.right && y >= r.bot && y <= r.top) {
		over = 1;
       return true;
    }
	over = 0;
    return false;
}
void Button::setRect(float width, float height, float left, float bot) {
	r.width = width;
	r.height = height;
	r.left = left;
	r.bot = bot;

	r.right = r.left + r.width;
	r.top = r.bot + r.height;

	r.centerx = (r.left + r.right) / 2;
	r.centery = (r.bot + r.top) / 2;
}
void Button::setColor(float color1, float color2, float color3) {
	color[0] = color1;
	color[1] = color2;
	color[2] = color3;

	dColor[0] = color1 * 0.5f;
	dColor[1] = color2 * 0.5f;
	dColor[2] = color3 * 0.5f;
}
void Button::draw() {

	for(int i = 0; i < g.pauseMenu.nButtons; i++) {
        if (over) {
            int outlineOffset = 2;
            glColor3f(1.0f, 1.0f, 0.0f);
            //draw a highlight around button
            glLineWidth(3);

            glBegin(GL_LINE_LOOP);
                glVertex2i(r.left - outlineOffset,  r.bot - outlineOffset);
                glVertex2i(r.left - outlineOffset,  r.top + outlineOffset);
                glVertex2i(r.right + outlineOffset, r.top + outlineOffset);
                glVertex2i(r.right + outlineOffset, r.bot - outlineOffset);
                glVertex2i(r.left - outlineOffset, r.bot - outlineOffset);
            glEnd();

            glLineWidth(1);
        }
        if (down) {
            glColor3fv(dColor);
        } else {
            glColor3fv(color);
        }

        glBegin(GL_QUADS);
            glVertex2i(r.left,  r.bot);
            glVertex2i(r.left,  r.top);
            glVertex2i(r.right, r.top);
            glVertex2i(r.right, r.bot);
        glEnd();
		Rect r2;
        r2.left = r.centerx;
        r2.bot  = r.centery-8;
        r2.center = 1;
        if (down) {
            ggprint16(&r2, 0, text_color, "Pressed!");
        } else {
            ggprint16(&r2, 0, text_color, text);
        }

    }
}

void Menu::draw() {
	static double tix = 0.0;
	static char* info_here = g.info.get_place();
	double startTime, endTime;
	startTime = current_time();

	glColor3ub(0, 153, 0);

	for(int loop = 0; loop < nButtons; loop++) {
		buttons[loop].draw();
	} 

	glPushMatrix();	
		glTranslatef(g.xres/2, g.yres/2, 0.0);

		glTranslatef(-50, 50.0, 0.0);
		Rect r;
		r.bot = 50;
		r.left = 50;
		r.center = 1;

	glPopMatrix();
	
	//return time spent
	endTime = current_time();
	tix += endTime - startTime;
	sprintf(info_here, "Pause Menu Function: %f", tix);
}
void MainMenu::draw() {
	glColor3f(65,135,22);
	glBegin(GL_QUADS);
		glVertex2i(0, 0);
		glVertex2i(0, g.yres);
		glVertex2i(g.xres, g.yres);
		glVertex2i(g.xres, 0);
	glEnd();
	for(int loop = 0; loop < nButtons; loop++) {
		buttons[loop].draw();
	}
	glPushMatrix();
		glTranslatef(g.xres/2, g.yres/2, 0.0);
		glTranslatef(-50, 50.0, 0.0);
	glPopMatrix();
}

int detectButtons(int lbutton, int x, int y) {
	if(g.state[S_STARTUP]) {
		int click_state = g.mainMenu.getOver(x, y);
		if(lbutton) {
			switch(click_state) {
				case C_STARTGAME:
					g.state[S_STARTUP] = 0;
					break;	
			}
		}
	}
	
	if(g.state[ S_PAUSED ]) {
		int click_state= g.pauseMenu.getOver(x, y);
		if (lbutton) {
			switch (click_state) {
				case C_RESUME:
					//resumeGame();
					g.state[S_PAUSED] ^= 1;
					break;
				case C_QUIT:
					printf("Quit was clicked!\n");
					return 1;
					break;
				case C_EDITOR:
    				toggleEditMode();
					resumeGame();
					break;
				case C_NONE:
					break;
				case C_:
					break;	
			}
		}
	}
	return 0;
}

void displayEnemiesKilled() {

	Rect killedRect;
	killedRect.bot  = 0;
	killedRect.right = g.xres;
	killedRect.top = 20.0;
	killedRect.left = g.xres-100;
	killedRect.center=1;
	
	glColor3f(255,0,0);
	//glPushMatrix();
	//glTranslatef(-50.0, 0.0, 0.0);
	
	glBegin(GL_QUADS);
		glVertex2i(killedRect.left, killedRect.bot);
		glVertex2i(killedRect.left, killedRect.top);
		glVertex2i(killedRect.right, killedRect.top);
		glVertex2i(killedRect.right, killedRect.bot);
	glEnd();
	
	Rect r;
	r.bot = 0;
	r.left = g.xres-50;
	r.center = 1;
	
	ggprint8b(&r, 16, 0x0000ff00, "Enemies Slain: %i", g.eKilled);
	//glPopMatrix();
}
