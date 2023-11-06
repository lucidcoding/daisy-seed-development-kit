# MonoSeed-09 Virtual Analogue Monosynth

## Author

Paul Davies

## Description

My first experiment using the Daisy Seed Development Kit. This is a virtual analogue monosynth with one knob per function. It started off as an emulation of a Roland SH-09 but seems to have diverged from that somewhere along the way.

## Environment Setup

Ensure you have LIBDAISY_DIR and DAISYSP_DIR environment variables set and pointing to the relevant libraries. If you are using Windows, ensure they use the forward slash. If you are on Linux, ensure the capitalisation is correct. You'll make your life a lot easier if you don't have spaces in the path:

```
LIBDAISY_DIR=/path/to/libdaisy
DAISYSP_DIR=/path/to/daisysp
```

## Getting up and running

In order to build the hardware, load each of the projects in to Kicad, generate Gerber files and upload to a PCB manufacturer such as https://jlcpcb.com/. I can go into more detail about this at a later point.

The to get started, load the MonoSeed09 project on to the Daisy Seed by pressing both buttons to put it into program mode, entering the relevant folder, then enetering the commands to upload the code onto the Seed:

```
cd Examples/MonoSeed09
make clean; make; make program-dfu;
```

Ensure the Daisy Seed is correctly seated in the breakout board. Then connect the potentiometer array to port 1 with a JST XH2.54 8 wire cable, and connect the MIDI adapter with a JST XH2.54 4 wire cable. Finally connect your MIDI sequencer to the MIDI in DIN connector and plug your headphones into the audio out jack socket. Set the MIDI transmit channel to 1 - this is what it defaults to.

## Disclaimer

Although I am a professional programmer, I am purely a hobbyist when it comes to hardware or embedded programming. There may well be some mistakes here and I accept no responsibility for damage to you Daisy Seed, external equipment or anything else. You use this code and harware at your own risk.
