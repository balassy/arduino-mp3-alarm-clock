#include "DisplayController.h"

void DisplayController::init() {
  Serial.print(F("[Display] Initializing..."));
  // By default, we'll generate the high voltage from the 3.3v line internally.
  _display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 64x48).
  Serial.println(F("DONE."));
}


void DisplayController::displayBootMessage() {
  // The display buffer is intialized with an Adafruit logo internally, so first let's delete that.
  _display.clearDisplay();

  _display.setTextColor(WHITE);
  _display.setTextSize(1);  // Size 1 means each character is 6px wide and 8px tall.
  _display.setCursor(20,20);
  _display.println("I am");
  _display.setCursor(5,30);
  _display.println("Iron Man!");

  _display.display();
}

void DisplayController::displayDateTime(String date, String time, String dayOfWeek) {
  _display.clearDisplay();

  _display.setTextSize(2);  // Size 2 means each character is 12px wide and 14px tall.
  _display.setCursor(2,0);
  _display.print(time);

  _display.setTextSize(1);  // Size 1 means each character is 6px wide and 8px tall.
  _display.setCursor(2,25);
  _display.print(date);

  _display.setCursor(4,40);
  _display.print(dayOfWeek);

  _display.display();
}

void DisplayController::blink() {
  _display.setTextSize(2);        // Size 2 means each character is 12px wide and 14px tall.
  _display.setCursor(2+12+12,0);  // Move right with the width of the 2 characters of the hour part.
  _display.setTextColor(INVERSE);
  _display.print(":");
  _display.display();
}
