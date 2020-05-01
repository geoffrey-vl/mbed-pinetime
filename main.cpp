#include "mbed.h"

#include <LittlevGL.h>
#include "ST7789LVGL.h"
#include "SPI4Wire.h"
#include "backlightcontroller.h"

#include "lv_disp.h"
#include "lv_label.h"
#include "lv_theme.h"
#include "lv_theme_nemo.h"

SPI4Wire display_interface(SPI_MOSI, SPI_MISO, SPI_SCK, SPI_CS_LCD, PIN_LCD_RS_OUT);
ST7789LVGL lcd(display_interface, PIN_LCD_RESET_OUT);
BacklightController backlight;

int main()
{
	printf("Initializing LVGL Demo...\r\n");

	lcd.init();
	wait_ms(100);
	lcd.display_on();
	backlight.setBrightness(Brightness::HIGH);

	LittlevGL& lvgl = LittlevGL::get_instance();
	lvgl.init();
	lvgl.add_display_driver(lcd);
	lvgl.set_default_display(lcd);

	// Initialize the GUI

	// Set the default theme to monochromatic
	lv_theme_t* theme = lv_theme_nemo_init(0, NULL);
	lv_theme_set_current(theme);


	//Create label on the screen. By default it will inherit the style of the screen
	lv_obj_t * title = lv_label_create(lv_scr_act(), NULL);
	lv_label_set_text(title, "Hello Pinetime!");
	lv_obj_align(title, NULL, LV_ALIGN_IN_TOP_MID, 0, 8);  //Align to the top
	lv_obj_set_hidden(title, false);

	lvgl.start();

	while(true) {
		lvgl.update();
		wait_ms(10);
	}
}