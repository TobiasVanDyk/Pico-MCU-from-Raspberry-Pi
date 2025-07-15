Compiled with Pico SDK 2.12 dev, Arduino Pico 4.6.0 and included Adafruit_TinyUSB_Arduino 3.4.4, and TFTeSPI 2.5.43
Pico 2 RP2350 and 3.5inch Touch Display Module for 150MHz Raspberry Pi Pico 2 included SDCard module https://www.waveshare.com/pico-restouch-lcd-3.5.htm
-------------------------------------------------------------------------------------------------------------------------------------------------
RP2350-E9: Adding absolute block to UF2 targeting 0x10ffff00
Using library Adafruit_TinyUSB_Arduino at version 3.4.4 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\4.6.0\libraries\Adafruit_TinyUSB_Arduino 
Using library SPI at version 1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\4.6.0\libraries\SPI 
Using library TFT_eSPI at version 2.5.43 in folder: C:\Users\Tobias\Documents\Arduino\libraries\TFT_eSPI 
Using library LittleFS at version 0.1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\4.6.0\libraries\LittleFS 
Using library SDFS at version 0.1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\4.6.0\libraries\SDFS 
Using library SdFat at version 2.3.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\4.6.0\libraries\SdFat 
Using library Time at version 1.6.1 in folder: C:\Users\Tobias\Documents\Arduino\libraries\Time 
"C:\\Users\\Tobias\\AppData\\Local\\Arduino15\\packages\\rp2040\\tools\\pqt-gcc\\4.1.0-1aec55e/bin/arm-none-eabi-size" -A "I:\\Data\\Win10\\Arduino/VolumeMacroPad308.ino.elf"
Sketch uses 241824 bytes (11%) of program storage space. Maximum is 2088960 bytes.
Global variables use 41216 bytes (7%) of dynamic memory, leaving 483072 bytes for local variables. Maximum is 524288 bytes.
C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\tools\pqt-python3\1.0.1-base-3a57aed-1/python3 -I C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\4.6.0/tools/uf2conv.py --serial COM3 --family RP2040 --deploy I:\Data\Win10\Arduino/VolumeMacroPad308.ino.uf2 
Resetting COM3
Converting to uf2, output size: 537600, start address: 0x2000
Scanning for RP2040 devices
Flashing D: (RP2350)
Wrote 537600 bytes to D:/NEW.UF2
----------------------------------------------------------------------------------------------------------------

To install new version of Arduino Pico first delete it from boards manager, then delete the folder 
C:\Users\Name\AppData\Local\Arduino15\packages\rp2040 then close and reopen Arduino IDE and then add the new Arduino Pico Board again.
If a different display is used the Arduino-Pico build code must be deleted before building the new TFT_eSPI build.

NB: Use 4MB Flash option with 2MB Sketch 2MB FS 
    Use USB Stack Adafruit TinyUSB

    Waveshare settings in https://files.waveshare.com/upload/f/fc/Pico-ResTouch-LCD-X_X_Code.zip are different:
    (1) #define ILI9488_DRIVER - Bodmer WARNING: Do not connect ILI9488 display SDO to MISO if other devices share the SPI bus 
        because TFT SDO does NOT tristate when CS is high https://github.com/Bodmer/TFT_eSPI/discussions/898
        Waveshare schematic https://files.waveshare.com/upload/8/85/Pico-ResTouch-LCD-3.5_Sch.pdf has LCD NOT connected to MISO 
        TFT connections is via ||->serial 74HC interface no MISO used there - Therefore Bodmer can be ignored
    (2) #define TFT_INVERSION_ON        // Waveshare has this commented out in their Setup60_RP2040_ILI9341.h

    Used here:  #define SPI_FREQUENCY       30000000   // Waveshare use 7MHz - 35MHz still seems ok
                #define SPI_READ_FREQUENCY  16000000   // 20MHz also ok
                #define SPI_TOUCH_FREQUENCY  500000    // 500kHz used - 250kHz or 500kHz needed for Pico 2

New changes:
1. *vx*000 to *vx*111 Volume enabled/disabled in Layouts 1,3,4 if enabled in Layout 2 
2. *dt*f,m,s or *dt*0,1,2 to adjust the delay times between macros/keys-pressed for slower or virtual machines, medium fast PCs and fast PCs.
3. Default is now long-press Volume Mute off i.e. now L1-L4 and not V1-V4 default.
4. Increased delay times in DoPowerKeys because in VM or slower PC first characters are missing in restart or shutdown string.
5. Switch Backlight Off/On via *Cmd *bl*0 = off *bl*1 = on - can use serial monitor and send <*bl*0> for blankscreen and
    <*bl*1> for full bright screen. For inbetween values use *bl*nn nn = 00 - 99

Previous changes:
0. Made changes to CapNumScrollLock handling
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




