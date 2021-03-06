/*
 * =====================================================================================
 *
 *       Filename:  set_temp_screen.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  15.01.22 20:34:20
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "set_temp_screen.h"
#include "screen.h"
#include "st7735.h"
#include "widgets.h"
#include "iron.h"
#include <string.h>

extern ucg_t ucg;
uint16_t m_temp;
/* uint16_t m_temp = 290; */

static void *getTemp() {
  m_temp = getSetTemperature();
  return &m_temp;
}

void setTemp(uint16_t *value) {
  m_temp = *value;
  setSetTemperature(m_temp);
}

static int set_temp_processInput(screen_t * scr, RE_Rotation_t input, RE_State_t *state) {
	if(input == Click)
		return screen_main;
	return default_screenProcessInput(scr, input, state);
}

static int tempProcessInput(widget_t *w, RE_Rotation_t r, RE_State_t *s) {
  if (r == Click)
    return screen_main;
  switch (w->editable.selectable.state) {
  case widget_selected:
    if(r == Click && getCurrentMode() != mode_set)
    	setCurrentMode(mode_set);
    break;
  case widget_edit:
     if(r != Rotate_Nothing && r != LongClick && getCurrentMode() != mode_set) { 
    	setCurrentMode(mode_set);
    	return -1;
    }
    break;
  default:
    break;
  }
  return default_widgetProcessInput(w, r, s);
}

void set_temp_screen_setup(screen_t *scr) {
	/* uint8_t hcenter = ucg_GetXDim(&ucg) / 2; */
	/* uint8_t swidth; */
	scr->draw = &default_screenDraw;
	scr->processInput = &set_temp_processInput;
	scr->init = &default_init;
	scr->update = &default_screenUpdate;
	widget_t *widget = screen_addWidget(scr);
	widgetDefaultsInit(widget, widget_label);
	char *s = "SET TEMP";
	strcpy(widget->displayString, s);
	widget->font = &font_18m;
	/* swidth = ucg_GetStrWidth(&ucg, widget->font, s); */
	widget->posX = 0;
	widget->posY = 1;
	widget->width = ucg_GetXDim(&ucg);
	widget->displayWidget.textAlign = align_center;
	widget->fcolor = C_YELLOW;
	widget->reservedChars = 8;
	widget->draw = default_widgetDraw;

	widget = screen_addWidget(scr);
	widgetDefaultsInit(widget, widget_editable);
	widget->editable.selectable.processInput = (int (*)(widget_t *, RE_Rotation_t, RE_State_t *)) &tempProcessInput;
	widget->posX = 1;
	widget->posY = 25;
	widget->font = &font_53;
	widget->fcolor = C_CYAN;
	widget->editable.inputData.getData = &getTemp;
	widget->editable.inputData.number_of_dec = 0;
	widget->editable.inputData.type = field_uinteger16;
	widget->editable.big_step = 10;
	widget->editable.step = 5;
	widget->editable.selectable.tab = 1;
	widget->editable.setData = (void (*)(void *)) & setTemp;
	widget->reservedChars = 3;
	widget->editable.selectable.state = widget_edit;
	widget->refresh = refresh_triggered;
	scr->current_widget = widget;

	widget = screen_addWidget(scr);
	widgetDefaultsInit(widget, widget_bmp);
	widget->displayBmp.img = &cel;
	widget->posX = ucg_GetXDim(&ucg) - widget->displayBmp.img->width;
	widget->posY = ucg_GetYDim(&ucg) - widget->displayBmp.img->height - 2;
	widget->draw = &default_widgetDraw;
	widget->fcolor = C_CYAN;
}
