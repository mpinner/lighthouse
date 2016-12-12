/** \file
   author: @mpinner @qrs

   much code came from : https://github.com/osresearch/lighthouse

   intent: play with light based on where it is

*/

#include "LighthouseSensor.h"
#include "LighthouseXYZ.h"

#include <math.h>
#include <EWMA.h>

#include <elapsedMillis.h>

#include "FastLED.h"



#define IR0 5
#define IR1 6
#define IR2 9
#define IR3 10
#define IR4 20
#define IR5 21
#define IR6 22
#define IR7 23

#define LED_PIN 2
#define LED_COUNT 8

CRGB leds[LED_COUNT];

EWMA ewma[20];

elapsedMillis lastUpdate;
int interval = 40;



// my lightsources from
// https://github.com/mpinner/openvr/tree/master/samples/hellovr_opengl
static lightsource lightsource1 = {
  { 0.779680, 0.024346, -0.625704,
    -0.178665, 0.966355, -0.185031,
    0.600148, 0.256057, 0.757798
  },
  { 1.560562, 2.315096, 1.997607}
};
static lightsource lightsource2 = {
  { -0.574520, 0.059408, 0.816332,
    0.234097, 0.967626, 0.094335,
    -0.784299, 0.245298, -0.569827
  },
  { -2.016330, 2.295804, -1.532343}
};


// Lighthouse sources rotation matrix & 3d-position
// needs to be computed and read from EEPROM instead of constant.
static lightsource lightsources[2] = {lightsource2, lightsource1};

LighthouseSensor sensors[4];
LighthouseXYZ xyz[4];

void setup()
{
  sensors[0].begin(0, IR0, IR1);
  //sensors[1].begin(1, IR2, IR3);
  sensors[1].begin(2, IR4, IR5);
  //sensors[3].begin(3, IR6, IR7);

  for (int i = 0 ; i < 4 ; i++)
    xyz[i].begin(i, &lightsources[0], &lightsources[1]);

  Serial.begin(115200);

  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, LED_COUNT);

  for (int i = 0; i < 3; i++) {
    ewma[i].init(ewma[i].periods(2));
  }

}



float mappedAxisValue(int axis, int position) {

  int value = map (abs(position * 100), 0.0, 1000.0, 0, 255);
  return ewma[axis].record(value);
}



void paintLeds() {

  lastUpdate = 0;

  int x = ewma[0].value();
  int y = ewma[1].value();
  int z = ewma[2].value();

// show left-to-right rainbow color
  FastLED.showColor(CHSV(x, 255, 128));

  //FastLED.showColor(CHSV(y, 255, 128));
  //FastLED.showColor(CHSV(z, 255, 128));

  return;
}




void loop()
{
  //  rainbowCycle();

  for (int i = 0 ; i < 2 ; i++)
  {
    LighthouseSensor * const s = &sensors[i];
    LighthouseXYZ * const p = &xyz[i];

    int ind = s->poll();
    if (ind < 0)
      continue;
    if (!p->update(ind, s->angles[ind]))
      continue;

    Serial.print(i);
    Serial.print(",");
    Serial.print((int)(p->xyz[0] * 100));
    Serial.print(",");
    Serial.print((int)(p->xyz[1] * 100));
    Serial.print(",");
    Serial.print((int)(p->xyz[2] * 100));
    Serial.print(", ");


    // map position to pixel color in ewma and output to console
    for (int i = 0; i < 3; i++) {
      Serial.print(mappedAxisValue(i, p->xyz[i]));
      Serial.print(",");
    }

    Serial.println(p->dist);

    // output to leds
    paintLeds();

  }

  if (lastUpdate > interval) {
    ewma[0].record(ewma[0].value() * 0.2);
    ewma[1].record(ewma[1].value() * 0.2);
    ewma[2].record(ewma[1].value() * 0.2);
    //  paintLeds();
  }

}


