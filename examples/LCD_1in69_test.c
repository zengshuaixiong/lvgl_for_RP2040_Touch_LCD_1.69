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
#include "lib/out/gui_guider.h"
#include "lib/out/events_init.h"

lv_ui guider_ui; ////using the NXP GUI_guider's ui

Touch_1IN69_XY XY;
UBYTE flag = 0;
UWORD *BlackImage;

void lv_example_get_started_1(void)
{
    lv_obj_t * btn = lv_btn_create(lv_scr_act());     /*Add a button the current screen*/
    lv_obj_set_pos(btn, 120-60, 10);                            /*Set its position*/
    lv_obj_set_size(btn, 120, 50);                          /*Set its size*/
    //lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_ALL, NULL);           /*Assign a callback to the button*/
    lv_obj_t * label = lv_label_create(btn);          /*Add a label to the button*/
    lv_label_set_text(label, "Button");                     /*Set the labels text*/
    lv_obj_center(label); 
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
    Paint_NewImage((UBYTE *)BlackImage, LCD_1IN69_WIDTH, LCD_1IN69_HEIGHT, 90, WHITE);
    Paint_SetScale(65);
    Paint_Clear(WHITE);
    DEV_Delay_ms(1000);
    
    
    
    lv_init();
    lv_port_disp_init();
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
        //lv_example_get_started_1();
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
    Paint_DrawString_CN(11, 125, "Î¢Ñ©µç×ÓAbc",  &Font24CN, WHITE, BLUE);
    LCD_1IN69_Display(BlackImage);
    DEV_Delay_ms(1000);

    Paint_NewImage((UBYTE *)BlackImage, LCD_1IN69_WIDTH, LCD_1IN69_HEIGHT, 0, WHITE);
    Paint_SetScale(65);
    Paint_DrawImage(gImage_1IN69_PIC,0,0,240,280);
    LCD_1IN69_Display(BlackImage);
    DEV_Delay_ms(1000);
#endif

#if 0
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
