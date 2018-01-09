#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>

#define LEDPIN 6  // The datapin for the LED rings

// Modes. Add or remove if need. Mostly for readability.
#define SPIRAL 0
#define EQUALIZER 1
#define BREATHE 2
#define RAINBOW 3
#define NUMMODES 4

const int MODE_ADDRESS = 0;      // The address where we store the mode. Don't really need to store the mode, but what the heck, why not?

// General LED ring info
const uint8_t NUMPIXELS = 60;    // Total  number of pixels
const uint8_t OUTER_START = 0;   // When the outer ring starts
const uint8_t OUTER_END = 24;    // When the outer ring ends + 1 (for looping)
const uint8_t CENTER_START = 24; // When the center ring starts
const uint8_t CENTER_END = 48;   // When the center ring ends + 1 (for looping)
const uint8_t INNER_START = 48;  // When the inner ring starts
const uint8_t INNER_END = 60;    // When the inner ring ends 0 1 (for looping)

// Colors. Some value between 0-255
// Neopixels are really bright, so you should keep these low unless you want a rave party
// Green Light
const uint8_t GREEN_R = 0;
const uint8_t GREEN_G = 50;
const uint8_t GREEN_B = 0;

// Yellow Light
const uint8_t YELLOW_R = 50;
const uint8_t YELLOW_G = 25;
const uint8_t YELLOW_B = 0;

// Orange Light
const uint8_t ORANGE_R = 50;
const uint8_t ORANGE_G = 10;
const uint8_t ORANGE_B = 0;

// Red Light
const uint8_t RED_R = 50;
const uint8_t RED_G = 0;
const uint8_t RED_B = 0;

// The LED Rings, connected in series
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, LEDPIN);

const uint8_t AUDIO_SIZE = 6;   // The number of filters we have. Change if needed
const uint8_t AUDIO_PINS[AUDIO_SIZE] = { A0, A1, A2, A3, A4, A5 };  // The pins. Add or remove if needed.
int16_t audioVals[AUDIO_SIZE];

uint8_t bands[6][10];   // 6 bands, each with 10 LEDs (I have 60 total). Change it to match your setup

const uint8_t SAMPLE_TIMES = 1;   // To amplify the audio signal by reading multiple times. Change if needed

uint8_t mode = EQUALIZER;   // The display mode.

void setup() {
//  analogReference(INTERNAL);    // The voltage from the audio is only a couple of millivolts, so INTERNAL is needed
  for (uint8_t i = 0; i < AUDIO_SIZE; i++) {
    analogRead(AUDIO_PINS[i]);    // Read analog pin to clear data
    audioVals[i] = 0;   // Initialize the audioVals array
  }
  
// Serial.begin(9600);    // Uncomment for debugging
  mode = EEPROM.read(MODE_ADDRESS);
  
  for (uint8_t n = 0; n < AUDIO_SIZE; n++) {    // Initialize bands[][]
    for (uint8_t i = 0; i < 2; i++) {
      bands[n][i] = INNER_START + i + n * 2;
    }
    for (uint8_t i = 0; i < 4; i++) {
      bands[n][i + 2] = CENTER_START + i + n * 4;
    }
    for (uint8_t i = 0; i < 4; i++) {
      bands[n][i +6] = OUTER_START + i + n * 4;
    }
  }

  for (uint8_t i = 0; i < 6; i++) {
    for (uint8_t j = 0; j < 10; j++) {
      Serial.print(bands[i][j]);
      Serial.print("\t");
    }
    Serial.println();
  }
  
  pixels.begin();
  attachInterrupt(0, switchMode, RISING);
}

void loop() {
  switch(mode) {
    case SPIRAL:
      spiral();
      break;
    case EQUALIZER:
      equalizer();
      break;
    case BREATHE:
      breathe();
      break;
    case RAINBOW:
      rainbow();
      break;
    default:
      equalizer();
  }
}

void spiral() {
  uint8_t color = 50;
  for(int8_t n = 0; n < 2; n++) {
    for(int8_t i = 59; i > 0; i-=2) {
      pixels.setPixelColor(i, color * n, color * n, color * n);
      pixels.show();
      delay(10);
    }
    for(uint8_t i = 0; i < NUMPIXELS; i+=2) {
      pixels.setPixelColor(i, color * n, color * n, color * n);
      pixels.show();
      delay(10);
    } 
  }
}

void breathe() {
  uint8_t red = 0;
  uint8_t green = 50;
  uint8_t blue = 50;
  for(uint8_t n = 0; n < 50; n++) {
    green--;
    blue--;
      for(uint8_t i = 0; i < NUMPIXELS; i++) {
        pixels.setPixelColor(i, red, green, blue);
      }
    pixels.show();
    delay(10);
  }
  for(uint8_t n = 0; n < 50; n++) {
    green++;
    blue++;
      for(uint8_t i = 0; i < NUMPIXELS; i++) {
        pixels.setPixelColor(i, red, green, blue);
      }
    pixels.show();
    delay(10);
  }
}

void rainbow() {
  uint8_t b_red = 50;
  uint8_t b_green = 0;
  uint8_t b_blue = 0;
  for(uint8_t x = 0; x < 6; x++) {
    for(uint8_t n = 0; n < 50; n++) {
      for(uint8_t i = 0; i < NUMPIXELS; i++) {
        pixels.setPixelColor(i, b_red, b_green, b_blue);
      }
      pixels.show();

      switch(x) {
        case 0:
        b_green++;
        break;
        case 1:
        b_red--;
        break;
        case 2:
        b_blue++;
        break;
        case 3:
        b_green--;
        break;
        case 4:
        b_red++;
        break;
        case 5:
        b_blue--;
        break;
      }
    }
  }  
}

void equalizer() {
  // Zero out audioVals
  for (uint8_t i = 0; i < AUDIO_SIZE; i++) {
    audioVals[i] = 0;
  }
  // Sample multiple times to increase effect. Audio signal is really weak from headphone jack
  for (uint8_t x = 0; x < SAMPLE_TIMES; x++) {
    for (uint8_t i = 0; i < AUDIO_SIZE; i++) {
      audioVals[i] += analogRead(AUDIO_PINS[i]);
    }
  }
  
  for (uint8_t i = 0; i < AUDIO_SIZE; i++) {
    Serial.print(audioVals[i]);
    Serial.print("\t");
    equalizerSetPixel(i, audioVals[i]);
  }
  Serial.println();
  pixels.show();
  delayMicroseconds(50);
}

void equalizerSetPixel(uint8_t band, uint16_t value) {
  uint8_t red, green, blue;
  for (uint8_t i = 0; i <= 10; i++) {
    if(i < 2) {
      red = GREEN_R;
      green = GREEN_G;
      blue = GREEN_B;
    } else if (i < 6) {
      red = ORANGE_R;
      green = ORANGE_G;
      blue = ORANGE_B;
    } else {
      red = RED_R;
      green = RED_G;
      blue = RED_B;
    }
      
    // Turn on pixel if it is > than the threshold
    if (value > (i + 1) * (i + 1)) {
      pixels.setPixelColor(bands[band][i], red, green, blue);
    } else {
      pixels.setPixelColor(bands[band][i], 0, 0, 0);
    }
  }
}

void switchMode() {
  mode = (mode + 1) % NUMMODES;
  EEPROM.put(MODE_ADDRESS, mode);
}

