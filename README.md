# mp3-nugget

A custom MP3 player, nicknamed **The Nugget** for its appearance (thanks, DankPods).

---

## Parts

- **Arduino Nano**
- **I2C OLED display**  
  - Mine is 128x32, but any size should work. You may need to modify some code for certain sizes.
- **YX5200-24SS based MP3 module**  
  - A DFPlayer Mini will work if you want a smaller Nugget, but I'm using the Keyestudio KS0387, which is larger and has some buttons built in. It also includes a 3.5mm headphone jack, and in my experience, has better sound quality.
- **1kΩ resistor**
- **3.7V to 5V converter**  
  - I'm using one meant for making USB battery packs, with the USB port removed. There are definitely smaller options, but I had this on hand.
- **Li-ion/LiPo battery**  
  - I'm using an 18650, though a 1S LiPo or any other 3.7v source will work.
  - for longer battery life you can add more, but make sure your converter board can handle it.
- **Wire**
- **Solderable breadboard (perfboard)**

**Optional:**

- Speaker  
- SPST switch to toggle speaker

---

## Tools

- **Soldering iron and solder**  
  - the Pinecil from Pine64 is great for things like this. (#notsponsored)
- **Wire cutters/pliers**  
  - Especially helpful to remove the USB header if your 3.7–5V converter has one.

---

## Building Instructions

1. Assemble components using the following schematic:  
   *(insert schematic)*

2. Design and layout is up to you. Here's what I ended up doing:

   - **Perfboard Side 1:**
     - Battery  
     - Converter  
     - Power switch  
     - Speaker  
     - Speaker switch  

   - **Perfboard Side 2:**
     - MP3 module  
     - OLED screen on top of MP3 module (add tape or something to insulate)  
     - Arduino  

3. **Connections:**

   - 5V converter Vout → Arduino Vin  
   - 5V converter GND → Arduino GND  
   - Arduino GND → OLED / MP3 module GND  
   - Arduino 5V → OLED / MP3 module Vin/Vcc  
   - Arduino D2 → 1k ohm resistor → MP3 module RX  
   - Arduino D3 → MP3 module TX  
   - Arduino A4 → OLED SDA  
   - Arduino A5 → OLED SCL  
   - MP3 module SPK+ / SPK– → Speaker + / – (SPST switch on SPK+)

4. Upload `nugget/nugget.ino` to the Arduino.

---

## Configuration stuff
1. The icon displayed on startup is a bitmap defined by a 2d array inside of `nugget.ino`
    -  A utility to easily create your own bitmap icon for monochrome OLED displays is available as a React web app under `icon-bitmap-maker`
    - You can use the available build on my website at [pages.beanfrog.xyz/icon-bitmap](https://pages.beanfrog.xyz/icon-bitmap)
    - Or you can run it yourself, especially if you want to modify it:
        - `cd icon-bitmap-maker`, `npm install`, `npm run start`
    - To use:
        - enter a size at the top from 8px to 64px
        - click a cell to turn it on or off
        - click the export button to export it as an array that you can paste into `nugget.ino` (or your own project!)
        - click the import button to paste a previously exported bitmap and edit it
            - you have to do this twice for it to work, IDK why and I'm too lazy to fix it right now... maybe later.
            - also you can't resize imported bitmaps. I might add this feature later too.

