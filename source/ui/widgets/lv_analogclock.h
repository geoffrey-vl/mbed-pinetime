/**
 * @file lv_analogclock.h
 *
 */

#ifndef LV_ANALOGCLOCK_H
#define LV_ANALOGCLOCK_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#ifdef LV_CONF_INCLUDE_SIMPLE
#include "lv_conf.h"
#else
#include "../../../lv_conf.h"
#endif

#if LV_USE_ANALOGCLOCK != 0

/*Testing of dependencies*/
#if LV_USE_LMETER == 0
#error "lv_analogclock: lv_lmeter is required. Enable it in lv_conf.h (LV_USE_LMETER  1) "
#endif

#include "../lv_core/lv_obj.h"
#include "lv_lmeter.h"
#include "lv_label.h"
#include "lv_line.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/*Data of analock clock*/
typedef struct
{
    lv_lmeter_ext_t lmeter;           /*Ext. of ancestor*/
    /*New data for this type */
    int16_t * values;                 /*Array of the set values (for needles) */
    const lv_color_t * needle_colors; /*Color of the needles (lv_color_t my_colors[needle_num])*/
    uint8_t needle_count;             /*Number of needles*/
} lv_analogclock_ext_t;

typedef struct 
{
    int tm_hour;		/* Hours.	[0-23] */
    int tm_min;			/* Minutes.	[0-59] */
    int tm_sec;			/* Seconds.	[0-60] (1 leap second) */ 
} lv_analogclock_time_t;


/*Styles*/
enum {
    LV_ANALOGCLOCK_STYLE_MAIN,
};
typedef uint8_t lv_analogclock_style_t;

enum {
    LV_ANALOGCLOCK_NEEDLE_HOURS=0,
    LV_ANALOGCLOCK_NEEDLE_MINUTES=1,
    LV_ANALOGCLOCK_NEEDLE_SECONDS=2
};
typedef uint8_t lv_analogclock_needleid_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Create a analogclock objects
 * @param par pointer to an object, it will be the parent of the new analogclock
 * @param copy pointer to a analogclock object, if not NULL then the new object will be copied from it
 * @return pointer to the created analogclock
 */
lv_obj_t * lv_analogclock_create(lv_obj_t * par, const lv_obj_t * copy);

/*=====================
 * Setter functions
 *====================*/

/**
 * Set the value of a needle
 * @param analogclock pointer to a analogclock
 * @param time the time of the day
 */
void lv_analogclock_set_value(lv_obj_t * analogclock, const lv_analogclock_time_t time);

/**
 * Set the color of a needle
 * @param analogclock pointer to analogclock object
 * @param colors the colors of the needles
 */
void lv_analogclock_set_needle_colors(lv_obj_t * analogclock, const lv_color_t colors[]);

/**
 * Set the styles of a analogclock
 * @param analogclock pointer to a analogclock object
 * @param type which style should be set (can be only `LV_ANALOGCLOCK_STYLE_MAIN`)
 * @param style set the style of the analogclock
 *  */
static inline void lv_analogclock_set_style(lv_obj_t * analogclock, lv_analogclock_style_t type, lv_style_t * style)
{
    (void)type; /*Unused*/
    lv_obj_set_style(analogclock, style);
}

/*=====================
 * Getter functions
 *====================*/

/**
 * Get the value of a needle
 * @param analogclock pointer to analogclock object
 * @return the time of the day
 */
lv_analogclock_time_t lv_analogclock_get_value(const lv_obj_t * analogclock);

/**
 * Get the scale number of a analogclock
 * @param analogclock pointer to a analogclock object
 * @return number of the scale units
 */
static inline uint16_t lv_analogclock_get_line_count(const lv_obj_t * analogclock)
{
    return lv_lmeter_get_line_count(analogclock);
}

/**
 * Get the scale angle of a analogclock
 * @param analogclock pointer to a analogclock object
 * @return angle of the scale
 */
static inline uint16_t lv_analogclock_get_scale_angle(const lv_obj_t * analogclock)
{
    return lv_lmeter_get_scale_angle(analogclock);
}

/**
 * Get the style of a analogclock
 * @param analogclock pointer to a analogclock object
 * @param type which style should be get (can be only `LV_ANALOGCLOCK_STYLE_MAIN`)
 * @return pointer to the analogclock's style
 */
static inline const lv_style_t * lv_analogclock_get_style(const lv_obj_t * analogclock, lv_analogclock_style_t type)
{
    (void)type; /*Unused*/
    return lv_obj_get_style(analogclock);
}

/**********************
 *      MACROS
 **********************/

#endif /*LV_USE_ANALOGCLOCK*/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_ANALOGCLOCK_H*/
