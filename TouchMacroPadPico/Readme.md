# Pico Touch Macropad

<p align="left">
<img src="images/pipc.png" height="260" /> 
<img src="images/lcdC2.png" height="260" /> 
<img src="macro.gif" height="260" /> 
</p>

The [**RPi Pico RP2040**](https://www.raspberrypi.org/products/raspberry-pi-pico/) is used as a **Touch LCD Macropad** with direct access to five file-types: (1) file-path-pointers, (2) macros, (3) textstrings, (4) linkfiles, and (5) \*codes. From Layout 2 (Configuration), 10,368 files or "nKeys", grouped as 36 sets of type (1) (aA01-xX96 and 001-996), and 1,440 files of types (2,3,4,5), are directly available (note 3). From Layouts 1, 3, and 4 (Macros M, S and T), nine sets of 24 keys of types 2, 3, 4, 5 (mM01-mM24, sS01-sS24, tT02-T24), as well as another six sets of 99 files (aA01-aA99 and kK01-kK99), are available (note 1). **"Macros" are defined here as a group of HID keys and buttons (or wheels or touch-surfaces), pressed (or moved) simultaneously or in sequence, in (almost) any combination, length, and speed (or direction).** Each of the Macro Layout keys and files, can be further linked through a plaintext list of 3-letter filenames with additional processing such as delay and repeat, storage switching, and chaining  or further linking (note 2). There are also another 456 (19 sets of 24 files), text files which can be accessed from the SDCard using the same macro-keys. Processing of very large text files, tested up to 64kB, but probably only limited by the SDFS or LittleFS or FAT32 filesystem constraints, are available through the nKeys<br> 

There are more than 50 different pages and layouts for actions such as Media-keys (Volume Mute Play-Stop Next-Previous Bass-Treble), inserting special characters such as the Greek alphabet, and various mathematics symbols, a Restart-Shutdown Timer and Time(Clock) Configuration, Timer-and-Time-executed repeat-and-oneshot macros, a Macro Composition, Manipulation, File, and Linking editor, a Mouse-button, Scroll-wheel and cursor movement Keypad, an enhanced 8 page Numberpad, and easy access for the display of PC sensor data (via HWInfo), Music Player information (via Foobar2000), PC Time or Date values, and many other manipulated data sources from a PC running GNU/Linux, and/or Windows.<br> 

Note 1: Macro definitions (A01-A99, K01-K99, S1-S24, T1-T24, M1-M24), and the 36 x 96 nKeys set, can be stored on the flash file-system as lowercase filenames (such as s01-s24 etc), or uppercase filenames (S01-S24), or on the SDCard as case-insensitive but case-preserving files.<br> 

Note 2: Linklist files can be chained to load another linklist file, through LxyXnn where Xnn is the name of the next XnnLink file to be loaded and executed according to its content, and Lxy indicates various SDCard/Flash options for where the next linklist is stored, and how any switch between Flash memory and SDCard storage, should be handled. A repeat = 20 rnn and Rnn with r = repeat 1-99 and R - repeat 100 to 9900, and three delays = 0.5 and 3 seconds, with dnn, ddn, Dnn, DDn with d 0.1-0.9, and 1-99 seconds, D 1-99 minutes, and DDn 1-9 hours, was used in the macrolink string *a50a51dd5a53d03r20a52d03U02* for the macro animation as shown in the right-hand picture above. Refer to the [**manual**](manual.md) and the Extras - DuckyPad section, for more details.<br>

<img src="images/nkeys3.jpg" width="40" height="30"/> Note 3: The 10k+ nKeys files are neither macros, textstrings, ,\*codes, nor linkfiles. For each alphanumeric first character (aA-xX and 0-9), there are 8 pages, each with 12 keys per page. They only point to another file through a a path - i.e. folder and file name, of any length less than 200 characters, on the same storage (Flash or SDCard) as the current set, to be executed as either a macro, textfile, linkfile, or \*code. There are (at least), 36 x 96 x 3 = 10,368 of these nKeys available for use from Layout 2. **Redirection** means that for example, if file n01 contains the text /passwords/BoA.txt then pressing nKey n01 will send the text from the file BoA.txt in folder /passwords to the current focus(s)ed PC application such as a browser or text editor. To change the nKeys first character n use a *code \*0n\*char - for example \*0n\*p will change the nKey set, and filenames used to p01 - p96. Use \*Code \*0s\*list-of-10-char to change a quick-access list. Press [Cfg][Opt] and Pad[o] to select the nKeys set or first character, from a list of 10 - the default is n o p q r s t m a k. Note that when the nKey first character is m,s,t,a,k then the nKeys list will interpret these as macros, textstrings, linkfiles or \*codes, and not as a filepath or filename.  From Layouts 1 ,2, and 3 there are 24 M, S, T keys, but 96 in Layout 2. Also note thta if the contents of for example, nKey n01 is the text "s11Link", it will attempt the execute the link list contained in the file named "s11Link".<br> 

The power inherent in these indirected set of files are in the *ease with which a large number of actions can be changed with a single folder rename*. For example: If there are three folders named AppA, AppB, and AppC, and each folder has 96 specialized macros or strings for the three different applications A, B, and C, then changing the folder name AppA, B, C to that contained in a nKey set such as CurrentApp, will redirect the nKey set of 96 keys to execute the specialized actions for each of the selected Apps A, B, or C.<br>

**Large text file processing:** The nKeys set can handle very large text strings - preferably stored on the SDCard - using nKeys such as those starting with the letters n,o,p,q,r - each with 96 keys over 8 pages. If the nKeys set is m,s, or t then large strings are also enabled for the M S T keys 01 - 96 in Layout 2. Keys MST 01-24 are best used for textfiles with less than 200 characters, and MST 25-96 for larger textfiles, or use any of the nKeys such as the NOPQR 01-96 sets, for large text files. Example: Copy two large (>20kB) files named L1 and S12 on the SDCard using a PC. Then program nKey n01 with the content L1 - Use the KeyBrd Editor - press Pad [k] and set n01 as the Source - in brown not white - and then do L[Add]1[Add][Sav]. Test by pressing the nKey [n01]. Change the nKey letter-set to S with [Cfg][Opt]PadKey[o] and then press the nKey [S12] - this was tested for both keys with notepad.<br>

Eight keys - normally the [Cut][Copy]Paste[Delete][Return][cX][cC][cV] keys - can be assigned from 54 functions in Layouts 1, 3 and 4 - the functions are: 1 Delete 2 Backspace 3 Tab  4 AltTab 5 Insert 6 Esc 7 PScr 8 AltPScr 9 Return 10 Snipping 11 OnScrKeyBrd 12 UnDo 13 ReDo 14 CtrAltDel 15 Cut 16 Copy 17 Paste 18 TaskMan 19 Run 20 GuiX 21 Copilot 22-27 Tskbar1-6 28-51 K1-K24 NumLock 52 CapsLock 53 ScrollLock 54. The 24 K keys use the plaintext linking method as explained above - refer to the Extras - DuckyPad section, or the manual, for examples.<br> 

Controlling the user interface appearance and functions are done using an extensive set of starcodes which can be run from files stored on Flash or the SDCard, or entered on the TouchLCD, or sent via a PC serial port, or a Powershell command to the MacroTouchpad. For example sending <\*x9\*0102030405060719> will set all 8 special keys - normally the Cut-Copy-Paste-Delete-Return keys, to new actions and labels - the 8 keys will then be Del Bks Tab aTb Ins Esc and [Del] = PrS and [Ret] = Run. Using a Powershell get-process, one can then set these keys according to which PC application is open. These \* commands can also be in a macrolink file.<br>

This means it is possible with a single macropad keypress to open for example photoshop on a PC, and at the same time re-configure the macropad keys for photoshop-specific functions. See example 8 in the [**manual**](Manual.md) for more detail.<br>

Six different LCD displays were tested with the MacroTouch firmware available here:
1. [**Waveshare IPS ST7789  LCD Pico Board 320x240 2.8 inch**](https://www.waveshare.com/pico-restouch-lcd-2.8.htm)
2. [**Waveshare TFT ILI9486 Raspberry Pi Type C 125MHz LCD 480x320 3.5 inch**](https://www.waveshare.com/3.5inch-rpi-lcd-c.htm)
3. [**Waveshare IPS ILI9486 Raspberry Pi Type B LCD 480x320 3.5 inch**](https://www.waveshare.com/product/3.5inch-RPi-LCD-B.htm)
4. [**Waveshare IPS ILI9488 Pico Evaluation Board Touch LCD 480x320 3.5 inch**](https://www.waveshare.com/pico-eval-board.htm)
5. [**Waveshare TFT ILI9486 Arduino LCD Shield 480x320 4 inch**](https://www.waveshare.com/4inch-tft-touch-shield.htm)
6. [**Waveshare IPS ILI9488 Pico Touch Display 480x320 3.5 inch**](https://www.waveshare.com/pico-restouch-lcd-3.5.htm)

LCDs 1, 4, 5, and 6 have built-in SDCard holders, and SDCard modules were added for LCDs 2 and 3. 

<img src="images/SDIO-SD.png" width="30" height="30"/> The [**Pico Evaluation Board**](https://www.waveshare.com/pico-eval-board.htm) was modified by adding 3 connecting wires to convert the SDCard SDIO interface to a standard 1-bit SD interface. Remove pins GP5 GP18 GP19 from the Pico MCU male header. Then use the female Pico SMD header connections on the display board to connect the three wires as indicated below - note that the Pico 1 and 2 SDIO interface is now (2025), supported by the filesystem used here:  
```
D3 CS GP22 stay as is
CLK GP5 disconnect from Pico and connect to GP10 SPI-1 CLK
CMD GP18 disconnect from Pico and connect to GP11 SPI-1 MOSI
D0 GP19 disconnect from Pico and connect to GP12 SPI-1 MISO
```
Not all SDCard brands work equally well - Sandisk, Toshiba, and Samsung SDCards have been used without any problems - if after a cold start the Touchpad does not respond, and only works after the HW reset button had been pressed, then replace the SDCard with another brand. As an alternative, enable the on-start-reset with \*r1\* - but doing this will prevent the Touchpad from being used during the Boot/BIOS phase.

<p align="left">
<img src="images/WavesharePicoBrd1.jpg" height="90" /> 
<img src="images/WaveshareTypeC.png" height="90" /> 
<img src="images/WaveshareTypeB.png" height="90" /> 
<img src="images/WavesharePicoBrd3.jpg" height="90" /> 
<img src="images/WaveshareArduinoLCD.png" height="90" /> 
<img src="images/WavesharePicoBrd2.jpg" height="90" /> 
</p>

Usability guidelines for touch macro pads are difficult to find, but these [**Material Design Layout Guidelines**](https://m2.material.io/design/layout/understanding-layout.html#layout-anatomy) are in part, applicable. Note that the older versions of these guidelines were much better suited for using as a usability design tool. 

<img src="images/handposition.png" width="40" height="30"/> The Touch LCD is used more conveniently when placed upright rather than lying flat, and then using your thumb-tip to press the 5 option pads, and the larger key pads. The single-touch characteristic of resistive touch overlays is in this case advantageous. Place one or two fingers on top or behind, the LCD to stabilise it when using your thumb to press the keys. This orientation is then similar to the current use of both thumbs to manipulate smartphones touch screens from the side, as opposed to using other fingers pushing on the touchscreen front. 

Of the 6 different LCD displays used here, the fast [**Waveshare 3.5 inch Type C 125MHz LCD display**](https://www.waveshare.com/3.5inch-rpi-lcd-c.htm) and the [**Waveshare 3.5inch Touch Display**](https://www.waveshare.com/pico-restouch-lcd-3.5.htm), are the preferred choices. The Type-C LCD is available [**here**](https://www.robotics.org.za/W15811). The difference in angled clarity between the TFT LCDs (the Arduino-type 4" and the 3.5" Type C LCDs), and the other four IPS type LCDs (three 3.5" and one 2.8"), are noticable, and therefore latter four LCDs may be preferred. The slower Waveshare 3.5" IPS Type B LCD was/is available [**here**](https://www.diyelectronics.co.za/store/displays/1632-5-inch-rpi-ips-lcd-320480.html).  

<img src="images/pico35inside1.jpg" width="40" height="30"/> The [**Waveshare 3.5inch Touch Display**](https://www.waveshare.com/pico-restouch-lcd-3.5.htm) for the Pico 1 is available [**here**](https://www.robotics.org.za/W19907). It and the other Waveshare Pico modules with a USB connection pointing up, can be modified by connecting a cable to the USB D+ and D- Pico bottom TP3 and TP2 pads, and then taking the USB cable out through the side of the enclosure, or adding a USB Type C breakout board such as [**this one**](https://www.pololu.com/product/2585).

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


