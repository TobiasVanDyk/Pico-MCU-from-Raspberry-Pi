# Pico Touch Macropad

<p align="left">
<img src="images/pipc.png" height="260" /> 
<img src="images/lcdC2.png" height="260" /> 
<img src="macro.gif" height="260" /> 
</p>

The [**RPi Pico**](https://www.raspberrypi.org/products/raspberry-pi-pico/) is used as a **Touch LCD Macropad** with direct access to 72 user-programmable macros, selectable from three banks of 24 macros, as well as another 99 programmable macro-files, each of which can be linked to almost infinity, through a plaintext list of the 3-letter macro-filenames. In addition 360 text files can be accessed directly from an SDCard using the same macro-keys. There are more than 50 different pages and layouts for actions such as Media-keys (Volume Mute Play-Stop Next-Previous Bass-Treble), inserting special characters and mathematics symbols, a Restart-Shutdown Timer and Time(Clock) Configuration, Timer-and-Time-executed repeat-and-oneshot macros, a Macro Composition, Manipulation, File, and Linking editor, a Mouse-button, Scroll-wheel and cursor movement Keypad, an enhanced 3 page Numberpad, and easy access for showing PC sensor data, Music Player information, or manipulating many other specialized data sources from a PC running GNU/Linux, and/or Windows. 

A repeat rnn and Rnn with r = repeat 1-99 and R - repeat 100 to 9900, and a delay dnn and Dnn with d 1-99 seconds and D 1-99 minutes, was used in the macrolink string **a50a51dd5a53d03r20a52d03U02** for the animation as shown in the right-hand picture above. Refer to the [**manual**](manual.md) and the Extras - DuckyPad section, for more details.

Five keys - normally the [Cut][Copy]Paste[Delete][Return[ keys - can be assigned from 52 functions in Layouts 1, 3 and 4 - the functions are: 1 Delete 2 Backspace 3 Tab  4 AltTab 5 Insert 6 Esc 7 PScr 8 AltPScr 9 Return 10 Snipping 11 OnScrKeyBrd 12 NumLock 13 CapsLock 14 ScrollLock 15 Cut 16 Copy 17 Paste 18 TaskMan 19 Run 20 GuiX 21 Copilot 22-24 Tskbar1-6 and K1-K24. The latter use the new plaintext linking method - again refer to the Extras - DuckyPad section for an example.

Five different LCD displays were tested with the MacroTouch firmware available here:
1. [**Waveshare IPS ST7789  LCD Pico Board 320x240 2.8 inch**](https://www.waveshare.com/pico-restouch-lcd-2.8.htm)
2. [**Waveshare TFT ILI9486 Raspberry Pi Type C 125MHz LCD 480x320 3.5 inch**](https://www.waveshare.com/3.5inch-rpi-lcd-c.htm)
3. [**Waveshare IPS ILI9486 Raspberry Pi Type B LCD 480x320 3.5 inch**](https://www.waveshare.com/product/3.5inch-RPi-LCD-B.htm)
4. [**Waveshare IPS ILI9488 Pico Evaluation Board Touch LCD 480x320 3.5 inch**](https://www.waveshare.com/pico-eval-board.htm)
5. [**Waveshare TFT ILI9486 Arduino LCD Shield 480x320 4 inch**](https://www.waveshare.com/4inch-tft-touch-shield.htm)

LCDs 1, 4 and 5 come with SDCard holders, and SDCard modules were added for LCDs 2 and 3.

<p align="left">
<img src="images/WavesharePicoBrd1.jpg" height="90" /> 
<img src="images/WaveshareTypeC.png" height="90" /> 
<img src="images/WaveshareTypeB.png" height="90" /> 
<img src="images/WavesharePicoBrd2.jpg" height="90" /> 
<img src="images/WaveshareArduinoLCD.png" height="90" /> 
</p>

The SDCard slot can be used for saving and reading files both text and Macro definition files. When textfiles are used the nine sets of 24 keys each send the SDCard files as keyboard characters to the PC - the size of the files/strings are only limited by the SDCard size. Change between five SDCard file-sets of 24 x 3 files each, with \*sd\*n where n=1 to 9. Refer to the [**manual**](Manual.md) for further instructions.

Usability guidelines for touch macro pads are difficult to find, but [**these Material Design Layout Guidelines**](https://m2.material.io/design/layout/understanding-layout.html#layout-anatomy) are in part, applicable. 

<img src="images/handposition.png" width="40" height="30"/> The Touch LCD is used more conveniently when placed upright rather than lying flat, and then using your thumb-tip to press the 5 option pads, and the larger key pads. The single-touch characteristic of resistive touch overlays is in this case advantageous. Place one or two fingers on top or behind, the LCD to stabilise it when using your thumb to press the keys. This orientation is then similar to the current use of both thumbs to manipulate smartphones touch screens from the side, as opposed to using other fingers pushing on the touchscreen front. 

Of the 5 different LCD displays used here, I prefer the fast [**Waveshare 3.5 inch Type C 125MHz LCD display**](https://www.waveshare.com/3.5inch-rpi-lcd-c.htm) (even though it is not an IPS screen). It is available [**here**](https://www.pishop.co.za/store/ws-35inch-rpi-lcd-c-480x320-125mhz-high-speed-spi?keyword=display%203.5%20inch&category_id=0). But note that the difference in clarity between the TFT LCD (the Arduino-type 4" and the 3.5" Type C LCDs), and the other three IPS type LCDs (3.5" and 2.8"), are obvious, and the latter three may be the preferred choice even though the larger 4" LCD panel is easier to use. The **Waveshare 3.5" IPS Type B LCD** is available [**here**](https://www.diyelectronics.co.za/store/displays/1632-5-inch-rpi-ips-lcd-320480.html).

Below are pictures taken whilst adding the SDCard modules to the Raspberry Pi Type C and type B LCDs with wiring diagrams:

<p align="left">
<img src="images/TypeC1.png" height="80" /> 
<img src="images/TypeC2.png" height="80" /> 
<img src="images/TypeC3.png" height="80" /> 
<img src="images/TypeC4.png" height="80" /> 
</p>

<p align="left">
<img src="images/TypeCWiring.png" height="100" /> 
<img src="images/TypeBWiring.png" height="100" /> 
<img src="images/SDCardModule1.png" height="100" /> 
</p>

Below are older pictures taken of four of the LCDs namely 1, 3, 4, and 5

<p align="left">
<img src="images/oled-volt.png" height="120" />  
<img src="images/s2.jpg" height="120" /> 
<img src="images/s3.jpg" height="120" /> 
</p>
 
<p align="left">
<img src="images/mth1.jpg" height="86" /> 
<img src="images/num0.jpg" height="86" /> 
<img src="images/num2.jpg" height="86" /> 
<img src="images/tim1.jpg" height="86" /> 
</p>

<p align="left">
<img src="images/cfg1.jpg" height="87" /> 
<img src="images/kbd1.jpg" height="87" /> 
<img src="images/mac3.jpg" height="87" /> 
<img src="images/med1.jpg" height="87" /> 
</p>


