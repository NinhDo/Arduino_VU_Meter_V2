#include <Adafruit_NeoPixel.h>

#define LEDPIN 6

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
const uint8_t ORANGE_G = 8;
const uint8_t ORANGE_B = 0;

// Red Light
const uint8_t RED_R = 50;
const uint8_t RED_G = 0;
const uint8_t RED_B = 0;

// Threshold of the colors (percentage). Red threshold is anything above ORANGE_THRESHOLD
const uint8_t GREEN_THRESHOLD = 60;
const uint8_t YELLOW_THRESHOLD = 75;
const uint8_t ORANGE_THRESHOLD = 90;

// The LED Rings, connected in series
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, LEDPIN, NEO_GRB + NEO_KHZ800);

const uint8_t AUDIO_SIZE = 6;
const uint8_t AUDIO_PINS[AUDIO_SIZE] = { A0, A1, A2, A3, A4, A5 }; 
int16_t audioVals[AUDIO_SIZE] = { 0, 0, 0, 0, 0, 0 };
uint8_t bands[6][10];
const uint8_t SAMPLE_TIMES = 5;

void setup() {
  analogReference(INTERNAL);
  for (uint8_t i = 0; i < AUDIO_SIZE; i++) {
    analogRead(AUDIO_PINS[i]);
  }
  //Serial.begin(9600);
  for (uint8_t n = 0; n < AUDIO_SIZE; n++) {
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
}

void loop() {
  // Zero out audioVals
  for (uint8_t i = 0; i < AUDIO_SIZE; i++) {
    audioVals[i] = 0;
  }
  // Sample multiple times to increase effect. Audio signal is really weak from headphone jack
  for (uint8_t x = 0; x < SAMPLE_TIMES; x++) {
    for (uint8_t i = 0; i < AUDIO_SIZE; i++) {
      audioVals[i] += analogRead(AUDIO_PINS[i]) * 2;
    }
  }
  /*for (uint8_t i = 0; i < AUDIO_SIZE; i++) {
    audioVals[i] = constrain(map(audioVals[i], 0, 200, 0, 1023), 0, 1000);
  }*/
  
  
  for (uint8_t i = 0; i < AUDIO_SIZE; i++) {
    Serial.print(audioVals[i]);
    Serial.print("\t");
    setPixel(i, audioVals[i]);
  }
  Serial.println();
  pixels.show();
  delayMicroseconds(50);
}

void setPixel(uint8_t band, uint16_t value) {
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

