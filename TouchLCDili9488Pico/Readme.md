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

[**VolumeMacroPad.ino**](VolumeMacroPad01.ino) is a combination of the [**Dustin Watts Pico Touch Macro Keyboard**](https://github.com/DustinWatts/Pico-Matrix-Touch-Keyboard), the [**AdafruitTinyUSB HID examples such as hid_composite.ino**](https://github.com/adafruit/Adafruit_TinyUSB_Arduino/blob/master/examples/HID/hid_composite/hid_composite.ino), and the [**Bodmer Keypad example**](https://github.com/Bodmer/TFT_eSPI/tree/master/examples) Keypad_480x320.ino. It was adapted for use on a Waveshare evaluation board with an ILI9488 480x320 3.5 inch Touch LCD. It also replaced the PicoSDK USB stack with the Adafruit TinyUSB stack - this allowed consumer keys such as Volume Up-Down-Mute to be added to the standard keyboard (touch) keys. 

VolumeMacroPad includes the means to execute a number of Math symbols and Windows or Linux key macros - refer to the layouts in the diagram below. It sets the Pico LED (and shows a small green "C" or "N" or "S") on the LCD, to reflect the state of the Caps-Num-Scroll-lock keys, and adds control of the LCD backlight to dim the display if not used, or set the normal use brightness. It enables a re-calibration of the touch interface, and store it as a preset, and has four layout modes (L1 L2 L3 L4) and two layers A/B, - switch layouts by pressing [L1][L2][L3][L4] or change layers with the the Volume Mute [Vo] key when in Media Key mode. Switch Layers A/B via the Cfg key + ArrowLeft [A-B] key. It also has a 4 page, 108 Math symbols keyboard, which send the standard Unicode symbol characters to the PC. This [**works on Linux Mint with LibreOffice**](images/Libreoffice1.jpg), as well as MSOffice, and use a superset of the key codes from [**Mathboard**](https://github.com/nup002/Mathboard) - refer to the content of **unicode_symbols.h** there. Note that you do not need additional software running on a Windows PC except MS Office.

Some of the layout, layer, keyboard, and configuration, modes are shown below in the pictures. The pictures shows a page from the Math Keyboard, and Layout 2 - configuration and navigation. Later pictures show the Caps-Num-Sroll-Lock key states as on as shown by the green C, N and S Letters. It is possible to send new text (or control character strings, up to 200 characters long (easily made longer in the program code), to be assigned to keys S1/T1 through to S12/T12 (or M1 - M12), via the Pico's serial USB - start each string with <1 to <6 followed by the string itself to be assigned to the keys - add a > at the end of the string. These strings are saved to the Pico's Flash memory using LittleFS. To enable the string update for layout 4 the screen must be in layout 4, for layout 3 it can be in layout 1, 2 or 3. (For the [M4] key in layout 1 use a <0 at the start of the string.) For each of the two layers A/B the upload will be to the layer that is current i.e. to keys S7 to S12 if layer B is current, and to S1 to S6 if layer A is current. A key auto-repeat that triggers after 500 milliseconds is also implemented - this can be used to instead of typing a character repeatedly, or changing the volume by just holding down the Volume Up/Down keys.

Other pictures below, shows the set of seven Media keys, and Layout 1 Layer A [M1] to [M6] programmable macro keys (Layer B would show the Mx keys as M7 to M12). The Volume Mute key is replaced by L1 to L4 in the Config Menu options. It enables faster layout 1 to 4 switching by disabling/enabling the Volume Mute action by pressing the Config and Home keys in sequence. Repeat the same sequence to enable Mute again. The label on the [Vo] key will change to indicate which layout is active L1, L2, L3, or L4. If PageUp is pressed after Config i.e. the [BsD]key, the VolumeUp and VolumeDown keys are replaced by a Delete/Backspace and an Enter key. Press [Cfg] + {PgU] again to switch back to the Volume Up and Down keys. If PageDown is pressed after Cfg the startup Layout is toggled between L1 and L2, i.e. the [S112] key. If End is pressed after the Cfg key all the text files and config settings are deleted from the Pico's Flash memory, and on restart a re-calibration is done. Note that to save the VolMute/L1-L2 and Vol+Vol-/Delete-Return choices on the Flash memory you have to press Cfg [Sav] key, once more after selecting these two options. This is not necessary for the L1/L2 startup selection. 

Press the small Green Pad on the Right to toggle the Media KeyPad on/off, press the small Red Pad to toggle the Number Keypad on/off, and press the small Blue Pad to toggle the Keyboard (5 sub-pages), on/off.

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
Layout 1 - M Keys - [M1]-[M12] - Cycle through Layout 1 to 4 press [L1-L4] or [Vo] 
------------------------------------------------------------------------------------------------
[Cut CtrlX] [ Copy CtrlC] [Paste CtrlV] [VolUp DelBspc]   [  Cut ] [  Cpy ] [  Pst ] [V+][ BsD ]
[AltPrtScr] [ Adm Pshell] [    Run    ] [VolMute L1-L4]   [M1  M7] [M2  M8] [M3  M9] [Vo][L1-L4]
[SendText ] [   Adm CMD ] [  TaskMan  ] [VolDwn  Enter]   [M4 M10] [M5 M11] [M6 M12] [V-][ Ret ]
          Caps          Num          Scroll                       C        N        S

Layout 2- Config - Cycle through Layout 1 to 4 press [L1-L4] or [Vo] 
--------------------------------------------------------------------------------------------------
[Home Stop   Mute] [UpArr BsDel] [PgeUp     DelRet] [VolUp  Delete] [Hme] [Up ] [Pgu] [V+][DelBs]
[ArrL Prev    A-B] [Config Save] [ArrR  Nxt MacTim] [VolMute L1-L4] [  <] [Cfg] [>  ] [Vo][L1-L4]
[End  PlayP Media] [DwnArr POff] [PgeDwn StartL1L2] [VolMute Enter] [End] [Dwn] [PgD] [V-][Enter]
     Macro   Src SD Dest      SrcNum            DestNum                 S-D   Num   Num
                Caps           Num               Scroll                  C     N     S

Layout 2 (Config) has five additional small pad-buttons on the far right side:
Red     Press to toggle the Math KeyPad on/off.
Blue    Press to toggle the Macro Composition Keyboard on/off.
Green   Press to toggle the Media Keypad on/off.
SkyBlue Press to toggle the Number Keypad on/off.
Yellow  Options Pad: KeyBrd Mode - Direct Mode On/Off (Blue "D" indicator).
                     NumPad Mode - Switch between 3 NumPad pages. 

Layout 3 - S Keys - [S1]-[S12] - Cycle through Layout 1 to 4 press [L1-L4] or [Vo] 
Layout 4 - T Keys - [T1]-[T12] - Cycle through Layout 1 to 4 press [L1-L4] or [Vo] 
--------------------------------------------------------------------------------------------------
[Cut Ctrl+X ] [Copy Ctrl+C] [Paste Ctrl+V] [VolUp DelBspc]   [  cX ] [ cC  ] [ cV  ] [V+][ BsD ]
[Send Text 1] [Send Text 2] [Send Text 3 ] [VolMute L1-L4]   [S1 T1] [S2 T2] [S3 T3] [Vo][L1-L4]
[Send Text 4] [Send Text 5] [Send Text 6 ] [VolMute Enter]   [S4 T4] [S5 T5] [S6 T6] [V-][ Ret ]
           Caps           Num           Scroll                      C       N       S

-------------------------------------------------------------------------------------------------
Layouts 1, 3 and 4, two Layers A/B each with A=M1-M6, S1-S6, T1-T6 and B=M7-M12, S7-S12, T7-T12
Press VolumeMute consecutively within 5 seconds else count is reset to 0 or press [L1-L4] once.
[L1-L4] has a repeat period of 0.5 seconds - after this key repeat is active

Press Config Key (new set of config keys change colour):
L14 (Home) - VolumeMute -> L1, L2, L3, L4 - repeat to restore Vo
BsD (ArrowUp) - Toggle Vol+ key to either Delete or Backspace
Vol (PageUp) - Vol+ -> Delete and Vol- -> Enter - repeat to restore V+ V-
A-B (ArrowLeft) - Layouts 1, 3, 4, change to Layer A or Layer B 
Med (End) - Change Layout 2 to Media Controls Previous-Next-PlayPause-Stop
McT (ArrowRight) - Macro Timer Trigger Panel Repeat and Onshot or Clock Timers.
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

Keyboard Page 1             Page 2          Page 3                 Page 4                Page 5
[abc] [def] [ghi] [EXE]  [ABC]-[XY_]  [012] [345] [678]   [Lst] [Ren] [Rmv] [Snd]   [ALT] [SHF] [CTR] 
[jkl] [mno] [pqr] [NXT]   Uppercase   [9+-] [/=*] [*Cm]   [Snd] [Cpy] [Lnk] [NXT]   [GUI] [TEI] [CRF] 
[stu] [vwx] [yz ] [ADD]     Page 1    [Sym] [Brc] [Fnn]   [Src] [Dst] [Num] [Sav]   [LHR] [UED] [UND]  
                                                              S-D   Snn   Dnn
                                              Source-Destination Source-num-Destination 
                                                     Macros Selection (MS 01 06 etc)
Page 1: [xy ] = x y space
Page 2: [XY_] = X Y underscore  Page 1 and 2 + Caplock reverse characters
Page 3: [Fnn] F1-F24  [Sym] 17 symbols 
        [Brc] 8 bracket symbols  
        [*Cm] Insert one of 15 * Star codes - do not press [ADD] but if required press Number[ADD][EXE]

Page 4: Macro Tools                                 Page 5: Modifiers 
[Src] Macro Source M S T or A(ll) = S in SD         [ALT] = [ Alt-L ]  [  Alt-R ]  [ PrintScr] [EXE]
[Dst] Macro Destination M S T     = D in SD         [SHF] = [Shift-L]  [Shift-R ]  [ Delete  ] 
[Num] Macro Number M S T = 1-12 A = 1-99 = Sn,Dn    [CTR] = [Contr-L]  [Contrl-R]  [BackSpace]  
[Cpy] Copy [Src][Num] to [Dst][Num] Macro           [GUI] = [ Win-L ]  [ Win-R  ]  [  NULL   ] [NXT]
[Rmv] Remove [Src][Num] Macro                       [TEI] = [  Tab  ]  [ Escape ]  [ Insert  ]  
[Ren] Rename current [Src][Num] Macro               [CRF] = [  C/R  ]  [   L/F  ]  [  Return ]   
[Lnk] Link A-Src[Num]+A-Dst[Num] Macro              [LHR] = [Arrow-L]  [  Home  ]  [ Arrow-R ] [ADD] 
      Then assoc with [Src]NumDisplayed             [UED] = [ArrowUp]  [  End   ]  [Arrow-Dwn]  
[Lst] List first 8 bytes contents of source macro   [UND] = [PageUp ]  [ Numlock]  [ PageDwn ]
[Snd] Send Source Macro directly 
[Sav] Save Source Macro to Flash (if not saved)
[Tmr] Macro Timer options (One-shot or Repeat)

Note: To choose between Macro A = 1 to 99 does not require 99 key-presses - just hold the [Num] key down for 
key-repeat - it cycles through 1-99 in about 5 seconds.
                                                                          
The keyboard has 5 pages - most with 9 triple function keys, and 3 control keys [EXE] [NXT] [ADD]. For example
page 1 has keys [abc], [def], to [y,z,space]. To select a or b or c press the abc key once, twice or thrice - 
to add it to a new macro press the ADD key else press another character-symbol modifier key or press [NXT] for
the next keyboard page. 

Press Pad 2 again to leave the keyboard or press [EXE] to send the macro to the PC and save to the SD 
(Source-Destination) Macro Key as indicated by the second number 1 to 12. Change the target number by pressing
the [Dst] then the [Num] key. The macro is saved when the Up-Arrow key is pressed after the [EXE] key. Refer
to the examples below on how to copy commands between the KeyBrd macros (A with 1-99 files), and the three 
sets of 12 keys M, S, and T.

The macros on page 5 are modifiers (simultaneously pressed keys) such as Control + Alt + Delete + GUI (maximum 
of 6 keys in macro). To send this sequence press [CTR][ADD][ALT][ADD][SHF]x3 (3 times for delete)[ADD][EXE]. 

To open the windows snipping tool press [GUI][ADD][SHF][ADD][NXT][s][ADD][EXE] - the snipping tool will open. 
This can then be saved to [M2] by pressing the [Up] button. 

To send a sequence such as [WinKey] + "r" press [GUI][ADD][NXT][pqr]x3[ADD][EXE]. To assign Alt + PrintScr to
key [M2] press [NXT]4x[ALT][ADD][ALT]3x[ADD][EXE} and [Up] to save. Alt + PrtScr is then executed when pressing
[M2]. 

Another example: [GUI][ADD][NXT]1x[r][EXE][Up] then [n][ADD][o][ADD][t][ADD][e][ADD][p][ADD][a][ADD][d][ADD]
[NXT]3x[CRF][ADD][NXT][h][ADD][e][ADD][l][ADD][l][ADD][e][ADD][EXE][Up]. After this press just two keys 
[M2][M1] to open notepad and insert the text "hello". To make the text larger [CTR][ADD][NXT]3x[9+-]2x to make
the text larger.

Toggle the sticky keys: Press [SHF][ADD] 5 times then press [EXE] and save to key [M2]. (For a complete 
macro add [C/R] [ADD] key at the end of this sequence.) 

The option to use the combined modifier bit instead of a modifier byte, is used in the top row Cut-Copy-Paste 
keys, and also for some of the pre-programmed examples for the M1-M12 keys. 

Macros sent to the PC from the built-in keyboard will be saved to file MacroX X=1-99, and if selected, 
assigned to key [MX]. Press [Up] after sending the macros to the PC to assign them to [MX]key. 

To set it up at first (all red SD SrcNum DstNum) goto page 4 and press [Src] once then [Dst] - SD will now show 
MM 01 01 i.e. the source and destination is M1 and M1. 

Note: The [Cpy] key on page 4 is now the most direct way to copy the [Src][Num] to [Dst][Num] Macro.
      The [*Cm] key cycles through all available star codes and insert it directly (no [ADD] required except if
      the *code* is followed by numbers then press [ADD] after inserting the number). For example press [*Cm]
      until *bb* is displayed then press then press [678][ADD][EXE] to set the LCD Brightness to 25 percent.
      
Example 1: Set up MM 01 01 as SD SrcNum DestNum - during same session press [CTR][SHF][Esc][EXE][UP] - save to M01
           Can check in Terminal File list has Macro01 4bytes MtrData1 4bytes (only autosave for 01)
           Press [M1] and TaskMan opens
Example 2: Set up MM 01 02 - press [GUI][r][EXE][Up] - save to M02 - filelist only has Macro02 3bytes
           Must therefore copy it to M02
           Set up AM 02 02 - then [Cpy] - file list has MtrData2 4 (added 0x00) press [M2] open run window
Example 3: Set up AS 02 01 - then [Cpy] - file list has StrData1 4 press [S1] open run command window
Example 4: Set up AS 01 02 - then [Cpy] - file list has StrData2 5 press [S2] open TaskMan
Example 5: Set up MM 04 04 - press [GUI][r][EXE][Up] - press [M4] to open run window
           Set up MS 04 04 - then [Cpy] - press [S2] open run windows 
Example 6: Set up AT 04 07 - Rename Macro04 (must exist) to TtrData7 - press [Ren] - then press [T7]

Linking Macros Examples:

Example 1: Program M1 with a Open Run windows [GUI][r] and [M2] with a notepad+C/R
           Set up MM 01 02 - Press Link [Lnk], then press [M1] it opens the run window, then runs notepad.
Example 2: Program M1 with a Open Run windows [GUI][r] and [M4] with a [Ctr+Shft+Esc
           Set up MM 04 01 - Press Link [Lnk], then press [M1] it open TaskMan, then opens the run window.
Example 3: Program M1 M2 and M4 as in example 7 and example 8 - open Run window, notepad+C/R and open TaskMan
           Set up MM 01 02 - Press Link [Lnk], then press [M1] it opens run window and notepad. Then add
           a 3rd link Set up MM 01 04, press [Lnk] - then press [M1] it opens notepad via run window and TaskMan
Example 4: Program M1 and M4 (GUI+r and Ctr+Shf+Esc) i.e. MM 01 04. Then go to the numbers page and add 5 i.e. press
           [345[3x[ADD] then goto the Macro Tools page where MM 01 04 is still visible and press [Lnk]. Key [M5]
           will then Open the Run window, and then also open the TaskMan.           
            
Note 1: For linking example 3 before linking there must be three files Macro01 (3 bytes), Macro02 (9 bytes),
        and Macro04 (4 bytes) - press [Cfg] to an open serial monitor to check. There are other linking options
        when numbers have been added through [ADD] (see example 4 above), but for examples 1 to 3 use [Lnk] on 
        a clean display i.e. the status will only display "Destination Macro Number", or "Source Macro" or 
        "Destinatio Macro" before pressing [Lnk].      
Note 2: To unlink send *ul* with the Macro Key to be unlinked visible as the Source Macro such a Mx mm xx.
Note 3: Linked Macro Data will be lost after a power cycle or reset inless the save-restore option is turned on
        by executing an *lr* command - then also press the black [Cfg] button twice to save the option to flash.
        An *ld* will send a Link and Macro datadump to the serial port.
Note 4: Use Source = Destination then press [Lnk] - if the intention is to repeat the same macro more than once.
Note 3: Pressing [Cpy] is the same as *cm* [EXE]. Chaining macros have been implemented for [M1] to [M12].

F1-F24 keys are all one key [Fnn] and are sent as keycodes (simultaneous) and not keypress types - to send [F3] 
open the macro keyboard then press [NXT]2x[Fnn]3x[ADD][EXE]. Press [Up] to assign it to a [Dst][Num] key.

The Keyboard has a Direct (to PC) Mode - use the Yellow Options Pad on the bottom right, to switch Direct Mode On/Off.
A Blue "D" indicator will show if it is on. Any character selected (shows in status bar), will be sent to the PC by 
pressing [EXE] - [ADD] is not necessary. If a character or more than one characters have been [ADD]ed they will only 
be sent after Direct Mode is switched off.

The Keyboard can be used to change various options by sending *option*value commands. The [*Cm] key can also be used 
and then pressing the [Add] key after the last * is not necessary:
(a) Macro Timer Keys time values - default Short Time = 30 seconds and default Long Time 10 minutes. To change Timer
    values send Repeat Macro Timer *mt*num or *mT*num or Oneshot Macro Timer *nt*num or *nT*num where num 0 is
    3 hours t time and 300 hours T time mode. 1 = 30 seconds (2 hours for T) to 9 = 1 hour (48 hours for T). Associate 
    a Macro with a Timer in the KeyBrd Macro Tools Page using key [Tmr] after 1 2 3 4 have been [ADD]ed to information
    line at the bottom. Then the Source Number Macro will be linked to (1) and (2) Repeat-Timer T and t, (3) and (4)
    Oneshot-Timer T and t. Ensure that the matching key on the timer keybrd is ppressed i.e. if you used 2 then press 
    key [R-t] else the timer will not fire.
(b) LCD blank timeout - Send the macro *tb*num with the built-in keyboard where num = 0,1-9 - *tb*0 = 120 hours, 
    *tb*1 = 30 seconds.  
(c) LCD blank dim value - Send the macro *db*num with the built-in keyboard where num = 0,1-9 - *db*0 = no dimming
    - just on/off after blank timeout, and 1-9 = 3 to 30 % PWM instead of blank. 
(d) LCD screen brightness - Send the macro *bb*num with the built-in keyboard where num = 0,1-9 - *bb*0 = full
    brightness and 1-9 = 3 to 50 % PWM brightness.
(e) Power Keys Menu/Command Option - Send the macro *po* with the built-in keyboard to toggle the Power Keys Option
    to use the Menu (GUI+X + U + i,u,r or the run command GUI+R + shutdown + options command.
(f) Power Keys time values - Default Short Time = 30 seconds and Default Long Time 600 seconds (10 minutes). To change 
    Time values send PowerOff *ot*num or *oT*num or Restart *rt*num or *rT*num where num 0 = 6000 second (100 minutes)
    1 = 30 second (x100 for T) to 9 = 300 second (x100 for T).
(g) Calibration Enable/Disable - Send the macro *ca* with the built-in keyboard to set/clear re-calibration option after 
    a restart to ON or OFF. 
(h) LCD Orientation 180 degree change - Send the macro *ro* with the built-in keyboard - the USB cable will then be on
    the opposite side. This forces a re-calibration on restart. 
(i) Button Font Bold/Normal change - Send the macro *fo* with the built-in keyboard to turn the labels on the buttons from
    normal to bold.
(j) Macro Copy - Copy macro01-99 to M,S,T keys. Can use *cm* if the SD SrcNum DstNum is set up - see the four examples 
    above. Else compose *cm*nnXmm via [ADD] where: nn = macro01-macro99 X = Keys M S T mm = 01-12
(k) Macro Unlink - unlink *ul* with the Macro Key to be unlinked visible as the Source Macro such a Mx mm xx.
(l) Macro Link Save-Restore On-Off - Linked Macro Data will be lost after a power cycle or reset unless the save-restore 
    option is turned on by executing *lr* command - then also press the black [Cfg] button twice to save the values to 
    flash. An *ld* will send a Link and Macro datadump to the serial port.
(m) To fill M S T 1-12 with hard-coded text string examples send *fm* *fs* *ft* or *fa* (all three) commands.
(n) To delete all macro and config files use "*de*" - will re-calibrate on restart. Remove macro files with the [Rmv] or 
    rename with [Ren] keys.

Math-Greek-Algebra Keyboard:
This is a triple-key macro keyboard with 4 pages and 4 x 9 x 3 = 108 math and Greek algebra
symbols. It is sent to the PC as (hex) Unicode + [ALT] + [x] and can be directly entered into MSWord and in LibreOffice
(also on Linux). The three control keys are [EXE] - send the symbol to the PC, [NXT] - next page of symbols, and [ESC]
go back to the main Config layout - or press the small red pad again to leave the Math keyboard. Watch out for MSWord 
insists on capitalising the first letter of a sentence - which will change the first math symbol in a sentence/line 
after a second symbol or letter or enter is typed. 

The [M6] key section in the code has a few examples of using Alt + Number Keypad for Maths and special symbols or 
characters. There are 2 ways to do it - the first is to type the character's Unicode (hex without the 0x or decimal) into 
MSWord and then press [ALT] + [x]. The second method is to hold the Alt key down, then type in a special code number using
the number keypad and then release the Alt key. There are examples of entering the open infinity symbol and the small pi 
symbol in the [M6] key section. 

Numeric Keypad    [ BackSpc] [ 7 Spc aA ] [ 8 % bB ] [ 9 xX cC ]  Press 4th Pad - toggle the Number Keypad on/off.
                  [ Return ] [ 4  ,  dD ] [ 5 . eE ] [ 6 =  fF ]  Press 5th Pad - switch Num Pages 1-3 - CapsLock a-A       
                  [ 0  + ( ] [ 1  -   ) ] [ 2 *  [ ] [ 3 /   ] ]  Macro *kb from the built-in Keyboard show keybrd

Power Restart KeyPad    [Restart  Long Timer] [ Stop ] [PowerOff  Long Timer]       [R-T] [Stp] [O-T] [ ]   
                        [Restart Short Timer] [ Cfg  ] [PowerOff Short Timer]       [R-t] [Cfg] [O-t] [ ]       
                        [Restart   Immediate] [Logoff] [PowerOff    Imediate]       [Rst] [Log] [Off] [ ]
                        
Macro Timers KeyPad     [Repeat  Long Timer] [  Stop  ] [Onceof  Long Timer]        [R-T] [Stp] [O-T] [ ]   
                        [Repeat Short Timer] [   Cfg  ] [Onceof Short Timer]        [R-t] [Cfg] [O-t] [ ]       
                        [Repeat  Clock Time] [Repeat x] [Onceof  Clock Time]        [R-C] [Rep] [O-C] [ ]
  
To cancel a timed shutdown press the [Stop] key. To exit the Macro Timer or the Power Restart Keypad press any of the
[black] keys or press [Cfg][McT] again. This is also a way to check if any of the timers are running - press [McT] then
press one of the black keys to exit - the display should indicate if a Macro Timer is running or not.

Macro Timer Example:

Program key [M2] with the text hello - In the KeyBrd press [Src] and [Num] until the first letters show M and 02 - then 
press [mno]2x[ADD] etc. Press [EXE] then [Up] to save the text to key M2]. Enter the KeyBrd and if required again set up 
the source as M 02 (i.e. Mx 02 xx where xx is anything. [ADD] a number 1 to 4 - for example press [012]2x[ADD]. Then press 
[NXT][Tmr] It will display a message that Macro Timer M 02 has been set up for Repeat Timer T. If 2 or 3 or 4 had been 
added then the timer used would be Repeat t, Oneshot T, and Oneshot t where t = short time , and T = Long Time. 
Exit the KeyBrd and press the black [Cfg] then [McT] and then make sure to press the same button that have been set 
up - in this case [R-T]. Open notepad and wait for the repeating hello to appear.

Change the timer values with *mt*x *mT*x *nt*x *nT*x where x = 0-9 - you can use the [*Cm] key and [ADD] one number 0-9
then press [EXE].

Currently only single macros from M, S and T 1-12 can been used for the timers - if a chained (linked) macro is used 
it will run the last macro used by the chain. The Timers are programmed as Time-Fire-Time-Fire. There will be an option
later to change this to Fire-Time-Fire-Time for the Repeat timers. The two real-time (Clock) Timers will be 
implemented once a satisfactory task scheduler is found for sending time from the PC to the Pico. (This could also be 
done via a special Capslock-Numlock sequence from the PC to the Pico.) It may also be possible to simulate a real-time
clock on the Pico's only HW timer. The Repeat-only mode is not implemented as yet.

Panic mode reset. If for any reason your keypad becomes unresponsive or behaves strangely reset it as follows:

(1) If the Config button [Cfg] still works press it once and then press the [Del]ete key at the left-hand bottom. Then 
    unplug or reset the keypad. This will delete all files. macros and settings and you should have a re-calibration at
    start-up.
(2) Press either the white button at the bottom and hold it in, then press the black reset button at the back and release
    it (or unplug and re-plug the USB cable instead of the rest button), then only release the white button at the bottom.
    The file manager should show a new storage device named RPI-RP2. Drag and drop any of the code.UF2 files to this 
    device. It will restart after a second or two. If this still does not reset the keypad then instead of the code.UF2 
    file drag and drop the file flash_nuke.uf2, wait a few seconds and then drag the code.UF2 file to the device.

On First Start: 

You will have to do a four arrow corner calibration - press at the TIP of each arrow just ONCE. If you make a mistake 
and press the same corner twice it is likely that you will need a full reset with the nuke.uf2 file as described above
because the LCD will not pick up the correct buttons being touched.  

The default LCD settings are full brightness and full blank or off. Change these by pressing the second Pad on the right
(blue) and then press [NCT]4x (four times), then press [*Cm]2x - when *bb* shows in info bar at bottom press [678] key 
once, press [ADD] then [EXE]. The LCD Brightness is then set to 25 percent. Do then same for the blank setting use *db* 
and 2 - this sets the blank LCD to 6 percent.   
```

Another use of the two main layouts could be to have one customised for Linux - although all the keys in layout 1 and 2 except the run dialog, and the powershell and command prompt, function the same under Linux.

The list of key names that can be used is in TinyUSB stack's hid.h, as included here. Use the configuration descrition as shown below to set up the TFT_eSPI driver. The sampled calibration data can be adjusted - see the comments in the program for instructions. Compile with the USB stack set to TinyUSB not PicoSDK. 

*In contrast, this [**Pico mbed RP2040 volume and macro keyboard**](https://www.hackster.io/Murchx/pi-pico-macro-keyboard-d0bd1c) uses the Arduino mbed-based RP2040 board definitions (instead of the earlephilhower version). The USB Keyboard supports both normal and consumer keys using only one include **#include <USBKeyboard.h>**. The article is [**here as a pdf file**](Pi-Pico-mbed-RP2040-volume-and-macro-keyboard.pdf). The code was tested using push-buttons on both Arduino 2.0 and 1.8.19 IDE's, and was working (could adjust the volume up or down). [**This discussion**](https://github.com/Bodmer/TFT_eSPI/discussions/1558) notes that the Pico mbed Arduino board does not offer touch support when used with the TFT_eSPI drivers.*

STL files for the case - based on [**Customizable LCD Box**](https://www.thingiverse.com/thing:57427), are in the STL folder when available for the specific LCD. To protect the LCD resistive film use a sheet of laser print transparent film cut to the right size for the LCD.

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


