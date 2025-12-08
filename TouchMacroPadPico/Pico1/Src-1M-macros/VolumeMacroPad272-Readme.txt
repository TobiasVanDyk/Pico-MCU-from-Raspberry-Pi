Compiled with Pico SDK 2.21-develop, Arduino Pico 5.4.3 and included Adafruit_TinyUSB_Arduino 3.7.2, and TFTeSPI 2.5.43
Pico 1 RP2040 and 3.5inch Touch Display Module for 200MHz Raspberry Pi Pico included SDCard module https://www.waveshare.com/pico-restouch-lcd-3.5.htm
-------------------------------------------------------------------------------------------------------------------------------------------------
Using library Adafruit_TinyUSB_Arduino at version 3.7.2 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.4.3\libraries\Adafruit_TinyUSB_Arduino 
Using library SPI at version 1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.4.3\libraries\SPI 
Using library TFT_eSPI at version 2.5.43 in folder: C:\Users\Tobias\Documents\Arduino\libraries\TFT_eSPI 
Using library LittleFS at version 0.1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.4.3\libraries\LittleFS 
Using library SDFS at version 0.1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.4.3\libraries\SDFS 
Using library SdFat at version 2.3.1 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\5.4.3\libraries\SdFat 
"C:\\Users\\Tobias\\AppData\\Local\\Arduino15\\packages\\rp2040\\tools\\pqt-gcc\\4.1.0-1aec55e/bin/arm-none-eabi-size" -A "I:\\Data\\Win10\\Arduino/VolumeMacroPad272.ino.elf"
Sketch uses 250364 bytes (23%) of program storage space. Maximum is 1044480 bytes.
Global variables use 46508 bytes (17%) of dynamic memory, leaving 215636 bytes for local variables. Maximum is 262144 bytes.
Resetting COM9
Converting to uf2, output size: 561152, start address: 0x2000
Scanning for RP2040 devices
Flashing D: (RPI-RP2)
Wrote 561152 bytes to D:/NEW.UF2
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
1. Fixed Starcodes not-found check=100
2. Consolidate Mouse Controls by *codes - refer to section (m) in manual.h.
   Fix coding d99 for c99 in SendBytesStarCodes()
   *mm*udlr,UDLR,nn = Cursor moves 0 - 999 pixels Up Down Left Righ UDLR = 10 x udlr nn = 01-99 pixels move
   Monitor Corner for mouse zero: *mZ*n n=0,1,2,3 0=LB 1=LT 2=RT 3=RB Saved in Config1 as MouseZ. Default is LB = Left Bottom.
   Mouse Wiggler: *mw*nn and *mW*nn Mouse Wiggler non-blocking clockwise and blocking anti-clockwise, with n time in hours and nn time in minutes for active wiggler.
   Mouse Cursor Positioning: Add Mouse Position Cursor m0*nn at 0,0 if no nn, or at nn,nn or n,n on Taskbar.
   Note it assumes 4K max resolution monitor, and if a 2nd monitor attached it is on right-hand side. Mouse Position works by moving
   the cursor 2160 pixels up or down and 3840 pixels left or right, and then moving it to the XY n,n position up/down and right/left.
   Mouse buttons *mb*lrm,LRMd (lrm single-click,L=dRM doubleclick), and mouse scrollwheel *ms*du,DUnn nn=01-99 pixels and UD=10xscroll              	                            
3. Fixed History now saved after [k] exit and after [EXE] pressed (not after [Sav] and [Snd]
   Reverted clumsy [*Cm]+[ADD] keys fixes because of side-effects - if [ADD] pressed after [*Cm] press [*Cm] again
   Add green [s] Pad to save current entry in MacroEditor - Pads [h]istory -> [r]ecall after [s]ave pressed after at least one [ADD]
   Also minor cosmetic changes to key [Fsp]->[F+N] and shows F+0,9 not f01,9
4. MacroEditor history added to existing entries instead of replacing them
5. Additional macro processing options through first char 0xF2 and 0xF3. Add 0xF2 or 0xF3 through pressing key [F+N] in MacroEditor:
   If 0xF3 added as first char can construct a hex string in MacroEditor that will be sent unchanged (but without the first 0xF3) to the PC.
   For example construct [F+N]4x[ADD]D[ADD]F[ADD]8[ADD]9[ADD][Snd] and the hex characters 0xDA 0x89 will be sent to the PC. Can [Sav] or
   use [EXE] as well to save or execute and then save the string.
   If 0xF2 added as first char then macrokeys M1-M24, S1-S23, T1-T24 behave as nKeys i.e. they contain a filename that will be executed.
   For example file a01 has Ctrl+Shft+Esc, file m07 has filename a01 but with F2 at start i.e. m07 content 0xF2 0x61 0x30 0x31 when key [M7] pressed TaskManager opens
   Construct m07 in macroeditor set source to M07 white, press [F+N]3x[ADD] then a01 via [ADD] then [Sav]. File a01 is already saved in flash content 0xE0 0xE1 0x20 
6. Further fixed MacroBuff not cleared now working with [EXE] in MacroEditor
7. Fixed filenumbers > 24 for saved filename construction
8. Fixes for Function keys F1-F24 and all 17 KeyPad keys if they are the first action in the macro to be executed. As a result Macro strings starting with 0xF0, 0xF1, and 0xFF 
   are treated differently (for Function keys, KeyPad keys, and Modifier-byte respectively).   
9. New third option via [UDM] -> "Mod" key in MacroEditor to force the use of the Modifier byte for Control, Shift, Alt and Gui keys, instead of one or more of the 6 available 
   HID simultaneous keycode slots.
10. Added [KPd] key on MacroEditor for adding the 17 KeyPad keys. Read wiki for examples. Read line 886 in manual.h for 3 methods to execute em-dash.


Previous changes:
1. Added History key, green Pad [h], to MacroEditor to access previously added strings. 4 green Pads dedicated to Macroeditor [k][<][h][o].
   Direct Mode (Blue D indicator in MacroEditor), accessed via *md* to switch on. To switch off press [*Cm] key.
2. Can send serial string <c ... > directly to MacroEditor buffer for processing. For example send <cmanual.txt=usermanual.txt> with white A and Macroeditor open.
   Then in macroeditor switch to sdcard (both src and dst brown and then press [Ren] and file manual.txt on SDCard is renamed usermanual.txt. If MacroEditor is 
   closed when string is sent then use the history Pad [h] to access the serial string.
3. Added Backspace Delete key, green Pad [<], to MacroEditor to correct [ADD]ed entries. 4 green Pads dedicated to Macroeditor [k][<][h][o].
4. *c1* = copy all Flash Files to folder Flash on SDCard.
   *c2* = copy files in SDcard folder Flash to root of Flash memory i.e. restore previously copied Flash files
5. Completed all Numkey (option instead of nKeys) key values.
6. Fix PadKey status message for MouseKeys, SymbolsPages, MacroEditor, and fix Src Dst left in display if MacroMbrd not cancelled
7. Fix Black Text label on Black Key unreadable
8. Can now use up to 1080 Special Symbols (Math and Greek etc). 
   These symbols using 4-char Unicode and Alt+X expanded to up to 10 different symbol sets, each with 108 symbols (4 pages of 9 keys each with 3 symbols = 4 x 9 x 3 = 108.
   Load symbol set 0-9 using *ma*0-9 or by using the [Load] key in the Symbols page - if the file Math0 to Math9 exists on the SDCard it is loaded as the current symbol set.
   Read mathKeys.h for more instructions - you can use *ma* with no number added to save the 3 Math Arrays in mathKeys.h to the SDCard as file MathX
   The Symbols keys can now have up to 5 character labels (previously 3). 
9. Fixed Media Controls precedence and new options for media *e0* to *e6*
10. *vx*000 to *vx*111 Volume enabled/disabled in Layouts 1,3,4 if enabled in Layout 2 
11. *dt*f,m,s or *dt*0,1,2 to adjust the delay times between macros/keys-pressed for slower or virtual machines, medium fast PCs and fast PCs.
12. Default is now long-press Volume Mute off i.e. now L1-L4 and not V1-V4 default.
13. Increased delay times in DoPowerKeys because in VM or slower PC first characters are missing in restart or shutdown string.
14. Switch Backlight Off/On via *Cmd *bl*0 = off *bl*1 = on - can use serial monitor and send <*bl*0> for blankscreen and
    <*bl*1> for full bright screen. For inbetween values use *bl*nn nn = 00 - 99
