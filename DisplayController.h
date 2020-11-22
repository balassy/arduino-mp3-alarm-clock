#ifndef display_controller_h
#define display_controller_h

// Standard Arduino libraries.
#include <SPI.h>                  // I2C for the display (https://www.arduino.cc/en/Reference/SPI)
#include <Wire.h>                 // I2C for the display (https://www.arduino.cc/en/Reference/Wire)

// Third-party libraries.
#include <Adafruit_SSD1306.h>     // Adafruit SSD1306 Wemos Mini OLED by Adafruit + mcauser (https://github.com/stblassitude/Adafruit_SSD1306_Wemos_OLED)

// SCL GPIO5 D1
// SDA GPIO4 D2
#define OLED_RESET_PIN 0  // GPIO0

#if (SSD1306_LCDHEIGHT != 48)
  #error("[Display] Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

class DisplayController {
  public:
    void init();
    void displayBootMessage();
    void displayDateTime(String date, String time, String dayOfWeek);
    void blink();

  private:
    Adafruit_SSD1306 _display = { OLED_RESET_PIN };    // _display = Adafruit_SSD1306(OLED_RESET_PIN);
};

#endif /* display_controller_h */
