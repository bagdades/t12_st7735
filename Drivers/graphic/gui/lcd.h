#ifndef  lcd_INC
#define  lcd_INC

#include "rotary_encoder.h"
#include "screen.h"

//Prototypes
screen_t *lcd_addScreen(uint8_t index);
void lcd_draw(void);
// void lcd_init(void);
void lcd_init(RE_Rotation_t (*GetData)(RE_State_t*), RE_State_t *State);
void lcd_processInput(void);
void lcd_update(void);

#endif   /* ----- #ifndef lcd_INC  ----- */
