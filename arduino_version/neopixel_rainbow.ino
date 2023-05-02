#include <Adafruit_NeoPixel.h>
#include <stdlib.h>

#define LIGHT_PIN   6
#define N_PIXELS    10
#define BRIGHTNESS  50 

Adafruit_NeoPixel pixels(N_PIXELS, LIGHT_PIN, NEO_GRBW + NEO_KHZ800);

char serial;

void setup() {  
  Serial.begin(9600);

  pixels.begin();           
  pixels.show();            
  pixels.setBrightness(BRIGHTNESS);
}

void loop() {
  delay(500);

  if (Serial.available() > 0) {
    serial = Serial.read();
    Serial.println(serial, HEX);

    if (serial == 'r')
      getWav();
  } else
    rainbowPixels();
}

// Thank you NeoPixel Library examples
void rainbowPixels() {
  int fadeVal=0, fadeMax=100;

  for (uint32_t curHue = 0; curHue < 3*65536; curHue += 256) { // for each hue in color wheel

    for (int i = 0; i < pixels.numPixels(); i++) { // for each pixel in strip
      uint32_t iHue = curHue + (i * 65536L / pixels.numPixels());
      pixels.setPixelColor(i, pixels.gamma32(pixels.ColorHSV(iHue, 255, 255 * fadeVal / fadeMax)));
    }

    pixels.show();
    delay(3);

    if(curHue < 65536) {                   // first loop           
      if(fadeVal < fadeMax) fadeVal++;     // fading in                  
    } else if(curHue >= ((2) * 65536)) {   // last loop           
      if(fadeVal > 0) fadeVal--;           // fading out                  
    } else {
      fadeVal = fadeMax;                   // consitent hue quality every other time
    }
  }
  
  // // A pulse of white
  // for(int k = 0; k < 1; k++) {

  //   // turns up white hue
  //   for(int j = 0; j < 256; j++) { 
  //     pixels.fill(pixels.Color(0, 0, 0, pixels.gamma8(j)));
  //     pixels.show();
  //   }

  //   delay(1000); 

  //   // turns down white hue
  //   for(int j = 255; j >= 0; j--) { 
  //     pixels.fill(pixels.Color(0, 0, 0, pixels.gamma8(j)));
  //     pixels.show();
  //   }
  // }

  delay(500);
}

void getWav() {  
  // Gets the length of the wav file from python
  while (Serial.available() == 0) {}
  int wavs = Serial.parseInt();
  bool switch_lights = true;
  while (Serial.available() == 0) {}
  int fromPy = Serial.parseInt();

  for (int i = 0; i < wavs; i++) {
    float db = fromPy * 10 / 255;
    Serial.print(" "); Serial.print(db);
    pixels.clear();

    if (switch_lights) { // every even pixel light up
      Serial.println("even");
      for (int j = 0; j < pixels.numPixels(); j++) {
        if (j % 2 == 0) pixels.setPixelColor(j, pixels.Color(245, 255 * db, 255));
      }
    } else { // every odd pixel light up
      Serial.println("odd");      
      for (int j = 0; j < pixels.numPixels(); j++) {
        if (j % 2 != 0) pixels.setPixelColor(j, pixels.Color(245, 255 * db, 255));
      }
    }

    pixels.show();
    switch_lights = !switch_lights;
    delay(200);

    while (Serial.available() == 0) {}
    fromPy = Serial.parseInt();
    Serial.println(fromPy);
  }

  // Purple wave (testing)
  // for(int i = 0; i < wavs; i++) {
  //   for (int j = 0; j < pixels.numPixels(); j++) { // for each pixel in strip
  //     pixels.setPixelColor(j, pixels.Color(255-pixels.numPixels()-j, 255 * fadeVal / fadeMax, 255));
  //     pixels.show();
  //     delay(250);
  //     pixels.clear();

  //     if (fadeVal < fadeMax) fadeVal += 100 / wavs;
  //   }

  //   for (int j = pixels.numPixels(); j > 0; j--) { // for each pixel in strip
  //     pixels.setPixelColor(j, pixels.Color(255-pixels.numPixels()+j, 255 * fadeVal / fadeMax, 255));
  //     pixels.show();
  //     delay(250);
  //     pixels.clear();

  //     if (fadeVal > 0) fadeVal -= 100 / wavs;
  //   }
  //   delay(3);
  // }
}
