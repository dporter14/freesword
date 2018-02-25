#include <iostream>

#include "global.h"


Menu::Menu() 
{
	m_height = 0;
	m_width = 0;
	m_buttonTitle = "";
}
	
void mason_func()
{
	strcpy(g.title.text,"Mason: Test test");
	g.title.text_color = 0x00ff00ff;
}
