Compiled with Pico SDK 2.12 dev, Arduino Pico 4.5.4 and included Adafruit_TinyUSB_Arduino 3.4.4, and TFTeSPI 2.5.43
Pico 1 RP2040 and 3.5inch Touch Display Module for 200MHz Raspberry Pi Pico included SDCard module https://www.waveshare.com/pico-restouch-lcd-3.5.htm
-------------------------------------------------------------------------------------------------------------------------------------------------
Using library Adafruit_TinyUSB_Arduino at version 3.4.4 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\4.5.4\libraries\Adafruit_TinyUSB_Arduino 
Using library SPI at version 1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\4.5.4\libraries\SPI 
Using library TFT_eSPI at version 2.5.43 in folder: C:\Users\Tobias\Documents\Arduino\libraries\TFT_eSPI 
Using library LittleFS at version 0.1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\4.5.4\libraries\LittleFS 
Using library SDFS at version 0.1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\4.5.4\libraries\SDFS 
Using library SdFat at version 2.3.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\4.5.4\libraries\SdFat 
"C:\\Users\\Tobias\\AppData\\Local\\Arduino15\\packages\\rp2040\\tools\\pqt-gcc\\4.0.1-8ec9d6f/bin/arm-none-eabi-size" -A "I:\\Data\\Win10\\Arduino/VolumeMacroPad285.ino.elf"
Sketch uses 233956 bytes (22%) of program storage space. Maximum is 1044480 bytes.
Global variables use 38832 bytes (14%) of dynamic memory, leaving 223312 bytes for local variables. Maximum is 262144 bytes.
C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\tools\pqt-python3\1.0.1-base-3a57aed-1/python3 -I C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\4.5.4/tools/uf2conv.py --serial COM5 --family RP2040 --deploy I:\Data\Win10\Arduino/VolumeMacroPad285.ino.uf2 
Resetting COM5
Converting to uf2, output size: 513536, start address: 0x2000
Scanning for RP2040 devices
Flashing D: (RPI-RP2)
Wrote 513536 bytes to D:/NEW.UF2
----------------------------------------------------------------------------------------------------------------

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
                #define SPI_TOUCH_FREQUENCY  2500000   // 2.5MHz max 250kHz or 600kHz needed for Pico 2
----------------------------------------------------------------------------------------------------------------

New changes:
1. Made changes to CapNumScrollLock handling

Previous changes:
1. 5 Pads 50% bigger and increased space between Pads and Keys
2. Added Layout, Layer, and storage changes  via starcodes *ad*, *ae*, and *lx* - also via serial <*ad* > <*ae* > <*lx* >
See manual.h section (L)
3. Added File Copy via starcode *cf*source=destination, copy file or /folder/file named in source to file or /folder/file
named in destination. *cf*0 or *cf*00 -> Copy all six default label files from SDCard to Flash, *cf*1 or *cf*01 is Flash to 
SDCard. For control via serial terminal use <*cf*source=destination>.
4. Change logic in custom label setting - if a filename is added after *lm,s,t* then the result is the custom label enable is 
always ON
5. (Different) custom label files can be on both Flash and SDCard FS. When the [Cfg][Opt]Custom-Label is used it will toggle
the SDCard option on/off if A-D is brown, and the Flash FS option on/off if A-D is white. 
6. Add custom labels up to five characters for keys M1-M24, S1-S24, T1-T24. Keys M, S, T 1-24 in Layouts 1, 3, and 4, 
can have descriptive and easily changeable, 5-character-maximum-length, labels. All the custom label definition files are 
saved on the SDCard through the content in files LabelM, LabelS, and LabelT, which contain the path i.e. /folder/filename 
of the file that has the custom key labels. By default these files are named label1, label2, and label3. Refer to the manual 
section (K) for more details.
7. Fixed Config80 arrangement when using strcpy()




