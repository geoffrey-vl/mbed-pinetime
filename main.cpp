#include "mbed.h"

#include "digitalclockwindow.h"
#include "analogclockwindow.h"

#include <LittlevGL.h>
#include "ST7789LVGL.h"
#include "SPI4Wire.h"
#include "peripherals.h"

#include "lv_theme.h"
#include "lv_theme_alien.h"

#include "gcc_timestamp.h"


SPI4Wire display_interface(SPI_MOSI, SPI_MISO, SPI_SCK, SPI_CS_LCD, PIN_LCD_RS_OUT);
ST7789LVGL lcd(display_interface, PIN_LCD_RESET_OUT);


void initRtc() {
    time_t now = time(0); //make sure to do a read of the RTC first
    time_t compile_time = UNIX_TIMESTAMP;
    
	if(now < (compile_time-(24*60*60))) { //use 24 hour margin
		set_time(compile_time);
	}
}

int main()
{
	initRtc();
	lcd.init();
	ThisThread::sleep_for(100);
	lcd.display_on();
	Peripherals::backlight.setBrightness(Brightness::HIGH);

	// initialize LVGL
	LittlevGL& lvgl = LittlevGL::get_instance();
    lvgl.init();
	lvgl.add_display_driver(lcd);
	lvgl.set_default_display(lcd);
    // set the default theme to monochromatic
	lv_theme_t* theme = lv_theme_alien_init(0, NULL);
	lv_theme_set_current(theme);
    lvgl.start();

	//BaseWindow* window = new DigitalClockWindow(lcd);
	BaseWindow* window = new AnalogClockWindow(lcd);

	bool isPushBtnHigh = Peripherals::pushButton.isHigh();
	time_t tsPushBtnHigh = time(0);

	while(true)
	{
		window->draw();
		ThisThread::sleep_for(10);

		//control brightness on/off through pushbutton
		if(isPushBtnHigh != Peripherals::pushButton.isHigh()) {
			// pushbutton changed state!
			isPushBtnHigh = Peripherals::pushButton.isHigh();
			if(isPushBtnHigh) {
				tsPushBtnHigh = time(0);
				Peripherals::backlight.setBrightness(Brightness::HIGH);
			}
		}
		else { 
			//btn didn't change state, but display is still on after 10 seconds?
			if(Peripherals::backlight.brightness() != Brightness::OFF && (time(0) > (tsPushBtnHigh+10)) ) {
				Peripherals::backlight.setBrightness(Brightness::OFF);
				//delete window;
				//window = new AnalogClockWindow(lcd);
			}
		}
	}
}