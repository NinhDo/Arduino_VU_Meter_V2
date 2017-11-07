# Arduino_VU_Meter_V2
A VU meter using 6 bandpass filter circuits

---
The VU meter uses 2 24-LED rings and 1 12-LED ring with addressable LEDS, for a total of 60 LEDs. The LEDs are separated into 6 sections, each section filtered to one specific frequency range:
 ~20Hz - 220Hz
 220Hz - 440Hz
 440Hz - 880Hz
 880Hz - 1760Hz
 1760Hz - 3620Hz
 3620Hz - ++
 
Each filter is fed into an analog pin, read multiple times to amplify the effect and then displayed.
The amplification is necessary because the audio signal is only a couple of milliVolts.
