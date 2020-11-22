// Network configuration.
const char* WIFI_AP_SSID = "MiniPlayer";       // The name of the wireless network to create if cannot connect using the previously saved credentials.
const char* WIFI_AP_PASSWORD = "MiniPlayer!";  // The password required to connect to the wireless network used to configure the network parameters.

// Pin configuration.
#define BUTTON_PIN D7             // Wemos pin - button - Wemos GND
#define TO_PLAYER_PIN D5          // Wemos pin -> DFPlayer RX pin
#define FROM_PLAYER_PIN D6        // Wemos pin <- DFPlayer TX pin

// Time server configuration.
const char* TIMEZONE_NAME = "Europe/Budapest"; // https://en.wikipedia.org/wiki/List_of_tz_database_time_zones

// Sound configuration.
// Get MP3 files from https://www.101soundboards.com/.
// IMPORTANT: The order in which the files are written to the SD card determines the file number, not the file name!
#define BOOT_SOUND_FILE_NUMBER 1
#define ALARM_SOUND_FILE_NUMBER 2
