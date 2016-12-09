/** \file
 * XYZ position using the Vive Lighthouse beacons.
 *
 * This uses up to four Triad Semiconductor TS3633-CM1 sensor modules,
 * which have a built-in 850nm photodiode and an amplifier circuit.
 *
 * The pulses are captured with the Teensy 3's "flexible timer"
 * that uses the 48 MHz system clock to record transitions on the
 * input lines.
 *
 * The sync pulses are at 120 Hz, or roughly 8000 usec.  This means that
 * any pulse *longer* than 8 usec can be discarded since it is not
 * a valid measurement.
 *
 * The Teensy FTM might be able to capture both rising and falling edges,
 * but we can fake it by using all eight input channels for four
 * sensors.
 *
 * If we can't see the lighthouse that this
 * time slot goes with, we'll see the next sync pulse at 8 usec later.
 * If we do see this lighthouse, we'll see a sweep pulse at time T,
 * then roughly 8 usec - T later the next sync.
 *
 * Meaning of the sync pulses lengths:
 * https://github.com/nairol/LighthouseRedox/blob/master/docs/Light%20Emissions.md
 */

#include "LighthouseSensor.h"
#include "LighthouseXYZ.h"


#define IR0 5
#define IR1 6
#define IR2 9
#define IR3 10
#define IR4 20
#define IR5 21
#define IR6 22
#define IR7 23

// my lightsources from https://github.com/mpinner/openvr/tree/master/samples/hellovr_opengl
static lightsource lightsource1 = {
    { 0.779680, 0.024346, -0.625704,
      -0.178665, 0.966355, -0.185031,
      0.600148, 0.256057, 0.757798},
    { 1.560562, 2.315096, 1.997607}
};
static lightsource lightsource2 = {
    { -0.574520, 0.059408, 0.816332,
      0.234097, 0.967626, 0.094335,
      -0.784299, 0.245298, -0.569827},
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

	for(int i = 0 ; i < 4 ; i++)
		xyz[i].begin(i, &lightsources[0], &lightsources[1]);

	Serial.begin(115200);
}


void loop()
{
	for(int i = 0 ; i < 2 ; i++)
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
	/*	Serial.print(s->raw[0]);
		Serial.print(",");
		Serial.print(s->raw[1]);
		Serial.print(",");
		Serial.print(s->raw[2]);
		Serial.print(",");
		Serial.print(s->raw[3]);
		Serial.print(",");
	*/
	  Serial.print((int)(p->xyz[0]*100));
		Serial.print(",");
		Serial.print((int)(p->xyz[1]*100));
		Serial.print(",");
		Serial.print((int)(p->xyz[2]*100));
		Serial.print(",");
		Serial.println(p->dist);
	}
}
