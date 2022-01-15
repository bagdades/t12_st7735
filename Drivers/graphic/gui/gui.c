/*
 * =====================================================================================
 *
 *       Filename:  gui.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  31.12.21 20:40:05
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "gui.h"
#include "lcd.h"
#include "main_screen.h"
#include "set_tip_screen.h"
#include "settings_screen.h"
#include "set_temp_screen.h"
#include "splash.h"
#include "st7735.h"
#include "screen.h"
#include "widgets.h"

extern ucg_t ucg;

void guiInit(void) {
	screen_t *scr = lcd_addScreen(screen_splash);
	splash_setup(scr);
	scr = lcd_addScreen(screen_main);
	main_screen_setup(scr);
	scr = lcd_addScreen(screen_settings);
	settings_screen_setup(scr);
	scr = lcd_addScreen(screen_set_temp);
	set_temp_screen_setup(scr);
	scr = lcd_addScreen(screen_set_tip);
	set_tip_screen_setup(scr);
	/* ucg.x_dim = ST7735_WIDTH; */
	/* ucg.y_dim = ST7735_HEIGHT; */
	/* ucg_Init(); */
}
