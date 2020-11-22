// Network configuration.
const char* WIFI_AP_SSID = "MiniPlayer";       // The name of the wireless network to create if cannot connect using the previously saved credentials.
const char* WIFI_AP_PASSWORD = "MiniPlayer!";  // The password required to connect to the wireless network used to configure the network parameters.

// Pin configuration.
#define BUTTON_PIN D7             // Wemos pin - button - Wemos GND
#define TO_PLAYER_PIN D5          // Wemos pin -> DFPlayer RX pin
#define FROM_PLAYER_PIN D6        // Wemos pin <- DFPlayer TX pin
