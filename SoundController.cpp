#include "SoundController.h"

void SoundController::init(int8_t rxPin, int8_t txPin, int initSoundFileNumber) {
  Serial.println(F("[Player] Initializing... (may take 3-5 seconds)"));

  _softwareSerial = new SoftwareSerial(rxPin, txPin);
  _softwareSerial->begin(9600);

  while (!_player.begin(*_softwareSerial)) {
    Serial.println(F("[Player] Unable to begin:"));
    Serial.println(F("[Player] 1.Please recheck the connection!"));
    Serial.println(F("[Player] 2.Please insert the SD card!"));
   // while(true){
   //   delay(0); // Code to compatible with ESP8266 watch dog.
   // }
  }
  Serial.println(F("[Player] Initialization completed successfully."));

  _player.volume(20);  // Set volume value. From 0 to 30
  _player.play(initSoundFileNumber);

}


void SoundController::play(int fileNumber) {
  Serial.println(F("[Player] Playing..."));
  _player.play(fileNumber);
}

void SoundController::pause() {
  Serial.println(F("[Player] Pausing..."));
  _player.pause();
}


void SoundController::start() {
  Serial.println(F("[Player] Starting..."));
  _player.start();
}


void SoundController::printStatus() {
  if (_player.available()) {
    uint8_t type = _player.readType();
    int value = _player.read();

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
