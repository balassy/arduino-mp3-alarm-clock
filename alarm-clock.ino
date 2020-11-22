// Third-party libraries.
#include <OneButton.h>            // OneButton by Matthias Hertel (https://github.com/mathertel/OneButton)
#include <WiFiManager.h>          // To manage network configuration and connection.

// Local libraries.
#include "DisplayController.h"
#include "SoundController.h"
#include "TimeController.h"

// Configuration.
#include "config.h"

// Global feature controllers.
WiFiManager wifi;
DisplayController display;
SoundController sound;
TimeController ntpClient;
OneButton btn;

// Global variables.
unsigned long lastTimeUpdate = millis();
unsigned long lastBlinkUpdate = millis();

void setup()
{
  // Setup serial connection.
  Serial.begin(115200);
  Serial.println(F("Starting..."));

  // Initialize the display.
  display.init();
  display.displayBootMessage();

  // Initialize the MP3 player.
  sound.init(FROM_PLAYER_PIN, TO_PLAYER_PIN, BOOT_SOUND_FILE_NUMBER); // RX, TX

  // Configure the buttons.
  btn = OneButton(BUTTON_PIN, true, true);  // true: button is active LOW (connected to GND), true: internal pull-up resistor enabled.
  btn.attachClick(onButtonClicked);
  btn.attachDoubleClick(onButtonDoubleClicked);

  // Connect to the wireless network.
  Serial.printf("[Network] Initializing with MAC address %s (SSID: %s)...\n", WiFi.macAddress().c_str(), WIFI_AP_SSID);
  wifi.autoConnect(WIFI_AP_SSID, WIFI_AP_PASSWORD);
  Serial.printf("[Network] Connected. IP address: %s, MAC address: %s\n", WiFi.localIP().toString().c_str(), WiFi.macAddress().c_str());

  // Connect to the NTP server.
  ntpClient.init(TIMEZONE_NAME);
  setAlarmTrigger();

  displayCurrentDateTime();
}

void loop()
{
  // Update the button state.
  btn.tick();

  // Send player debug information.
  sound.printStatus();

  // Update the NTP clock and trigger alarms.
  ntpClient.processEvents();

  updateClockDisplay();
}

void displayCurrentDateTime() {
  display.displayDateTime(ntpClient.getCurrentDate(), ntpClient.getCurrentTime(), ntpClient.getCurrentDayOfWeek());
}

void setAlarmTrigger() {
  ntpClient.setEvent(onAlarmTriggered);
}

void onAlarmTriggered() {
  Serial.println(F("[NTP] Alarm triggered!"));

  // sound.play(ALARM_SOUND_FILE_NUMBER);

  // Event is fired only once, so we have to set it up again.
  setAlarmTrigger();
}

void onButtonClicked() {
  Serial.println(F("[Button] Clicked"));
  sound.pause();
}

void onButtonDoubleClicked() {
  Serial.println(F("[Button] Double clicked"));
  sound.start();
}

void updateClockDisplay() {
  unsigned long currentTime = millis();

  // Update the date and time in every minute.
  if(currentTime - lastTimeUpdate > 60000) {
    displayCurrentDateTime();
    lastTimeUpdate = currentTime;
  }

  // Make the ":" blink in every second.
  if(currentTime - lastBlinkUpdate > 1000) {
    display.blink();

    lastBlinkUpdate = currentTime;
  }
}
