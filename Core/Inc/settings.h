/*
 * settings.h
 *
 *  Created on: Sep 13, 2017
 *      Author: jose
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

#include "stm32f1xx_hal.h"
// #include "pid.h"
#include "iron.h"
// #include "tempsensors.h"
#include <stdint.h>
#include "stm32f1xx_hal_flash.h"

#define SETTINGSVERSION 2 /*Change this if you change the struct below to prevent people getting out of sync*/

struct systemSettings {
	uint8_t version;				//Used to track if a reset is needed on firmware upgrade
	uint8_t contrast;
	ironBoost_t boost;
	ironSleep_t sleep;
	tipData ironTips[10];
	uint8_t currentNumberOfTips;
	uint8_t currentTip;
	uint16_t setTemperature;
} systemSettings;

__attribute__((aligned(4))) typedef struct{
  int8_t        coefficient;          // Filter normally applied
  int8_t        counter;              // Counter for threshold limit
  int8_t        min;                  // Minimum filtering when decreasing
  int8_t        step;                 // Start decreasing the filter coefficient, assume it's a fast temperature change, so provide faster response
  int8_t        count_limit;          // Count the spikes, if exceeding this limit, start reducing the filter coefficient.
  uint16_t      threshold;            // Base noise limit, if diff exceeds this limit, trigger threshold limit and start decreasing filtering
  uint16_t      reset_threshold;      // Threshold for completely resetting the filter
}filter_t;

void saveSettings();
void restoreSettings();
void resetSettings();

#endif /* SETTINGS_H_ */
