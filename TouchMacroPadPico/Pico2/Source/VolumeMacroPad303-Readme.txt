Compiled with Pico SDK 2.12 dev, Arduino Pico 4.5.3 and included Adafruit_TinyUSB_Arduino 3.4.4, and TFTeSPI 2.5.43
Pico 2 RP2350 and 3.5inch Touch Display Module for 200MHz Raspberry Pi Pico included SDCard module https://www.waveshare.com/pico-restouch-lcd-3.5.htm
-------------------------------------------------------------------------------------------------------------------------------------------------
RP2350-E9: Adding absolute block to UF2 targeting 0x10ffff00
Using library Adafruit_TinyUSB_Arduino at version 3.4.4 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\4.5.3\libraries\Adafruit_TinyUSB_Arduino 
Using library SPI at version 1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\4.5.3\libraries\SPI 
Using library TFT_eSPI at version 2.5.43 in folder: C:\Users\Tobias\Documents\Arduino\libraries\TFT_eSPI 
Using library LittleFS at version 0.1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\4.5.3\libraries\LittleFS 
Using library SDFS at version 0.1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\4.5.3\libraries\SDFS 
Using library SdFat at version 2.3.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\4.5.3\libraries\SdFat 
Using library Time at version 1.6.1 in folder: C:\Users\Tobias\Documents\Arduino\libraries\Time 
"C:\\Users\\Tobias\\AppData\\Local\\Arduino15\\packages\\rp2040\\tools\\pqt-gcc\\4.0.1-8ec9d6f/bin/arm-none-eabi-size" -A "I:\\Data\\Win10\\Arduino/VolumeMacroPad303.ino.elf"
Sketch uses 237420 bytes (11%) of program storage space. Maximum is 2088960 bytes.
Global variables use 41568 bytes (7%) of dynamic memory, leaving 482720 bytes for local variables. Maximum is 524288 bytes.
C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\tools\pqt-python3\1.0.1-base-3a57aed-1/python3 -I C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\4.5.3/tools/uf2conv.py --serial COM14 --family RP2040 --deploy I:\Data\Win10\Arduino/VolumeMacroPad303.ino.uf2 
Resetting COM14
Converting to uf2, output size: 529408, start address: 0x2000
Scanning for RP2040 devices
Flashing D: (RP2350)
Wrote 529408 bytes to D:/NEW.UF2
----------------------------------------------------------------------------------------------------------------

To install new version of Arduino Pico first delete it from boards manager, then delete the folder 
C:\Users\Name\AppData\Local\Arduino15\packages\rp2040 then close and reopen Arduino IDE and then add the new Arduino Pico Board again.
If a different display is used the Arduino-Pico build code must be deleted before building the new TFT_eSPI build.

NB: Use 4MB Flash option with 2MB Sketch 2MB FS 
    Use USB Stack Adafruit TinyUSB
    Use 150 MHz CPU:  #define SPI_TOUCH_FREQUENCY  5000000 used (or try 1000000 to 250000 range if button freeze)
                      #define SPI_READ_FREQUENCY   15000000
                      #define SPI_FREQUENCY        35000000 

New changes:
1. Add custom labels up to five characters for keys M1-M24, S1-S24, T1-T24. Keys M, S, T 1-24 in Layouts 1, 3, and 4, 
can have descriptive and easily changeable, 5-character-maximum-length, labels. All the custom label definition files are 
saved on the SDCard through the content in files LabelM, LabelS, and LabelT, which contain the path i.e. /folder/filename 
of the file that has the custom key labels. By default these files are named label1, label2, and label3. Refer to the manual 
section (K) for more details.
2. Fixed Config80 arrangement when using strcpy()

manual.h
(K) Toggle custom key labels for keys M,S,T 1-24 on/off - use *lm* *ls* *lt* + optional filename that contains 24 key
    labels seperated by a NULL character. For example *lt*label1 -> LabelT now has content label1 and keys T1-T24 will 
    have the labels defined in file1 where file1 can have a path before it such as /app1/label1. If only one char added 
    after *lm,s,t* such as *lm,s,t*x then the three files FileM,S,T are reset with default custom text files label1,2,3. 
    
    Can also use [Cfg][[Opt][ M,S,T ] Custom Label and press Pad (o) to toggle it on/off. Can also send a new custom 
    label filename  by using <m,s,tfilename> via serial port making sure that A-D is brown when sent, i.e it is saved on
    the SDCard. All the custom label files are saved on the SDCard namelt files LabelM, LabelS, LabelT which contains the
    path+name of the file that has the custom key labels by default this is label1, label2, label3. 

    Create the new labels (maximum 5 characters) in a text editor with a six character spacing for each label. Then use a 
    free hex editor such as HxD (https://mh-nexus.de/en/hxd/), to replace the 6,12,18,24 etc character with a NULL = 00. 
    The file SDCard-Labels.zip in the Extras folder has examples of custom label files.

    Copy the files in SDCard-Labels.zip to the SDCard for a first test of the customlabels and then use the option in
    [Cfg][[Opt][M] Custom Label+ press Pad (o) to switch the labels for keys M on/off. Without a filename in LabelM such
    as label1, and without a key label definition in file label1, Keys M1-M24 will be blank - without any labels if the
    custom label option is switched on.


