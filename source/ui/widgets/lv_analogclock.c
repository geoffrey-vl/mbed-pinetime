/**
 * @file lv_analogclock.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_analogclock.h"
#if LV_USE_ANALOGCLOCK != 0

#include "../lv_core/lv_debug.h"
#include "../lv_draw/lv_draw.h"
#include "../lv_themes/lv_theme.h"
#include "../lv_misc/lv_txt.h"
#include "../lv_misc/lv_math.h"
#include "../lv_misc/lv_utils.h"
#include <stdio.h>
#include <string.h>

/*********************
 *      DEFINES
 *********************/
#define LV_OBJX_NAME "lv_analogclock"

#define LV_ANALOGCLOCK_DEF_NEEDLE_COLOR LV_COLOR_RED
#define LV_ANALOGCLOCK_DEF_LINE_COUNT 61
#define LV_ANALOGCLOCK_DEF_ANGLE 360
#define LV_ANALOGCLOCK_INTERPOLATE_SHIFT 5 /*Interpolate the needle drawing between to degrees*/
#define LV_ANALOGCLOCK_INTERPOLATE_MASK 0x1F
#define LV_ANALOGCLOCK_NEEDLE_HOURS_MIN_VALUE 0
#define LV_ANALOGCLOCK_NEEDLE_HOURS_MAX_VALUE 23
#define LV_ANALOGCLOCK_NEEDLE_MINUTES_MIN_VALUE 0
#define LV_ANALOGCLOCK_NEEDLE_MINUTES_MAX_VALUE 59
#define LV_ANALOGCLOCK_NEEDLE_SECONDS_MIN_VALUE 0
#define LV_ANALOGCLOCK_NEEDLE_SECONDS_MAX_VALUE 59

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static bool lv_analogclock_design(lv_obj_t * analogclock, const lv_area_t * mask, lv_design_mode_t mode);
static lv_res_t lv_analogclock_signal(lv_obj_t * analogclock, lv_signal_t sign, void * param);
static void lv_analogclock_draw_needle(lv_obj_t * analogclock, const lv_area_t * mask);

/**
 * Set the number of needles
 * @param analogclock pointer to analogclock object
 * @param needle_cnt new count of needles
 * @param colors an array of colors for needles (with 'num' elements)
 */
static void lv_analogclock_set_needle_count(lv_obj_t * analogclock, uint8_t needle_cnt, const lv_color_t colors[]);

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_design_cb_t ancestor_design;
static lv_signal_cb_t ancestor_signal;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Create a analogclock objects
 * @param par pointer to an object, it will be the parent of the new analogclock
 * @param copy pointer to a analogclock object, if not NULL then the new object will be copied from it
 * @return pointer to the created analogclock
 */
lv_obj_t * lv_analogclock_create(lv_obj_t * par, const lv_obj_t * copy)
{
    LV_LOG_TRACE("analogclock create started");

    /*Create the ancestor analogclock*/
    lv_obj_t * new_analogclock = lv_lmeter_create(par, copy);
    LV_ASSERT_MEM(new_analogclock);
    if(new_analogclock == NULL) return NULL;

    /*Allocate the analogclock type specific extended data*/
    lv_analogclock_ext_t * ext = lv_obj_allocate_ext_attr(new_analogclock, sizeof(lv_analogclock_ext_t));
    LV_ASSERT_MEM(ext);
    if(ext == NULL) return NULL;

    /*Initialize the allocated 'ext' */
    ext->needle_count  = 0;
    ext->values        = NULL;
    ext->needle_colors = NULL;
    if(ancestor_signal == NULL) ancestor_signal = lv_obj_get_signal_cb(new_analogclock);
    if(ancestor_design == NULL) ancestor_design = lv_obj_get_design_cb(new_analogclock);

    /*The signal and design functions are not copied so set them here*/
    lv_obj_set_signal_cb(new_analogclock, lv_analogclock_signal);
    lv_obj_set_design_cb(new_analogclock, lv_analogclock_design);

    /*Init the new analogclock analogclock*/
    if(copy == NULL) {
        lv_lmeter_set_scale(new_analogclock, LV_ANALOGCLOCK_DEF_ANGLE, LV_ANALOGCLOCK_DEF_LINE_COUNT);
        lv_analogclock_set_needle_count(new_analogclock, 3, NULL);
        lv_obj_set_size(new_analogclock, 2 * LV_DPI, 2 * LV_DPI);

        /*Set the default styles*/
        lv_theme_t * th = lv_theme_get_current();
        if(th) {
            lv_style_t* style = th->style.gauge;
            style->body.padding.left /= 2;
            style->body.padding.right /= 2;
            lv_analogclock_set_style(new_analogclock, LV_ANALOGCLOCK_STYLE_MAIN, style);
        } else {
            lv_analogclock_set_style(new_analogclock, LV_ANALOGCLOCK_STYLE_MAIN, &lv_style_pretty_color);
        }
    }
    /*Copy an existing analogclock*/
    else {
        lv_analogclock_ext_t * copy_ext = lv_obj_get_ext_attr(copy);
        lv_analogclock_set_needle_count(new_analogclock, copy_ext->needle_count, copy_ext->needle_colors);

        uint8_t i;
        for(i = 0; i < ext->needle_count; i++) {
            ext->values[i] = copy_ext->values[i];
        }
        /*Refresh the style with new signal function*/
        lv_obj_refresh_style(new_analogclock);
    }

    LV_LOG_INFO("analogclock created");

    return new_analogclock;
}

/*=====================
 * Setter functions
 *====================*/

/**
 * Set the number of needles
 * @param analogclock pointer to analogclock object
 * @param needle_cnt new count of needles
 * @param colors an array of colors for needles (with 'num' elements)
 */
void lv_analogclock_set_needle_count(lv_obj_t * analogclock, uint8_t needle_cnt, const lv_color_t colors[])
{
    LV_ASSERT_OBJ(analogclock, LV_OBJX_NAME);

    lv_analogclock_ext_t * ext = lv_obj_get_ext_attr(analogclock);

    if(ext->needle_count != needle_cnt) {
        if(ext->values != NULL) {
            lv_mem_free(ext->values);
            ext->values = NULL;
        }

        ext->values = lv_mem_realloc(ext->values, needle_cnt * sizeof(int16_t));
        LV_ASSERT_MEM(ext->values);
        if(ext->values == NULL) return;

        uint8_t n;
        for(n = ext->needle_count; n < needle_cnt; n++) {
            ext->values[n] = 0;
        }

        ext->needle_count = needle_cnt;
    }

    ext->needle_colors = colors;
    lv_obj_invalidate(analogclock);
}

/**
 * Set the value of a needle
 * @param analogclock pointer to a analogclock
 * @param time the time of the day
 */
void lv_analogclock_set_value(lv_obj_t * analogclock, const lv_analogclock_time_t time)
{
    LV_ASSERT_OBJ(analogclock, LV_OBJX_NAME);

    lv_analogclock_ext_t * ext = lv_obj_get_ext_attr(analogclock);

    if(time.tm_hour < LV_ANALOGCLOCK_NEEDLE_HOURS_MIN_VALUE || time.tm_hour > LV_ANALOGCLOCK_NEEDLE_HOURS_MAX_VALUE)
        return;

    if(time.tm_min < LV_ANALOGCLOCK_NEEDLE_MINUTES_MIN_VALUE || time.tm_min > LV_ANALOGCLOCK_NEEDLE_MINUTES_MAX_VALUE)
        return;

    if(time.tm_sec < LV_ANALOGCLOCK_NEEDLE_SECONDS_MIN_VALUE || time.tm_sec > LV_ANALOGCLOCK_NEEDLE_SECONDS_MAX_VALUE)
        return;

    ext->values[LV_ANALOGCLOCK_NEEDLE_HOURS] = time.tm_hour;
    ext->values[LV_ANALOGCLOCK_NEEDLE_MINUTES] = time.tm_min;
    ext->values[LV_ANALOGCLOCK_NEEDLE_SECONDS] = time.tm_sec;

    lv_obj_invalidate(analogclock);
}

/**
 * Set the color of a needle
 * @param analogclock pointer to analogclock object
 * @param colors the colors of the needles
 */
void lv_analogclock_set_needle_colors(lv_obj_t * analogclock, const lv_color_t colors[])
{
    LV_ASSERT_OBJ(analogclock, LV_OBJX_NAME);

    lv_analogclock_ext_t * ext = lv_obj_get_ext_attr(analogclock);

    ext->needle_colors = colors;
    lv_obj_invalidate(analogclock);
}

/*=====================
 * Getter functions
 *====================*/

/**
 * Get the value of a needle
 * @param analogclock pointer to analogclock object
 * @return the time of the day
 */
lv_analogclock_time_t lv_analogclock_get_value(const lv_obj_t * analogclock)
{
    LV_ASSERT_OBJ(analogclock, LV_OBJX_NAME);

    lv_analogclock_ext_t * ext = lv_obj_get_ext_attr(analogclock);

    lv_analogclock_time_t tm;
    tm.tm_hour = ext->values[LV_ANALOGCLOCK_NEEDLE_HOURS];
    tm.tm_min = ext->values[LV_ANALOGCLOCK_NEEDLE_MINUTES];
    tm.tm_sec = ext->values[LV_ANALOGCLOCK_NEEDLE_SECONDS];
    return tm;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * Handle the drawing related tasks of the analogclocks
 * @param analogclock pointer to an object
 * @param mask the object will be drawn only in this area
 * @param mode LV_DESIGN_COVER_CHK: only check if the object fully covers the 'mask_p' area
 *                                  (return 'true' if yes)
 *             LV_DESIGN_DRAW: draw the object (always return 'true')
 *             LV_DESIGN_DRAW_POST: drawing after every children are drawn
 * @param return true/false, depends on 'mode'
 */
static bool lv_analogclock_design(lv_obj_t * analogclock, const lv_area_t * mask, lv_design_mode_t mode)
{
    /*Return false if the object is not covers the mask_p area*/
    if(mode == LV_DESIGN_COVER_CHK) {
        return false;
    }
    /*Draw the object*/
    else if(mode == LV_DESIGN_DRAW_MAIN) {

        /* Store the real pointer because of 'lv_group'
         * If the object is in focus 'lv_obj_get_style()' will give a pointer to tmp style
         * and to the real object style. It is important because of style change tricks below*/
        const lv_style_t * style_ori_p = analogclock->style_p;
        const lv_style_t * style       = lv_obj_get_style(analogclock);
        lv_analogclock_ext_t * ext           = lv_obj_get_ext_attr(analogclock);

        /*Draw the ancestor line meter with max value to show the rainbow like line colors*/
        uint16_t line_cnt_tmp = ext->lmeter.line_cnt;
        ancestor_design(analogclock, mask, mode); /*To draw lines*/

        /*Temporally modify the line meter to draw longer lines where labels are*/
        lv_style_t style_tmp;
        lv_style_copy(&style_tmp, style);
        ext->lmeter.line_cnt         = 13;
        style_tmp.body.padding.left  = style_tmp.body.padding.left * 2;  /*Longer lines*/
        style_tmp.body.padding.right = style_tmp.body.padding.right * 2; /*Longer lines*/
        analogclock->style_p               = &style_tmp;

        ancestor_design(analogclock, mask, mode); /*To draw lines*/

        ext->lmeter.line_cnt = line_cnt_tmp; /*Restore the parameters*/
        analogclock->style_p       = style_ori_p;  /*Restore the ORIGINAL style pointer*/

        lv_analogclock_draw_needle(analogclock, mask);

    }
    /*Post draw when the children are drawn*/
    else if(mode == LV_DESIGN_DRAW_POST) {
        ancestor_design(analogclock, mask, mode);
    }

    return true;
}

/**
 * Signal function of the analogclock
 * @param analogclock pointer to a analogclock object
 * @param sign a signal type from lv_signal_t enum
 * @param param pointer to a signal specific variable
 * @return LV_RES_OK: the object is not deleted in the function; LV_RES_INV: the object is deleted
 */
static lv_res_t lv_analogclock_signal(lv_obj_t * analogclock, lv_signal_t sign, void * param)
{
    lv_res_t res;

    /* Include the ancient signal function */
    res = ancestor_signal(analogclock, sign, param);
    if(res != LV_RES_OK) return res;
    if(sign == LV_SIGNAL_GET_TYPE) return lv_obj_handle_get_type_signal(param, LV_OBJX_NAME);

    lv_analogclock_ext_t * ext = lv_obj_get_ext_attr(analogclock);
    if(sign == LV_SIGNAL_CLEANUP) {
        lv_mem_free(ext->values);
        ext->values = NULL;
    }

    return res;
}

/**
 * Draw the needles of a analogclock
 * @param analogclock pointer to analogclock object
 * @param mask mask of drawing
 */
static void lv_analogclock_draw_needle(lv_obj_t * analogclock, const lv_area_t * mask)
{
    lv_style_t style_needle;
    lv_analogclock_ext_t * ext     = lv_obj_get_ext_attr(analogclock);
    const lv_style_t * style = lv_analogclock_get_style(analogclock, LV_ANALOGCLOCK_STYLE_MAIN);
    lv_opa_t opa_scale       = lv_obj_get_opa_scale(analogclock);

    lv_coord_t r      = lv_obj_get_width(analogclock) / 2 - style->body.padding.left;
    lv_coord_t x_ofs  = lv_obj_get_width(analogclock) / 2 + analogclock->coords.x1;
    lv_coord_t y_ofs  = lv_obj_get_height(analogclock) / 2 + analogclock->coords.y1;
    int16_t angle_ofs = 270;
    lv_point_t p_mid;
    lv_point_t p_end;
    lv_point_t p_end_low;
    lv_point_t p_end_high;

    lv_style_copy(&style_needle, style);

    p_mid.x = x_ofs;
    p_mid.y = y_ofs;
    for(uint8_t i = 0; i < ext->needle_count; i++) {
        int16_t value = ext->values[i];
        lv_coord_t r = 0;
        if(i==LV_ANALOGCLOCK_NEEDLE_HOURS) {
            value = value * 30; //in 360 degrees we can fit 30 times 12h
            r = (lv_obj_get_width(analogclock) / 2 - style->body.padding.left) /2;
        }
        else {
            value = value * 6; //in 360 degrees we can fit 6 times 60 minutes/seconds
            r = lv_obj_get_width(analogclock) / 2 - style->body.padding.left;
        }

        /*Calculate the end point of a needle*/
        int16_t needle_angle = value * LV_ANALOGCLOCK_DEF_ANGLE * (1 << LV_ANALOGCLOCK_INTERPOLATE_SHIFT) / 359;

        int16_t needle_angle_low  = (needle_angle >> LV_ANALOGCLOCK_INTERPOLATE_SHIFT) + angle_ofs;
        int16_t needle_angle_high = needle_angle_low + 1;

        p_end_low.y = (lv_trigo_sin(needle_angle_low) * r) / LV_TRIGO_SIN_MAX + y_ofs;
        p_end_low.x = (lv_trigo_sin(needle_angle_low + 90) * r) / LV_TRIGO_SIN_MAX + x_ofs;

        p_end_high.y = (lv_trigo_sin(needle_angle_high) * r) / LV_TRIGO_SIN_MAX + y_ofs;
        p_end_high.x = (lv_trigo_sin(needle_angle_high + 90) * r) / LV_TRIGO_SIN_MAX + x_ofs;

        uint16_t rem  = needle_angle & ((1 << LV_ANALOGCLOCK_INTERPOLATE_SHIFT) - 1);
        int16_t x_mod = ((LV_MATH_ABS(p_end_high.x - p_end_low.x)) * rem) >> LV_ANALOGCLOCK_INTERPOLATE_SHIFT;
        int16_t y_mod = ((LV_MATH_ABS(p_end_high.y - p_end_low.y)) * rem) >> LV_ANALOGCLOCK_INTERPOLATE_SHIFT;

        if(p_end_high.x < p_end_low.x) x_mod = -x_mod;
        if(p_end_high.y < p_end_low.y) y_mod = -y_mod;

        p_end.x = p_end_low.x + x_mod;
        p_end.y = p_end_low.y + y_mod;

        /*Draw the needle with the corresponding color*/
        if(ext->needle_colors == NULL)
            style_needle.line.color = LV_ANALOGCLOCK_DEF_NEEDLE_COLOR;
        else
            style_needle.line.color = ext->needle_colors[i];

        if(i == LV_ANALOGCLOCK_NEEDLE_HOURS)
            style_needle.line.width = 7;
        else if(i == LV_ANALOGCLOCK_NEEDLE_MINUTES)
            style_needle.line.width = 3;
        else 
            style_needle.line.width = 1;

        lv_draw_line(&p_mid, &p_end, mask, &style_needle, opa_scale);
    }

    /*Draw the needle middle area*/
    lv_style_t style_neddle_mid;
    lv_style_copy(&style_neddle_mid, &lv_style_plain);
    style_neddle_mid.body.main_color = style->body.border.color;
    style_neddle_mid.body.grad_color = style->body.border.color;
    style_neddle_mid.body.radius     = LV_RADIUS_CIRCLE;

    lv_area_t nm_cord;
    lv_coord_t radius_middle_area = style->body.radius/3;
    nm_cord.x1 = x_ofs - radius_middle_area;
    nm_cord.y1 = y_ofs - radius_middle_area;
    nm_cord.x2 = x_ofs + radius_middle_area;
    nm_cord.y2 = y_ofs + radius_middle_area;

    lv_draw_rect(&nm_cord, mask, &style_neddle_mid, lv_obj_get_opa_scale(analogclock));
}

#endif
