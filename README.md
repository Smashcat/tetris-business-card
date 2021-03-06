# tetris-business-card
Business card that plays Tetris. Used as xmas toys for clients in 2021!

This repo includes the Eagle design files, production files to use with JLCPCB for full assembly, and firmware to play Tetris and show a couple of animations. All of the parts used here are available on JLC for PCBA, so it should be simple to have them produced and save some soldering! The only thing that might be hard to source at times of the ATMega328pb microcontroller, but it appears from time to time in their stock!

The card is designed for a CR2016 battery. On our cards, we solded a U shaped piece of wire from the pads on each side to hold it in place and keep the card thin enough to go into a wallet. An ideal piece of wire I found was U-shaped sewing pins!

The firmware will compule with Microchip Studio. The card uses pads for tag-connect pogo programmer. I use this cable with an ATMEL ICE programmer, so if you don't have this setup you might want to change this to make it easier to solder wires or something for flashing the firmware. Otherwise maybe flash the firmware on the MCU using an external socket before soldering it on the board.

Although this is kind of specialized for Tetris, I'd be interested in seeing any other games or animations people make!

![3D render](images/3d-render.png)
