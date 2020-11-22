#ifndef time_controller_h
#define time_controller_h

#include <Arduino.h>

// Third-party libraries.
#include <ezTime.h>   // ezTime by Rop Gonggrijp (https://github.com/ropg/ezTime)

class TimeController {
  public:
    void init(String timeZoneName);
    String getCurrentDate();
    String getCurrentTime();
    String getCurrentDayOfWeek();
    uint8_t setEvent(void (*function)());
    void processEvents();

  private:
    Timezone _timeZone;
};

#endif /* time_controller_h */
