# Pico Volume and Macro Touch Keyboard 480x320

<p align="left">
<img src="images/1.jpg" height="240" />   
<img src="images/2.jpg" height="240" /> 
<img src="images/3.jpg" height="240" /> 
</p>

Use the [**Waveshare Pico Evaluation board**](https://www.waveshare.com/pico-eval-board.htm) Pico-Evaluation-Board [**480x320 3.5 inch IPS LCD**](https://www.waveshare.com/wiki/Pico-Eval-Board), connected as shown [**here**](images/connections.jpg). 

This has the following components: 
```
Second Micro-USB connector: CP2102 communication or battery recharge
CP2102 USB to UART converter
ETA6096 battery recharger, 1A recharging current, PH2.0 battery header for connecting 3.7V Li-po battery
SP0503 diode for protecting the chip from ESD
PCM5101A stereo audio DAC chip supports analog audio output to 3.5mm audio jack headphone interface
Micro SD card slot: using SDIO interface with faster access speed
W2812B RGB LED
Indicators PWR: power indicator, TXD: CP2102 UART TX, RXD: CP2102 UART RX
Passive buzzer with controllable sound frequency
Photoresistor converting light intensity to voltage
ICM20948 9-axis motion sensor, allows posture sensing
User-defined key, connects to GP2
Reset key for Pico reset
XPT2046 resistive touch controller
LSD0204 voltage level translator
ILI9488 LCD controller via 74HC4040, 74HC04D, 74HC4094
```

The [**Raspberry Pi Pico Touch Macro Keyboard by Dustin Watts**](https://github.com/DustinWatts/Pico-Matrix-Touch-Keyboard) - the leftmost picture below, can be used unchanged on this display, provided the TFT_eSPI LCD driver is configured as shown at the bottom below - the sketch code and uf2 file are included in here (TouchMacro1.ino and TouchMacro1.uf2). If the calibration colours are unclear check that #define TFT_INVERSION_ON in User_Setup.h has been uncommented. Remember to choose the PicoSDK as the USB stack. 

[**VolumeMacroPad4.ino**](VolumeMacroPad4.ino) is a combination of the [**Dustin Watts Pico Touch Macro Keyboard**](https://github.com/DustinWatts/Pico-Matrix-Touch-Keyboard), the [**AdafruitTinyUSB HID examples such as hid_composite.ino**](https://github.com/adafruit/Adafruit_TinyUSB_Arduino/blob/master/examples/HID/hid_composite/hid_composite.ino), and the [**Bodmer Keypad example**](https://github.com/Bodmer/TFT_eSPI/tree/master/examples) Keypad_480x320.ino. It was adapted for use on a Waveshare evaluation board with an ILI9488 480x320 3.5 inch Touch LCD. It also replaced the PicoSDK USB stack with the Adafruit TinyUSB stack - this allowed consumer keys such as Volume Up-Down-Mute to be added to the standard keyboard (touch) keys. 

This sketch includes macros for sending large text sections - press [5] and then [6], or [5] and then [M2], and it sets the Neopixel RGB LED red to reflect the state of Caps-Lock, and adds control of the backlight to dim the display if not used. 
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
(1) LCD blank timeout: Send the macro *tb*num with the built-in keyboard where num = 0,1-9 - *tb*0 = 120 hours, 
*tb*1 = 30 seconds.  
(2) LCD blank dim value: Send the macro *db*num with the built-in keyboard where num = 0,1-9 - *db*0 = no
dimmming - just on/off after blank timeout, and 1-9 = 3 to 30 % PWM instead of blank. 
(3) LCD screen brightness: Send the macro *bb*num with the built-in keyboard where num = 0,1-9 - *bb*0 = full
brightness and 1-9 = 3 to 50 % PWM brightness.
(4) Power Keys menu/Command Option: Send the  macro *po* with the built-in keyboard to toggle the Power Keys Option
to use the Menu (GUI+X + i,u,r or the GUI+R + shutdown + options command.
(5) Power Keys time values: Default Short Time = 30 seconds and Default Long Time 600 seconds (10 minutes). To change 
Time values send PowerOff *ot*num or *oT*num or Restart *rt*num or *rT*num where num 0 = 6000 second (100 minutes)
1 = 30 second (x100 for T) to 9 = 300 second (x100 for T).
(6) Calibration Enable: Send the macro *ca* with the built-in keyboard to set/clear re-calibration option after a 
restart to ON or OFF.
(7) LCD Orientation 180 degree change: Send the macro *ro* with the built-in keyboard - the USB cable will then 
be on the opposite side.

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
 
To cancel a timed shutdown press the [Stop] key. To exit the Power Restart Keypad press any of the [black] non-
functional keys or press [Cfg][ROf] again.                       
```

Use the configuration description as shown below to set up the TFT_eSPI driver. Compile with the USB stack set to TinyUSB not PicoSDK.

Arduino support is through the [**Bodmer TFT_eSPI graphics library**](https://github.com/Bodmer/TFT_eSPI) and the [**Earlephilhower Arduino Pico Port**](https://github.com/earlephilhower/arduino-pico/). XPT2046 touch screen controller support is for SPI based displays only.

<p align="left">
<img src="images/5.jpg" height="240" />   
<img src="images/lcd30.jpg" height="240" /> 
<img src="images/lcd31.jpg" height="240" /> 
</p>

**The install order on Windows 10x64**
1. [**Arduino IDE**](https://www.arduino.cc/en/software) version 1.8.19.
2. [**Earlephilhower Aduino Pico port**](https://github.com/earlephilhower/arduino-pico/)
3. [**Adafruit GFX library**](https://github.com/adafruit/Adafruit-GFX-Library).
4. [**Bodmer TFT_eSPI graphics library**](https://github.com/Bodmer/TFT_eSPI).
5. [**Adafruit TinyUSB library**](https://github.com/adafruit/Adafruit_TinyUSB_Arduino)
6. [**Adafruit Neopixel library**](https://github.com/adafruit/Adafruit_NeoPixel)

Specify the display and touch gpio in User_Setup.h (included [**here**](User_Setup.h)).

```
#define RPI_DISPLAY_TYPE // 20MHz maximum SPI works with waveshare serial to parallel displays

#define ILI9488_DRIVER   //

#define TFT_INVERSION_ON

#define TFT_BL   13            // LED back-light control pin
#define TFT_BACKLIGHT_ON HIGH  // Level to turn ON back-light (HIGH or LOW)

#define TFT_MISO 12
#define TFT_MOSI 11
#define TFT_SCLK 10
#define TFT_CS   9     // Chip select control pin
#define TFT_DC   8     // Data Command control pin
#define TFT_RST  15    // Reset pin (could connect to RST pin)

#define TOUCH_CS 16    // Chip select pin (T_CS) of touch screen

#define SPI_TOUCH_FREQUENCY   2500000
#define SPI_READ_FREQUENCY    8000000
#define SPI_FREQUENCY         14000000

#define TFT_SPI_PORT 1 // Set to 0 if SPI0 pins are used, or 1 if spi1 pins used

#define SUPPORT_TRANSACTIONS
```

<p align="left">
<img src="images/lcd10.jpg" height="350" /> 
<img src="images/lcd9.jpg" height="350" /> 
</p>

Note 1: The RPI Display Type is selected - [**see the comment here**](https://github.com/moononournation/Arduino_GFX/issues/151): *"ILI9488 configured in 16 bit parallel mode with a complicated spi to parallel bus converter using 2 74hc4094 chips, a 74hc4040 and a 74hc04d."* It is also described at the Note 2 URL as: *"The ILI9486 RPi display must be of the Waveshare design and use a 16 bit serial interface based on the 74HC04, 74HC4040 and 2 x 74HC4094 logic chips."*

Note 2: The SPI frequency settings are conservative and has a slowish screen redraw.

Note 3: A suitable user_setup.h for this board is [**shown here**](https://github.com/Bodmer/TFT_eSPI/discussions/1554?sort=new) towards the end.

Note 4: This [**discussion**](https://forum.pjrc.com/threads/59334-3-5-quot-display-using-ILI9488-will-not-work-with-XPT2048_Touchscreen) examines problems with the ILI9488 chipset sharing the SPI MISO bus. Also see the [**TFTeSPI Hardware modification**](https://github.com/Bodmer/TFT_eSPI/tree/master/docs/RPi_TFT_connections) of Raspberry Pi type LCDs. Refer to [**this guide**](https://www.pjrc.com/better-spi-bus-design-in-3-steps/) for better SPI shared bus design.

Note 5: The Windows 10 SDK setup as [**previously described**](https://github.com/TobiasVanDyk/Pico-MCU-from-Raspberry-Pi/blob/main/Install-PicoSDK-in-Windows10x64-July2022.pdf), can be used to compile the waveshare examples for the this board - [**instructions are here**](WavePicoEvaluationBoard.pdf)


