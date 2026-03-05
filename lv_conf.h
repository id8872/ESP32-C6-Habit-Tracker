/**
 * lv_conf.h
 * Place this file next to your .ino sketch file
 * OR in your Arduino/libraries folder
 */

#if 1  /* Set it to "1" to enable content */

#ifndef LV_CONF_H
#define LV_CONF_H

#include <stdint.h>

/*====================
   COLOR SETTINGS
 *====================*/
#define LV_COLOR_DEPTH 16
#define LV_COLOR_16_SWAP 1   /* swap bytes for SPI displays */

/*====================
   MEMORY SETTINGS
 *====================*/
#define LV_MEM_CUSTOM 0
#define LV_MEM_SIZE   (48 * 1024U)   /* 48 kB heap */

/*====================
   HAL SETTINGS
 *====================*/
#define LV_TICK_CUSTOM 1
#define LV_TICK_CUSTOM_INCLUDE "Arduino.h"
#define LV_TICK_CUSTOM_SYS_TIME_EXPR (millis())

/*====================
   LOGGING
 *====================*/
#define LV_USE_LOG 0

/*====================
   FONT USAGE
 *====================*/
#define LV_FONT_MONTSERRAT_8  0
#define LV_FONT_MONTSERRAT_10 0
#define LV_FONT_MONTSERRAT_12 0
#define LV_FONT_MONTSERRAT_14 1   /* activity name labels */
#define LV_FONT_MONTSERRAT_16 0
#define LV_FONT_MONTSERRAT_18 0
#define LV_FONT_MONTSERRAT_20 0
#define LV_FONT_MONTSERRAT_22 0
#define LV_FONT_MONTSERRAT_24 0
#define LV_FONT_MONTSERRAT_26 0
#define LV_FONT_MONTSERRAT_28 0
#define LV_FONT_MONTSERRAT_30 0
#define LV_FONT_MONTSERRAT_32 0
#define LV_FONT_MONTSERRAT_36 1   /* icon symbols */
#define LV_FONT_MONTSERRAT_38 0
#define LV_FONT_MONTSERRAT_40 0
#define LV_FONT_MONTSERRAT_42 0
#define LV_FONT_MONTSERRAT_44 0
#define LV_FONT_MONTSERRAT_46 0
#define LV_FONT_MONTSERRAT_48 0

#define LV_FONT_DEFAULT &lv_font_montserrat_14

/*====================
   WIDGET USAGE
 *====================*/
#define LV_USE_ARC        1
#define LV_USE_BTN        1
#define LV_USE_BTNMATRIX  1
#define LV_USE_CANVAS     0
#define LV_USE_CHECKBOX   0
#define LV_USE_DROPDOWN   1
#define LV_USE_IMG        1
#define LV_USE_LABEL      1
#define LV_USE_LINE       0
#define LV_USE_ROLLER     0
#define LV_USE_SLIDER     0
#define LV_USE_SWITCH     0
#define LV_USE_TEXTAREA   1
#define LV_USE_TABLE      0
#define LV_USE_BAR        0
#define LV_USE_CHART      0

/*====================
   ANIMATION
 *====================*/
#define LV_USE_ANIMATION 1

/*====================
   ASYNC
 *====================*/
#define LV_USE_ASYNC_CALL 1

/*====================
   SCREEN ANIMATION
 *====================*/
#define LV_USE_FLEX  1
#define LV_USE_GRID  0

#endif /* LV_CONF_H */
#endif /* Enable content */
