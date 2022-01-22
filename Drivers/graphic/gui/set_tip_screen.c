/*
 * =====================================================================================
 *
 *       Filename:  set_tip_screen.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  15.01.22 22:03:44
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "set_tip_screen.h"
#include "settings.h"

extern ucg_t ucg;
uint16_t m_tip= 0;
/* static char *tipstr[3]; */
static char *tipstr[sizeof(systemSettings.ironTips) / sizeof(systemSettings.ironTips[0])];
static multi_option_widget_t *tipsWidget = NULL;

static void *gTip() {
	m_tip = systemSettings.currentTip;
	return &m_tip;
}

void sTip(uint16_t *value) {
	m_tip = *value;
	systemSettings.currentTip = m_tip;
	saveSettings();
	/* setCurrentTip(m_tip); */
}

static int set_tip_processInput(screen_t * scr, RE_Rotation_t input, RE_State_t *state) {
	if(input == Click)
		return screen_main;
	return default_screenProcessInput(scr, input, state);
}

/* static int tipProcessInput(widget_t *w, RE_Rotation_t r, RE_State_t *s) { */
/*   if (r == Click) */
/*     return screen_main; */
/*   switch (w->editable.selectable.state) { */
/* 	  case widget_selected: */
/* 		  if(r == Click && getCurrentMode() != mode_set) */
/* 			  #<{(| setCurrentMode(mode_set); |)}># */
/* 		  break; */
/* 	  case widget_edit: */
/* 		  #<{(| if(r != Rotate_Nothing && r != LongClick && getCurrentMode() != mode_set) |)}># */
/* 		  if(r != Rotate_Nothing && r != LongClick) */
/* 		  {  */
/* 			  #<{(| setCurrentMode(mode_set); |)}># */
/* 			  return -1; */
/* 		  } */
/* 		  break; */
/* 	  default: */
/* 		  break; */
/*   } */
/*   return default_widgetProcessInput(w, r, s); */
/* } */

void set_tip_screen_setup(screen_t *scr) {
	uint8_t hcenter = ucg_GetXDim(&ucg) / 2;
	uint8_t swidth;
	for(int x = 0; x < sizeof(systemSettings.ironTips) / sizeof(systemSettings.ironTips[0]); ++x) {
		tipstr[x] = systemSettings.ironTips[x].name;
	}
	/* tipstr[0] = "K"; */
	/* tipstr[1] = "BL"; */
	/* tipstr[2] = "KU"; */
	scr->draw = &default_screenDraw;
	scr->processInput = &set_tip_processInput;
	scr->init = &default_init;
	scr->update = &default_screenUpdate;
	widget_t *widget = screen_addWidget(scr);
	widgetDefaultsInit(widget, widget_label);
	char *s = "Tip Select";
	strcpy(widget->displayString, s);
	widget->font = &Font_11x18;
	swidth = ucg_GetStrWidth(&ucg, widget->font, s);
	widget->posX = hcenter - (swidth  / 2);
	widget->posY = 1;
	widget->fcolor = C_YELLOW;
	widget->reservedChars = 10;
	widget->draw = default_widgetDraw;

	// tips
	widget = screen_addWidget(scr);
	widgetDefaultsInit(widget, widget_multi_option);
	widget->posX = 48;
	widget->posY = 35;
	widget->font = &Font_16x26;
	widget->fcolor = C_CYAN;
	/* widget->multiOptionWidget.editable.selectable.processInput = (int (*)(widget_t *, RE_Rotation_t, RE_State_t *)) &tipProcessInput; */
	widget->multiOptionWidget.editable.inputData.getData = &gTip;
	widget->multiOptionWidget.editable.inputData.number_of_dec = 0;
	widget->multiOptionWidget.editable.inputData.type = field_uinteger16;
	widget->multiOptionWidget.editable.big_step = 0;
	widget->multiOptionWidget.editable.step = 0;
	widget->multiOptionWidget.editable.selectable.tab = 0;
	widget->multiOptionWidget.editable.selectable.state = widget_edit;
	widget->multiOptionWidget.editable.setData = (void (*)(void *))&sTip;

	widget->reservedChars = 3;

	widget->multiOptionWidget.options = tipstr;
	/* widget->multiOptionWidget.numberOfOptions = 3; */
	widget->multiOptionWidget.numberOfOptions = systemSettings.currentNumberOfTips;
	tipsWidget = &widget->multiOptionWidget;
	widget->multiOptionWidget.currentOption = 0;
	widget->multiOptionWidget.defaultOption = 0;
	widget->refresh = refresh_triggered;
	scr->current_widget = widget;
}
