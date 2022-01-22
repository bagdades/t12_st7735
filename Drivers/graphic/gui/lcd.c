/*
 * =====================================================================================
 *
 *       Filename:  lcd.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  31.12.21 22:11:15
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdlib.h>
#include "fonts.h"
#include "lcd.h"
#include "st7735.h"

extern ucg_t ucg;

static screen_t *screens = NULL;
static screen_t *current_screen;
RE_Rotation_t (*RE_GetData)(RE_State_t*);
RE_Rotation_t RE_Rotation;
static RE_State_t* RE_State;


screen_t *lcd_addScreen(uint8_t index) {
	screen_t *ret = malloc(sizeof(screen_t));
	if(!ret)
		Error_Handler();
	ret->index = index;
	ret->next_screen = NULL;
	ret->init = NULL;
	ret->draw = NULL;
	ret->onExit = NULL;
	ret->onEnter = NULL;
	ret->processInput = NULL;
	ret->widgets = NULL;
	ret->current_widget = NULL;
	if(screens == NULL) {
		screens = ret;
	}
	else {
		screen_t *temp = screens;
		while(temp->next_screen) {
			temp = temp->next_screen;
		}
		temp->next_screen = ret;
	}
	return ret;
}

void lcd_draw(void) {
	current_screen->draw(current_screen);
}

/* void lcd_init(void) { */
void lcd_init(RE_Rotation_t (*GetData)(RE_State_t*), RE_State_t *State) {
  RE_State = State;
  RE_GetData = GetData;
	screen_t *scr = screens;
	while(scr) {
		if(scr->index == 0) {
			scr->init(scr);
			current_screen = scr;
			return;
		}
	}
}

void lcd_processInput(void) {
	RE_Rotation = (*RE_GetData)(RE_State);
	int ret = current_screen->processInput(current_screen, RE_Rotation, RE_State);
	/* int ret = current_screen->processInput(current_screen, input, state); */
	if(ret != -1) {
		screen_t *scr = screens;
		while(scr) {
			if(scr->index == ret) {
				scr->refresh = screen_Erase;
				ucg_SetForeColor(&ucg, C_BLACK);
				ucg_FillScreen(&ucg);
				/* UG_FillScreen(C_BLACK); */
				if(current_screen->onExit)
					current_screen->onExit(scr);
				if(scr->onEnter)
					scr->onEnter(current_screen);
				scr->init(scr);
				if(scr->update)
					scr->update(scr);
				current_screen = scr;
				return;
			}
			scr = scr->next_screen;
		}
	}
}

void lcd_update(void) {
	if(current_screen->update)
		current_screen->update(current_screen);
	lcd_draw();
}
