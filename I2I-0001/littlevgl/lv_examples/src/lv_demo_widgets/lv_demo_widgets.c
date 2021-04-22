/**
 * @file lv_demo_widgets.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "../../lv_examples.h"
#include "lv_demo_widgets.h"

#if LV_USE_DEMO_WIDGETS

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void status_create(lv_obj_t * parent);
static void settings_create(lv_obj_t * parent);
static void debug_create(lv_obj_t * parent);
static void batterybar_anim(lv_task_t * t);
static void debugupdate_anim(lv_task_t * t);
static void bar_anim(lv_task_t * t);
static void priority_anim(lv_task_t * t);
static void led1_anim(lv_task_t * t);
static void led2_anim(lv_task_t * t);
static void led3_anim(lv_task_t * t);
static void table_event_cb(lv_obj_t * table, lv_event_t e);
static void event_handler(lv_obj_t * table, lv_event_t e);
#if LV_USE_THEME_MATERIAL
static void color_chg_event_cb(lv_obj_t * sw, lv_event_t e);
#endif
#if LV_DEMO_WIDGETS_SLIDESHOW
static void tab_content_anim_create(lv_obj_t * parent);
static void tab_changer_task_cb(lv_task_t * task);
#endif

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_obj_t * tv;
static lv_obj_t * t1;
static lv_obj_t * t2;
static lv_obj_t * t3;
static lv_obj_t * kb;
static lv_obj_t * src1label;
static lv_obj_t * src2label;
static lv_obj_t * src3label;

static lv_style_t style_box;

extern int led1active;
extern int led2active;
extern int led3active;
extern int batterycharge;
extern int priorityindex;
extern int adcbattery;
extern int adcin1v;
extern int adcin1a;
extern int adcin2v;
extern int adcin2a;
extern int adcin3v;
extern int adcin3a;
extern int adcout1v;
extern int adcout1a;
extern int adcout2v;
extern int adcout2a;
extern int adcout3v;
extern int adcout3a;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_demo_widgets(void)
{
    tv = lv_tabview_create(lv_scr_act(), NULL);
#if LV_USE_THEME_MATERIAL
    if(LV_THEME_DEFAULT_INIT == lv_theme_material_init) {
        lv_disp_size_t disp_size = lv_disp_get_size_category(NULL);
        if(disp_size >= LV_DISP_SIZE_MEDIUM) {
            lv_obj_set_style_local_pad_left(tv, LV_TABVIEW_PART_TAB_BG, LV_STATE_DEFAULT, LV_HOR_RES / 2);
            lv_obj_t * sw = lv_switch_create(lv_scr_act(), NULL);
            if(lv_theme_get_flags() & LV_THEME_MATERIAL_FLAG_DARK)
                lv_switch_on(sw, LV_ANIM_OFF);
            lv_obj_set_event_cb(sw, color_chg_event_cb);
            lv_obj_set_pos(sw, LV_DPX(10), LV_DPX(10));
            lv_obj_set_style_local_value_str(sw, LV_SWITCH_PART_BG, LV_STATE_DEFAULT, "Dark");
            lv_obj_set_style_local_value_align(sw, LV_SWITCH_PART_BG, LV_STATE_DEFAULT, LV_ALIGN_OUT_RIGHT_MID);
            lv_obj_set_style_local_value_ofs_x(sw, LV_SWITCH_PART_BG, LV_STATE_DEFAULT, LV_DPI/35);
        }
    }
#endif

    t1 = lv_tabview_add_tab(tv, "Status");
    t2 = lv_tabview_add_tab(tv, "Settings");
    t3 = lv_tabview_add_tab(tv, "Debug");
    //t4 = lv_tabview_add_tab(tv, "Controls");
    //t5 = lv_tabview_add_tab(tv, "Visuals");
	//t6 = lv_tabview_add_tab(tv, "Selectors");

    lv_style_init(&style_box);
    lv_style_set_value_align(&style_box, LV_STATE_DEFAULT, LV_ALIGN_OUT_TOP_LEFT);
    lv_style_set_value_ofs_y(&style_box, LV_STATE_DEFAULT, - LV_DPX(10));
    lv_style_set_margin_top(&style_box, LV_STATE_DEFAULT, LV_DPX(30));

    status_create(t1);
	settings_create(t2);
	debug_create(t3);

   // controls_create(t4);
  //  visuals_create(t5);
   // selectors_create(t6);

#if LV_DEMO_WIDGETS_SLIDESHOW
    lv_task_create(tab_changer_task_cb, 8000, LV_TASK_PRIO_LOW, NULL);
#endif

}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void status_create(lv_obj_t * parent)
{
    //lv_page_set_scrl_layout(parent, LV_LAYOUT_PRETTY_MID);

    lv_disp_size_t disp_size = lv_disp_get_size_category(NULL);

//********************************************************************************************************
    //create container for Source #1
    lv_obj_t * source01 = lv_cont_create(parent, NULL);
    lv_obj_set_pos(source01, 2, 0);
    lv_obj_set_size(source01, 170, 46);
    src1label = lv_label_create(source01, NULL);
    lv_label_set_text(src1label, "Source 1");
    lv_obj_align(src1label, NULL, LV_ALIGN_IN_LEFT_MID, 10, 0);
    lv_obj_t * led1 = lv_led_create(source01, NULL);
    lv_coord_t led_size = lv_obj_get_height_fit(source01);
    lv_obj_set_size(led1, led_size, led_size);
    lv_obj_align(led1, NULL, LV_ALIGN_IN_RIGHT_MID, -10, 0);

//********************************************************************************************************
    //create container for Source #2
    lv_obj_t * source02 = lv_cont_create(parent, NULL);
    lv_obj_set_pos(source02, 2, 46);
    lv_obj_set_size(source02, 170, 46);
    src2label = lv_label_create(source02, NULL);
    lv_label_set_text(src2label, "Source 2");
    lv_obj_align(src2label, NULL, LV_ALIGN_IN_LEFT_MID, 10, 0);
    lv_obj_t * led2  = lv_led_create(source02, led1);
    lv_obj_set_size(led2, led_size, led_size);
    lv_obj_align(led2, NULL, LV_ALIGN_IN_RIGHT_MID, -10, 0);


//********************************************************************************************************
    //create container for Source #3
    lv_obj_t * source03 = lv_cont_create(parent, NULL);
    lv_obj_set_pos(source03, 2, 92);
    lv_obj_set_size(source03, 170, 46);
    src3label = lv_label_create(source03, NULL);
    lv_label_set_text(src3label, "Source 3");
    lv_obj_align(src3label, NULL, LV_ALIGN_IN_LEFT_MID, 10, 0);
    lv_obj_t * led3  = lv_led_create(source03, led1);
    lv_obj_set_size(led3, led_size, led_size);
    lv_obj_align(led3, NULL, LV_ALIGN_IN_RIGHT_MID, -10, 0);

//********************************************************************************************************
    //create container for Battery info
    lv_obj_t * battery01 = lv_cont_create(parent, NULL);
    lv_obj_set_pos(battery01, 2, 138);
    lv_obj_set_size(battery01, 170, 46);

    lv_obj_t * batterybar = lv_bar_create(battery01, NULL);
    lv_obj_set_width(batterybar, lv_obj_get_width_fit(battery01));
    lv_obj_set_style_local_value_font(batterybar, LV_BAR_PART_BG, LV_STATE_DEFAULT, lv_theme_get_font_small());
    lv_obj_set_style_local_value_align(batterybar, LV_BAR_PART_BG, LV_STATE_DEFAULT, LV_ALIGN_OUT_BOTTOM_MID);
    lv_obj_set_style_local_value_ofs_y(batterybar, LV_BAR_PART_BG, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_margin_bottom(batterybar, LV_BAR_PART_BG, LV_STATE_DEFAULT, 0);
    lv_obj_align(batterybar, NULL, LV_ALIGN_IN_TOP_MID, 0, 10);


//********************************************************************************************************
    //create container for Priority Info
    lv_obj_t * priority01 = lv_cont_create(parent, NULL);
    lv_obj_set_pos(priority01, 172, 0);
    lv_obj_set_size(priority01, 139, 184);

    lv_obj_t * table2 = lv_table_create(priority01, NULL);
    lv_table_set_col_cnt(table2, disp_size > LV_DISP_SIZE_SMALL ? 3 : 2);
    lv_table_set_col_width(table2, 0, 20);
    lv_table_set_col_width(table2, 1, 116);

    lv_table_set_cell_value(table2, 0, 0, "#");
    lv_table_set_cell_value(table2, 1, 0, "1");
    lv_table_set_cell_value(table2, 2, 0, "2");
    lv_table_set_cell_value(table2, 3, 0, "3");
    lv_table_set_cell_value(table2, 4, 0, " ");

    lv_table_set_cell_value(table2, 0, 1, "Priority");
    lv_table_set_cell_value(table2, 1, 1, "No Priority Set");
    lv_table_set_cell_value(table2, 2, 1, "No Priority Set");
    lv_table_set_cell_value(table2, 3, 1, "No Priority Set");

    lv_task_create(priority_anim, 100, LV_TASK_PRIO_LOW, table2);
    lv_task_create(batterybar_anim, 100, LV_TASK_PRIO_LOW, batterybar);
    lv_task_create(led1_anim, 100, LV_TASK_PRIO_LOW, led1);
	lv_task_create(led2_anim, 100, LV_TASK_PRIO_LOW, led2);
	lv_task_create(led3_anim, 100, LV_TASK_PRIO_LOW, led3);
}

static void settings_create(lv_obj_t * parent)
{
    lv_page_set_scrl_layout(parent, LV_LAYOUT_PRETTY_MID);

    lv_disp_size_t disp_size = lv_disp_get_size_category(NULL);
    lv_coord_t grid_h = lv_page_get_height_grid(parent, 1, 1);
    lv_coord_t grid_w = lv_page_get_width_grid(parent, 2, 1);

    lv_obj_t * settings01 = lv_cont_create(parent, NULL);
    lv_obj_set_drag_parent(settings01, true);

	lv_obj_set_click(settings01, false);
	lv_obj_set_style_local_bg_opa(settings01, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
	lv_obj_set_style_local_border_opa(settings01, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
	lv_obj_set_style_local_pad_left(settings01, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
	lv_obj_set_style_local_pad_right(settings01, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
	lv_obj_set_style_local_pad_top(settings01, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
	lv_obj_set_size(settings01, LV_MATH_MIN(grid_h, grid_w), LV_MATH_MIN(grid_h, grid_w));
	lv_cont_set_layout(settings01, LV_LAYOUT_PRETTY_TOP);

    lv_obj_t * dd2 = lv_dropdown_create(settings01, NULL);
    lv_obj_add_style(dd2, LV_CONT_PART_MAIN, &style_box);
    lv_obj_set_style_local_value_str(dd2, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, "Select Output Priority");
    lv_obj_set_width(dd2, lv_obj_get_width_grid(settings01, disp_size <= LV_DISP_SIZE_SMALL ? 1 : 2, 1));
    lv_dropdown_set_options(dd2, "None\n1-2-3\n1-3-2\n2-1-3\n2-3-1\n3-1-2\n3-2-1");
    lv_obj_set_event_cb(dd2, event_handler);
}

static void event_handler(lv_obj_t* obj, lv_event_t event)
{
    if (event == LV_EVENT_VALUE_CHANGED) {
    	priorityindex = lv_dropdown_get_selected(obj);
	}
}

static void debug_create(lv_obj_t * parent)
{
	lv_page_set_scrl_layout(parent, LV_LAYOUT_PRETTY_MID);

	lv_disp_size_t disp_size = lv_disp_get_size_category(NULL);
	lv_coord_t grid_h = lv_page_get_height_grid(parent, 1, 1);
	lv_coord_t grid_w = lv_page_get_width_grid(parent, 1, 1);


	lv_obj_t * debugpage = lv_page_create(parent ,NULL);
	lv_obj_set_size(debugpage, grid_w, grid_h);
	lv_coord_t table_w_max = lv_page_get_width_fit(debugpage);
	lv_page_set_scroll_propagation(debugpage, true);

	lv_obj_t * table1 = lv_table_create(debugpage, NULL);

	lv_obj_clean_style_list(table1, LV_TABLE_PART_BG);
	lv_obj_set_drag_parent(table1, true);
	lv_table_set_col_cnt(table1, disp_size > LV_DISP_SIZE_SMALL ? 3 : 2);
	lv_table_set_col_width(table1, 0, 200);
	lv_table_set_col_width(table1, 1, 100);


	lv_table_set_cell_value(table1, 0, 0, "Data Point");
	lv_table_set_cell_value(table1, 1, 0, "Battery Voltage");
	lv_table_set_cell_value(table1, 2, 0, "Input 1 Voltage");
	lv_table_set_cell_value(table1, 3, 0, "Input 1 Current");
	lv_table_set_cell_value(table1, 4, 0, "Input 2 Voltage");
	lv_table_set_cell_value(table1, 5, 0, "Input 2 Current");
	lv_table_set_cell_value(table1, 6, 0, "Input 3 Voltage");
	lv_table_set_cell_value(table1, 7, 0, "Input 3 Current");
	lv_table_set_cell_value(table1, 8, 0, "Output 1 Voltage");
	lv_table_set_cell_value(table1, 9, 0, "Output 1 Current");
	lv_table_set_cell_value(table1, 10, 0, "Output 2 Voltage");
	lv_table_set_cell_value(table1, 11, 0, "Output 2 Current");
	lv_table_set_cell_value(table1, 12, 0, "Output 3 Voltage");
	lv_table_set_cell_value(table1, 13, 0, "Output 3 Current");

	lv_table_set_cell_value(table1, 0, 1, "Value");

	lv_task_create(debugupdate_anim, 100, LV_TASK_PRIO_LOW, table1);
}

static void led1_anim(lv_task_t * t)
{
	lv_obj_t * led1 = t->user_data;

	if (led1active == 1)
	{
		lv_led_set_bright(led1, LV_LED_BRIGHT_MAX);
	}
	else if (led1active == 0)
	{
		lv_led_set_bright(led1, LV_LED_BRIGHT_MIN);
	}
}
static void led2_anim(lv_task_t * t)
{
	lv_obj_t * led2 = t->user_data;

	if (led2active == 1)
	{
		lv_led_set_bright(led2, LV_LED_BRIGHT_MAX);
	}
	else if (led2active == 0)
	{
		lv_led_set_bright(led2, LV_LED_BRIGHT_MIN);
	}
}
static void led3_anim(lv_task_t * t)
{
	lv_obj_t * led3 = t->user_data;

	if (led3active == 1)
	{
		lv_led_set_bright(led3, LV_LED_BRIGHT_MAX);
	}
	else if (led3active == 0)
	{
		lv_led_set_bright(led3, LV_LED_BRIGHT_MIN);
	}
}

static void priority_anim(lv_task_t * t)
{
    lv_obj_t * table2 = t->user_data;

    if (priorityindex == 0)
    {
        lv_table_set_cell_value(table2, 1, 1, "No Priority Set");
        lv_table_set_cell_value(table2, 2, 1, "No Priority Set");
        lv_table_set_cell_value(table2, 3, 1, "No Priority Set");
    }
    else if (priorityindex == 1)
    {
        lv_table_set_cell_value(table2, 1, 1, "Output 1");
        lv_table_set_cell_value(table2, 2, 1, "Output 2");
        lv_table_set_cell_value(table2, 3, 1, "Output 3");
    }
    else if (priorityindex == 2)
    {
        lv_table_set_cell_value(table2, 1, 1, "Output 1");
        lv_table_set_cell_value(table2, 2, 1, "Output 3");
        lv_table_set_cell_value(table2, 3, 1, "Output 2");
    }
    else if (priorityindex == 3)
    {
        lv_table_set_cell_value(table2, 1, 1, "Output 2");
        lv_table_set_cell_value(table2, 2, 1, "Output 1");
        lv_table_set_cell_value(table2, 3, 1, "Output 3");
    }
    else if (priorityindex == 4)
    {
        lv_table_set_cell_value(table2, 1, 1, "Output 2");
        lv_table_set_cell_value(table2, 2, 1, "Output 3");
        lv_table_set_cell_value(table2, 3, 1, "Output 1");
    }
    else if (priorityindex == 5)
    {
        lv_table_set_cell_value(table2, 1, 1, "Output 3");
        lv_table_set_cell_value(table2, 2, 1, "Output 1");
        lv_table_set_cell_value(table2, 3, 1, "Output 2");
    }
    else if (priorityindex == 6)
    {
        lv_table_set_cell_value(table2, 1, 1, "Output 3");
        lv_table_set_cell_value(table2, 2, 1, "Output 2");
        lv_table_set_cell_value(table2, 3, 1, "Output 1");
    }
}

static void batterybar_anim(lv_task_t * t)
{
    lv_obj_t * bar = t->user_data;

    static char buf[64];
    lv_snprintf(buf, sizeof(buf), "Battery Level %d/%d", batterycharge, lv_bar_get_max_value(bar));
    lv_obj_set_style_local_value_str(bar, LV_BAR_PART_BG, LV_STATE_DEFAULT, buf);

    lv_bar_set_value(bar, batterycharge, LV_ANIM_OFF);
}

static void debugupdate_anim(lv_task_t * t)
{
	lv_obj_t * table1 = t->user_data;

	char str01[6];
	char str02[6];
	char str03[6];
	char str04[6];
	char str05[6];
	char str06[6];
	char str07[6];
	char str08[6];
	char str09[6];
	char str10[6];
	char str11[6];
	char str12[6];
	char str13[6];

	snprintf(str01, 6, "%i", adcbattery);
	snprintf(str02, 6, "%i", adcin1v);
	snprintf(str03, 6, "%i", adcin1a);
	snprintf(str04, 6, "%i", adcin2v);
	snprintf(str05, 6, "%i", adcin2a);
	snprintf(str06, 6, "%i", adcin3v);
	snprintf(str07, 6, "%i", adcin3a);
	snprintf(str08, 6, "%i", adcout1v);
	snprintf(str09, 6, "%i", adcout1a);
	snprintf(str10, 6, "%i", adcout2v);
	snprintf(str11, 6, "%i", adcout2a);
	snprintf(str12, 6, "%i", adcout3v);
	snprintf(str13, 6, "%i", adcout3a);

	lv_table_set_cell_value(table1, 1, 1, str01);
	lv_table_set_cell_value(table1, 2, 1, str02);
	lv_table_set_cell_value(table1, 3, 1, str03);
	lv_table_set_cell_value(table1, 4, 1, str04);
	lv_table_set_cell_value(table1, 5, 1, str05);
	lv_table_set_cell_value(table1, 6, 1, str06);
	lv_table_set_cell_value(table1, 7, 1, str07);
	lv_table_set_cell_value(table1, 8, 1, str08);
	lv_table_set_cell_value(table1, 9, 1, str09);
	lv_table_set_cell_value(table1, 10, 1, str10);
	lv_table_set_cell_value(table1, 11, 1, str11);
	lv_table_set_cell_value(table1, 12, 1, str12);
	lv_table_set_cell_value(table1, 13, 1, str13);
}

#if LV_USE_THEME_MATERIAL
static void color_chg_event_cb(lv_obj_t * sw, lv_event_t e)
{
    if(LV_THEME_DEFAULT_INIT != lv_theme_material_init) return;
    if(e == LV_EVENT_VALUE_CHANGED) {
        uint32_t flag = LV_THEME_MATERIAL_FLAG_LIGHT;
        if(lv_switch_get_state(sw)) flag = LV_THEME_MATERIAL_FLAG_DARK;

        LV_THEME_DEFAULT_INIT(lv_theme_get_color_primary(), lv_theme_get_color_secondary(),
                flag,
                lv_theme_get_font_small(), lv_theme_get_font_normal(), lv_theme_get_font_subtitle(), lv_theme_get_font_title());
    }
}
#endif

#if LV_DEMO_WIDGETS_SLIDESHOW

static void tab_content_anim_create(lv_obj_t * parent)
{
    lv_anim_t a;
    lv_obj_t * scrl = lv_page_get_scrl(parent);
    lv_coord_t y_start = lv_obj_get_style_pad_top(parent, LV_PAGE_PART_BG);
    lv_coord_t anim_h = lv_obj_get_height(scrl) - lv_obj_get_height_fit(parent);
    uint32_t anim_time = lv_anim_speed_to_time(LV_DPI, 0, anim_h);

    lv_anim_init(&a);
    lv_anim_set_var(&a, scrl);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_y);
    lv_anim_set_values(&a, y_start, y_start - anim_h);
    lv_anim_set_time(&a, anim_time);
    lv_anim_set_playback_time(&a, anim_time);
    lv_anim_set_playback_delay(&a, 200);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_repeat_delay(&a, 200);
    lv_anim_start(&a);
}
static void tab_changer_task_cb(lv_task_t * task)
{
    uint16_t act = lv_tabview_get_tab_act(tv);
    act++;
    if(act >= 3) act = 0;

    lv_tabview_set_tab_act(tv, act, LV_ANIM_ON);

    switch(act) {
    case 0:
        tab_content_anim_create(t1);
        break;
    case 1:
        tab_content_anim_create(t2);
        break;
    case 2:
        tab_content_anim_create(t3);
        break;
    }
}
#endif

#endif
