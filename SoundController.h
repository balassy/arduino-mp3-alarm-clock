#ifndef sound_controller_h
#define sound_controller_h

// Standard Arduino library.
#include <SoftwareSerial.h>       // Serial communication for the DFPlayer (https://www.arduino.cc/en/Reference/softwareSerial)

// Third-party DFPlayer Mini board controller library.
#include <DFRobotDFPlayerMini.h>  // DFRobotDFPlayerMini library by DFRobot (https://github.com/DFRobot/DFRobotDFPlayerMini), MP3 source: https://www.101soundboards.com/

class SoundController {
  public:
    void init(int8_t rxPin, int8_t txPin, int initSoundFileNumber);
    void play(int fileNumber=1);
    void pause();
    void start();
    void printStatus();

  private:
    SoftwareSerial* _softwareSerial;
    DFRobotDFPlayerMini _player;
};

#endif /* sound_controller_h */
