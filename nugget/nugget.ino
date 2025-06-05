
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>
#include <Wire.h>

// OLED display configuration

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// MP3 module serial configuration
#define MP3_RX 2
#define MP3_TX 3  

SoftwareSerial mp3Serial(MP3_RX, MP3_TX);


// EQ button configuration
#define EQ_BUTTON_PIN 8
bool lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;
const char* eqModes[] = {"Normal",  "Pop", "Rock", "Jazz","Classic", "Bass"};
uint8_t currentEQ = 0;

// Serial commands as hex
const byte GET_CURRENT_TRACK[] = {0x7E, 0xFF, 0x06, 0x4C, 0x00, 0x00, 0x00, 0xFE, 0xAF, 0xEF};
const byte GET_EQ_MODE[] = {0x7E, 0xFF, 0x06, 0x44, 0x00, 0x00, 0x00, 0xFE, 0xB7, 0xEF};
const byte CYCLE_EQ[] = {0x7E, 0xFF, 0x06, 0x07, 0x00,0x00, 0x00, 0xFE, 0xF3, 0xEF};

// Splash icon
// See README.md for an easy way to edit this
void drawIcon() {
  const uint8_t iconBitmap[32][32] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0},
  {0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0},
  {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
  {0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0},
  {1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1},
  {1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1},
  {1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1},
  {1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1},
  {0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0},
  {0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0},
  {0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0},
  {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
  {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
  {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

// if you add your own bitmap, make sure to edit the value below 
// it should match the numbers in iconBitmap[num][num]
  int bitmapPx = 32;
  int xOffset = (SCREEN_WIDTH - bitmapPx) / 2;
  int yOffset = (SCREEN_HEIGHT - bitmapPx) / 2;
  for (uint8_t y = 0; y < bitmapPx; y++) {
    for (uint8_t x = 0; x < bitmapPx; x++) {
      if (iconBitmap[y][x]) {
        display.drawPixel(x + xOffset, y + yOffset, SSD1306_WHITE);
      }
    }
  }

  display.display();
}

void setup() {
  pinMode(EQ_BUTTON_PIN, INPUT_PULLUP);
  
  // Start serial communication
  mp3Serial.begin(9600);

  // OLED init
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for (;;);
  }

  display.clearDisplay();
  drawIcon();
  display.display();
  delay(2000);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.display();
}

void loop() {

  // EQ button
  bool reading = digitalRead(EQ_BUTTON_PIN);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (lastButtonState == HIGH && reading == LOW) {  // Button press detected
      currentEQ = (currentEQ + 1) % 6;
      byte setEQCommand[] = {0x7E, 0xFF, 0x06, 0x07, 0x00,0x00, currentEQ, 0x00, 0x00, 0xEF};
      uint16_t checksum = calculateChecksum(setEQCommand);

      setEQCommand[7] = (checksum >> 8) & 0xFF;  // High byte
      setEQCommand[8] = checksum & 0xFF;  // Low byte

      mp3Serial.write(setEQCommand, sizeof(setEQCommand));
    }
  }

  lastButtonState = reading;

  // track num request
  mp3Serial.write(GET_CURRENT_TRACK, sizeof(GET_CURRENT_TRACK));

  delay(200);
  uint16_t trackNumber = readTrackNumber();

  // EQ mode request
  mp3Serial.write(GET_EQ_MODE, sizeof(GET_EQ_MODE));
  delay(200);
  uint8_t eqMode = readEQMode();
  if (eqMode < 6) {
    currentEQ = eqMode;
  }

  // display update
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Track: ");
  display.println(trackNumber);
  display.print("EQ: ");
  display.println(eqModes[currentEQ]);
  display.display();
  delay(500);
}

uint16_t calculateChecksum(byte* command) {
  uint16_t sum = 0;

  for (int i = 1; i < 7; i++) {
    sum += command[i];
  }

  return 0xFFFF - sum + 1;
}

uint16_t readTrackNumber() {
  byte response[10];

  int index = 0;

  unsigned long startTime = millis();

  while (millis() - startTime < 500) {
    if (mp3Serial.available()) {
      byte b = mp3Serial.read();

      if (index < 10) {
        response[index++] = b;
      }
    }
  }

  if (index >= 10 && response[3] == 0x4C) {
    return ((uint16_t)response[5] << 8) | response[6];
  }

  return 0;
}

uint8_t readEQMode() {
  byte response[10];

  int index = 0;

  unsigned long startTime = millis();

  while (millis() - startTime < 500) {
    if (mp3Serial.available()) {
      byte b = mp3Serial.read();

      if (index < 10) {
        response[index++] = b;
      }
    }
  }

  if (index >= 10 && response[3] == 0x44) {
    return response[6];
  }

  return currentEQ;
}