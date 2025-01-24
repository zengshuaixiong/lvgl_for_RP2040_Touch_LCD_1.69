/*****************************************************************************
* | File      	:   Readme_EN.txt
* | Author      :   
* | Function    :   Help with use
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2024-07-24
* | Info        :   Here is an English version of the documentation for your quick use.
******************************************************************************/
This file is to help you use this demo.
Here is a brief description of the use of this project:

1. Basic information:
This demo is verified using RP2040-LCD-1.69. 
You can view the corresponding test demo in the project;

2. Pin connection:
You can check the pin connection at RP2040-LCD-1.69.py, and repeat it here:
LCD         =>     Pico
VCC    	->    	 3.3V
GND    	->    	 GND
MISO    	->    	 12
MOSI    	->    	 11
SCLK   	 ->    10
LCD_CS  	->    	  9
LCD_DC  	->    	  8
LCD_RST 	->      13
LCD_BL  	->      25

Touch  	=>	  Pico    
TP_SDA 	->       6
TP_SCL  	->       7
TP_INT  	->      21
TP_RST 	 ->     22
3. Basic use:
You need to execute:
    If the directory already exists, you can enter directly. If there is no build directory, execute
         mkdir build
     Enter the build directory and type in the terminal:
         cd build
         export PICO_SDK_PATH=../../pico-sdk
     Where ../../pico-sdk is your installed SDK directory
     Execute cmake, automatically generate Makefile file, enter in the terminal:
         cmake ..
     Execute make to generate an executable file, and enter in the terminal:
         make
     Copy the compiled uf2 file to pico

4. Directory structure (selection):

If you use our products frequently, you will be very familiar with our program directory structure. We have a copy of the specific function.
The API manual for the function, you can download it on our WIKI or request it as an after-sales customer service. Here is a brief introduction:
Config\: This directory is a hardware interface layer file. You can see many definitions in DEV_Config.c(.h), including:
   data type;
    GPIO;
    Read and write GPIO;
    Delay: Note: This delay function does not use an oscilloscope to measure specific values,so it will be inaccurate.
    Module Init and exit processing:
        void DEV_Module_Init(void);
        void DEV_Module_Exit(void);
             
\lib\GUI\: This directory is some basic image processing functions, in GUI_Paint.c(.h):
    Common image processing: creating graphics, flipping graphics, mirroring graphics, setting pixels, clearing screens, etc.
    Common drawing processing: drawing points, lines, boxes, circles, Chinese characters, English characters, numbers, etc.;
    Common time display: Provide a common display time function;
    Commonly used display pictures: provide a function to display bitmaps;
    
\lib\Fonts\: for some commonly used fonts:
    Ascii:
        Font8: 5*8
        Font12: 7*12
        Font16: 11*16
        Font20: 14*20
        Font24: 17*24
    Chinese:
        font12CN: 16*21
        font24CN: 32*41
        
\lib\LCD\: This directory is the LCD driver functions and touch driver functions;
examples\: This directory is the test program of LCD, you can see the specific usage method in it;