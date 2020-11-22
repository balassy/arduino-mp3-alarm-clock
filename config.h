// Network configuration.
const char* WIFI_AP_SSID = "RocketClock";  // The name of the wireless network to create if cannot connect using the previously saved credentials.
const char* WIFI_AP_PASSWORD = "Rocket!";  // The password required to connect to the wireless network used to configure the network parameters.

// Time server configuration.
const char* NTP_SERVER_HOST = "hu.pool.ntp.org";
const float TIME_ZONE = 1.0;  // Time zone in floating point (e.g. for India).
const int DST_ADJUSTMENT = 1; // 1 for European summer time; 2 for US daylight saving time; 0 for no DST adjustment.

// Define this constant to log debug messages from the NTPTimeESP library.
// #define DEBUG_ON

// Time period configuration.
const byte DAYTIME_BEGIN_HOUR = 7;
const byte DAYTIME_END_HOUR = 20;

// Time server access.
const int NTP_MAX_RETRY_COUNT = 10;
const int NTP_RETRY_DELAY_MSEC = 5000;

// Pin configuration.
const uint8_t PIN_DAY_LED = D6;
const uint8_t PIN_NIGHT_LED = D5;

