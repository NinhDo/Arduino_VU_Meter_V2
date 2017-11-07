# Arduino_VU_Meter_V2
A VU meter using 6 bandpass filter circuits

---
The VU meter uses 2 24-LED rings and 1 12-LED ring with addressable LEDS, for a total of 60 LEDs. The LEDs are separated into 6 sections, each section filtered to one specific frequency range:

0. ~20Hz - 220Hz
1. 220Hz - 440Hz
2. 440Hz - 880Hz
3. 880Hz - 1760Hz
4. 1760Hz - 3620Hz
5. 3620Hz - ++
 
The analog reference is set to INTERNAL, so we have a reference point of 1.1V instead of 5V, which is pretty much as close as we can get.

Each filter is fed into an analog pin and then read multiple times to amplify the effect before being displayed.
The amplification is necessary because the audio signal is only a couple of milliVolts, and even with analogReference(INTERNAL), it's still really weak.
