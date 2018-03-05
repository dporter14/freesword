#include <iostream>
#include "GL/glx.h"
#include "fonts.h"
#include "global.h"


void mason_func()
{
	strcpy(g.title.text,"Mason: Test test");
	g.title.text_color = 0x00ff00ff;
}

Menu::Menu() 
{
	m_height = 0;
	m_width = 0;
	m_buttonTitle = "";
}

bool pause() 
{
	bool paused = true;
	Rect rects [2];


	static float angle = 0.0;
	glColor3ub(0, 255, 0);
	glPushMatrix();
	glTranslatef(x, y, 0.0);
	glRotatef(angle, 0.0f, 0.0f, 1.0f);
	glTranslatef(-50.0, -50.0, 0);
	glBegin(GL_QUADS);
		glVertex2i(0,     0);
		glVertex2i(0,   200);
		glVertex2i(200, 100);
		glVertex2i(200,   0);
	glEnd();
	glClear(GL_COLOR_BUFFER_BIT);

	glPopMatrix();	
}
