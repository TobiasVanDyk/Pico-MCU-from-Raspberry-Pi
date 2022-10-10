

# Pico Volume and Macro Touch Keyboard 480x320 4 inch ILI9486

[**VolumeMacroPad**](VolumeMacroPad5.ino) is a combination of the [**Dustin Watts Pico Touch Macro Keyboard**](https://github.com/DustinWatts/Pico-Matrix-Touch-Keyboard), the [**AdafruitTinyUSB HID examples such as hid_composite.ino**](https://github.com/adafruit/Adafruit_TinyUSB_Arduino/blob/master/examples/HID/hid_composite/hid_composite.ino), and the [**Bodmer Keypad example**](https://github.com/Bodmer/TFT_eSPI/tree/master/examples) Keypad_480x320.ino. They were adapted for use on a  [**Waveshare ILI9486 480x320 4.0 inch Arduino-styled interface Touch LCD**](https://www.waveshare.com/4inch-tft-touch-shield.htm) by replacing the PicoSDK USB stack with the Adafruit TinyUSB stack - this allowed the use of multimedia keys such as Volume Up-Down-Mute to be added to the standard keyboard touch buttons. Refer also to [**Keyboard shortcuts in Windows**](https://support.microsoft.com/en-us/windows/keyboard-shortcuts-in-windows-dcc61a57-8ff0-cffe-9796-cb9706c75eec). 

<p align="left">
<img src="images/pic10.jpg" height="200" /> 
<img src="images/p1.jpg" height="200" /> 
<img src="images/stripboard1.jpg" height="200" /> 
</p>

The notch in the stripboard picture above is because of the Arduino Uno Layout Ooops. 

The connections between the ILI9486 LCD, TOUCH CONTROLLER, SDCARD, RESET BUTTON and the Pico are: 

|ILI9486| GND | +5V  | MOSI | MISO | SCLK |LCD-CS|LCD-DC|LCD-RST|LCD-BL|
|:------|:----|:-----|:-----|:-----|:-----|:-----|:-----|:------|:-----|
|  PICO | GND | VBUS | GP11 | GP12 | GP10 | GP9  | GP8  | GP15  | GP13 |

|XPT2046|TP-CS|TP-IRQ|TP-BUSY|SD-CS|RESET-BUTTON|
|:------|:--- |:-----|:------|:----|:-----------|
|  PICO |GP16 | NC   | NC    |GP22 |   RUN      |

VolumeMacroPad includes a number of example Math and Windows macros - refer to the four layouts in the diagram below. It sets the Pico LED (and shows a small green "C" or "N" or "S"), to reflect the state of the Caps-Num-Scroll-lock keys, and adds control of the LCD backlight to dim the display if not used. It includes preset (typical) touch calibration values, and has four layout modes and two layers A/B, for layouts L1, L3, and L4- switch layouts by pressing [L1] to [L4] or via the Volume Mute [Vo] key, and switch layers via the Cfg key + ArrowLeft. It also has a 3 page, 81 symbols Math keyboard, which send Unicode characters to the PC. This [**works on Linux Mint with LibreOffice**](images/Libreoffice1.jpg) and use the key codes from [**Mathboard**](https://github.com/nup002/Mathboard) - refer to the content of **unicode_symbols.h** there. Note that you do not need additional software running on a Windows PC except MS Office. 

Some of the four modes are shown below in the pictures. The pictures shows a page of the Math Keyboard, and a picture of the Layout 2 - config navigation, with Caps-Num-Sroll-Lock key states on as shown by the green C, N and S Letters. It is possible to send new text (or control character strings, up to 200 characters long (easily made longer in the program code), to be assigned to keys S1/T1 through to S12/T12 via the Pico's serial USB - start each string with <1 to <6 followed by the string itself to be assigned to the keys - add a > at the end of the string. These strings are saved to the Pico's Flash memory using LittleFS. To enable the string update for layout 4 the screen must be in layout 4, for layout 3 it can be in layout 1, 2 or 3. (For the [M4] key in layout 1 use a <0 at the start of the string.) For each of the two layers A/B the upload will be to the layer that is current i.e. to keys S7 to S12 if layer B is current, and to S1 to S6 if layer A is current. A key auto-repeat that triggers after 500 milliseconds is also implemented.

Other pictures in the set below, shows the Media keys, and Layout 1 Layer A [M1] to [M6] programmable macro keys (Layer B would show the Mx keys as M7 to M12). The Volume Mute key is replaced by L1 to L4 in the Config Menu options. It enables faster layout 1 to 4 switching by disabling/enabling the Volume Mute action by pressing the Config and Home keys in sequence. Repeat the same sequence to enable Mute again. The label on the [Vo] key will change to indicate which layout is active L1, L2, L3, or L4. There is a timeout on the Layout switch key (Vo or L1-L4), i.e. the key presses must happen fairly quickly to cycle through the four layouts. If PageUp is pressed after config the VolumeUp and VolumeDown keys are replaced by a Delete and an Enter key. Press [Cfg] + {PgU] again to switch back to the Volume Up and Down keys. If PageDown is pressed after Cfg the startup Layout is toggled between L1 and L2. If End is pressed after the Cfg key all the text files and config settings are deleted from the Pico's Flash memory. Note that to save the VolMute/L1-L2 and Vol+Vol-/Delete-Return choices on the Flash memory you have to press Cfg by itself, at least once more after selecting these two options. This is not necessary for the L1/L2 startup selection.

Press the small Green Pad on the Right to toggle the Media KeyPad on/off, press the small Red Pad to toggle the Number Keypad on/off, and press the small Blue Pad to toggle the Keyboard (5 sub-pages), on/off.

<p align="left">
<img src="images/picG.jpg" height="180" /> 
<img src="images/picB.jpg" height="180" /> 
<img src="images/picC.jpg" height="180" /> 
<img src="images/picD.jpg" height="180" /> 
</p>

Using a terminal such as RealTerm it is possible to send non-ASCI characters and numbers instead of just text strings to the six keys labelled T1 to T6 - this may then perform various macro key actions - refer to the first picture in the second set of four images below. Other approaches considered included a decoder for encoded [**duckyscripts**](https://github-wiki-see.page/m/hak5darren/USB-Rubber-Ducky/wiki/Duckyscript), but directly programming these macros seems to be more efficient - refer to the built-in keyboard macro examples below.

Another way to save large text files or non-ASCII macro files directly in the LittleFS formatted Flash memory of the Pico is described [**here**](https://github.com/earlephilhower/arduino-pico-littlefs-plugin/blob/master/README.md) and [**here**](https://arduino-pico.readthedocs.io/en/latest/fs.html). You can therefore copy your files (ASCII text or non-ASCII macro files) using the names StrData1 or TtrData1 through to StrData12 or TtrData12 directly to the Pico Flash filesystem - see the 3rd picture in the second set below for a file list.

The keys labelled [Cut], [Cpy], and [Pst] are cut, copy, and paste actions, respectively. The code has two examples for each of these three operations: using the modifier bit (0-7) byte (each of the Alt Shift Control and Gui keys has a bit position associated with them for both Left and Right keys - i.e. 8 bits), or alternatively using the modifier Control-Left as the first byte, and either X, C or V as the second byte in a the keycode array. Both approaches work equally well.

Other example macros are opening an admin UAC powershell [M2] (Note 1, 4, 5) or admin command prompt [M5]. [M1] is Alt + PrintSceen (Note 2). [M6] opens the Task Manager, and [M3] opens the Run dialog box. It will be easy to convert Layer 2 into a Mouse keys keypad - [**refer to this diagram**](https://github.com/TobiasVanDyk/Pico-MCU-from-Raspberry-Pi/blob/main/TouchLCDst7789Pico/Windows%20Keyboard%20Shortcuts/mouse-keys.jpg).

* Note 1 - Because VolumeMacroPad is a trusted device (keyboard) it is able to bypass Windows UAC level 4 - see Note 4.
* Note 2 - Some Mini-keyboards do not have a PrintScreen key.
* Note 3 - Microsoft Office Excel still has a functional use for the Scroll-lock.
* Note 4 - If the Macro Keypad triggers a Windows-based Macro or Shortcut Keys (including [**AutoIt**](https://www.autoitscript.com/site/)), it cannot bypass UAC.
* Note 5 - The UAC bypass is time sensitive and keydelay3 = 500 may have to be adjusted in the program.
* Note 6 - Change to Layout 1 to 4 by pressing VolumeMute or L1 to L4 consecutively. 

``` 
Layout 1  Cycle through Layout 1 to 4 press [L1-L4] once or [Vo] 3 or more times
------------------------------------------------------------------------------------------------
[Cut Ctrl+X] [  Copy Ctrl+C   ] [Paste Ctrl+V] [VolUp  Del-Bs]    [cX]  [cC]  [cV]  [V+][Del-Bs]
[Alt+PrtScr] [Admin Powershell] [ Run window ] [VolMute L1-L4]    [M1]  [M2]  [M3]  [Vo][L1-L4 ]
[Send Text ] [Admin CMD Prompt] [Task Manager] [VolDwn  Enter]    [M4]  [M5]  [M6]  [V-][Enter ]
          Caps                Num           Scroll                    C     N     S

Layout 2  Cycle through Layout 1 to 4 press [L1-L4] once or [Vo] 3 or more times
--------------------------------------------------------------------------------------------------
[Home  Prev  Mute] [UpArr  BsDel] [PgeUp Nxt DelRet] [VolUp  Delete] [Hme] [Up ] [Pgu] [V+][DelBs]
[ArrL PlayPse A-B] [Config  Save] [ArrR  Stop Media] [VolMute L1-L4] [  <] [Cfg] [>  ] [Vo][L1-L4]
[End   CfgFileDel] [DwnArr  POff] [PgeDwn StartL1L2] [VolMute Enter] [End] [Dwn] [PgD] [V-][Enter]
                Caps            Num               Scroll                  C     N    S

Layout 2  (Config Layout) has five additional small buttons on the far right side:
Red     Press the small Red Pad on the Right to toggle the Math KeyPad on/off.
Blue    Press the small Blue Pad on the Right to toggle the Keyboard on/off.
Green   Press the small Green Pad on the Right to toggle the Media Keypad on/off.
SkyBlue Press the small SktBlue Pad on the Right to toggle the Number Keypad on/off.
Yellow  Not assigned.

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
[abc] [def] [ghi] [ESC]  [ABC] to [XY_]  [012] to [9+-]  [1F3] to [1F2]  [ALT] [SHF] [CTR] [EXE]
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
[ESC-EXE] [NXT] [ADD]. For example page 1 has keys [abc], [def], to [y,z,space]. To select a or b or c press the
abc key once, twice or thrice - to add it to a new macro press the ADD key else press another character-symbol
modifier key or press [NXT] for the next keyboard page. Press [ESC] to leave the keyboard or on page 5 press [EXE]
to send the macro to the PC. The maximum length of a text macro is 200 characters.

The macros on page 5 are modifiers (simultaneously pressed keys) such as Control + Alt + Delete + GUI (maximum of 
6 keys in macro). To send this sequence press [CTR] [ADD] [ALT] [ADD] [SHF]x3 (3 times for delete) [ADD] [EXE]. 
To send a sequence such as [WinKey] + "r" press [GUI] [ADD] [NXT] [pqr]x3 [ADD] [NXT]x4 [EXE]. To assign Alt + 
PrintScreen to key [M2] press [Cfg] [Kbd] [NXT] 4 times then [ALT] [ADD] [ALT] 3 x then [ADD] [EXE} [Up]. Alt + 
PrtScr is then executed when pressing [M2]. Another example: [Cfg] [Kbd] [NXT]4x [SHF] [NXT] [abc] [ADD] [NXT]4x 
[EXE] will send "A" not "a" because Shift is pressed at the same time as "a". Toggle the sticky keys press the 
[SHF] [ADD] 5 times then press [EXE] and save to key [M2]. (For a complete macro add [C/R] [ADD] key at the end of 
this sequence.) 

The option to use the combined modifier bit instead of a modifier byte, is used in the top row Cut-Copy-Paste 
keys, and also for some of the pre-programmed examples for the M1-M12 keys. It will be added later as an option for
the built-in macro keyboard.

Macros sent to the PC from the built-in keyboard will be saved to file KeyBrdMacro2 and if so selected, assigned
to key [M2] if of the modifiers type, and is saved as KeyBrdMacro1 an assigned to key [M1], if of the sequence type 
(200 chars max). Press [Up] after sending the macros to the PC to assign them to [M1] or [M2]. Combining macros of 
both types are planned.

F1-F12 keys are sent as keycodes (simultaneous) and not keypress types - to send [F3] press [Cfg] [Kbd] [NXT] 3 times 
then [1F3] 3 times then [ADD] [NXT] [EXE]. Press [Up] to assign it to key [M2]. Once assigned it will survive a reboot.

The Keyboard can be used to change various options by sending *option*value command such as:
(1) Change the LCD blank timeout: Send the macro *tb*num with the built-in keyboard where num = 0,1-9 - *tb*0 = 120 hours, 
*tb*1 = 30 seconds. Too change the normally-on-brightness-level use *bb*num ith the built-in keyboard where num = 0,1-9 
- *bb*0 no dimming full normal brightness, and 1-9 = 8/255 to 128/255 % PWM instead of full brightness. 
(2) Send the macro *po* with the built-in keyboard to toggle the Power Keys Option to use the Menu (GUI+X + i,u,r or the 
GUI+R + shutdown + options command
(3) Send the macro *ca* with the built-in keyboard to set/clear re-calibration option after a restart to ON or OFF
(4) Change the LCD screen-dimmer value: Send the macro *db*num with the built-in keyboard where num = 0,1-9 - *db*0 = no
dimmming - just on/off after blank timeout, and 1-9 = 8/255 to 80/255 % PWM instead of blank. 
(5) Change the LCD Brightness value: Send the macro *bb*num with the built-in keyboard where num = 0,1-9 - *bb*0 = full
brightness and 1-9 = 8/255 to 128/255 % PWM brightness.
(6) Change the LCD Orientation by 180 degrees: Send the macro *ro* with the built-in keyboard.

Math-Greek-Algebra Keyboard:
This is a triple-key macro keyboard with 4 pages and 4 x 9 x 3 = 108 math and Greek algebra
symbols. It is sent to the PC as (hex) Unicode + [ALT] + [x] and can be directly entered into MSWord and in LibreOffice
(also on Linux). The three control keys are [EXE] - send the symbol to the PC, [NXT] - next page of symbols, and [ESC]
go back to the main Config layout.

The [M6] key section in the code has a few examples of using Alt + Number Keypad for Maths and special symbols or 
characters. There are 2 ways to do it - the first is to type the character's Unicode (hex without the 0x or decimal) into 
MSWord and then press [ALT] + [x]. The second method is to hold the Alt key down, then type in a special code number using
the number keypad and then release the Alt key. There are examples of entering the open infinity symbol and the small pi 
synbol in the [M6] key section. 

Numeric Keypad    [Bsp] [7] [8] [9]  Press the small SktBlue Pad on the Right to toggle the Number Keypad on/off.
                  [Ret] [4] [5] [6]  Also show the numeric keypad by sending macro *kb from the built-in Keyboard       
                  [ 0 ] [1] [2] [3]  

Power Restart KeyPad    [Restart  Long Timer] [ Stop ] [PowerOff  Long Timer]        [R-T] [Stp] [O-T] [ ]   
                        [Restart Short Timer] [  Cfg ] [PowerOff Short Timer]        [R-t] [Cfg] [O-t] [ ]       
                        [Restart   Immediate] [Logoff] [PowerOff    Imediate]        [Rst] [Log] [Off] [ ]
 
(1) Default time values: Short Time = 30 seconds Long Time 600 seconds (10 minutes). Change Time values send the macro with
the built-in keyboard for PowerOff *ot*num or *oT*num or Restart *rt*num or *rT*num where num 0 = 6000 second (100 minutes)
1 = 30 second (x100 for T) to 9 = 300 second (x100 for T).
(2) Send the macro *po* with the built-in keyboard to toggle the Power Keys Option to use the Menu (GUI+X + i,u,r or the 
GUI+R + shutdown + options command.
(3) To cancel a timed shutdown press the [Stop] key. To exit the Power Restart Keypad press any of the [black] non-
functional keys or press [Cfg][Rof] again.
```

<p align="left">
<img src="images/picE.jpg" height="200" /> 
<img src="images/picF.jpg" height="200" /> 
<img src="images/picA.jpg" height="200" /> 
</p>

Another use of the two main layouts could be to have one customised for Linux - although all the keys in layout 1 and 2 except the run dialog, and the powershell and command prompt, function the same under Linux.

The list of key names that can be used is in TinyUSB stack's hid.h, as included here. Use the configuration descrition as shown below to set up the TFT_eSPI driver. The sampled calibration data can be adjusted - see the comments in the program for instructions. Compile with the USB stack set to TinyUSB not PicoSDK. 

STL files for the case - based on [**Customizable LCD Box**](https://www.thingiverse.com/thing:57427), are in the STL folder. To protect the LCD resistive film use a sheet of laser print transparent film cut to the right size for the LCD.

*In contrast, this [**Pico mbed RP2040 volume and macro keyboard**](https://www.hackster.io/Murchx/pi-pico-macro-keyboard-d0bd1c) uses the Arduino mbed-based RP2040 board definitions (instead of the earlephilhower version). The USB Keyboard supports both normal and consumer keys using only one include **#include <USBKeyboard.h>**. The article is [**here as a pdf file**](Pi-Pico-mbed-RP2040-volume-and-macro-keyboard.pdf). The code was tested using push-buttons on both Arduino 2.0.0-rc9.2 and 1.8.19 IDE's, and was working (could adjust the volume up or down). [**This discussion**](https://github.com/Bodmer/TFT_eSPI/discussions/1558) notes that the Pico mbed Arduino board does not offer touch support when used with the TFT_eSPI drivers.*

### LCD ILI9486 480x320 Resistive Touch SPI driver

Use the [**Waveshare ILI9486**](https://www.waveshare.com/4inch-tft-touch-shield.htm) 4inch Touch LCD Shield for Arduino [**480x320 4 inch LCD**](https://www.waveshare.com/wiki/4inch_TFT_Touch_Shield), connected as shown below in the TFT_eSPI User_Setup.h. *Note that the numbers such as MISO 11 and MOSI 12 are Pico GPIO numbers and not Pin numbers - connect the LCD Mosi to the Pico GPIO 11 which is the Pico Pin 15.*

Arduino support is through the [**Bodmer TFT_eSPI graphics library**](https://github.com/Bodmer/TFT_eSPI) and the [**Earlephilhower Arduino Pico Port**](https://github.com/earlephilhower/arduino-pico/), and the Adafruit GFX library (for some of the button classes). XPT2046 touch screen controller support is for SPI based displays only.

**The install order on Windows 10x64**
1. [**Arduino IDE**](https://www.arduino.cc/en/software) version 1.8.19.
2. [**Earlephilhower Aduino Pico port**](https://github.com/earlephilhower/arduino-pico/)
3. [**Adafruit GFX library**](https://github.com/adafruit/Adafruit-GFX-Library).
4. [**Bodmer TFT_eSPI graphics library**](https://github.com/Bodmer/TFT_eSPI).
5. [**Adafruit TinyUSB library**](https://github.com/adafruit/Adafruit_TinyUSB_Arduino)


Specify the display and touch gpio in User_Setup.h (included [**here**](User_Setup.h)).


```

#define RPI_DISPLAY_TYPE // 20MHz maximum SPI works with waveshare serial to parallel displays

#define ILI9486_DRIVER

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

#define SPI_FREQUENCY        24000000
#define SPI_TOUCH_FREQUENCY   2500000
#define SPI_READ_FREQUENCY   10000000

```
