# OscPocket VA Synth - Daisy Pod Port
<h1>Introduction</h1>
Alright, this is the OscPocketD VASynth, created by Staffan Melin, ported to work on the Daisy Pod. The goal of this project is to make a full-featured virtual-analog synth run on a fairly inexpensive platform (the Pod). This is mostly a personal project that I hope to share with others. I want to share everything because:

a) Open-source is great and I strongly believe that sharing everything you make will benefit the creator and the user
and, b) I don't really know what I'm doing and I hope someone else will come in and make this better

<h1>Install</h1>
If you just want to use this synth on you Daisy Pod, download the .bin file and flash it using the web programmer on electro-smith.com.

CURRENT MAJOR MISSING FUNCTION: I can't figure out how to save current settings/patches to be loaded again in the future. So that means when you power off your Pod, all settings will be lost. Thats a bummer I know, and I'm working on it/asking other people to help me out.

<h1>Controls</h1>

First, you will need a midi keyboard or some sort of midi source to make any sort of sound out of this. When you start your Pod up, you will see one of the two LEDs on you Pod light up red. The other will stay unlit. These two LEDs represent the menu to change the perameters of your synth. When you push the first button on your Pod, the first LED will cycle through colors, which will represent different categories, and when you push the second button, the second LED will also cycle through various colors, which represent specific sets of perameters that can be adjusted with the knobs. Each time the colors of the LED change, the function of the two knobs also change. It's not the best system, but again, the whole point of this is to have a compact, cheap, and high quality digital synth.

<h2>Encoder</h2>
Push Encoder - Cycle through 16 save slots (there is currently no feedback or indicators for which slot you are on, but I'm having a little trouble figuring it out for now, but I'll try to get something implimented in the near future)
Turn Encoder to the left - Save
Turn Encoder to the right - Load
<h2>Category 1: Red: Oscillators</h2>

RED/UNLIT
Knob1: Oscillator 1 waveshape
Knob2: Oscillator 1 level

RED/RED
Knob1: Oscillator 2 waveshape
Knob2: Oscillator 2 detune // this is not working exactly right. All the way counter-clockwise will detune second oscillator out of existance. But if you turn it past noon toward the right, you can get some more usable range.

RED/GREEN

Knob1: Number of voices (1, 5, or 8) // I only have three options here at the moment because previously if you turned the knob too fast, the Daisy Seed would hang up
Knob2: Amount of portamento // This is a little wonky at the moment and can be a little sensitive, but you can still dial in some nice portamento effect. Best used with just one voice

RED/BLUE
Knob1: Transpose Osc2 (Down 1 octave, no transpose, up 1 octave) 
Knob2: level of Osc2

RED/AQUA
Knob1: Pitch Envelope Attack
Knob2: Pitch Envelope Release

RED/PURPLE
Knob1: Pitch Envelope Decay
Knob2: Pitch Envelope Sustain

<h2>Category 2: Green: Amp Envelope</h2>

GREEN/UNLIT
Knob1: Attack
Knob2: Release

GREEN/RED
Knob1: Decay
Knob2: Sustain

GREEN/GREEN
Knob1: Noise
Knob2: Pan

<h2>Category 3: Blue: LFO</h2>

BLUE/UNLIT
Knob1: LFO Amount/Intensity
Knob2: LFO Frequency

BLUE/RED
Knob1: LFO Shape
Knob2: LFO Target: None, Pitch, Filter, Amp EG // to-do, I've been working on adding an option to target the Pan, but I haven't finished it yet

<h2>Category 4: Aqua: Filter</h2>

AQUA/UNLIT
Knob1: Filter Frequency
Knob2: Resonance

AQUA/RED
Knob1: Filter Envelope Attack
Knob2: Filter Envelope Release

AQUA/GREEN
Knob1: Filter Envelope Decay
Knob2: Filter Envelope Sustain //to-do, I want an option so you can set the filter envelope to match the amp evenlope

AQUA/BLUE
Knob1: Filter Type: in order CCW to CW: Low pass, high pass, band pass, notch, and peak
Knob2: Filter amount/Intensity // I may need to tweak this a bit, but I think it is giving the desired result of attenuating the filter envelope


<h2>Category 5: Purple: FX</h2>

PURPLE/UNLIT
Knob1: Delay Volume
Knob2: Reverb Volume // The volume of the reverb can only go as "loud" as the current wet setting. These two controls are meant to be quick access to tweaking the amount of FX after you have locked in the other settings. 

PURPLE/RED
Knob1: Delay time
Knob2: Delay Feedback

PURPLE/GREEN
Knob1: Reverb Filter
Knob2: Reverb Feedback

PURPLE/BLUE
Knob1: Dry signal amount
Knob2: Wet signal amount

<h1>To-Do List and Known Issues</h1>

<h2>Features I Want to Add</h2>
<li>Save and load, either to onboard memory or to SD Card or both
<li>Additional effects such as wave-folder, chorus, distortion, etc 
<li>Add filter to delay line
<li>More/different reverb
<li>LFO target for panning and for PWM
<li>I've added/tweaked a few things so I need to add them to the preset template. Running list here: delay level, reverb level, filter envelope amount 

<h2>Issues I Want to Fix</h2>
<li>Knob perimeter takeover needs to fine tuned. I implemented a brute-force solution by creating if-statements for every set of parameters to check if the knob position has changed. It works well enough but surely there is a better way to do it. 
<li>The settings for the various ADARs are off. It is very difficult to dial in a shorter attack and releases. The setting seems to jump for VERY short to noticeably long too quickly. I've tried a few things, but I just need to give it a little more time to get it right 
<li>Detune is functional but it is not working correctly at the moment. Originally, detune was the value of the knob, added to the frequency of the note. The problem with that is that the detune is more noticeable at lower frequencies. So the detune would need to exponentially scale along with the note frequency. For now,
I set it up so that the frequency is multiplied by the negative of the knob value. This allows the difference to remain more consistent at different frequencies, but as you lower the knob value, it goes all the way to zero, which is an extreme amount of detune. I'm bad at math so I just need to figure out the right way to set this. 
<li>To select the number of voices, I originally implemented an option to turn a knob and depending on what percentage of the knob was turned, anywhere between 1-8 voices could be set. The problem was that if you quickly turned the knob or if you were playing while turning the knob, the Daisy Pod would just freeze. Too much thinking I suppose. So I changed it to select either 1, 5 (the safe amount), or 8 (the max amount). I feel like this can be made more intuitive or done in a way that doesn't cause problems. 
<li>I need to figure out various forms of voice priority/stealing. I don't know the correct terminology for this, but I want to be able to hold a low note or chord and play a melody without the lower notes getting cut off, which is what is happening now
<li>Fix the delay so that changing delay time doesn't cause nasty digital glitching
