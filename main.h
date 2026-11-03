/* 
 * File:   main.h
 * Author: LALITH
 *
 * Created on 22 May, 2025, 8:05 AM
 */

#ifndef MAIN_H
#define	MAIN_H

#include <xc.h>
#include "adc.h"
#include "clcd.h"
#include "digital_keypad.h"
#include "i2c.h"
#include "ds1307.h"
#include "car_black_box.h"
#include <string.h>
#include "EEprom.h"
#include "timers.h"
#include "uart.h"

#define DASHBOARD_SCREEN   0x00
#define LOGIN_SCREEN       0x01
#define MAIN_MENU_SCREEN   0x02
#define VIEW_LOG_SCREEN    0x03
#define CLEAR_LOG_SCREEN   0x04
#define DOWNLOAD_LOG_SCREEN 0x05
#define CHANGE_PASSWORD_SCREEN   0x06
#define SET_TIME_SCREEN    0x07

#define RESET_PASSWORD     0x11
#define RESET_NOTHING      0x22
#define RETURN_BACK        0x33
#define LOGIN_SUCCESS      0x44
#define RESET_MENU         0x55
#define RESET_VIEW_LOG_POS 0x66
#define RESET_DOWNLOAD_LOG 0x77
#define RESET_TIME         0x88
#define RESET_MEMORY       0x99
#define RESET_MAIN_MENU_SCREEN 0x10

#define TASK_SUCCESS  0xAA
#define TASK_FAILURE  0xBB




#endif	/* MAIN_H */

