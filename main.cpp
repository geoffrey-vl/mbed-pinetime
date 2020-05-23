#include "mbed.h"

#include "configurationwindow.h"
#include "digitalclockwindow.h"
#include "analogclockwindow.h"

#include <LittlevGL.h>
#include "peripherals.h"

#include "lv_theme.h"
#include "lv_theme_night.h"

#include "gcc_timestamp.h"

#include "platform/Callback.h" //input events: touch, button



lv_point_t touchpoint;
lv_indev_state_t touchstate;
time_t buttonTimestamp;


void initRtc() {
    time_t now = time(0); //make sure to do a read of the RTC first
    time_t compile_time = UNIX_TIMESTAMP;
    
	if(now < (compile_time-(24*60*60))) { //use 24 hour margin
		set_time(compile_time);
	}
}


void on_touch_event(struct CST0xx::ts_event event)
{
	touchpoint.x = (lv_coord_t)event.x;
	touchpoint.y = (lv_coord_t)event.y;
	touchstate = LV_INDEV_STATE_PR;
	buttonTimestamp = time(0);
}


void on_btn_event(bool btnPressed)
{
	if(btnPressed) {
		buttonTimestamp = time(0);
		Peripherals::backlight.setBrightness(Brightness::HIGH);
		Peripherals::touchPad.enableIRQ();
	}
}


/**
 * read_cb is a function pointer which will be called periodically 
 * to report the current state of an input device. 
 * It can also buffer data and return false when no more data to 
 * be read or true when the buffer is not empty.
 */ 
bool my_input_read(lv_indev_drv_t* drv, lv_indev_data_t* data)
{
	//update coords
    data->point = touchpoint;
    data->state = touchstate;
	if (touchstate == LV_INDEV_STATE_PR)
		touchstate = LV_INDEV_STATE_REL; //reset touchstate
    return false;
}


int main()
{
	Peripherals::i2c.frequency(400000);
	Peripherals::spiDisplayInterface.frequency(8000000);
	Peripherals::spiDisplayInterface.format(8,3);

	initRtc();
	Peripherals::touchPad.init();
	Peripherals::touchPad.enableIRQ();
	Peripherals::lcd.init();
	ThisThread::sleep_for(100);
	Peripherals::lcd.display_on();
	Peripherals::backlight.setBrightness(Brightness::HIGH);

	// initialize LVGL
	LittlevGL& lvgl = LittlevGL::get_instance();
    lvgl.init();
	lvgl.add_display_driver(Peripherals::lcd);
	lvgl.set_default_display(Peripherals::lcd);
    // set the default theme
	lv_theme_t* theme = lv_theme_night_init(0, NULL);
	lv_theme_set_current(theme);

	// hook in touch driver
	Peripherals::touchPad.setCallBack(callback(&on_touch_event));
	Peripherals::pushButton.setCallBack(callback(&on_btn_event));

	//lvgl touch handler
	lv_indev_drv_t indev_drv;
	lv_indev_drv_init(&indev_drv);      //Basic initialization
	indev_drv.type = LV_INDEV_TYPE_POINTER;
	indev_drv.read_cb = my_input_read;
	//Register the driver in LittlevGL and save the created input device object
	lv_indev_t * my_indev = lv_indev_drv_register(&indev_drv);


    lvgl.start();

	BaseWindow* window = new ConfigurationWindow();

	buttonTimestamp = time(0);
	touchpoint = { 0, 0};
	touchstate = LV_INDEV_STATE_REL; //touchpad released

	while(true)
	{
		if(window->isFinished()) // user selected clock
		{
			uint16_t selectedID = ((ConfigurationWindow*)window)->selectedId();
			delete window;
			if(selectedID==1)
				window = new DigitalClockWindow();
			else 
				window = new AnalogClockWindow();
		}

		window->draw();
		ThisThread::sleep_for(10);

		//set brightness off after 10 seconds of inactivity
		if(Peripherals::backlight.brightness() != Brightness::OFF && (time(0) > (buttonTimestamp+10)) ) {
			Peripherals::backlight.setBrightness(Brightness::OFF);
			Peripherals::touchPad.disableIRQ();
		}
	}
}