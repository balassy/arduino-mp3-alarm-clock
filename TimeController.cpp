#include "TimeController.h"

void TimeController::init(String timeZoneName) {
  Serial.println(F("[NTP] Initializing time server connection..."));
  setDebug(DEBUG);

  waitForSync();

  _timeZone.setLocation(timeZoneName);  // https://en.wikipedia.org/wiki/List_of_tz_database_time_zones
  Serial.print(F("[NTP] Current local time: "));
  Serial.println(_timeZone.dateTime());
}

String TimeController::getCurrentDate() {
  return _timeZone.dateTime("Y.m.d");
}

String TimeController::getCurrentTime() {
  return _timeZone.dateTime("H:i");
}

String TimeController::getCurrentDayOfWeek() {
  return _timeZone.dateTime("l");
}

uint8_t TimeController::setEvent(void (*function)()) {
  Serial.println(F("[NTP] Setting event."));
  time_t alarmTime = makeTime(_timeZone.hour(), _timeZone.minute()+1, _timeZone.second(), _timeZone.day(), _timeZone.month(), _timeZone.year());
  return _timeZone.setEvent(function, alarmTime);
}

void TimeController::processEvents() {
  events();
}
