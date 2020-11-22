// Standard Arduino libraries.
#include <SoftwareSerial.h>       // Serial communication for the DFPlayer (https://www.arduino.cc/en/Reference/softwareSerial)
#include <SPI.h>                  // I2C for the display (https://www.arduino.cc/en/Reference/SPI)
#include <Wire.h>                 // I2C for the display (https://www.arduino.cc/en/Reference/Wire)

// Third-party libraries.
#include <DFRobotDFPlayerMini.h>  // DFRobotDFPlayerMini library by DFRobot (https://github.com/DFRobot/DFRobotDFPlayerMini), MP3 source: https://www.101soundboards.com/
#include <OneButton.h>            // OneButton by Matthias Hertel (https://github.com/mathertel/OneButton)
#include <Adafruit_SSD1306.h>     // Adafruit SSD1306 Wemos Mini OLED by Adafruit + mcauser (https://github.com/stblassitude/Adafruit_SSD1306_Wemos_OLED)
#include <WiFiManager.h>          // To manage network configuration and connection.
#include <ezTime.h>               // ezTime by Rop Gonggrijp (https://github.com/ropg/ezTime)

// Pin configuration.
#define BUTTON_PIN D7             // Wemos pin - button - Wemos GND
#define TO_PLAYER_PIN D5          // Wemos pin -> DFPlayer RX pin
#define FROM_PLAYER_PIN D6        // Wemos pin <- DFPlayer TX pin

// Display connection.
// SCL GPIO5 D1
// SDA GPIO4 D2
#define OLED_RESET_PIN 0  // GPIO0
Adafruit_SSD1306 display(OLED_RESET_PIN);

#if (SSD1306_LCDHEIGHT != 48)
#error("[Display] Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

// DFPlayer connection.
SoftwareSerial playerSoftwareSerial(FROM_PLAYER_PIN, TO_PLAYER_PIN); // RX, TX
DFRobotDFPlayerMini player;

// Button connections.
OneButton btn = OneButton(BUTTON_PIN, true, true);  // true: button is active LOW (connected to GND), true: internal pull-up resistor enabled.

// Network configuration.
const char* WIFI_AP_SSID = "MiniPlayer";       // The name of the wireless network to create if cannot connect using the previously saved credentials.
const char* WIFI_AP_PASSWORD = "MiniPlayer!";  // The password required to connect to the wireless network used to configure the network parameters.

WiFiManager wifiManager;

// Time server connection.
Timezone timeZone;

void setup()
{
  setupSerial();
  setupDisplay();
  setupPlayer();
  setupButton();
  setupNetwork();
  setupTimeServerConnection();

  displayBootMessage();

  displayDateTime();
}

void setupSerial() {
  Serial.begin(115200);
  Serial.println(F("Starting..."));
}

void setupDisplay() {
  Serial.print(F("[Display] Initializing..."));
  // By default, we'll generate the high voltage from the 3.3v line internally.
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 64x48).
  Serial.println(F("DONE."));
}

void setupPlayer() {
  Serial.println(F("[Player] Initializing... (may take 3-5 seconds)"));

  playerSoftwareSerial.begin(9600);

  while (!player.begin(playerSoftwareSerial)) {
    Serial.println(F("[Player] Unable to begin:"));
    Serial.println(F("[Player] 1.Please recheck the connection!"));
    Serial.println(F("[Player] 2.Please insert the SD card!"));
   // while(true){
   //   delay(0); // Code to compatible with ESP8266 watch dog.
   // }
  }
  Serial.println(F("[Player] Initialization completed successfully."));

  player.volume(20);  // Set volume value. From 0 to 30
  player.play(1);     // Play the first mp3, remove this line if you do not want a voice line on power-up
}

void setupButton() {
  btn.attachClick(onButtonClicked);
  btn.attachDoubleClick(onButtonDoubleClicked);
}

void setupNetwork() {
  Serial.printf("[Network] Initializing with MAC address %s (SSID: %s)...\n", WiFi.macAddress().c_str(), WIFI_AP_SSID);
  wifiManager.autoConnect(WIFI_AP_SSID, WIFI_AP_PASSWORD);
  Serial.printf("[Network] Connected. IP address: %s, MAC address: %s\n", WiFi.localIP().toString().c_str(), WiFi.macAddress().c_str());
}

void setupTimeServerConnection() {
  Serial.println(F("[NTP] Initializing time server connection..."));
  setDebug(DEBUG);

  waitForSync();

  timeZone.setLocation(F("Europe/Budapest"));  // https://en.wikipedia.org/wiki/List_of_tz_database_time_zones
  Serial.print(F("[NTP] Current local time: "));
  Serial.println(timeZone.dateTime());

  setAlarmTrigger();
}

void displayBootMessage() {
  // The display buffer is intialized with an Adafruit logo internally, so first let's delete that.
  display.clearDisplay();

  display.setTextColor(WHITE);
  display.setTextSize(1);  // Size 1 means each character is 6px wide and 8px tall.
  display.setCursor(20,20);
  display.println("I am");
  display.setCursor(5,30);
  display.println("Iron Man!");

  display.display();
}

unsigned long lastTimeUpdate = millis();
unsigned long lastBlinkUpdate = millis();

void loop()
{
  // Update the button state.
  btn.tick();

  // Send debug information.
  printPlayerStatus();

  events();

  unsigned long currentTime = millis();

  if(currentTime - lastTimeUpdate > 60000) {
    displayDateTime();
    lastTimeUpdate = currentTime;
  }

  if(currentTime - lastBlinkUpdate > 1000) {
    display.setTextSize(2);        // Size 2 means each character is 12px wide and 14px tall.
    display.setCursor(2+12+12,0);  // Move right with the width of the 2 characters of the hour part.
    display.setTextColor(INVERSE);
    display.print(":");
    display.display();

    lastBlinkUpdate = currentTime;
  }
}

void displayDateTime() {
  display.clearDisplay();

  display.setTextSize(2);  // Size 2 means each character is 12px wide and 14px tall.
  display.setCursor(2,0);
  display.print(timeZone.dateTime("H:i"));

  display.setTextSize(1);  // Size 1 means each character is 6px wide and 8px tall.
  display.setCursor(2,25);
  display.print(timeZone.dateTime("Y.m.d"));

  display.setCursor(4,40);
  display.print(timeZone.dateTime("l"));

  display.display();
}

void setAlarmTrigger() {
  time_t alarmTime = makeTime(timeZone.hour(), timeZone.minute()+1, timeZone.second(), timeZone.day(), timeZone.month(), timeZone.year());
  timeZone.setEvent(onAlarmTriggered, alarmTime);
}

void onAlarmTriggered() {
  Serial.println(F("[NTP] Alarm triggered!"));

  // player.play(2);

  // Event is fired only once, so we have to set it up again.
  setAlarmTrigger();
}

void onButtonClicked() {
  Serial.println(F("[Button] Clicked"));
  player.pause();
}

void onButtonDoubleClicked() {
  Serial.println(F("[Button] Double clicked"));
  player.start();
}

void printPlayerStatus() {
  if (player.available()) {
    uint8_t type = player.readType();
    int value = player.read();

    switch (type) {
      case TimeOut:
        Serial.println(F("[Player] Time Out!"));
        break;
      case WrongStack:
        Serial.println(F("[Player] Stack Wrong!"));
        break;
      case DFPlayerCardInserted:
        Serial.println(F("[Player] Card Inserted!"));
        break;
      case DFPlayerCardRemoved:
        Serial.println(F("[Player] Card Removed!"));
        break;
      case DFPlayerCardOnline:
        Serial.println(F("[Player] Card Online!"));
        break;
      case DFPlayerPlayFinished:
        Serial.print(F("[Player] Number:"));
        Serial.print(value);
        Serial.println(F(" Play Finished!"));
        break;
      case DFPlayerError:
        Serial.print(F("[Player] DFPlayerError:"));
        switch (value) {
          case Busy:
            Serial.println(F("Card not found"));
            break;
          case Sleeping:
            Serial.println(F("Sleeping"));
            break;
          case SerialWrongStack:
            Serial.println(F("Get Wrong Stack"));
            break;
          case CheckSumNotMatch:
            Serial.println(F("Check Sum Not Match"));
            break;
          case FileIndexOut:
            Serial.println(F("File Index Out of Bound"));
            break;
          case FileMismatch:
            Serial.println(F("Cannot Find File"));
            break;
          case Advertise:
            Serial.println(F("In Advertise"));
            break;
          default:
            break;
        }
        break;
      default:
        break;
    }
  }
}
