// Author: Mason Pierce
// Draws Menu Buttons on pause and other overlay items.

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
	//m_height = 0;
	//m_width = 0;
	//m_buttonTitle = "";
    //m_position = 0;
}
clickState Menu::getOver(float x, float y) {
    for(int loop = 0; loop < nButtons; loop++) {

        if(buttons[loop].isOver(x, y) ) { 
            return buttons[loop].state;
        }
    }
    return C_NONE;
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

	for(int i=0; i<g.pauseMenu.nButtons; i++) {
        if (over) {
            int w=2;
            glColor3f(1.0f, 1.0f, 0.0f);
            //draw a highlight around button
            glLineWidth(3);

            glBegin(GL_LINE_LOOP);
                glVertex2i(r.left-w,  r.bot-w);
                glVertex2i(r.left-w,  r.top+w);
                glVertex2i(r.right+w, r.top+w);
                glVertex2i(r.right+w, r.bot-w);
                glVertex2i(r.left- w, r.bot-w);
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
	buttons[0].draw();
	buttons[1].draw();
	buttons[2].draw();

	glPushMatrix();	
	glTranslatef(g.xres/2, g.yres/2, 0.0);

	glTranslatef(-50, 50.0, 0.0);
	Rect r;
	r.bot = 50;
	r.left = 50;
	r.center = 1;
	ggprint8b(&r, 16, 0x00000000, "Pause Menu");

	glPopMatrix();
	
	//return time spent
	endTime = current_time();
	tix += endTime - startTime;
	sprintf(info_here, "Pause Menu Function: %f", tix);
}
void displayEnemiesKilled() {

	Rect killedRect;
	killedRect.bot  = 0;
	killedRect.right = g.xres;
	killedRect.top = 20.0;
	killedRect.left = g.xres;
	
	glColor3f(255,0,0);
	glPushMatrix();
	glTranslatef(-50.0, 0.0, 0.0);
	
	glBegin(GL_QUADS);
		glVertex2i(killedRect.left-50.0, killedRect.bot);
		glVertex2i(killedRect.left-50.0, killedRect.top);
		glVertex2i(killedRect.right+50.0, killedRect.top);
		glVertex2i(killedRect.right+50.0, killedRect.bot);
	glEnd();

	ggprint8b(&killedRect, 16, 0x0000ff00, "Enemies Slain: %i", g.eKilled);
	glPopMatrix();
}
