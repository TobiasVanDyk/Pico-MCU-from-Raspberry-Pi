# Pico Volume and Macro Touch Keyboard 320x240

[**VolumeMacroPad**](VolumeMacroPad203.ino) is a combination of the [**Dustin Watts Pico Touch Macro Keyboard**](https://github.com/DustinWatts/Pico-Matrix-Touch-Keyboard), the [**AdafruitTinyUSB HID examples such as hid_composite.ino**](https://github.com/adafruit/Adafruit_TinyUSB_Arduino/blob/master/examples/HID/hid_composite/hid_composite.ino), and the [**Bodmer Keypad examples**](https://github.com/Bodmer/TFT_eSPI/tree/master/examples). They were adapted for use on a  Waveshare ST7789 320x240 2.8-inch Touch LCD by replacing the PicoSDK USB stack with the Adafruit TinyUSB stack - this allowed the use of multimedia keys such as Volume Up-Down-Mute to be added to the standard keyboard touch buttons. Refer also to [**Keyboard shortcuts in Windows**](https://support.microsoft.com/en-us/windows/keyboard-shortcuts-in-windows-dcc61a57-8ff0-cffe-9796-cb9706c75eec), or read the examples given for both Linux and Windows shortcut key combinations, in the source code. 

VolumeMacroPad includes the means to execute a number of Math symbols and Windows or Linux key macros - refer to the layouts in the diagram below. It sets the Pico LED (and shows a small green "C" or "N" or "S") on the LCD, to reflect the state of the Caps-Num-Scroll-lock keys, and adds control of the LCD backlight to dim the display if not used, or set the normal use brightness. It enables a re-calibration of the touch interface, and store it as a preset, and has four layout modes (L1 L2 L3 L4) and two layers A/B, - switch layouts by pressing [L1][L2][L3][L4] or change layers with the the Volume Mute [Vo] key when in Media Key mode. Switch Layers A/B via the Cfg key + ArrowLeft [A-B] key. It also has a 4 page, 108 Math symbols keyboard, which send the standard Unicode symbol characters to the PC. This [**works on Linux Mint with LibreOffice**](images/Libreoffice1.jpg), as well as MSOffice, and use a superset of the key codes from [**Mathboard**](https://github.com/nup002/Mathboard) - refer to the content of **unicode_symbols.h** there. Note that you do not need additional software running on a Windows PC except MS Office. <img src="images/Mathboard.png" width="16" height="16"/>

Some of the layout, layer, keyboard, and configuration, modes are shown below in the pictures. The pictures shows a page from the Math Keyboard, and Layout 2 - configuration and navigation. Later pictures show the Caps-Num-Sroll-Lock key states as on as shown by the green C, N and S Letters. It is possible to send new text (or control character strings, up to 200 characters long (easily made longer in the program code), to be assigned to keys S1/T1 through to S12/T12 (or M1 - M12), via the Pico's serial USB - start each string with <1 to <6 followed by the string itself to be assigned to the keys - add a > at the end of the string. These strings are saved to the Pico's Flash memory using LittleFS. To enable the string update for layout 4 the screen must be in layout 4, for layout 3 it can be in layout 1, 2 or 3. (For the [M4] key in layout 1 use a <0 at the start of the string.) For each of the two layers A/B the upload will be to the layer that is current i.e. to keys S7 to S12 if layer B is current, and to S1 to S6 if layer A is current. A key auto-repeat that triggers after 500 milliseconds is also implemented - this can be used instead of typing a character repeatedly, or changing the volume by just holding down the Volume Up/Down keys.

Other pictures below, shows the set of seven Media keys, and Layout 1 Layer A [M1] to [M6] programmable macro keys (Layer B would show the Mx keys as M7 to M12). The Volume Mute key is replaced by L1 to L4 in the Config Menu options. It enables faster layout 1 to 4 switching by disabling/enabling the Volume Mute action by pressing the Config and Home keys in sequence. Repeat the same sequence to enable Mute again. The label on the [Vo] key will change to indicate which layout is active L1, L2, L3, or L4. If PageUp is pressed after Config i.e. the [BsD]key, the VolumeUp and VolumeDown keys are replaced by a Delete/Backspace and an Enter key. Press [Cfg] + {PgU] again to switch back to the Volume Up and Down keys. If PageDown is pressed after Cfg the startup Layout is toggled between L1 and L2, i.e. the [S112] key. If End is pressed after the Cfg key all the text files and config settings are deleted from the Pico's Flash memory, and on restart a re-calibration is done. Note that to save the VolMute/L1-L2 and Vol+Vol-/Delete-Return choices on the Flash memory you have to press Cfg [Sav] key, once more after selecting these two options. This is not necessary for the L1/L2 startup selection. 

Press the small Green Pad on the Right to toggle the Media KeyPad on/off, press the small Red Pad to toggle the Number Keypad on/off, and press the small Blue Pad to toggle the Keyboard (5 sub-pages), on/off.

<p align="left">
<img src="images/new2.jpg" height="200" /> 
<img src="images/new3.jpg" height="200" /> 
<img src="images/new4.jpg" height="200" /> 
<img src="images/new5.jpg" height="200" /> 
</p>

Using a terminal such as RealTerm it is possible to send non-ASCI characters and numbers instead of just text strings to the six keys labelled T1 to T6 - this may then perform various macro key actions - refer to the first picture in the second set of four images below. Other approaches considered included a decoder for encoded [**duckyscripts**](https://github-wiki-see.page/m/hak5darren/USB-Rubber-Ducky/wiki/Duckyscript), but directly programming these macros seems to be more efficient - refer to the built-in keyboard macro examples below. For example to open the windows snipping tool press open the macro keyboard and goto page 4 then press [GUI][ADD][SHF][ADD][NXT][s][ADD][EXE] - the snipping tool will open. This can then be saved to [M2] by pressing the [Up] button. 

Another way to save large text files or non-ASCII macro files directly in the LittleFS formatted Flash memory of the Pico is described [**here**](https://github.com/earlephilhower/arduino-pico-littlefs-plugin/blob/master/README.md) and [**here**](https://arduino-pico.readthedocs.io/en/latest/fs.html). You can therefore copy your files (ASCII text or non-ASCII macro files) using the names StrData1 or TtrData1 through to StrData12 or TtrData12 directly to the Pico Flash filesystem - see the picture below for a file list.

The keys labelled [Cut], [Cpy], and [Pst] are cut, copy, and paste actions, respectively, and execute the operating system agnostic Control + X, + C, and + V key operations. 

Other example macros are opening an admin UAC powershell [M2] (Note 1 and 4) or admin command prompt [M5]. [M1] is Alt + PrintSceen (Note 2). [M6] opens the Task Manager, and [M3] opens the Run dialog box. It will be easy to convert Layer 2 into a Mouse keys keypad - [**refer to this diagram**](https://github.com/TobiasVanDyk/Pico-MCU-from-Raspberry-Pi/blob/main/TouchLCDst7789Pico/Windows%20Keyboard%20Shortcuts/mouse-keys.jpg).

The keys M1-M12, S1-S12, T1-T12 can also be used to trigger [**AutoHotKey**](https://github.com/Lexikos/AutoHotkey_L) or [**AutoIt**](https://www.autoitscript.com/site/) macros, scripts and GUI-shells - but see Note 1 below.

* Note 1 - Because VolumeMacroPad is a trusted device (keyboard) it is able to bypass Windows UAC level 4.
* Note 2 - Some Mini-keyboards do not have a PrintScreen key.
* Note 3 - Microsoft Office Excel still has a functional use for the Scroll-lock.
* Note 4 - The UAC bypass is time sensitive and keydelay3 = 500 may have to be adjusted in the program.


``` 
Layout 1  Cycle through Layout 1 to 4 press [L1-L4] once or [Vo] 3 or more times
------------------------------------------------------------------------------------------------
[Cut Ctrl+X] [  Copy Ctrl+C   ] [Paste Ctrl+V] [VolUp  Del-Bs]    [cX]  [cC]  [cV]  [V+][Del-Bs]
[Alt+PrtScr] [Admin Powershell] [ Run window ] [VolMute L1-L4]    [M1]  [M2]  [M3]  [Vo][L1-L4 ]
[Send Text ] [Admin CMD Prompt] [Task Manager] [VolDwn  Enter]    [M4]  [M5]  [M6]  [V-][Enter ]
          Caps                Num           Scroll                    C     N     S

Layout 2  Cycle through Layout 1 to 4 press [L1-L4] once or [Vo] 3 or more times
--------------------------------------------------------------------------------------------------
[Home Stop    Mute] [UpArr BsDel] [PgeUp     DelRet] [VolUp  Delete] [Hme] [Up ] [Pgu] [V+][DelBs]
[ArrL Prev     A-B] [Config Save] [ArrR  Nxt  Media] [VolMute L1-L4] [  <] [Cfg] [>  ] [Vo][L1-L4]
[End  PlayP CfgDel] [DwnArr POff] [PgeDwn StartL1L2] [VolMute Enter] [End] [Dwn] [PgD] [V-][Enter]
                 Caps           Num               Scroll                  C     N     S

Layout 2  (Config Layout) has five additional small pad-buttons on the far right side:
Red     Press the small Red Pad on the Right to toggle the Math KeyPad on/off.
Blue    Press the small Blue Pad on the Right to toggle the Keyboard on/off.
Green   Press the small Green Pad on the Right to toggle the Media Keypad on/off.
SkyBlue Press the small SkyBlue Pad on the Right to toggle the Number Keypad on/off.
Yellow  Options Pad. KeyBrd Mode switch Direct Mode On/Off (Blue "D" indicator).
                     NumPad mode switch between 3 NumPad pages. 

Layout 3+4  Cycle through Layout 1 to 4 press [L1-L4] once or [Vo] 3 or more times
--------------------------------------------------------------------------------------------------
[Cut Ctrl+X ] [Copy Ctrl+C] [Paste Ctrl+V] [VolUp  Del-Bs]   [ cX ]  [ cC  ]  [ cV  ] [V+][Del-Bs]
[Send Text 1] [Send Text 2] [Send Text 3 ] [VolMute L1-L4]   [S1 T1] [S2 T2]  [S3 T3] [Vo][L1-L4 ]
[Send Text 4] [Send Text 5] [Send Text 6 ] [VolMute Enter]   [S4 T4] [S5 T5]  [S6 T6] [V-][Enter ]
           Caps           Num           Scroll                      C       N        S

-------------------------------------------------------------------------------------------------
Layouts 1, 3 and 4, two Layers A/B each with A=M1-M6, S1-S6, T1-T6 and B=M7-M12, S7-S12, T7-T12
Press VolumeMute consecutively within 5 seconds else count is reset to 0 or press [L1-L4] once.
[L1-L4] has a repeat period of 0.5 seconds - after this key repeat is active

Press Config Key (new set of config keys change colour):
L14 (Home) - VolumeMute -> L1, L2, L3, L4 - repeat to restore Vo
BsD (ArrowUp) - Toggle Vol+ key to either Delete or Backspace
Vol (PageUp) - Vol+ -> Delete and Vol- -> Enter - repeat to restore V+ V-
A-B (ArrowLeft) - Layouts 1, 3, 4, change to Layer A or Layer B 
Med (ArrowRight) - Change Layout 2 to Media Controls Previous-Next-PlayPause-Stop
Del (End) - Delete all files on Flash (Strings and Config) - will re-calibrate on restart
S12 (PageDown) - Start with Layout 1 or Layout 2 on powerup - must also press Sav(e) (Cfg)
Sav (Cfg) Info and File List to send Serial Monitor and Text/Macro and Config files saved
ROf (ArrowDwn] - Restart-PowerOff-Logoff - Bottom row [Rst][Log][Off] - cancel by pressing 
    [Cfg][ROf] - There are long and short Timer options as well

Text Strings: 
Send new text strings up to 200 characters to keys S1/T1 - S12/T12 via USBserial
Start string with <1 to <6 followed by the string assigned to S1/T1-S12/T12 - end string with >
If current Layout is L3 then S1 to S6 changed if Layout is L4 then T1 to T6 changed
If current Layer is A then S1/T1-S6/T6 changed, layer B S7/T7-S12/T12 changed. 

To send the text "This is a string for key [S2]." use a serial terminal to send 
<2This is a string for key [S2].> with Layer 3 visible.

Send new text for Key [M4] - start with <0 end with > and send it with Layers 1 or 3 visible - if 
sent from Layer 2 it will be ignored. 
Example 1: Send <0This is [M4] Text> with Layers 1 or 3 visible - pressing [M4] will send the text 
string "This is [M4] Text" to the PC. 
Example 2: Send the macro 0x3C 0x30 0xE0 0xE1 0x29 0x3E (which is <0 Control Shift Escape >), 
with Layer 4 visible, then pressing [M4] will open the Task Manager - refer to Key-M4-Examples.jpg below for 
this example.

Macro Composition Keyboard:

Keyboard Page 1              Page 2          Page 3          Page 4              Page 5
[abc] [def] [ghi] [EXE]  [ABC] to [XY_]  [012] to [9+-]  [1F3] to [1F2]  [ALT] [SHF] [CTR] [EXE]
[jkl] [mno] [pqr] [NXT]    Uppercase     Numbers            F1 -  F12    [GUI] [TEI] [CRF] [NXT]
[stu] [vwx] [yz ] [ADD]    Uppercase     Symbols          Symbols        [LHR] [UED] [UND] [ADD] 

Page 5      [ALT] = [ Alt-L ]  [  Alt-R ]  [ PrintScr]   [EXE]
Keyboard    [SHF] = [Shift-L]  [Shift-R ]  [ Delete  ]
            [CTR] = [Contr-L]  [Contrl-R]  [BackSpace]
            [GUI] = [ Win-L ]  [ Win-R  ]  [  NULL   ]   [NXT]   
            [TEI] = [  Tab  ]  [ Escape ]  [ Insert  ]
            [CRF] = [  C/R  ]  [   L/F  ]  [  Return ]
            [LHR] = [Arrow-L]  [  Home  ]  [ Arrow-R ]   [ADD]
            [UED] = [ArrowUp]  [  End   ]  [Arrow-Dwn]
            [UND] = [PageUp ]  [ Numlock}  [ PageDwn ]
       
The keyboard is has 5 pages each with 9 triple function keys (that is 135 different keys), and 3+1 control keys 
[EXE] [NXT] [ADD]. For example page 1 has keys [abc], [def], to [y,z,space]. To select a or b or c press the
abc key once, twice or thrice - to add it to a new macro press the ADD key else press another character-symbol
modifier key or press [NXT] for the next keyboard page. Press the small blue pad again to leave (escape) the 
keyboard or press [EXE] to send the macro to the PC. The maximum length of a text macro is 200 characters.

The macros on page 5 are modifiers (simultaneously pressed keys) such as Control + Alt + Delete + GUI (maximum 
of 6 keys in macro). To send this sequence press [CTR][ADD][ALT][ADD][SHF]x3 (3 times for delete)[ADD][EXE]. 

To open the windows snipping tool press [GUI][ADD][SHF][ADD][NXT][s][ADD][EXE] - the snipping tool will open. 
This can then be saved to [M2] by pressing the [Up] button. 

To send a sequence such as [WinKey] + "r" press [GUI][ADD][NXT][pqr]x3[ADD][EXE]. To assign Alt + PrintScr to
key [M2] press [NXT]4x[ALT][ADD][ALT]3x[ADD][EXE} and [Up] to save. Alt + PrtScr is then executed when pressing
[M2]. 

Another example: [NXT]4x[SHF][NXT][abc][ADD][EXE] will send "A" not "a" because Shift is pressed at the same time
as "a". Toggle the sticky keys press [SHF][ADD] 5 times then press [EXE] and save to key [M2]. (For a complete 
macro add [C/R] [ADD] key at the end of this sequence.) 

The option to use the combined modifier bit instead of a modifier byte, is used in the top row Cut-Copy-Paste 
keys, and also for some of the pre-programmed examples for the M1-M12 keys. 

Macros sent to the PC from the built-in keyboard will be saved to file KeyBrdMacro2 and if so selected, assigned
to key [M2] if of the modifiers type, and is saved as KeyBrdMacro1 an assigned to key [M1], if of the sequence type 
(200 chars max). Press [Up] after sending the macros to the PC to assign them to [M1] or [M2]. Chaining macros of 
both types are planned.

F1-F12 keys are sent as keycodes (simultaneous) and not keypress types - to send [F3] open the macro keyboard then
press [NXT]3x[1F3]3x[ADD][EXE]. Press [Up] to assign it to key [M2]. Once assigned it will survive a reboot.

The Keyboard has a Direct (to PC) Mode - use the Yellow Options Pad on the bottom right, to switch Direct Mode On/Off.
A Blue "D" indicator will show if its is on. Any character selected (shows in status bar), will be sent to the PC by 
pressing [EXE] - [ADD] is not necessary. If a character or more that one characters have been [Add]ed they will only 
be sent after the Direct Mode is switched off.

The Keyboard can be used to change various options by sending *option*value command such as:
* 1 LCD blank timeout - Send the macro *tb*num with the built-in keyboard where num = 0,1-9 - *tb*0 = 120 hours, 
*tb*1 = 30 seconds.  
* 2 LCD blank dim value - Send the macro *db*num with the built-in keyboard where num = 0,1-9 - *db*0 = no
dimmming - just on/off after blank timeout, and 1-9 = 3 to 30 % PWM instead of blank. 
* 3 LCD screen brightness - Send the macro *bb*num with the built-in keyboard where num = 0,1-9 - *bb*0 = full
brightness and 1-9 = 3 to 50 % PWM brightness.
* 4 Power Keys Menu/Command Option - Send the  macro *po* with the built-in keyboard to toggle the Power Keys Option
to use the Menu (GUI+X + U + i,u,r or the run command GUI+R + shutdown + options command.
* 5 Power Keys time values - Default Short Time = 30 seconds and Default Long Time 600 seconds (10 minutes). To change 
Time values send PowerOff *ot*num or *oT*num or Restart *rt*num or *rT*num where num 0 = 6000 second (100 minutes)
1 = 30 second (x100 for T) to 9 = 300 second (x100 for T).
* 6 Calibration Enable/Disable - Send the macro *ca* with the built-in keyboard to set/clear re-calibration option after a 
restart to ON or OFF. 
* 7 LCD Orientation 180 degree change - Send the macro *ro* with the built-in keyboard - the USB cable will then 
be on the opposite side. This forces a re-calibration on restart. See the panic mode reset at the end of this description.
* 8 Button Font Bold/Normal change - Send the macro *fo* with the built-in keyboard to turn the labels on the buttons from
normal to bold. 

Math-Greek-Algebra Keyboard:
This is a triple-key macro keyboard with 4 pages and 4 x 9 x 3 = 108 math and Greek algebra
symbols. It is sent to the PC as (hex) Unicode + [ALT] + [x] and can be directly entered into MSWord and in LibreOffice
(also on Linux). The three control keys are [EXE] - send the symbol to the PC, [NXT] - next page of symbols, and [ESC]
go back to the main Config layout - or press the small red pad again to leave the Math keyboard. Watch out for MSWord 
insists on capitilizing the first letter of a sentence - which will change the first math symbol in a sentence/line 
after a second symbol or letter or enter is typed. 

The [M6] key section in the code has a few examples of using Alt + Number Keypad for Maths and special symbols or 
characters. There are 2 ways to do it - the first is to type the character's Unicode (hex without the 0x or decimal) into 
MSWord and then press [ALT] + [x]. The second method is to hold the Alt key down, then type in a special code number using
the number keypad and then release the Alt key. There are examples of entering the open infinity symbol and the small pi 
synbol in the [M6] key section. 

Numeric Keypad    [ BackSpc] [ 7 Spc aA ] [ 8 % bB ] [ 9 xX cC ]  Press 4th Pad - toggle the Number Keypad on/off.
                  [ Return ] [ 4  ,  dD ] [ 5 . eE ] [ 6 =  fF ]  Press 5th Pad - switch Num Pages 1-3 - CapsLock a-A       
                  [ 0  + ( ] [ 1  -   ) ] [ 2 *  [ ] [ 3 /   ] ]  Macro *kb from the built-in Keyboard show keybrd

Power Restart KeyPad    [Restart  Long Timer] [ Stop ] [PowerOff  Long Timer]        [R-T] [Stp] [O-T] [ ]   
                        [Restart Short Timer] [  Cfg ] [PowerOff Short Timer]        [R-t] [Cfg] [O-t] [ ]       
                        [Restart   Immediate] [Logoff] [PowerOff    Imediate]        [Rst] [Log] [Off] [ ]
 
To cancel a timed shutdown press the [Stop] key. To exit the Power Restart Keypad press any of the [black] non-
functional keys or press [Cfg][ROf] again.

Panic mode reset. If for any reason your keypad becomes unresponsive or behaves strangely (*) reset it as follows:

(1) If the Config button [Cfg] still works press it once and then press the [Del]ete key at the left-hand bottom. Then 
    unplug or reset the keypad. This will delete all files. macros and settings and you should have a re-calibration at
    start-up.
(2) Press either the white button at the bottom and hold it in, then press the black reset button at the back and release
    it (or unplug and re-plug the USB cable instead of the rest button), then only release the white button at the bottom.
    The file manager should show a new storage device named RPI-RP2. Drag and drop any of the code.UF2 files to this 
    device. It will restart after a second or two. If this still does not reset the keypad then instead of the code.UF2 
    file drag and drop the file flash_nuke.uf2, wait a few seconds and then drag the code.UF2 file to the device.

(*) Writing Greek letters is not behaving strangely.
```
<p align="left">
<img src="images/picD.jpg" height="200" /> 
<img src="images/picA.jpg" height="200" /> 
<img src="images/picF.jpg" height="200" /> 
<img src="images/picB.jpg" height="200" /> 
</p>

Another use of the two main layouts could be to have one customised for Linux - although all the keys in layout 1 and 2 except the run dialog, and the powershell and command prompt, function the same under Linux.

The list of key names that can be used is in TinyUSB stack's hid.h, as included here. Use the configuration descrition as shown below to set up the TFT_eSPI driver. The sampled calibration data can be adjusted - see the comments in the program for instructions. Compile with the USB stack set to TinyUSB not PicoSDK. 

*In contrast, this [**Pico mbed RP2040 volume and macro keyboard**](https://www.hackster.io/Murchx/pi-pico-macro-keyboard-d0bd1c) uses the Arduino mbed-based RP2040 board definitions (instead of the earlephilhower version). The USB Keyboard supports both normal and consumer keys using only one include **#include <USBKeyboard.h>**. The article is [**here as a pdf file**](Pi-Pico-mbed-RP2040-volume-and-macro-keyboard.pdf). The code was tested using push-buttons on both Arduino 2.0 and 1.8.19 IDE's, and was working (could adjust the volume up or down). [**This discussion**](https://github.com/Bodmer/TFT_eSPI/discussions/1558) notes that the Pico mbed Arduino board does not offer touch support when used with the TFT_eSPI drivers.*

<p align="left">
<img src="images/Key-M4-Examples.jpg" height="160"/> 
<img src="images/case3.jpg" height="160" /> 
<img src="images/info.jpg" height="160" /> 
<img src="images/macro3.jpg" height="160" />  
</p>

STL files for the case - based on [**Customizable LCD Box**](https://www.thingiverse.com/thing:57427), are in the STL folder when available for the specific LCD. To protect the LCD resistive film use a sheet of laser print transparent film cut to the right size for the LCD.

### LCD ST7789 320x240 Resistive Touch SPI driver

Use the [**Waveshare ST7789**](https://www.waveshare.com/pico-restouch-lcd-2.8.htm) Pico-ResTouch-LCD-2.8 [**320x240 2.8 inch IPS LCD**](https://www.waveshare.com/wiki/Pico-ResTouch-LCD-2.8), connected as shown [**here**](images/connections.jpg). Note that the LCD board is jumper hard-wired to be non-SDIO - that means five of the six GPIO SDIO pins are available for other use (gpio pins 5, and 18 to 21). 

<p align="left">
<img src="images/macro2.jpg" height="160" />  
<img src="images/pcb1.jpg" height="160" /> 
<img src="images/pcb2.jpg" height="160" /> 
<img src="images/picC.jpg" height="160" />          
</p>

Arduino support is through the [**Bodmer TFT_eSPI graphics library**](https://github.com/Bodmer/TFT_eSPI) and the [**Earlephilhower Arduino Pico Port**](https://github.com/earlephilhower/arduino-pico/), and the Adafruit GFX library (for some of the button classes). XPT2046 touch screen controller support is for SPI based displays only.

**The install order on Windows 10x64**
1. [**Arduino IDE**](https://www.arduino.cc/en/software) version 1.8.19.
2. [**Earlephilhower Aduino Pico port**](https://github.com/earlephilhower/arduino-pico/)
3. [**Adafruit GFX library**](https://github.com/adafruit/Adafruit-GFX-Library).
4. [**Bodmer TFT_eSPI graphics library**](https://github.com/Bodmer/TFT_eSPI).
5. [**Adafruit TinyUSB library**](https://github.com/adafruit/Adafruit_TinyUSB_Arduino)

<p align="left">
<img src="images/lcd1.jpg"height="200" /> 
<img src="images/lcd2.jpg" height="200" /> 
<img src="images/mode0.jpg" height="200" />  
<img src="images/mode1.jpg" height="200" /> 
</p>


Specify the display and touch gpio in User_Setup.h (included [**here**](User_Setup.h)).


```
#define TFT_WIDTH  240 // ST7789 240 x 240 and 240 x 320
#define TFT_HEIGHT 320 // ST7789 240 x 320

#define ST7789_2_DRIVER    // Minimal configuration option, define additional parameters below for this display

#define TFT_BL   13            // LED back-light control pin
#define TFT_BACKLIGHT_ON HIGH  // Level to turn ON back-light (HIGH or LOW)

#define TFT_MISO 12
#define TFT_MOSI 11
#define TFT_SCLK 10
#define TFT_CS   9     // Chip select control pin
#define TFT_DC   8     // Data Command control pin
#define TFT_RST  15    // Reset pin (could connect to RST pin)

#define TOUCH_CS 16    // Chip select pin (T_CS) of touch screen

#define TFT_SPI_PORT 1 // Set to 0 if SPI0 pins are used, or 1 if spi1 pins used

```

