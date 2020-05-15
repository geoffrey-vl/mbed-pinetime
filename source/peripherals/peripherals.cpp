#include "peripherals.h"

//busses
mbed::I2C Peripherals::i2c(I2C_SDA, I2C_SCL);
SPI4Wire Peripherals::spiDisplayInterface(SPI_MOSI, SPI_MISO, SPI_SCK, SPI_CS_LCD, PIN_LCD_RS_OUT);

//peripherals
ST7789LVGL Peripherals::lcd(Peripherals::spiDisplayInterface, PIN_LCD_RESET_OUT);
BacklightController Peripherals::backlight(LCD_BACKLIGHT_LOW, LCD_BACKLIGHT_MID, LCD_BACKLIGHT_HIGH);
BatteryMonitor Peripherals::batteryMonitor(BATTERY_VOLTAGE, PIN_CHARGE_INDICATION_IN);
PushButton Peripherals::pushButton(PIN_PUSH_BUTTON_IN, PIN_PUSH_BUTTON_OUT);
CST0xx Peripherals::touchPad(&Peripherals::i2c, PIN_TOUCHPAD_INTERRUPT, PIN_TOUCHPAD_RESET_OUT);