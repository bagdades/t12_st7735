/*
 * =====================================================================================
 *
 *       Filename:  splash.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02.01.22 17:53:42
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "splash.h"
#include "fonts.h"
#include "st7735.h"
#include "stm32f1xx_hal.h"
#define SPLASH_TIMEOUT 2000
extern ucg_t ucg;

static int splash_processInput(screen_t * scr, RE_Rotation_t input, RE_State_t *);
static void splash_init(screen_t * scr);
static void splash_draw(screen_t * scr);

static uint32_t splash_time;
void splash_setup(screen_t * scr) {

	scr->processInput = &splash_processInput;
	scr->init = &splash_init;
	scr->draw = &splash_draw;
	scr->update = NULL;
}

static int splash_processInput(screen_t * scr, RE_Rotation_t input, RE_State_t *state) {
	if(HAL_GetTick() - splash_time > SPLASH_TIMEOUT)
		return screen_main;
		/* return screen_set_temp; */
		/* return screen_settings; */
	return -1;
}

static void splash_init(screen_t * scr) {
	splash_time = HAL_GetTick();
}

static void splash_draw(screen_t * scr) {
	ucg_SetFont(&ucg, &Font_16x26);
	ucg_SetForeColor(&ucg, C_RED);
	ucg_WriteString(&ucg, 29, 29, "Hello!");
}
