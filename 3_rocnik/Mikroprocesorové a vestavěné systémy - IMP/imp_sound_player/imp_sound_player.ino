#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include "sounds.h" // Zahrnutí knihovny se zvuky

#define OLED_MOSI 23
#define OLED_CLK 18
#define OLED_DC 17
#define OLED_CS 5
#define OLED_RESET 16

#define TOUCH_PIN 25 // GPIO 25 for touch sensor
#define AUDIO_PIN 26 // DAC2 for audio output

Adafruit_SSD1306 display(128, 64, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

int menuIndex = 0;        // Index of the current main menu item
int submenuIndex = 0;     // Index of the current submenu item
int soundSelection = -1;  // Selected sound in submenu
bool inSubmenu = false;   // Indicates if we are in the submenu

const char *menuItems[] = {"Play Sound", "Visualization", "Effects", "Choose a Sound"};
const int menuItemsCount = sizeof(menuItems) / sizeof(menuItems[0]);

const char *sounds[] = {"Crowd Laugh", "Bit Melody", "Brainstorm", "Mario", "Test", "Alarm", "Crash Bandicoot", "Cartoon", "Back"};
const uint8_t *soundData[] = {crowd_laugh, bit_melody, brainstorm, mario, test, alarm_sound, crash_bandicoot, cartoon};
const size_t soundLengths[] = {
    sizeof(crowd_laugh), sizeof(bit_melody), sizeof(brainstorm),
    sizeof(mario), sizeof(test), sizeof(alarm_sound),
    sizeof(crash_bandicoot), sizeof(cartoon)
};
const int soundsCount = sizeof(sounds) / sizeof(sounds[0]);

const int displayLines = 6; // Maximum number of lines visible on display at once
int displayOffset = 0;      // Offset for scrolling through menu


const char *effects[] = {"No Effect", "Reverse", "Noise", "Speed up","Slow down", "Back"};
int selectedEffect = 0; // Default: no effect
const int effectsCount = sizeof(effects) / sizeof(effects[0]);
bool inEffectsMenu = false; // Indicates if we're in the effects menu

//Function for playing a sound
void playSound(const uint8_t *data, size_t length) {
    //Display clear
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextColor(SSD1306_WHITE);

    if (selectedEffect == 0) { // Normal Playback
        // Display message during playback
        display.println("Audio Playing");
        display.println("With No Effect");
        display.display();
        for (size_t i = 0; i < length; i++) {
            dacWrite(AUDIO_PIN, data[i]);
            delayMicroseconds(125);  // Maintain sample rate
        }
    } else if (selectedEffect == 1) { // Reverse Playback
          // Display message during playback
          display.println("Audio Playing");
          display.println("With Reverse Effect");
          display.display();

        for (size_t i = length - 1; i > 0; i--) {
            dacWrite(AUDIO_PIN, data[i]);
            delayMicroseconds(125);  // Maintain sample rate
        }
    } else if (selectedEffect == 2) { // White Noise Effect
        // Display message during playback
        display.println("Audio Playing");
        display.println("With Noise Effect");
        display.display();

        for (size_t i = 0; i < length; i++) {
            uint8_t noisySample = data[i] + random(-20, 20); // Add random noise
            noisySample = constrain(noisySample, 0, 255); // Ensure within valid range
            dacWrite(AUDIO_PIN, noisySample);
            delayMicroseconds(125);  // Maintain sample rate
        }
    } else if (selectedEffect == 3) { // Speed Up
        // Display message during playback
        display.println("Audio Playing");
        display.println("With Speed Up Effect");
        display.display();

        for (size_t i = 0; i < length; i += 2) {
            dacWrite(AUDIO_PIN, data[i]);

            delayMicroseconds(125);  // Maintain sample rate
        }
    } else if (selectedEffect == 4) { // Slow Down
          
          // Display message during playback
          display.clearDisplay();
          display.setCursor(0, 0);
          display.setTextColor(SSD1306_WHITE);
          display.println("Audio Playing");
          display.println("With Slow Down Effect");
          display.display();

        for (size_t i = 0; i < length; i++) {
            dacWrite(AUDIO_PIN, data[i]);
            delayMicroseconds(250);  // Double delay for slower playback
        }
    }

    // Stop audio and return to the main menu
        dacWrite(AUDIO_PIN, 0);  // Stop DAC output
        display.clearDisplay();
        displayMenu();  // Return to the main menu
  
}

//Function for sound with visualisation
void playSoundWithVisualization(const uint8_t *data, size_t length) {
    display.clearDisplay();
    size_t updateInterval = 400; // Update visualization every x samples
    for (size_t i = 0; i < length; i++) {
        dacWrite(AUDIO_PIN, data[i]);

        // Update visualization every updateInterval samples
        if (i % updateInterval == 0) {
            int amplitude = map(data[i], 0, 255, 0, 64); // Map amplitude to bar height
            display.clearDisplay();
            display.fillRect(10, 64 - amplitude, 108, amplitude, SSD1306_WHITE); // Draw bar
            display.setCursor(0, 0);
            display.setTextColor(SSD1306_WHITE);
            display.println("Sound Visualization");
            display.display();
        }

        delayMicroseconds(125); // Maintain sample rate
    }

    // Clear display after playback
    display.clearDisplay();
    displayMenu();
}


void displayMenu() {
    display.clearDisplay();
    if (inEffectsMenu) { // Effects menu
        for (int i = 0; i < effectsCount; i++) {
            display.setCursor(0, i * 10);
            if (i == submenuIndex) {
                display.fillRect(0, i * 10, 128, 10, SSD1306_WHITE);
                display.setTextColor(SSD1306_BLACK);
            } else {
                display.setTextColor(SSD1306_WHITE);
            }
            display.println(effects[i]);
        }
    } else if (inSubmenu) { // Sounds menu
        for (int i = displayOffset; i < displayOffset + displayLines && i < soundsCount; i++) {
            display.setCursor(0, (i - displayOffset) * 10);
            if (i == submenuIndex) {
                display.fillRect(0, (i - displayOffset) * 10, 128, 10, SSD1306_WHITE);
                display.setTextColor(SSD1306_BLACK);
            } else {
                display.setTextColor(SSD1306_WHITE);
            }
            display.println(sounds[i]);
        }
    } else { // Main menu
        for (int i = 0; i < menuItemsCount; i++) {
            display.setCursor(0, i * 10);
            if (i == menuIndex) {
                display.fillRect(0, i * 10, 128, 10, SSD1306_WHITE);
                display.setTextColor(SSD1306_BLACK);
            } else {
                display.setTextColor(SSD1306_WHITE);
            }
            display.println(menuItems[i]);
        }
    }
    display.display();
}


void handleTouch() {
    static unsigned long touchStartTime = 0;
    static bool isLongPress = false;

    int touchState = digitalRead(TOUCH_PIN);

    if (touchState == HIGH) {
        if (touchStartTime == 0) {
            touchStartTime = millis();
        } else if (millis() - touchStartTime >= 500 && !isLongPress) {
            isLongPress = true;

            if (!inSubmenu && !inEffectsMenu) {
                if (menuIndex == 0 && soundSelection != -1) {
                    playSound(soundData[soundSelection], soundLengths[soundSelection]);
                } else if (menuIndex == 1 && soundSelection != -1) {
                    playSoundWithVisualization(soundData[soundSelection], soundLengths[soundSelection]);
                } else if ((menuIndex == 0 || menuIndex == 1) && soundSelection == -1) {
                    display.clearDisplay();
                    display.setCursor(0, 0);
                    display.setTextColor(SSD1306_WHITE);
                    display.println("No Sound Selected");
                    display.display();
                    delay(1000);
                    displayMenu();
                } else if (menuIndex == 2) { // Enter Effects menu
                    inEffectsMenu = true;
                    submenuIndex = 0;
                    displayMenu();
                } else if (menuIndex == 3) { // Enter Sounds menu
                    inSubmenu = true;
                    submenuIndex = 0;
                    displayOffset = 0;
                    displayMenu();
                }
            } else if (inEffectsMenu) { // Handle Effects menu selection
                if (submenuIndex != effectsCount - 1) { // Not "Back"
                    selectedEffect = submenuIndex;
                }
                inEffectsMenu = false;
                displayMenu();
            } else if (inSubmenu) { // Handle Sounds menu selection
                if (submenuIndex != soundsCount - 1) { // Not "Back"
                    soundSelection = submenuIndex;
                }
                inSubmenu = false;
                displayMenu();
            }
        }
    } else {
        if (touchStartTime > 0 && !isLongPress) {
            if (!inSubmenu && !inEffectsMenu) {
                menuIndex = (menuIndex + 1) % menuItemsCount;
            } else if (inEffectsMenu) {
                submenuIndex = (submenuIndex + 1) % effectsCount;
            } else {
                submenuIndex = (submenuIndex + 1) % soundsCount;
                if (submenuIndex >= displayOffset + displayLines) {
                    displayOffset++;
                } else if (submenuIndex < displayOffset) {
                    displayOffset = 0;
                }
            }
            displayMenu();
        }

        touchStartTime = 0;
        isLongPress = false;
    }
}


void setup() {
    Serial.begin(115200);
    pinMode(TOUCH_PIN, INPUT);
    pinMode(AUDIO_PIN, OUTPUT);

    if (!display.begin(SSD1306_SWITCHCAPVCC)) {
        Serial.println("OLED Initialization Failed");
        while (true);
    }
    displayMenu();
}

void loop() {
    handleTouch();
}
