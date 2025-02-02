/*****************************************************************************
* | File      	:   LCD_1in69_Touch_test.c
* | Author      :   Waveshare team
* | Function    :   1.3inch LCD  test demo
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2024-03-25
* | Info        :
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#include "LCD_Test.h"
#include "LCD_1in69.h"
#include "Touch_1in69.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "lvgl/lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lib/out/gui_guider.h"
#include "lib/out/events_init.h"


lv_ui guider_ui; ////using the NXP GUI_guider's ui

Touch_1IN69_XY XY;
UBYTE flag = 0;
UWORD *BlackImage;

#include <math.h>
#define WIN_WIDTH  LV_HOR_RES
#define WIN_HEIGHT LV_VER_RES

// 定义表盘半径和中心点
#define CLOCK_RADIUS 110
#define CLOCK_CENTER_X LV_HOR_RES/2
#define CLOCK_CENTER_Y LV_VER_RES/2

// 定义指针长度
#define HOUR_HAND_LENGTH   70
#define MINUTE_HAND_LENGTH 80
#define SECOND_HAND_LENGTH 90

// 定义指针颜色
#define HOUR_HAND_COLOR   lv_color_hex(0x0000FF)   // 蓝色
#define MINUTE_HAND_COLOR lv_color_hex(0x00FF00)   // 绿色
#define SECOND_HAND_COLOR lv_color_hex(0xFF0000)   // 红色

//数字显示文本的长度和字体和颜色
#define LABEL_COLOR   lv_color_hex(0x000000)
#define LABEL_FONT    lv_font_montserrat_14
#define LABEL_LENGTH  CLOCK_RADIUS-12

// 全局变量：指针对象
static lv_obj_t* hour_hand;
static lv_obj_t* minute_hand;
static lv_obj_t* second_hand;

// 定义直线的起点和终点坐标
    static lv_point_t line_points_hour[] = {
        {120, 140},  // 起点 (x1, y1)
        {200, 100} // 终点 (x2, y2)
    };

        // 定义直线的起点和终点坐标
    static lv_point_t line_points_min[] = {
        {120, 140},  // 起点 (x1, y1)
        {150, 50} // 终点 (x2, y2)
    };

        // 定义直线的起点和终点坐标
    static lv_point_t line_points_sec[] = {
        {120, 140},  // 起点 (x1, y1)
        {40, 100} // 终点 (x2, y2)
    };


// 计算指针端点坐标
static void calculate_hand_position(double angle, int length, lv_point_t* point) {
    // point->x = CLOCK_CENTER_X + ((length * lv_trigo_sin(angle)) >> 10);
    // point->y = CLOCK_CENTER_Y - ((length * lv_trigo_cos(angle)) >> 10);
    double radians = angle * M_PI / 180.0; // 角度转弧度
    point->x = CLOCK_CENTER_X + (int)(length * sin(radians));
    point->y = CLOCK_CENTER_Y - (int)(length * cos(radians));
}

// 更新指针位置
static void update_clock_hands(int hour, int min, int sec) {
    // // 计算角度（LVGL 的三角函数使用 0-3600 表示 0-360 度）
    // int hour_angle = (hour % 12) * 300 + min * 5;   // 每小时 30 度，每分钟 0.5 度
    // int min_angle = min * 60;                       // 每分钟 6 度
    // int sec_angle = sec * 60;                       // 每秒 6 度

    // 计算角度
    double hour_angle = (hour % 12) * 30.0 + min * 0.5;   // 每小时 30 度，每分钟 0.5 度
    double min_angle = min * 6.0;                         // 每分钟 6 度
    double sec_angle = sec * 6.0;                         // 每秒 6 度

    // 计算指针端点坐标
    lv_point_t hour_point, min_point, sec_point;
    calculate_hand_position(hour_angle, HOUR_HAND_LENGTH, &hour_point);
    calculate_hand_position(min_angle, MINUTE_HAND_LENGTH, &min_point);
    calculate_hand_position(sec_angle, SECOND_HAND_LENGTH, &sec_point);

    line_points_hour[1] = hour_point;
    line_points_min[1] = min_point;
    line_points_sec[1] = sec_point;

    lv_line_set_points(hour_hand, line_points_hour, 2);
    lv_line_set_points(minute_hand, line_points_min, 2);
    lv_line_set_points(second_hand, line_points_sec, 2);

}


/* 按钮事件处理函数 */
static void btn_event_cb(lv_event_t * e) {
   lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * btn = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED) {
        static uint8_t cnt = 0;
        cnt++;

        /*Get the first child of the button which is the label and change its text*/
        lv_obj_t * label = lv_obj_get_child(btn, 0);
        lv_label_set_text_fmt(label, "Button: %d", cnt);
    }
}

void lv_example_get_started_1(void)
{

    //lv_obj_t * btn = lv_btn_create(lv_scr_act());     /*Add a button the current screen*/
    //lv_obj_set_pos(btn, 100, 10);                            /*Set its position*/
    //lv_obj_set_size(btn, 120, 50);                          /*Set its size*/
    //lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_ALL, NULL);           /*Assign a callback to the button*/
    //lv_obj_t * label = lv_label_create(btn);          /*Add a label to the button*/
    //lv_label_set_text(label, "temp");                     /*Set the labels text*/
    //lv_obj_center(label); 

	// 获取默认屏幕
    lv_obj_t* scr = lv_scr_act();

     // 创建圆形钟表背景
    lv_obj_t * clock = lv_obj_create(scr);

    // 设置圆形的宽度和高度相等，确保它是一个圆形
    lv_obj_set_size(clock, CLOCK_RADIUS*2, CLOCK_RADIUS*2);  // 250x250 像素的钟表背景

    // 设置钟表的位置
    lv_obj_align(clock, LV_ALIGN_CENTER, 0, 0);  // 将钟表置于屏幕中心

    // 设置钟表背景样式
    static lv_style_t style;
    lv_style_init(&style);

    // 设置背景颜色为灰色
    lv_style_set_bg_color(&style, lv_color_hex(0x808080));

    // 设置圆形的圆角为半径大小，使其成为圆形
    lv_style_set_radius(&style, LV_RADIUS_CIRCLE);

    // 设置阴影效果
    lv_style_set_shadow_color(&style, lv_color_hex(0x333333)); // 阴影颜色
    lv_style_set_shadow_width(&style, 10);  // 阴影宽度
    lv_style_set_shadow_ofs_x(&style, 5);   // 阴影在 x 轴方向的偏移
    lv_style_set_shadow_ofs_y(&style, 5);   // 阴影在 y 轴方向的偏移

    // 应用样式到钟表对象
    lv_obj_add_style(clock, &style, 0);

    // 创建一条直线
    hour_hand = lv_line_create(scr);
    // 设置直线的点坐标
    lv_line_set_points(hour_hand, line_points_hour, 2);
    // 设置直线的样式
    lv_obj_set_style_line_width(hour_hand, 2, 0);              // 线宽为 2
    lv_obj_set_style_line_color(hour_hand, lv_color_hex(0xFF0000), 0); // 红色
    lv_obj_set_style_line_opa(hour_hand, LV_OPA_COVER, 0);     // 完全不透明

    // 创建一条直线
    minute_hand = lv_line_create(scr);
    // 设置直线的点坐标
    lv_line_set_points(minute_hand, line_points_min, 2);
    // 设置直线的样式
    lv_obj_set_style_line_width(minute_hand, 3, 0);              // 线宽为 2
    lv_obj_set_style_line_color(minute_hand, lv_color_hex(0x00FF00), 0); // 红色
    lv_obj_set_style_line_opa(minute_hand, LV_OPA_COVER, 0);     // 完全不透明

    // 创建一条直线
    second_hand = lv_line_create(scr);
    // 设置直线的点坐标
    lv_line_set_points(second_hand, line_points_sec, 2);
    // 设置直线的样式
    lv_obj_set_style_line_width(second_hand, 4, 0);              // 线宽为 2
    lv_obj_set_style_line_color(second_hand, lv_color_hex(0x00FFFF), 0); // 红色
    lv_obj_set_style_line_opa(second_hand, LV_OPA_COVER, 0);     // 完全不透明

    // 创建标签并设置文本
    lv_obj_t* label12 = lv_label_create(scr);
    lv_label_set_text(label12, "12"); // 设置文本内容
    lv_obj_set_style_text_color(label12, LABEL_COLOR, 0); // 文本颜色
    lv_obj_set_style_text_font(label12, &LABEL_FONT, 0); // 字体大小
    lv_obj_align(label12, LV_ALIGN_CENTER, 0, -(LABEL_LENGTH)); // 位置显示

    lv_obj_t* label3 = lv_label_create(scr);
    lv_label_set_text(label3, "3"); // 设置文本内容
    lv_obj_set_style_text_color(label3, LABEL_COLOR, 0); // 文本颜色
    lv_obj_set_style_text_font(label3, &LABEL_FONT, 0); // 字体大小
    lv_obj_align(label3, LV_ALIGN_CENTER, LABEL_LENGTH, 0); // 位置显示

    lv_obj_t* label6 = lv_label_create(scr);
    lv_label_set_text(label6, "6"); // 设置文本内容
    lv_obj_set_style_text_color(label6, LABEL_COLOR, 0); // 文本颜色
    lv_obj_set_style_text_font(label6, &LABEL_FONT, 0); // 字体大小
    lv_obj_align(label6, LV_ALIGN_CENTER, 0, LABEL_LENGTH); // 位置显示

    lv_obj_t* label9 = lv_label_create(scr);
    lv_label_set_text(label9, "9"); // 设置文本内容
    lv_obj_set_style_text_color(label9, LABEL_COLOR, 0); // 文本颜色
    lv_obj_set_style_text_font(label9, &LABEL_FONT, 0); // 字体大小
    lv_obj_align(label9, LV_ALIGN_CENTER, -(LABEL_LENGTH), 0); // 位置显示

    lv_obj_t* label1 = lv_label_create(scr);
    lv_label_set_text(label1, "1"); // 设置文本内容
    lv_obj_set_style_text_color(label1, LABEL_COLOR, 0); // 文本颜色
    lv_obj_set_style_text_font(label1, &LABEL_FONT, 0); // 字体大小
    lv_obj_align(label1, LV_ALIGN_CENTER, (LABEL_LENGTH)*sin(30.0 * M_PI / 180.0), -(LABEL_LENGTH)*cos(30.0 * M_PI / 180.0)); // 位置显示

    lv_obj_t* label2 = lv_label_create(scr);
    lv_label_set_text(label2, "2"); // 设置文本内容
    lv_obj_set_style_text_color(label2, LABEL_COLOR, 0); // 文本颜色
    lv_obj_set_style_text_font(label2, &LABEL_FONT, 0); // 字体大小
    lv_obj_align(label2, LV_ALIGN_CENTER, (LABEL_LENGTH)*sin(60.0 * M_PI / 180.0), -(LABEL_LENGTH)*cos(60.0 * M_PI / 180.0)); // 位置显示

    lv_obj_t* label4 = lv_label_create(scr);
    lv_label_set_text(label4, "4"); // 设置文本内容
    lv_obj_set_style_text_color(label4, LABEL_COLOR, 0); // 文本颜色
    lv_obj_set_style_text_font(label4, &LABEL_FONT, 0); // 字体大小
    lv_obj_align(label4, LV_ALIGN_CENTER, (LABEL_LENGTH)*sin(120.0 * M_PI / 180.0), -(LABEL_LENGTH)*cos(120.0 * M_PI / 180.0)); // 位置显示

    lv_obj_t* label5 = lv_label_create(scr);
    lv_label_set_text(label5, "5"); // 设置文本内容
    lv_obj_set_style_text_color(label5, LABEL_COLOR, 0); // 文本颜色
    lv_obj_set_style_text_font(label5, &LABEL_FONT, 0); // 字体大小
    lv_obj_align(label5, LV_ALIGN_CENTER, (LABEL_LENGTH)*sin(150.0 * M_PI / 180.0), -(LABEL_LENGTH)*cos(150.0 * M_PI / 180.0)); // 位置显示

    lv_obj_t* label7 = lv_label_create(scr);
    lv_label_set_text(label7, "7"); // 设置文本内容
    lv_obj_set_style_text_color(label7, LABEL_COLOR, 0); // 文本颜色
    lv_obj_set_style_text_font(label7, &LABEL_FONT, 0); // 字体大小
    lv_obj_align(label7, LV_ALIGN_CENTER, (LABEL_LENGTH)*sin(210.0 * M_PI / 180.0), -(LABEL_LENGTH)*cos(210.0 * M_PI / 180.0)); // 位置显示

    lv_obj_t* label8 = lv_label_create(scr);
    lv_label_set_text(label8, "8"); // 设置文本内容
    lv_obj_set_style_text_color(label8, LABEL_COLOR, 0); // 文本颜色
    lv_obj_set_style_text_font(label8, &LABEL_FONT, 0); // 字体大小
    lv_obj_align(label8, LV_ALIGN_CENTER, (LABEL_LENGTH)*sin(240.0 * M_PI / 180.0), -(LABEL_LENGTH)*cos(240.0 * M_PI / 180.0)); // 位置显示

    lv_obj_t* label10 = lv_label_create(scr);
    lv_label_set_text(label10, "10"); // 设置文本内容
    lv_obj_set_style_text_color(label10, LABEL_COLOR, 0); // 文本颜色
    lv_obj_set_style_text_font(label10, &LABEL_FONT, 0); // 字体大小
    lv_obj_align(label10, LV_ALIGN_CENTER, (LABEL_LENGTH)*sin(300.0 * M_PI / 180.0), -(LABEL_LENGTH)*cos(300.0 * M_PI / 180.0)); // 位置显示

    lv_obj_t* label11 = lv_label_create(scr);
    lv_label_set_text(label11, "11"); // 设置文本内容
    lv_obj_set_style_text_color(label11, LABEL_COLOR, 0); // 文本颜色
    lv_obj_set_style_text_font(label11, &LABEL_FONT, 0); // 字体大小
    lv_obj_align(label11, LV_ALIGN_CENTER, (LABEL_LENGTH)*sin(330.0 * M_PI / 180.0), -(LABEL_LENGTH)*cos(330.0 * M_PI / 180.0)); // 位置显示

    update_clock_hands(11,30,45);
    // 创建定时器，每 100ms 更新一次
    //lv_timer_create(timer_callback, 100, NULL);
 
    
}

int LCD_1in69_test(void)
{

    if (DEV_Module_Init() != 0)
    {
        return -1;
    }
    /* LCD Init */
    printf("1.69inch LCD demo...\r\n");
    DEV_SET_PWM(100);
    LCD_1IN69_Init(VERTICAL);
    
    DEV_IRQ_SET(DEV_I2C_INT, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &Touch_INT_callback);
    
    UDOUBLE Imagesize = LCD_1IN69_HEIGHT * LCD_1IN69_WIDTH * 2;
    if ((BlackImage = (UWORD *)malloc(Imagesize)) == NULL)
    {
        printf("Failed to apply for black memory...\r\n");
        exit(0);
    }
    /*1.Create a new image cache named IMAGE_RGB and fill it with white*/
    //Paint_NewImage((UBYTE *)BlackImage, LCD_1IN69_WIDTH, LCD_1IN69_HEIGHT, 90, WHITE);
    //Paint_SetScale(65);
    //Paint_Clear(WHITE);
    
    
    lv_init();
    lv_port_disp_init();
    
    lv_port_indev_init();
    

    //Touch_HandWriting();
    lv_example_get_started_1();
    //lv_task_handler();
    //DEV_Delay_ms(10000);
    
    //setup_ui(&guider_ui); //using the NXP GUI_guider's ui
    //events_init(&guider_ui); //using the NXP GUI_guider's ui
    
    
    
   while (true)
    {
    	
        DEV_Delay_ms(30);   /*Sleep for 5 millisecond*/
        lv_task_handler();
        lv_tick_inc(30);      /*Tell LVGL that 5 milliseconds were elapsed*/
       	
    }
    
    
#if 0
    printf("drawing...\r\n");
    /*2.Drawing on the image*/
    Paint_DrawPoint(12,28, BLACK, DOT_PIXEL_1X1,  DOT_FILL_RIGHTUP);
    Paint_DrawPoint(12,30, BLACK, DOT_PIXEL_2X2,  DOT_FILL_RIGHTUP);
    Paint_DrawPoint(12,33, BLACK, DOT_PIXEL_3X3, DOT_FILL_RIGHTUP);
    Paint_DrawPoint(12,38, BLACK, DOT_PIXEL_4X4, DOT_FILL_RIGHTUP);
    Paint_DrawPoint(12,43, BLACK, DOT_PIXEL_5X5, DOT_FILL_RIGHTUP);

    Paint_DrawLine( 20,  5, 80, 65, MAGENTA, DOT_PIXEL_2X2, LINE_STYLE_SOLID);
    Paint_DrawLine( 20, 65, 80,  5, MAGENTA, DOT_PIXEL_2X2, LINE_STYLE_SOLID);

    Paint_DrawLine( 148,  35, 208, 35, CYAN, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine( 178,   5,  178, 65, CYAN, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);

    Paint_DrawRectangle(20, 5, 80, 65, RED, DOT_PIXEL_2X2,DRAW_FILL_EMPTY);
    Paint_DrawRectangle(85, 5, 145, 65, BLUE, DOT_PIXEL_2X2,DRAW_FILL_FULL);

    Paint_DrawCircle(178, 35, 30, GREEN, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(240, 35, 30, GREEN, DOT_PIXEL_1X1, DRAW_FILL_FULL);

    Paint_DrawString_EN(11, 70, "AaBbCc123", &Font16, RED, WHITE);
    Paint_DrawString_EN(11, 85, "AaBbCc123", &Font20, 0x000f, 0xfff0);
    Paint_DrawString_EN(11, 105, "AaBbCc123", &Font24, RED, WHITE);   
    Paint_DrawString_CN(11, 125, "微雪电子Abc",  &Font24CN, WHITE, BLUE);
    LCD_1IN69_Display(BlackImage);
    DEV_Delay_ms(1000);

    Paint_NewImage((UBYTE *)BlackImage, LCD_1IN69_WIDTH, LCD_1IN69_HEIGHT, 0, WHITE);
    Paint_SetScale(65);
    Paint_DrawImage(gImage_1IN69_PIC,0,0,240,280);
    LCD_1IN69_Display(BlackImage);
    DEV_Delay_ms(1000);
#endif

#if 1
    Touch_Gesture();
    Touch_HandWriting();
#endif
    /* Module Exit */
    free(BlackImage);
    BlackImage = NULL;

    DEV_Module_Exit();
    return 0;
}

void Touch_HandWriting()
{
    XY.mode = 1;
    XY.color = BLACK;
    flag = TOUCH_INIT;
    /* TP Init */
    if(Touch_1IN69_init(XY.mode) == true)
        printf("OK!\r\n");
    else
        printf("NO!\r\n");
    DEV_Delay_ms(10);
    Paint_Clear(WHITE);
    LCD_1IN69_Display(BlackImage);
    Paint_DrawPoint(120, 140, XY.color, DOT_PIXEL_6X6, DOT_FILL_AROUND);
    LCD_1IN69_DisplayWindows(120, 140, 120, 140, BlackImage);
    while(1)
    {
        DEV_Delay_us(100);
        if (flag == TOUCH_IRQ)
        {
            printf("X:%d Y:%d \r\n",XY.x_point,XY.y_point);
            Paint_DrawPoint(XY.x_point, XY.y_point, XY.color, DOT_PIXEL_6X6, DOT_FILL_AROUND);
            LCD_1IN69_DisplayWindows(XY.x_point, XY.y_point, XY.x_point, XY.y_point, BlackImage);
            flag = TOUCH_DRAW;
        }
    }
}

void Touch_Gesture()
{
    XY.mode = 0;
    /* TP Init */
    if(Touch_1IN69_init(XY.mode) == true)
        printf("OK!\r\n");
    else
        printf("NO!\r\n");
    Paint_Clear(WHITE);
    Paint_DrawString_EN(35, 90, "Gesture test", &Font20, BLACK, WHITE);
    Paint_DrawString_EN(10, 120, "Complete as prompted", &Font16, BLACK, WHITE);
    LCD_1IN69_Display(BlackImage);
    Paint_Clear(WHITE);
    DEV_Delay_ms(1500);
    Paint_DrawImage(gImage_up, 45, 30, 150, 150);
    LCD_1IN69_Display(BlackImage);
    Paint_DrawString_EN(105, 190, "Up", &Font24, 0X647C, WHITE);
    LCD_1IN69_DisplayWindows(105,190,145,215,BlackImage);
    while(XY.Gesture != UP)
    {
        DEV_Delay_ms(100);
    }
    Paint_ClearWindows(105,190,145,215,WHITE);
    Paint_DrawImage(gImage_down, 45, 30, 150, 150);
    LCD_1IN69_DisplayWindows(45, 30, 150, 150, BlackImage);
    Paint_DrawString_EN(85, 190, "Down", &Font24, 0X647C, WHITE);
    LCD_1IN69_DisplayWindows(85,190,155,215,BlackImage);
    while(XY.Gesture != Down)
    {
        DEV_Delay_ms(100);
    }
    Paint_ClearWindows(85,190,155,215,WHITE);
    Paint_DrawImage(gImage_left, 45, 30, 150, 150);
    LCD_1IN69_DisplayWindows(45, 30, 150, 150, BlackImage);
    Paint_DrawString_EN(85, 190, "Left", &Font24, 0X647C, WHITE);
    LCD_1IN69_DisplayWindows(85,190,155,215,BlackImage);
    while(XY.Gesture != LEFT)
    {
        DEV_Delay_ms(100);
    } 
    Paint_ClearWindows(85,190,155,215,WHITE);
    Paint_DrawImage(gImage_right, 45, 30, 150, 150);
    LCD_1IN69_DisplayWindows(45, 30, 150, 150, BlackImage);
    Paint_DrawString_EN(80, 190, "Right", &Font24, 0X647C, WHITE);
    LCD_1IN69_DisplayWindows(80,190,160,215,BlackImage);
    while(XY.Gesture != RIGHT)
    {
        DEV_Delay_ms(100);
    }
    Paint_ClearWindows(80,190,170,215,WHITE);
    Paint_DrawImage(gImage_long_press, 45, 30, 150, 150);
    LCD_1IN69_DisplayWindows(45, 30, 150, 150, BlackImage);
    Paint_DrawString_EN(47, 190, "Long Press", &Font20, 0X647C, WHITE);
    LCD_1IN69_DisplayWindows(47,190,180,215,BlackImage);
    while(XY.Gesture != LONG_PRESS)
    {
        DEV_Delay_ms(100);
    }
    Paint_ClearWindows(47,190,200,215,WHITE);
    Paint_DrawImage(gImage_double_click, 45, 30, 150, 150);
    LCD_1IN69_DisplayWindows(45, 30, 150, 150, BlackImage);
    Paint_DrawString_EN(35, 185, "Double Click", &Font20, 0X647C, WHITE);
    LCD_1IN69_DisplayWindows(35,185,210,215,BlackImage);
    while(XY.Gesture != DOUBLE_CLICK)
    {
        DEV_Delay_ms(100);
    }
    Paint_ClearWindows(35,185,210,215,WHITE);
    flag = TOUCH_OUT_GESTURE; //exit gesture mode
}

void Touch_INT_callback(uint gpio, uint32_t events)
{
    if (XY.mode == 0)
    {
        XY.Gesture = DEV_I2C_Read_Byte(address,0x01);
        flag = TOUCH_IRQ;
    }
    else
    {
        flag = TOUCH_IRQ;
        XY = Touch_1IN69_Get_Point();  
    }
}
