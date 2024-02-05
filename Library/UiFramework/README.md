# UI Framework

## Author

Paul Davies

## Description

This is a framework for building menus and settings pages for various types of display, with the current implementation for an SSD1306 OLED display. 


## Getting up and running

This is part of the Development Kit I have been building for the Daisy Seed, so you will need to pull the entire repository two levels up from this and follow the README from there.

Once you have done that, enter the /Tests/UnitTests folder and run:

```
cls & make * ./Main.text.exe
```

to run the unit tests. In order to see it in action, connect an encoder to the encoder port of the breakout board, and an SSD1306 to the I2C port. Then enter the /Tests/ManualTests folder and run:

```
make clean; make; make program-dfu;
```

I'd love to go into more detail here but life's getting in the way I'm afraid. Maybe at some point I'll do an in depth tutorial.

## Demo

Here it is in action:

[![MonoSeed-09 demo](https://img.youtube.com/vi/dcl3H62zlCA/0.jpg)](https://youtu.be/dcl3H62zlCA)

## Issues

As can be seen in the video, updating the display can interfere with the audio. This is annoying but not a deal breaker, as I don't see this being used for parameters that will be changed while performing. A potentiometer array can be used for that, and this can be for the parameters that are left over.

## Disclaimer

Usual thing: this is hobbyist stuff. Use at your own risk, don't blame me if it blows up your Daisy Seed/computer/studio/house.
