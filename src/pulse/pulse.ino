#define PIXEL_PIN A1
#define NUM_LEDS 30
#define MIC_PIN A5

#include <FFT.h>
#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

int rgb[3] = {255, 0, 0};
uint32_t leds[NUM_LEDS];

unsigned int signalMax = 0;
unsigned int signalMin = 1024;
unsigned int peakToPeak;

uint32_t curColor;

int switch_count = 0;

uint32_t makeColor() {
  int change_prob = random(100);
  if (change_prob > 99) {
    int change_color = random(3);
    rgb[change_color] = random(150);
  }
//  if (2 * rgb[0] - rgb[1] - rgb[2] >= -5 && 2 * rgb[0] - rgb[1] - rgb[2] <= 5) {
//    switch_count++;
//  } else {
//    switch_count = 0;
//  }
  switch_count++;
  if (switch_count == 50) {
    for (int i = 0; i < 3; i++) {
      rgb[i] = random(255);
    }
    switch_count = 0;
  }
  int max = 0;
  for (int i = 0; i < 3; i++) {
    if (rgb[i] > rgb[max]) {
      max = i;
    }
  }
  rgb[max] -= 5;
  rgb[(max + 1) % 3] += 5;
  return strip.Color(rgb[0], rgb[1], rgb[2]);
}

void updateStripSnake(uint32_t newColor) {
  for (int i = NUM_LEDS; i != 0; i--) {
    leds[i] = leds[i - 1];
  }
  leds[0] = newColor;
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, leds[i]);
  }
  strip.show();
}

void updateStripPulse(uint32_t newColor)
{
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, newColor);
  }
  strip.show();
}

const int sampleWindow = 30; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;
 
void setup() 
{
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  Serial.begin(9600);
}
 
 
void loop() 
{
   unsigned long startMillis= millis();  // Start of sample window
   unsigned int peakToPeak = 0;   // peak-to-peak level
 
   unsigned int signalMax = 0;
   unsigned int signalMin = 1024;
 
   // collect data for 50 mS
   while (millis() - startMillis < sampleWindow)
   {
      sample = analogRead(MIC_PIN);
      if (sample < 1024)  // toss out spurious readings
      {
         if (sample > signalMax)
         {
            signalMax = sample;  // save just the max levels
         }
         else if (sample < signalMin)
         {
            signalMin = sample;  // save just the min levels
         }
      }
   }
   peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
   double volts = (peakToPeak * 3.3) / 1024;  // convert to volts
 
//  Serial.println(volts);
  
  curColor = makeColor();
  Serial.println(rgb[0]);
  Serial.println(rgb[1]);
  Serial.println(rgb[2]);
  Serial.println();
  updateStripSnake(curColor);
}
  

//void loop()
//{
//  unsigned int sample = analogRead(MIC_PIN);
//  Serial.println(sample);

//}


