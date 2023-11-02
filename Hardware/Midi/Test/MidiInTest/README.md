# MidiInTest

This tests and demonstrates the MIDI receive functionality.

## Hardware Setup

Connect the MIDI port of the Breakout Board to the 3.3V MIDI Adapter, connect a sequencer transmitting MIDI messages to the MIDI IN socket of the MIDI adapter to the MIDI OUT socket of the sequencer. Ensure the sequencer is transmitting on channel 1. For now, it only works with monophonic sequences.

## Build & Run

In the terminal, navigate to this folder and type: `make clean; make; make program-dfu;`; As soon as the code is loaded onto the Daisy Seed and the Seed has booted, you should here the notes being played.
