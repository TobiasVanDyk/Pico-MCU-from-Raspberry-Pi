Compiled with Pico SDK 2.12 dev, Arduino Pico 4.5.3 and included Adafruit_TinyUSB_Arduino 3.4.4, and TFTeSPI 2.5.43
Pico 1 RP2040 and 3.5inch Touch Display Module for 200MHz Raspberry Pi Pico included SDCard module https://www.waveshare.com/pico-restouch-lcd-3.5.htm
-------------------------------------------------------------------------------------------------------------------------------------------------
Using library Adafruit_TinyUSB_Arduino at version 3.4.4 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\4.5.4\libraries\Adafruit_TinyUSB_Arduino 
Using library SPI at version 1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\4.5.4\libraries\SPI 
Using library TFT_eSPI at version 2.5.43 in folder: C:\Users\Tobias\Documents\Arduino\libraries\TFT_eSPI 
Using library LittleFS at version 0.1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\4.5.4\libraries\LittleFS 
Using library SDFS at version 0.1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\4.5.4\libraries\SDFS 
Using library SdFat at version 2.3.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\4.5.4\libraries\SdFat 
"C:\\Users\\Tobias\\AppData\\Local\\Arduino15\\packages\\rp2040\\tools\\pqt-gcc\\4.0.1-8ec9d6f/bin/arm-none-eabi-size" -A "I:\\Data\\Win10\\Arduino/VolumeMacroPad272.ino.elf"
Sketch uses 234164 bytes (22%) of program storage space. Maximum is 1044480 bytes.
Global variables use 38832 bytes (14%) of dynamic memory, leaving 223312 bytes for local variables. Maximum is 262144 bytes.
C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\tools\pqt-python3\1.0.1-base-3a57aed-1/python3 -I C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\4.5.4/tools/uf2conv.py --serial COM5 --family RP2040 --deploy I:\Data\Win10\Arduino/VolumeMacroPad272.ino.uf2 
Resetting COM5
Converting to uf2, output size: 514048, start address: 0x2000
Scanning for RP2040 devices
Flashing D: (RPI-RP2)
Wrote 514048 bytes to D:/NEW.UF2
-------------------------------------------------------------------------------------------------------------------------------------------------

To install new version of Arduino Pico first delete it from boards manager, then delete the folder 
C:\Users\Name\AppData\Local\Arduino15\packages\rp2040 then close and reopen Arduino IDE and then add the new Arduino Pico Board again.
If a different display is used the Arduino-Pico build code must be deleted before building the new TFT_eSPI build.

NB: Use 2MB Flash option with 1MB Sketch 1 MB FS 
    Use USB Stack Adafruit TinyUSB
    Use 200MHz CPU 

    Waveshare settings in https://files.waveshare.com/upload/f/fc/Pico-ResTouch-LCD-X_X_Code.zip are different:
    (1) #define ILI9488_DRIVER - Bodmer WARNING: Do not connect ILI9488 display SDO to MISO if other devices share the SPI bus 
        because TFT SDO does NOT tristate when CS is high https://github.com/Bodmer/TFT_eSPI/discussions/898
        Waveshare schematic https://files.waveshare.com/upload/8/85/Pico-ResTouch-LCD-3.5_Sch.pdf has LCD NOT connected to MISO 
        TFT connections is via ||->serial 74HC interface no MISO used there - Therefore Bodmer can be ignored
    (2) #define TFT_INVERSION_ON        // Waveshare has this commented out in their Setup60_RP2040_ILI9341.h

    Used here:  #define SPI_FREQUENCY       30000000   // Waveshare use 7MHz - 35MHz still seems ok
                #define SPI_READ_FREQUENCY  16000000   // 20MHz also ok
                #define SPI_TOUCH_FREQUENCY  2500000   // 1MHz max 250kHz or 600kHz needed for Pico 2

New changes:
0. Made changes to CapNumScrollLock handling
1. 5 Pads 50% bigger and increased space between Pads and Keys
2. Added Layout, Layer, and storage changes  via starcodes *ad*, *ae*, and *lx* - also via serial <*ad* > <*ae* > <*lx* >
See manual.h section (L)
2. Added File Copy via starcode *cf*source=destination, copy file or /folder/file named in source to file or /folder/file
named in destination. *cf*0 or *cf*00 -> Copy all six default label files from SDCard to Flash, *cf*1 or *cf*01 is Flash to 
SDCard. For control via serial terminal use <*cf*source=destination>.

Previous changes:
1. Change logic in custom label setting - if a filename is added after *lm,s,t* then the result is the custom label enable is 
always ON
2. (Different) custom label files can be on both Flash and SDCard FS. When the [Cfg][Opt]Custom-Label is used it will toggle
the SDCard option on/off if A-D is brown, and the Flash FS option on/off if A-D is white. 
3. Add custom labels up to five characters for keys M1-M24, S1-S24, T1-T24. Keys M, S, T 1-24 in Layouts 1, 3, and 4, 
can have descriptive and easily changeable, 5-character-maximum-length, labels. All the custom label definition files are 
saved on the SDCard through the content in files LabelM, LabelS, and LabelT, which contain the path i.e. /folder/filename 
of the file that has the custom key labels. By default these files are named label1, label2, and label3. Refer to the manual 
section (K) for more details.
4. Fixed Config80 arrangement when using strcpy()
Changes A:
1. Added LED indicators for Time Clock set and Alarm/Timer set
2. Fixed Serial data handling
3. Fixed *ua* and *ul* options. These were remnants from earlier mechanisms for linking macros. They now are the same as
   deleting specific files that end with "Link". See the manual.h section (c) for examples.
4. Added nKeys to Linkstring entries - 10 nKeys char from a list enabled in linkstrings. Default is
   'n','o','p','q','u','v','w','x','y','z'. Alternative list could be '0','1','2','3','4','5','6','7','8','9'.
   Change list with *0t*10-char-list. Can only use nKeys 00-99 in stringlist currently. For example a32r03n54a43,
   will do macro a32, then repeat nKey n54 3 times, then do macro a32 again. Cannot use dD rR lL as nKeys in stringlist
   as they are reserved for delay, repeat, link. Considering alternative choices of w (wait) for d (delay), and x (times)
   for r (repeat), as a configurable option.
   NB: On first load of this firmware the nKeys stringlist list will be empty use *0t*nopquvwxyz [EXE] to fill it.
   If the list is empty nKeys such as no2 wil print its contents as text instead of executing the contents.
Changes B:
1. Fixed nDir name save and restore in Config1
2. Added Config1 to showKeyData() - use *ld*
3. Changed *0d* handling for no char added
Changes C:
1. Corrected wrong code for *0d*
2. Extended nKeys to four digits which means there are 833 pages, each 12 keys or 9,996 nKeys for each of the 36 first 
characters which can be saved on Flash as Upper/Lowercase or SDCard as Upper=Lowercase, i.e. 3 x 36 x 12 x 9996 or
1,079,568 different nKeys. nKeys n01 to n99 are shown with two digits, n100 to n999 with three digits, and n1000 to
n9996 with four digits.

It is not certain if this version will be further developed - it seems likely that the previous version with 100,00+
nKeys are sufficient for most uses.

manual.h line 160
========================================================================================================================
The nKeys minimum to maximum number of pages is 1 to 833, each page with 12 keys per page. Use *0p*nnn nnn=01-833 to 
change the maximum pages. Use *0x* to toggle nKeys as 3-char numbers n01-n99 followed by 4-char numbers n100-n996,
followed by 5-char number n1000-n9996 (default). Or use 5-char number throughout namely n0001-n9996. Note that the 
3+4+5-char mode is the more compatible as it uses filenames n01-n99 (assuming n is used as the first char), whilst 
the 5-char mode will use a different fileset n0001-n0099 for the first 99 keys.
-----------------------------------------------------------------------------------------------------------------------
[n01] [n02] [n03] [n04]       [n13] [n14] [n15] [n16]               [n9985] [n9986] [n9987] [n9988]  [+]Page or nChar
[n05] [n06] [n07] [n08]       [n17] [n18] [n19] [n20]   --------->  [n9989] [n9990] [n9991] [n9992]  [k]
[n09] [n10] [n11] [n12]       [n21] [n22] [n23] [n24]   more pages  [n9993] [n9994] [n9995] [n9996]  [e]     or [s]
    Caps  Num  Scroll              C     N     S                           C      N      S           [n] aA-xX,0-9
                                                                                                     [-]Page or nChar
1. When middle pad is Blue [e] then use Red Pads [+][-] for PageUp or PageDown  
   When middle pad is Red  [s] then use Red Pads [+][-] for nChar change from a-z, 0-9, A-Z. The nKeys Char change is 
   shown on the fourth gray Pad [nChar]. This change is only shown on the 12 nkeys when the Pad Red [s] is changed to 
   Blue [e], and the PageUp/Dwn[+][-] Pads are pressed. To save the new nChar press [Cfg][Sav] from Layout 2.                      
2. Pad [e] when in nkeys mode toggle either nKeys execute (blue Pad [e]), or when pressed show content of nKey (red 
   Pad [s]).
3. nChar is also changable to 0-9, aA - xX with *0n*char, or from Layout 2 with [Cfg][Opt]Pad[o] from list of 10 chars.
========================================================================================================================

