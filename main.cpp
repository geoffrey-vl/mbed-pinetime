#include "mbed.h"

#include "mainwindow.h"

#include <LittlevGL.h>
#include "ST7789LVGL.h"
#include "SPI4Wire.h"
#include "backlightcontroller.h"

#include "gcc_timestamp.h"


SPI4Wire display_interface(SPI_MOSI, SPI_MISO, SPI_SCK, SPI_CS_LCD, PIN_LCD_RS_OUT);
ST7789LVGL lcd(display_interface, PIN_LCD_RESET_OUT);
BacklightController backlight;


void initRtc() {
    time_t now = time(0); //make sure to do a read of the RTC first
    time_t compile_time = UNIX_TIMESTAMP;
    
	if(now < (compile_time-(24*60*60))) { //use 24 hour margin
		printf("RTC illegal time, setting compile time: %d\n", (int)compile_time);
		set_time(compile_time);
	}
}

int main()
{
	printf("Init RTC...\r\n");
	initRtc();
	printf("Initializing LCD...\r\n");
	lcd.init();
	ThisThread::sleep_for(100);
	lcd.display_on();
	backlight.setBrightness(Brightness::HIGH);

	printf("Creating MainWindow...\r\n");
	MainWindow mainwindow(lcd);

	while(true)
	{
		mainwindow.draw();
		ThisThread::sleep_for(10);
	}
}