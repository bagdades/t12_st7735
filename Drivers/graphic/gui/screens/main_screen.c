/*
 * =====================================================================================
 *
 *       Filename:  main_screen.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  31.12.21 22:35:14
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "main_screen.h"
#include "fonts.h"
#include "rotary_encoder.h"
#include "iron.h"
#include "screen.h"
#include "st7735.h"
#include "stm32f1xx_hal.h"
#include "widgets.h"
#include "settings.h"
#include "fonts.h"
#include "tempsensors.h"


#define NO_IRON_ADC 5000
/* static uint8_t hasIron = 1; */
extern uint16_t m_tip;
static uint16_t m_mode = 0;
extern uint16_t m_temp;
static char *modestr[] = {"STB:", "BOO:", "SLP:", "SET:"};
/* static char *tipstr[4]; */
static char *tipstr[sizeof(systemSettings.ironTips) / sizeof(systemSettings.ironTips[0])];
static multi_option_widget_t *tipsWidget = NULL;
static widget_t *ironTempWidget;
static widget_t *ironTempLabelWidget;
extern ucg_t ucg;
/* static widget_t *noIronWidget; */
static uint16_t temp = 200;
/* static uint16_t temp1 = 200; */
/* static uint16_t power = 100; */

int boostOn(widget_t *w) {
	setCurrentMode(mode_boost);
	m_mode = 1;
	return -1;
}

static void * main_screen_getIronTemp() {
	temp = readTipTemperatureCompensated(0);
	/* HAL_Delay(500); */
	return &m_temp;
	/* return &temp; */
}

static void *getTemp() {
  m_temp = getSetTemperature();
  return &m_temp;
}

void setMode(uint16_t *value) {
	m_mode = *value;
	setCurrentMode((iron_mode_t)m_mode);
}

void * getMode() {
	m_mode = getCurrentMode();
	return &m_mode;
}

void setTip(uint16_t *value) {
	m_tip = *value;
	systemSettings.currentTip = m_tip;
	saveSettings();
	setCurrentTip(m_tip);
}

void * getTip() {
	m_tip = systemSettings.currentTip;
	return &m_tip;
}

/* static void * main_screen_getAmbTemp() { */
/* 	#<{(| temp = readColdJunctionSensorTemp_mC() / 100; |)}># */
/* 	temp1 = 50; */
/* 	return &temp1; */
/* } */

static void main_screen_init(screen_t *scr) {
	tipsWidget->numberOfOptions = systemSettings.currentNumberOfTips;
	default_init(scr);
}

void main_screenUpdate(screen_t *scr) {
	/* uint16_t t = iron_temp_adc_avg; */
	/* if((t > NO_IRON_ADC) && hasIron) { */
	/* 	UG_FillScreen(C_BLACK); */
	/* 	ironTempLabelWidget->enabled = 0; */
	/* 	ironTempWidget->enabled = 0; */
	/* 	noIronWidget->enabled = 1; */
	/* 	buzzer_alarm_start(); */
	/* 	hasIron = 0; */
	/* } */
	/* else if((t <= NO_IRON_ADC) && !hasIron){ */
	/* 	UG_FillScreen(C_BLACK); */
		/* ironTempLabelWidget->enabled = 1; */
		/* ironTempWidget->enabled = 1; */
		/* noIronWidget->enabled = 0; */
	/* 	buzzer_alarm_stop(); */
	/* 	hasIron = 1; */
	/* } */
	default_screenUpdate(scr);

}

/* static void * main_screen_getIronPower() { */
/* 	#<{(| temp = getCurrentPower(); |)}># */
/* 	#<{(| power = 100; |)}># */
/* 	return &power; */
/* } */

int main_screenProcessInput(screen_t * scr, RE_Rotation_t input, RE_State_t *state) {
	uint8_t r = input;
	int ret =  1;
	switch (r) {
		case Click:
			ret = screen_set_temp;
			setCurrentMode(mode_set);
			return ret;
		case LongClick:
			ret = screen_settings;
			return ret;
		case Rotate_Increment_while_click:
			ret = screen_set_tip;
			return ret;
	}
	return default_screenProcessInput(scr, input, state);
}

void main_screen_setup(screen_t *scr) {
	for(int x = 0; x < sizeof(systemSettings.ironTips) / sizeof(systemSettings.ironTips[0]); ++x) {
		tipstr[x] = systemSettings.ironTips[x].name;
	}
	/* tipstr[0] = "K"; */
	/* tipstr[1] = "BL"; */
	/* tipstr[2] = "KU"; */
	scr->draw = &default_screenDraw;
	scr->processInput = &main_screenProcessInput;
	/* scr->processInput = &default_screenProcessInput; */
	scr->init = &main_screen_init;
	scr->update = &main_screenUpdate;

	//iron tip temperature display
	widget_t *widget = screen_addWidget(scr);
	widgetDefaultsInit(widget, widget_display);
	widget->posX = 1;
	widget->posY = 25;
	widget->font = &font_53;
	widget->displayWidget.getData = &main_screen_getIronTemp;
	widget->displayWidget.number_of_dec = 0;
	widget->displayWidget.type = field_uinteger16;
	widget->reservedChars = 3;
	widget->fcolor = C_GREEN;
	widget->refresh = refresh_triggered;
	ironTempWidget = widget;

	//power display
	/* widget = screen_addWidget(scr); */
	/* widgetDefaultsInit(widget, widget_display); */
	/* widget->posX = 114; */
	/* widget->posY = 1; */
	/* widget->font = &Font_11x18; */
	/* widget->displayWidget.getData = &main_screen_getIronPower; */
	/* widget->displayWidget.number_of_dec = 0; */
	/* widget->displayWidget.type = field_uinteger16; */
	/* widget->reservedChars = 3; */
	/* widget->fcolor = C_GREEN; */
	/* widget->refresh = refresh_triggered; */

	//power percentage symbol
	/* widget = screen_addWidget(scr); */
	/* widgetDefaultsInit(widget, widget_label); */
	/* char *s = "%"; */
	/* strcpy(widget->displayString, s); */
	/* widget->posX = 148; */
	/* widget->posY = 1; */
	/* widget->font = &Font_11x18; */
	/* widget->reservedChars = 1; */

	//ºC label next to iron tip temperature
	widget = screen_addWidget(scr);
	widgetDefaultsInit(widget, widget_bmp);
	widget->displayBmp.img = &cel;
	widget->posX = ucg_GetXDim(&ucg) - widget->displayBmp.img->width;
	widget->posY = ucg_GetYDim(&ucg) - widget->displayBmp.img->height - 2;
	widget->draw = &default_widgetDraw;
	widget->fcolor = C_GREEN;
	ironTempLabelWidget = widget;

	/* widget = screen_addWidget(scr); */
	/* widgetDefaultsInit(widget, widget_label); */
	/* strcpy(widget->displayString, "NO IRON"); */
	/* widget->posX = 20 - 3; */
	/* widget->posY = 20 + 5 - 2; */
	/* widget->font = &Font_11x18; */
	/* widget->reservedChars = 7; */
	/* widget->draw = &default_widgetDraw; */
	/* noIronWidget = widget; */
	/* widget->enabled = 0; */


	//Ambient temperature display
	/* widget = screen_addWidget(scr); */
	/* widgetDefaultsInit(widget, widget_display); */
	/* widget->posX = 115; */
	/* widget->posY = 65; */
	/* widget->font = &Font_11x18; */
	/* widget->displayWidget.getData = &main_screen_getAmbTemp; */
	/* widget->displayWidget.number_of_dec = 1; */
	/* widget->displayWidget.type = field_uinteger16; */
	/* widget->reservedChars = 4; */
	/* widget->fcolor = C_BLUE; */
	/* widget->refresh = refresh_triggered; */

	// mode
	widget = screen_addWidget(scr);
	widgetDefaultsInit(widget, widget_multi_option);
	widget->posX = 1;
	widget->posY = 1;
	widget->font = &font_18m;
	widget->fcolor = C_YELLOW;
	widget->multiOptionWidget.editable.inputData.getData = &getMode;
	widget->multiOptionWidget.editable.inputData.number_of_dec = 0;
	widget->multiOptionWidget.editable.inputData.type = field_uinteger16;
	widget->multiOptionWidget.editable.big_step = 0;
	widget->multiOptionWidget.editable.step = 0;
	widget->multiOptionWidget.editable.selectable.tab = 0;
	widget->multiOptionWidget.editable.setData = (void (*)(void *))&setMode;
	widget->refresh = refresh_triggered;
	widget->multiOptionWidget.editable.selectable.state = widget_edit;

	widget->reservedChars = 4;

	widget->multiOptionWidget.options = modestr;
	widget->multiOptionWidget.numberOfOptions = 4;
	widget->multiOptionWidget.currentOption = 0;
	widget->multiOptionWidget.defaultOption = 0;
	scr->current_widget = widget;

	// tip temperature setpoint
	widget = screen_addWidget(scr);
	widgetDefaultsInit(widget, widget_display);
	/* widgetDefaultsInit(widget, widget_editable); */
	/* widget->editable.selectable.processInput = (int (*)(widget_t*, RE_Rotation_t, RE_State_t *))&tempProcessInput; */
	widget->posX = 48;
	widget->posY = 1;
	widget->font = &font_18m;
	widget->fcolor = C_MAGENTA;
	widget->displayWidget.getData = &getTemp;
	widget->displayWidget.number_of_dec = 0;
	widget->displayWidget.type = field_uinteger16;
	widget->reservedChars = 3;
	widget->refresh = refresh_triggered;
	/* widget->editable.inputData.getData = &getTemp; */
	/* widget->editable.inputData.number_of_dec = 0; */
	/* widget->editable.inputData.type = field_uinteger16; */
	/* widget->editable.big_step = 10; */
	/* widget->editable.step = 1; */
	/* widget->editable.selectable.tab = 1; */
	/* widget->editable.setData = (void (*)(void *))&setTemp; */
	/* widget->reservedChars = 3; */
	/* widget->editable.selectable.state = widget_edit; */
	/* widget->editable.selectable.longPressAction = &boostOn; */
	/* widget->refresh = refresh_triggered; */
	/* scr->current_widget = widget; */

	// tips
	widget = screen_addWidget(scr);
	widgetDefaultsInit(widget, widget_multi_option);
	widget->posX = 115;
	widget->posY = 1;
	widget->width = 44;
	widget->font = &font_18m;
	widget->fcolor = C_CYAN;
	widget->multiOptionWidget.editable.inputData.getData = &getTip;
	/* widget->multiOptionWidget.editable.inputData.textAlign = align_center; */
	widget->multiOptionWidget.editable.inputData.number_of_dec = 0;
	widget->multiOptionWidget.editable.inputData.type = field_uinteger16;
	widget->multiOptionWidget.editable.inputData.textAlign = align_right;
	widget->reservedChars = 4;

	widget->multiOptionWidget.options = tipstr;
	/* widget->multiOptionWidget.numberOfOptions = 3; */
	widget->multiOptionWidget.numberOfOptions = systemSettings.currentNumberOfTips;
	tipsWidget = &widget->multiOptionWidget;
	widget->multiOptionWidget.currentOption = 0;
	widget->multiOptionWidget.defaultOption = 0;
	widget->inverted = true;
}
