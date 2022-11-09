///////////////////////////////////////////////////////////////////////////////////////////
// Combination of (1) Raspberry Pi Pico Touch Macro Keyboard by Dustin Watts
//                    https://github.com/DustinWatts/Pico-Matrix-Touch-Keyboard
//                (2) AdafruitTinyUSB HID examples hid_composite.ino hid_boot_keyboard.ino
//                    https://github.com/adafruit/Adafruit_TinyUSB_Arduino/
//                (3) Bodmer-Adafruit Keypad Keypad_480x320.ino
//                    https://github.com/Bodmer/TFT_eSPI/examples/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Adapted by T van Dyk Aug - Nov 2022 for ILI9486/ILI9488 480x320 and Adafruit TinyUSB stack
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
/************************************************************************************
 Adafruit invests time and resources providing this open source code, please support 
 Adafruit and open-source hardware by purchasing products from Adafruit
 MIT license, check LICENSE for more information
 Copyright (c) 2019 Ha Thach for Adafruit Industries
*************************************************************************************/

/************************************************************************************************************
Layout 1 - M Keys - [M1]-[M12] - Cycle through Layout 1 to 4 press [L1-L4] or [Vo] 
------------------------------------------------------------------------------------------------
[Cut CtrlX] [ Copy CtrlC] [Paste CtrlV] [VolUp DelBspc]   [  Cut ] [  Cpy ] [  Pst ] [V+][ BsD ]
[AltPrtScr] [ Adm Pshell] [    Run    ] [VolMute L1-L4]   [M1  M7] [M2  M8] [M3  M9] [Vo][L1-L4]
[SendText ] [   Adm CMD ] [  TaskMan  ] [VolDwn  Enter]   [M4 M10] [M5 M11] [M6 M12] [V-][ Ret ]
          Caps          Num          Scroll                       C        N        S

Note: The macros listed above are default macros - use the macro editor to configure new macros or
copy, rename and/or chain/link new or existing macros.

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

The 5 Pads has the following functions in Layouts 1, 3, and 4 (M S T Keys): [A][B]
[A][B] Toggle Layers A or B M1-M6->M7-M12, S1-S6->S7-S12, T1-T6->T7-T12
[M][S][T] Move to Keys M, S or T

Layout 3 - S Keys - [S1]-[S12] - Cycle through Layout 1 to 4 press [L1-L4] or [Vo] 
Layout 4 - T Keys - [T1]-[T12] - Cycle through Layout 1 to 4 press [L1-L4] or [Vo] 
--------------------------------------------------------------------------------------------------
[Cut Ctrl+X ] [Copy Ctrl+C] [Paste Ctrl+V] [VolUp DelBspc]   [  cX ] [ cC  ] [ cV  ] [V+][ BsD ]
[Send Text 1] [Send Text 2] [Send Text 3 ] [VolMute L1-L4]   [S1 T1] [S2 T2] [S3 T3] [Vo][L1-L4]
[Send Text 4] [Send Text 5] [Send Text 6 ] [VolMute Enter]   [S4 T4] [S5 T5] [S6 T6] [V-][ Ret ]
           Caps           Num           Scroll                      C       N       S

-------------------------------------------------------------------------------------------------
Layouts 1, 3 and 4, two Layers A/B each with A=M1-M6, S1-S6, T1-T6 and B=M7-M12, S7-S12, T7-T12
To cycle through the Layouts press VolumeMute or press [L1-L4] - whichever is active at the time.
[L1-L4] has the same repeat period of 0.5 seconds as all other keys - after this time has elapsed
the key repeat is active.

Press Config Key (new set of config keys change colour):
L14 (Home) - VolumeMute -> L1, L2, L3, L4 - repeat to restore Vo
BsD (ArrowUp) - Toggle Vol+ key to either Delete or Backspace
Vol (PageUp) - Vol+ -> Delete and Vol- -> Enter - repeat to restore V+ V-
A-B (ArrowLeft) - Layouts 1, 3, 4, change to Layer A or Layer B 
Med (End) - Change Layout 2 to Media Controls Previous-Next-PlayPause-Stop
McT (ArrowRight) - Macro Timer Trigger Repeat and Oneshot Countdown or Clock Timers.
S12 (PageDown) - Start with Layout 1 or Layout 2 on powerup - must also press Sav(e) (Cfg)
Sav (Cfg) Info and File List to send Serial Monitor and Text/Macro and Config files saved
ROf (ArrowDwn] - Restart-PowerOff-Logoff - Bottom row [Rst][Log][Off] - cancel by pressing 
    [Cfg][ROf] - Includes long or short Timer options as well and Countdown and Clock Timers.

Text Strings: 
Send new text strings up to 200 characters to keys S1/T1 - S12/T12 via USBserial
Start string with <1 to <6 followed by the string assigned to S1/T1-S12/T12 - end string with >
If current Layout is L3 then S1 to S6 changed if Layout is L4 then T1 to T6 changed
If current Layer is A then S1/T1-S6/T6 changed, if Layer B S7/T7-S12/T12 changed. 

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
                                               Source-Destination SrcNum DestNum 
                                               Macro Selection - MM 01 12 - ST 12 01 - AS 99 09  etc
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

Macros sent to the PC from the built-in keyboard will be saved to file MacroX X = 1-99, and if selected, 
assigned to key [MX]. Press [Up] after sending the macros to the PC to assign them to [MX]key. 

To set it up at first (displays red xx = SD SrcNum DstNum) press [Src] once then [Dst] - SD will now show 
MM 01 01 i.e. the source and destination macro is M1 and M1. 

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

Linking Macro Examples:

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
Note 3: Pressing [Cpy] is the same as *cm* [EXE]. Chaining macros have been implemented for all 36 MST 1-12 keys.

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
(i) Key Font Bold/Normal change - Send the macro *fo* with the built-in keyboard to toggle the labels on the buttons from
    normal to bold.
(j) Key Font White/Black change - Send the macro *fc* with the built-in keyboard to toggle the labels on the buttons from
    white to black.
(k) Macro Copy - Copy macro01-99 to M,S,T keys. Can use *cm* if the SD SrcNum DstNum is set up - see the four examples 
    above. Else compose *cm*nnXmm via [ADD] where: nn = macro01-macro99 X = Keys M S T mm = 01-12
(l) Macro Unlink - unlink *ul* with the Macro Key to be unlinked visible as the Source Macro such a Mx mm xx.
(m) Macro Link Save-Restore On-Off - Linked Macro Data will be lost after a power cycle or reset unless the save-restore 
    option is turned on by executing *lr* command - then also press the black [Cfg] button twice to save the values to 
    flash. An *ld* will send a Link and Macro datadump to the serial port. A save setting for the linked macros do make
    the [Cfg] key slower as well as pressing the [Sav] key - it takes about 1-2 seconds to respond.
(n) To fill M S T 1-12 with hard-coded text string examples send *fm* *fs* *ft* or *fa* (all three) commands.
(o) To delete all macro and config files use "*de*" - will re-calibrate on restart. Remove macro files with the [Rmv] or 
    rename with [Ren] keys
(p) *ct* display four time clocks with a 1 second delay - Time, Macro[R-C][O-C], Macro[Rct][OcT] Power[R-C][O-C].

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

Power Restart KeyPad [Restart Timer] [ Stop ] [Power Timer] [Restart Clock]     [R-T] [Stp] [O-T] [R-C]   
                     [Restart timer] [ Cfg  ] [Power timer] [Power   Clock]     [R-t] [Cfg] [O-t] [O-C]       
                     [Restart   Now] [Logoff] [Power   Now] [    Exit     ]     [Rst] [Log] [Off] [   ]
                        
Macro Timers KeyPad  [Repeat Timer] [  Stop  ] [Onceof Timer] [Repeat  CTimer]  [R-T] [Stp] [O-T] [RcT]   
                     [Repeat timer] [   Cfg  ] [Onceof timer] [Oneshot CTimer]  [R-t] [Cfg] [O-t] [OcT]       
                     [Repeat Clock] [Repeat x] [Onceof Clock] [    Exit      ]  [R-C] [Rep] [O-C] [   ]
  
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

Currently only single macros from M, S and T 1-12 can be used for the timers - if a chained (linked) macro is used 
it will run the last macro used by the chain. The Timers are programmed as Time-Fire-Time-Fire. There will be an option
later to change this to Fire-Time-Fire-Time for the Repeat timers. The two real-time (using the Pico's HW RTC or Clock)
timers are configured by first setting the Clock Time by sending the string <tyymmddwhhmm> -> <t22110341439> is Thursday
3 Nov 2022 at 14h30. Then set the alarm time by sending the string <ayymmddwhhmm> -> <a22110601439> is Sunday 6 Nov 
2022 at 14h30. To send a repeat macro every 1 minute send <a-1-1-1--1-1> (the double -- is for the day of week not
significant), and associate with it 5 [R-C]. The clock time and alarm time are sent to a serial terminal and displayed
in the status bar by pressing [Cgf] twice. Send these either manually using a serial terminal or use a Proccessing 
script, or a scheduled task powershell script. Note that the Pico has a HW RTC but not with a dedicated battery backup.

The Repeat-only mode (i.e send macro fixed number of times with a delay or no delay, is not implemented as yet.

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
(blue) and then press [NXT]4x (four times), then press [*Cm]2x - when *bb* shows in info bar at bottom press [678] key 
once, press [ADD] then [EXE]. The LCD Brightness is then set to 25 percent. Do then same for the blank setting use *db* 
and 2 - this sets the blank LCD to 6 percent.   
         
***********************************************************************************************************************************/
#include "Adafruit_TinyUSB.h"
#include <SPI.h>
#include <TFT_eSPI.h>
#include <LittleFS.h>
#include <FS.h>
#include "pico/stdlib.h"
#include "pico/util/datetime.h"
#include "hardware/rtc.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Keys STRname S1-S12 TTRname T1-T12 MTRname M1-M12 and KBMacro 72 macros that can be chained
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const char STRname  [12][10] = 
{ "StrData1", "StrData2", "StrData3", "StrData4",  "StrData5",  "StrData6", "StrData7", "StrData8", "StrData9", "StrData10", "StrData11", "StrData12" };
static const char TTRname  [12][10] = 
{ "TtrData1", "TtrData2", "TtrData3", "TtrData4",  "TtrData5",  "TtrData6", "TtrData7", "TtrData8", "TtrData9", "TtrData10", "TtrData11", "TtrData12" };
static const char MTRname  [12][10] = 
{ "MtrData1", "MtrData2", "MtrData3", "MtrData4",  "MtrData5",  "MtrData6", "MtrData7", "MtrData8", "MtrData9", "MtrData10", "MtrData11", "MtrData12" };
static const char KBMacro  [100][8] = 
{ "Macro01","Macro02","Macro03","Macro04","Macro05","Macro06","Macro07","Macro08","Macro09","Macro10","Macro11","Macro12","Macro13","Macro14","Macro15",
  "Macro16","Macro17","Macro18","Macro19","Macro20","Macro21","Macro22","Macro23","Macro24","Macro25","Macro26","Macro27","Macro28","Macro29","Macro30",
  "Macro31","Macro32","Macro33","Macro34","Macro35","Macro36","Macro37","Macro38","Macro39","Macro40","Macro41","Macro42","Macro43","Macro44","Macro45",
  "Macro46","Macro47","Macro48","Macro49","Macro50","Macro51","Macro52","Macro53","Macro54","Macro55","Macro56","Macro57","Macro58","Macro59","Macro60",
  "Macro61","Macro62","Macro63","Macro64","Macro65","Macro66","Macro67","Macro68","Macro69","Macro70","Macro71","Macro72","Macro73","Macro74","Macro75",
  "Macro76","Macro77","Macro78","Macro79","Macro80","Macro81","Macro82","Macro83","Macro84","Macro85","Macro86","Macro87","Macro88","Macro89","Macro90",
  "Macro91","Macro92","Macro93","Macro94","Macro95","Macro96","Macro97","Macro98","Macro99","Macro00" };

int LCDBackLight = 13; // Will be set as OUTPUT and HIGH by TFT init or Brightness level via analog PWM

#define HIDCons RID_CONSUMER_CONTROL 
#define HIDKbrd RID_KEYBOARD         

// Report ID
enum
{
  RID_KEYBOARD = 1,
  RID_MOUSE,
  RID_CONSUMER_CONTROL, // Media, volume etc ..
};

// HID report descriptor using TinyUSB's template
uint8_t const desc_hid_report[] =
{
  TUD_HID_REPORT_DESC_KEYBOARD( HID_REPORT_ID(RID_KEYBOARD) ),
  TUD_HID_REPORT_DESC_MOUSE   ( HID_REPORT_ID(RID_MOUSE) ),
  TUD_HID_REPORT_DESC_CONSUMER( HID_REPORT_ID(RID_CONSUMER_CONTROL) )
};

// USB HID object desc report, desc len, protocol, interval, use out endpoint
Adafruit_USBD_HID usb_hid(desc_hid_report, sizeof(desc_hid_report), HID_ITF_PROTOCOL_NONE, 2, false);

#define ArraySz(x) (sizeof(x)/sizeof((x)[0]))
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// See the Arduino language specification discussion here: https://github.com/arduino/language/discussions/2
// on multitasking, timing logic and limitations of state machines overcome by threads.
////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
unsigned long NowMillis = 0;           // LCD Backlight Saver
unsigned long LastMillis = 0;          // LCD Backlight Saver
unsigned long TimePeriod = 120000;     // 30 seconds LCD Backlight Timer change with *tb*num=0,1-5 via keybrd 1=30 sec, 0=120 days
unsigned long RepTimePeriod = 500;     // After this key repeat is active
unsigned long RepLast = 0;             // Time when key first pressed
unsigned long RepNow = 0;              // Time when key still pressed
bool BackLightOn = true;               // LCD Backlight Saver
int VolMuteCount = 0;                  // Can be 0 1 in Layout1 2 Layout2 VolMute(On/Off) 3 Layout3 4 Layout4 
int Layout = 1;                        // Layout 1 2 3 4 = M Config+KeyBrds S T
int LayoutPrev = 1;                    // Save Layout temporarily while using Macro functions
int LayerAB = 0;                       // Switch [Layout A B] for [Layers 1 3 4] A = [M1-M6] B = [M7-M12] for M S T press [Sav](e)
char LayerABStr[8] = {"Layer X"};      // LayerAB = (LayerAB==0); [6] = 65+LayerAB

int DimVal = 0;                        // 0 -> Backlight ON/OFF 1-9 dimmed value
int NormVal = 0;                       // Max normal value LCD brightness
const static uint8_t dbArr[10] = {0,        4,    8,    16,   25,   38,   51,   64,  102,  128};
const static uint8_t bbArr[10] = {0,        8,   16,    25,   38,   51,   64,  102,  128,  166};
const static char DBArr[10][3] = { " 0", " 1", " 3",  " 6", "10", "15", "20", "25", "40", "50"};
const static char BBArr[10][4] = {"100", " 3", " 6",  "10", "15", "20", "25", "40", "50", "65"};

const static long unsigned int tbArray[10] = {600000000, 30000,   60000,   90000,    120000, 180000, 300000, 600000, 1800000, 3600000 };
const static char tbArrayStr[10][10]       = {"170 hrs","30 sec","60 sec","90 sec", "2 min","3 min","5 min","10 min","30 min","1 hrs" };
const static long unsigned int mtArray[10] = {10800000,  30000,   60000,    90000,   120000, 180000, 300000, 600000, 1800000, 3600000 };
const static char mtArrayStr[10][10]       = {"3 hrs",  "30 sec","60 sec","90 sec", "2 min","3 min","5 min","10 min","30 min","1 hrs" };
const static long unsigned int mTArray[10] = {10800000,  7200000, 10800000, 18000000, 21600000, 36000000, 43200000, 86400000, 172800000, 604800000 };
const static char mTArrayStr[10][10]       = {"300 hrs", "2 hrs", "3 hrs",   "5 hrs",  "6 hrs", "10 hrs", "12 hrs",  "24 hrs", "48 hrs",  "1 week" };
unsigned long timePowerOff = 30;   // seconds                       
unsigned long TimePowerOff = 3000;
unsigned long timeRestart = 30;                          
unsigned long TimeRestart = 3000;

unsigned long timeOnceof = 9000;                          
unsigned long TimeOnceof = 90000;
unsigned long timeRepeat = 9000;                          
unsigned long TimeRepeat = 90000;
unsigned long timeOnceofPrev = 0;                          
unsigned long TimeOnceofPrev = 0;
unsigned long timeRepeatPrev = 0;                          
unsigned long TimeRepeatPrev = 0;

//                    012345678901234567890123456
char BlankArr[]    = "Blank Time          seconds"; 
char DimArr[]      = "LCD Blank Dim       (x/255)";
char NormArr[]     = "LCD Brightness      (x/255)";
char mTArr[]       = "Oneshot Time               ";
char rTArr[]       = "Repeat Time                ";

bool CapsLock = false;                 // Green "C"
bool NumLock = false;                  // Green "N"
bool ScrollLock = false;               // Green "S" 
bool Change = false;                   // Indicators changed
int  ConfigKeyCount = 0;               // Start with Layout 2 Cfg key not pressed 
bool MuteDisable = true;               // Disable VolMute with Layout 1 2 3 4 select Default is L1-L4
bool VolDisable = false;               // Disable Vol+ Vol- with Del/BSpc Enter
bool MuteDisablePrev = true;           // Restore VolMute with Layout 1 2 3 4 select
bool VolDisablePrev = false;           // Restore Vol+ Vol- with Del/BSpc Enter
bool SaveLayout = false;               // Start with selected layout 
bool Media = false;                    // Four Media Keys in Layout 2
bool MediaPrev = false;                // Restore Four Media Keys in Layout 2
bool DelBS = false;                    // Delete or Backspace replace Vol+

//////////////////////////////////////////////////////////////////////////////////////////////
// To get new calibration data: 
// 1. Send the macro *ca* with the built-in keyboard to toggle re-calibration option to ON
// 2. Restart or Reset the macro pad
// 3. Do the calibration - press at the TIP of the arrow
// 4. Send the macro *ca* with the built-in keyboard to toggle re-calibration option to OFF
// Press [Del]ete in [Cfg] options - next power-up will do a re-calibration
// Typical Calibration Data: 426, 3464, 279, 3488, 5
///////////////////////////////////////////////////////////////////////////////////////////////
//uint16_t calData[5] = {404, 3468, 263, 3486, 5};     
uint16_t calData[5];                              
uint8_t  calDataOK = 0;
#define CalFile "TouchCalData"  // Name of the Touch calibration file
bool DoCal = false;             // Set true if you want the calibration to run on each boot 

uint16_t t_x = 0, t_y = 0; // Touch coordinates

/////////////////////////////////////////////////////////////////////////////////////////
// Layout 1 to 4 Keys
/////////////////////////////////////////////////////////////////////////////////////////
int ScrSizeType = 1;        // 1 = 480x320  0 = 320x240; 

// Key sizes and spacing
#define KEY_W 95            // Key width  95 x 4 = 380 + 15 x 4 = 440  
#define KEY_H 80            // Key height 80 x 3 = 240 + 12 x 4 = 288
#define KEY_SPACING_X 15    // X gap
#define KEY_SPACING_Y 12    // Y gap
#define KEY_X 50            // 30 is off-screen on left
#define KEY_Y 50            // 30 is the topmost with no cut-off

/////////////////////////////////////////////////////////////////////////////////////////////
// Layout 5 small Config Buttons M K M (Math Numeric Keyboard Media) between 1st and 3rd row
/////////////////////////////////////////////////////////////////////////////////////////////
#define PAD_W 30          //  Key width  PAD_X is middle -> Pad X is from 317 to 303 
#define PAD_H 40          //  Key height
#define PAD_SPACING_Y 54  // 
#define PAD_X 455         //  config buttons right side
#define PAD_Y 34          // config buttons top
//////////////////////////////////////////////////////////////////////////////////
// Indicators status Caps-Num-Scroll C N S XPos[7] = {0, 0, 105, 220, 0, 105, 220}
//////////////////////////////////////////////////////////////////////////////////
#define IND_X 105 //  Centred on this - Space in RH side about 16 pixels
#define IND_Y 180 //  
//////////////////////////////////////////////////////////////////////////////////
// Options Indicators status T D 1 2 XPos[7] = {0, 0, 105, 220, 0, 105, 220}
//////////////////////////////////////////////////////////////////////////////////
#define OPT_X 105  // Centred on this - Sppace in RH side about 16 pixels
#define OPT_Y 88   // 
////////////////////////////////////////////////////////////////////////////////
// Message status line at bottom
////////////////////////////////////////////////////////////////////////////////
#define STATUS_X 220 // Centred on this - not middle buttons are off-centre
#define STATUS_Y 296 // 305 bottom of y p etc cut-off

#define KEY_TEXTSIZE 1   // Font size multiplier

// Delay between keypress to give USB HID time to respond
uint8_t keydelay = 50;        // keycode[1,..,6] delay milliseconds
uint8_t keydelay2 = 25;       // keyPress delay
uint8_t keydelay1 = 200;      // keyPress delay for Logoff Poweroff Restart
uint16_t keydelay3 = 500;     // used in UAC confirm

// Create the screen object
TFT_eSPI tft = TFT_eSPI();

//////////////////////////////////////////////////////
// RGB565 Color Picker
// http://www.barth-dev.de/online/rgb565-color-picker/
//////////////////////////////////////////////////////
#define Black    0x0000      //   0,   0,   0 
#define LGrey2   0x39E7      //
#define LGrey    0x5AEB      //
#define Black1   0x2945      //   
#define Navy     0x000F      //   0,   0, 128 
#define DGreen   0x03E0      //   0, 128,   0 
#define DCyan    0x03EF      //   0, 128, 128 
#define Maroon   0x7800      // 128,   0,   0 
#define Purple   0x780F      // 128,   0, 128 
#define Olive    0x7BE0      // 128, 128,   0 
#define LGrey1   0xD69A      // 211, 211, 211
#define LGrey3   0xA534 
#define DGrey    0x7BEF      // 128, 128, 128 
#define Blue     0x001F      //   0,   0, 255 
#define Green    0x07E0      //   0, 255,   0 
#define Cyan     0x07FF      //   0, 255, 255 
#define Red      0xF800      // 255,   0,   0 
#define Magenta  0xF81F      // 255,   0, 255 
#define Yellow   0xFFE0      // 255, 255,   0 
#define White    0xFFFF      // 255, 255, 255 
#define Orange   0xFDA0      // 255, 180,   0 
#define Green1   0xB7E0      // 180, 255,   0 
#define Green2   0x26C8      
#define Green3   0x1750
#define Green4   0x65C8
#define Green5   0x5527
#define Pink1    0xFE19      // 255, 192, 203 
#define Pink     0xFBEE 
#define Brown    0x9A60      // 150,  75,   0 
#define Gold     0xFEA0      // 255, 215,   0 
#define Silver   0xC618      // 192, 192, 192 
#define SkyBlue1 0x867D      // 135, 206, 235 
#define SkyBlue2 0x35B9
#define SkyBlue3 0x2D57
#define SkyBlue  0x2CD5
#define Violet   0x915C      // 180,  46, 226 

///////////////////////////////////////////////////////////////////////////////////
// Four layout sets of 12 button colours - use the colours defined in TFT_eSPI.h
// Modified by Caps-Scroll-Num Lock Keys cannot use const - 16x12 = 192 16bit int
// 16 groups of 4 columns by 3 rows i.e. Layout 1 - 4 each has 4 Groups 0 - 3
///////////////////////////////////////////////////////////////////////////////////
#define BrG    Blue,    Red,     Green5
#define LG3    LGrey ,  LGrey ,  LGrey 
#define BLR    Blue,    LGrey ,  Red
#define LbL    LGrey ,  Black1, LGrey 
#define SB3    SkyBlue, SkyBlue, SkyBlue
#define Pk3    Pink,    Pink,    Pink

uint16_t Colours[16][12] = {
BrG, Orange,   // Row 1 Normal L1 ///////////////////// 12 Keys group 0 12x4=48
LG3, Orange,   // Row 2 12 keys [0][0] to [0][11]
LG3, Orange,   // Row 3 [0][8] Layer1 Key8
BLR, Orange,   // Navigation L2
LbL, Orange, 
BLR, Orange,   // [1][8] 
BrG, Orange,   // Text Macros S1-S6 L3
SB3, Orange, 
SB3, Orange,   // [2][8]
BrG, Orange,   // Text Macros T1-T6 l4
Pk3, Orange, 
Pk3, Orange,   // [3][8]
BrG, Orange,   // Normal + VolMute OFF L1 ///////////// 12 Keys group 1 12x8=96
LG3, Violet, 
LG3, Orange,   // [4][8]
BLR, Orange,   // Navigation + VolMute OFF L2
LbL, Violet, 
BLR, Orange,   // [5][8]
BrG, Orange,   // Text Macros S1-S6 + VolMute OFF L3
SB3, Violet, 
SB3, Orange,   // [6][8]
BrG, Orange,   // Text Macros T1-T6 + VolMute OFF L4
Pk3, Violet, 
Pk3, Orange,   // [7][8]
BrG, Blue,     // Normal + Vol OFF L1 ///////////////// 12 Keys group 2 12x12=144
LG3, Orange, 
LG3, Red,      // [8][8]
BLR, Blue,     // Navigation
LbL, Orange, 
BLR, Red,      // [9][8]
BrG, Blue,     // Text Macros S1-S6
SB3, Orange, 
SB3, Red,      // [10][8]
BrG, Blue,     // Text Macros T1-T6
Pk3, Orange, 
Pk3, Red,      // [11][8]
BrG, Blue,     // Normal + Vol OFF VolMute OFF //////// 12 Keys group 3 12x16=192
LG3, Violet, 
LG3, Red,      // [12][8]
BLR, Blue,     // Navigation + Vol OFF VolMute OFF 
LbL, Violet, 
BLR, Red,      // [13][8]
BrG, Blue,     // Text Macros S1-S6 + Vol OFF VolMute OFF 
SB3, Violet, 
SB3, Red,      // [14][8]
BrG, Blue,     // Text Macros T1-T6 + Vol OFF VolMute OFF 
Pk3, Violet, 
Pk3, Red };    // [15][8]
                             
uint16_t keyColor[12] = {0x0000}; // = {};  // construct = {0}; should init all elements 0x00 required for local vars

uint16_t BackgroundColor[4] = {Black, Black, Black, Black};    // 4 Layouts 1-4

uint16_t ButtonOutlineColor[4] = {White, White, White, White}; // 4 Layouts 1-4

uint16_t CfgColour     = SkyBlue;                               // Layout 2 KeyBrd
uint16_t MediaColour   = Orange;                                // Layout 2 Media Keys 
uint16_t MathColour0   = 0xFF51;                                // Math KeyBrd                               
uint16_t MathColour10  = 0xF692;                                // Teal colours
uint16_t MathColour3   = 0xEE0E;
uint16_t MathColour4   = 0xEDCB;
uint16_t MathColour20  = 0xE547;
uint16_t MathColour1   = 0xDD42;
uint16_t MathColour5   = 0xAC41;
uint16_t MathColour2   = 0x8B81;
//////////////////////////////////////////////////////////////////////////////////////////
// Consumer keys are 16 bits - names are long to use will use hex values or short alias
#define VolUp   0x00E9
#define VolDown 0x00EA
#define VolMute 0x00E2
#define BassUp  0x0152
#define BassDwn 0x0153
#define TrebUp  0x0154
#define TrebDwn 0x0155
#define PlayMed 0x00CD  // PlayPause
#define NxtMed  0x00B5  // Next
#define PrevMed 0x00B6  // Previous
#define StopMed 0x00B7  // Stop
////////////////////////////////////////////////////////////////////////////////////////
#define KeyCut   0x7B   // Ctrl + X
#define KeyCopy  0x7C   // Ctrl + C
#define KeyPaste 0x7D   // Ctrl + V
#define PrtScr   0x46
#define KeyTab   0x2B
#define ArrUp    0x52
#define ArrDown  0x51
#define ArrLeft  0x50
#define ArrRight 0x4F
#define KeyHome  0x4A
#define KeyEnd   0x4D
#define PageUp   0x4B
#define PageDwn  0x4E
#define KeyEnter 0x28 // KEYPAD_ENTER = 0x58 KEY_RETURN = 0x9E KEY_EXECUTE = 0x74
#define KeyYes   0x1C // "Y" or "y" note ACII Y = 0x59 y = 0x79 (difference of 61 decimal)
///////////////////////////////////////////////////////////////////////////////////////////
#define F01  0x3A  
#define F02  0x3B
#define F03  0x3C
#define F04  0x3D
#define F05  0x3E
#define F06  0x3F
#define F07  0x40
#define F08  0x41
#define F09  0x42
#define F10  0x43
#define F11  0x44 
#define F12  0x45
#define F13  0x68
#define F14  0x69
#define F15  0x6A
#define F16  0x6B
#define F17  0x6C
#define F18  0x6D
#define F19  0x6E
#define F20  0x6F
#define F21  0x70
#define F22  0x71
#define F23  0x72
#define F24  0x73
#define BShl 0x31   // Backslash
#define FShl 0x38   // Slash
#define Ins  0x49
#define Tab  0x2B
#define Spc  0x20
#define Esc  0x29
#define CtrL 0xE0  // Modifier values defined in button()
#define CtrR 0xE4
#define ShfL 0xE1
#define ShfR 0xE5
#define AltL 0xE2
#define AltR 0xE6
#define WinL 0xE3 
#define WinR 0xE7
#define GuiL 0xE3 // Super Key Left
#define GuiR 0xE7 // Super Key Right
#define BckS 0x2A
#define DelK 0x4C
#define NmL  0x53  //NumlockKey
////////////////////////////////////////////////////////////////////////////////////////////////
#define KpDiv             0x54  
#define KpMul             0x55
#define KpMin             0x56
#define KpAdd             0x57
#define KpRet             0x58
#define Kp1               0x59  // Keypad Number used in Alt + Number for special chars and Math Symbols
#define Kp2               0x5A  // Alternative is Number + Alt+X
#define Kp3               0x5B
#define Kp4               0x5C
#define Kp5               0x5D
#define Kp6               0x5E
#define Kp7               0x5F
#define Kp8               0x60
#define Kp9               0x61
#define Kp0               0x62
#define KpDot             0x63
#define KpEqu             0x67
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ctl + Alt + Backspace Logs you out of X back to the login screen - works with all desktop environments and window managers.
// Ctrl + Alt + Delete   Start the reboot process - works in all desktop environments and all window managers
// Ctrl + Alt + T is the shortcut to open terminal in Ubuntu.
// Alt + F2: Run console quick command, instead of opening a terminal and running the command there, you can use Alt+F2 to run the console.
// Alt + F1 (= Super key) Switch between the Activities overview and desktop
// Super + Tab Switch between windows. Hold down Shift for reverse order.
// Super +`~ Switch between windows from the same application
// Alt + Esc Switch between windows in the current workspace. Hold down Shift for reverse order.
// Ctrl + Alt + Tab Give keyboard focus to the top bar. 
// Super + A Show the list of applications.
// Super + PageUp/PageDown Switch between workspaces.
// Shift + Super + PageUp/PageDown Move the current window to a different workspace.
// Shift + Super + ? Move the current window one monitor to the left.
// Super + L Lock the screen.
// Super + V Show the notification list - Esc to close.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ctrl + Escape              - Open Start Menu
// GUI + Ctrl + Shft + B      - Wake PC from black or blank screen:
// GUI + X                    - Windows shutdown options
// Ctrl + Shft + Escape       - open Task Manager
// Ctrl + Shft + N            - create new Folder
// GUI + Shift + S            - Snipping Tool
// GUI + V                    - Clipboard window
// GUI + I                    - open Settings
// AltL + ShftL + Numlock     - Mouse Keys on/off
// Shft 5 times               - Sticky Keys on/off
// Numlock 5 times            - Toggle Keys on/off
// ShftR 8 times              - Filter Keys on/off
// GUI + Ctrl + O             - On-Screen Keyboard
// Shift + F10                - Menu (Mouse Right-click)
// GUI + . or GUI + ;         - Emoji and (some) Math Symbols (click on Omega) insert
// Alt + NumPad Number        - special symbols math characters etc - Numlock must be on Alt+3 Alt+24 Alt+8733 Alt+10102
// Number + Alt+X             - Special symbols
// GUI + R                    - Character Map
// GUI + Space                - Choose Key Layout
// F11                        - Browser Fullscreen
// A combination: https://www.tindie.com/products/dekuNukem/duckypad-do-it-all-mechanical-macropad/
// GUI + R + notepad + C/R + Hello World + Ctrl + (repeat 10x) - Open Notepad insert Hello World make it bigger 10 x
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Two Layers A B of two by four layout sets of 12 button labels <layer><layout><name>[<number-of-lables>][<number-of-chars-per-label]
// The number of chars per label include the termination char \0 => 3 character labels here:
// ~ https://wiki.sei.cmu.edu/confluence/display/c/STR11-C.+Do+not+specify+the+bound+of+a+character+array+initialized+with+a+string+literal
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  b = Layout - 1                               Layout 1 - 4  and b = 0 - 3 
//  b = 4,5,6,7      VolDisable)                 b = b + 8 
//  b = 8,9,10,11    MuteDisable                 b = b + 4 VolDisable AND MuteDisable  b = b + 12 
//  Labels[LayerAB][b][n][m]; 
//  Colours[b][n];
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Same 16 groups of 4 columns by 3 rows i.e. Layout 1 - 4 has 4 Groups 0 - 3 used in Colours[16][12]
// < 2 Layers A B > < 4 Groups of 4 Layouts 1 - 4 > < 12 Keys > < 4 chars > chars include \0
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CcP    "Cut", "Cpy", "Pst"
#define XCV    "cX",  "cC",  "cV"
#define HPU    "Hme", "Up",  "PgU"
#define LcR    "<",   "Cfg", ">"
#define EPD    "End", "Dwn", "PgD"
#define S13    "S1",  "S2",  "S3"
#define S46    "S4",  "S5",  "S6"
#define T13    "T1",  "T2",  "T3"
#define T46    "T4",  "T5",  "T6"
#define M13    "M1",  "M2",  "M3"
#define M46    "M4",  "M5",  "M6"
#define S79    "S7",  "S8",  "S9"
#define S12    "S10", "S11", "S12"
#define T79    "T7",  "T8",  "T9"
#define T12    "T10", "T11", "T12"
#define M79    "M7",  "M8",  "M9"
#define M12    "M10", "M11", "M12"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const static char Labels[2][16][12][4] =                                            
{ CcP,   "V+",  M13,  "Vo",  M46,   "V-",  // L1 Mute ON Vol=ON   0     Group 0 Layer A
  HPU,   "V+",  LcR,  "Vo",  EPD,   "V-",  // L2                  1
  XCV,   "V+",  S13,  "Vo",  S46,   "V-",  // L3                  2
  XCV,   "V+",  T13,  "Vo",  T46,   "V-",  // L4                  3
  CcP,   "V+",  M13,  "L1",  M46,   "V-",  // L1 Mute OFF Vol=ON    4    Group 1 Layer A 
  HPU,   "V+",  LcR,  "L2",  EPD,   "V-",  //                       5
  XCV,   "V+",  S13,  "L3",  S46,   "V-",  //                       6
  XCV,   "V+",  T13,  "L4",  T46,   "V-",  //                       7
  CcP,   "BsD", M13,  "Vo",  M46,   "Ret", // L1 Mute ON Vol=OFF  8      Group 2 Layer A
  HPU,   "BsD", LcR,  "Vo",  EPD,   "Ret", //                     9
  XCV,   "BsD", S13,  "Vo",  S46,   "Ret", //                     10
  XCV,   "BsD", T13,  "Vo",  T46,   "Ret", //                     11
  CcP,   "BsD", M13,  "L1",  M46,   "Ret", // L1 Mute OFF Vol=OFF    12  Group 3 Layer A
  HPU,   "BsD", LcR,  "L2",  EPD,   "Ret", //                        13
  XCV,   "BsD", S13,  "L3",  S46,   "Ret", //                        14
  XCV,   "BsD", T13,  "L4",  T46,   "Ret", //                        15
  CcP,   "V+",  M79,  "Vo",  M12,   "V-",  // L1 Mute ON Vol=ON          Group 0 Layer B 
  HPU,   "V+",  LcR,  "Vo",  EPD,   "V-",  // L2
  XCV,   "V+",  S79,  "Vo",  S12,   "V-",  // L3
  XCV,   "V+",  T79,  "Vo",  T12,   "V-",  // L4
  CcP,   "V+",  M79,  "L1",  M12,   "V-",  // L1 Mute OFF Vol=ON         Group 1 Layer B
  HPU,   "V+",  LcR,  "L2",  EPD,   "V-",
  XCV,   "V+",  S79,  "L3",  S12,   "V-",
  XCV,   "V+",  T79,  "L4",  T12,   "V-",
  CcP,   "BsD", M79,  "Vo",  M12,   "Ret", // L1 Mute ON Vol=OFF         Group 2 Layer B
  HPU,   "BsD", LcR,  "Vo",  EPD,   "Ret",
  XCV,   "BsD", S79,  "Vo",  S12,   "Ret",
  XCV,   "BsD", T79,  "Vo",  T12,   "Ret",
  CcP,   "BsD", M79,  "L1",  M12,   "Ret", // L1 Mute OFF Vol=OFF        Group 3 Layer B 
  HPU,   "BsD", LcR,  "L2",  EPD,   "Ret",
  XCV,   "BsD", S79,  "L3",  S12,   "Ret",
  XCV,   "BsD", T79,  "L4",  T12,   "Ret"};
//0 1 2      4   5 6 7   8   9 10 11   12
char keyLabel[12][4] = {""}; //  = {};                 // = ""; all 0x00  
/////////////////////////////////////////////////////////////////////////////////////////////////
//  If Cfg Key pressed new options (new Labels and new Colours)
//  Labels[0/1][1,5,9,13][0-11][0-3]
//  "Hme", "Up",  "PgU", "Del/V+", "<",   "Cfg", ">",  "V0/L14",  "End", "Dwn", "PgD", "V-/Ret",
//  "L14", "  ",  "Vol", "      ", "AB",  "Sav", " ",  "      ",  "Med", "   ", "S12", "      "
/////////////////////////////////////////////////////////////////////////////////////////////////
const static char CfgLabel[12][4] =                                          
{"L14", "BsD",  "Vol", "   ", "A-B",  "Sav", "McT",  "   ",  "Med", "ROf", "S12", "   "};  // All 3 chars + \0
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  If Media Controls Arrow Right Key pressed new options in different colour:                                        
//                 "Hme", "Up",  "PgU", "Del/V+", "<",    "Cfg",  ">",   "V0/L14",  "End", "Dwn",  "PgD", "V-/Ret"
// MediaLabel[x][] "Stp", "   ", "   ", "   ",    "Prv",  "   ",  "Nxt", "   ",     "P-P", "   ",  "  ",  "   " 
// MediaLabel[x][] "|||", "   ", "   ", "   ",    "|<",   "   ",  ">|",  "   ",     ">||", "   ",  "  ",  "   " 
// keyLabel[x][]     0                              4               6                8                            
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// const static char MediaLabel[12][4] = 
// {"Stp", "   ", "   ", "   ",    "Prv",  "   ",  "Nxt", "   ",  "P-P", "   ",  "   ", "   "}; 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const static char MediaLabel[12][4] = 
{   "|||", "   ", "   ", "   ",    "|<",   "   ",  ">|",  "   ",  ">||", "   ",  "   ", "   "}; 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// If Mouse Keys selected via KeyBrd command *mk* new options:                                          // not implemented
//                 "Hme", "Up",  "PgU", "DelV+",  "<",   "Cfg",  ">",   "V0L14",  "End", "Dwn",  "PgD", "V-Ret"
// Mousekeys1[x][] "LUp", "Up",  "RUp", "L-R",    "<",   "1-c",  ">",   "2-c",    "Ldw", "Dwn",  "RDw", "Hld" 
// keyLabel[x][]     0             2                4               6                                            
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const static char Mousekeys1[12][4] =                                                                    // not implemented
{"LUp", "Up",  "RUp", "L-R",    "<",   "1-c",  ">",   "2-c",    "Ldw", "Dwn",  "RDw", "Hld"}; 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  If Numeric Keys selected via KeyBrd command *kb* new options:
//                 "Hme", "Up", "PgU", "DelV+", "<",   "Cfg",  ">",  "V0L14", "End", "Dwn", "PgD", "V-Ret"
// Numkeys1[x][]   "BsD", "7",  "8",   "9",     "Ret",  "4",    "5",  "6",     "0",   "1",   "2",    "3"
// keyLabel[x][]     0             2                4               6                                            
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char NumkeysX[12][4] =  { 0 };
const static char Numkeys1[12][4] = 
{"BsD", "7",  "8",   "9",   "Ret",  "4",  "5",  "6",  "0",  "1",  "2", "3"}; 
const static char Numkeys2[12][4] = 
{"BsD", " ",  "%",   "xX",  "Ret",  ",",  ".",  "=",  "+",  "-",  "*", "/"}; 
const static char Numkeys3[12][4] = 
{"BsD", "aA", "bB",  "cC",  "Ret",  "dD", "eE", "fF", "(",  ")",  "[", "]"}; 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const static char KbrdLabel[5][12][4] =                                                    // Xlate only needed for kcode not Keypress
{"abc",  "def",  "ghi",  "EXE", "jkl", "mno", "pqr", "NXT", "stu", "vwx", "yz ", "ADD",    // ASCII->HID Subtract  dec
 "ABC",  "DEF",  "GHI",  "exe", "JKL", "MNO", "PQR", "nxt", "STU", "VWX", "YZ_", "add",    // ASCII->HID Subtract 61 dec
 "012",  "345",  "678",  "EXE", "9+-", "*=/", "*Cm", "NXT", "Sym", "Brc", "Fnn", "ADD",    // 1-9 subtract 19 dec 0 subtract 9 dec
 "Lst",  "Ren",  "Rmv",  "Snd", "Tmr", "Cpy", "Lnk", "NXT", "Src", "Dst", "Num", "Sav",    // *Cm=StarCodes Lst=MacroContent Rmv=RemoveMacro
 "ALT",  "SHF",  "CTR",  "EXE", "GUI", "TEI", "CRF", "NXT", "LHR", "UED", "UND", "ADD"};   // Mainly modifiers                                                                                      
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const static char KeyBrdSpc[3][12][4] = 
{"ALT",  "SHF",  "CTR",  "   ", "GUI", "Tab", "C/R", "  ", "A-L", "A-U", "PgU", "ADD",
 "A-R",  "S-R",  "C-R",  "   ", "G-R", "Esc", "L/F", "  ", "Hme", "End", "PgD", "ADD",
 "PrS",  "Del",  "BSp",  "   ", "Nul", "Ins", "RET", "  ", "A-R", "A-D", "Bsp", "ADD"};
const static byte KeyBrdNum[3][12] = 
{AltL,   ShfL,   CtrL,    0,    WinL,  Tab,   0x0D,  0, ArrLeft,  ArrUp,  PageUp, 0,
 AltR,   ShfR,   CtrR,    0,    WinR,  Esc,   0x0A,  0, KeyHome,  KeyEnd, PageDwn,0,
 PrtScr, DelK,   BckS,    0,    0x00,  Ins,   0x28,  0, ArrRight, ArrDown,NmL,    0};
const static byte KeyBrdFx[24] =   // F01 to F24 must use keycode keyPress 0x3C = < not F3 - else put a modifier before Fx such as Shift + F3 
{F01, F02, F03, F04, F05, F06, F07, F08, 
 F09, F10, F11, F12, F13, F14, F15, F16,               
 F17, F18, F19, F20, F21, F22, F23, F24 };
const static char KeyBrdFxChr[24][4] = // F01 to F24
{"F01", "F02", "F03", "F04", "F05", "F06", "F07", "F08",
 "F09", "F10", "F11", "F12", "F13", "F14", "F15", "F16",
 "F17", "F18", "F19", "F20", "F21", "F22", "F23", "F24" };
const static char KeyBrdBrackets[8][2] =    
{ "[", "]", "(", ")", "{", "}", "<", ">" };
const static char KeyPadNum[10][2] =           // not implemented use *kp* to toggle both numpad and keybrd numbers
{ "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"  };
const static byte KeyPadVal[10][2] =    
{ 0x62,0x59,0x5A,0x5B,0x5C,0x5D,0x5E,0x5F,0x60,0x61 };
const static char KeyBrdSymbols[17][2] =    
{ "@", "#", "?", "$", "%", "^", "&", ",", ".", "~", "`", "\\", "\'", "\"", "|", ":", ";" };
const static char StarCode[28][5] =    
{ "*bb*", "*ca*", "*cm*", "*ct*", "*de*", "*db*", "*fa*", "*fm*", "*fs*", "*ft*", "*fc*", "*fo*", "*kb*", "*lr*",
  "*ld*", "*mt*", "*mT*", "*nt*", "*nT*", "*ot*", "*oT*", "*po*", "*po*", "*rt*", "*rT*", "*tb*", "*ul*", "*ua*" };
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Unicodes from Mathboard at https://github.com/nup002/Mathboard (MIT license)
// Also see: https://en.wikipedia.org/wiki/Mathematical_operators_and_symbols_in_Unicode
// Greek Letters lower-case alpha to omega
// alpha beta gamma delta epsilon zeta eta theta iota kappa lambda
// mu nu xi omikron pi rho sigma(1,2) tau upsilon phi chi psi omega  
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Greek Letters Upper-case and Set theory and Various
// Gamma Delta Theta Lambda Xi Pi Sigma Phi Psi Omega 
// Set theory: not-subset-or-equal disjoint-union union intersection element-of not-element-of empty-set subset-of 
//             not-subset-of set-difference subset-or-equal plus-sign-above-equals-sign double-square-intersection
//             double-square-union intersection-with-dot amalgamation join
// Various:  dotproduct crossproduct plusminus parallel degree infinity 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Algebra and calculus: sum  n-ary-product  integral  double-integral  triple-integral  line-integral  surface-integral  
//                       volume-integral  square-root  cube-root  fourth-root  arrow  partial-derivative  nabla
// Comparison and equivalence: notequal almostequal proportional identicalto lessorequal greaterorequal 
// Modifiers: check combining-tilde combining-bar combining-arrow combining-dot combining-2dot prime double-prime 
//            sup-0 sup-1 sup-2 sup-n sup-i sup-j sub-0 sub-1 sub-2 sub-n sub-i sub-j circumflex 
// Logic: arrow-implies arrow-if-and-only-if there-exists there-does-not-exists uncoditionally-true 
//        unconditionally-false and or xor not proves models  
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const static char MathLabel[4][12][4] =                                                    
{"abg",  "dez",  "ehi",  "ESC", "klm", "nko", "prs", "NXT", "stu", "fcp", "oO ", "EXE",
 "GDH",  "LKP",  "SFP",  "ESC", "Os1", "Set", "Set", "NXT", "Set", "Var", "Var", "EXE",
 "Alg",  "Cal",  "Cal",  "ESC", "Alg", "Cal", "Com", "NXT", "Com", "Mod", "Mod", "EXE", 
 "Log",  "Log",  "Log",  "ESC", "Log", "Mod", "Mod", "NXT", "Mod", "Set", "Set", "EXE" };
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Type in Alt + Numpad key then after symbol show press Alt+X to get hex codes used below
// MathNum[4][3][12][5] -> 4 pages x 3 options per key x 12 keys x 5 chars per uni(hex)code
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const static char MathNum[4][3][12][5] = 
{"03B1",  "03B4",   "03B7",  "0   ",  "03BA",   "03BD",   "03C0",   "0  ",  "03C3",  "03C6",  "03C9", "0  ",
 "03B2",  "03B5",   "03B8",  "0   ",  "03BB",   "03BE",   "03C1",   "0  ",  "03C4",  "03C7",  "03A9", "0  ",
 "03B3",  "03B6",   "03B9",  "0   ",  "03BC",   "03BF",   "03C2",   "0  ",  "03C5",  "03C8",  "0020", "0  ",
 "0393",  "039B",   "03A3",  "0   ",  "03A9",   "222A",   "2209",   "0  ",  "2284",  "22C5",  "2225", "0  ",
 "0394",  "039E",   "03A6",  "0   ",  "2288",   "2229",   "2205",   "0  ",  "2216",  "2A2F",  "00B0", "0  ",
 "0398",  "03A0",   "03A8",  "0   ",  "2A06",   "2208",   "2282",   "0  ",  "2286",  "00B1",  "221E", "0  ",
 "2211",  "2230",   "222D",  "0   ",  "221C",   "220F",   "2260",   "0  ",  "2261",  "0303",  "0307", "0  ",
 "2192",  "222B",   "222E",  "0   ",  "221A",   "2202",   "2248",   "0  ",  "2264",  "0304",  "0308", "0  ",
 "2207",  "222C",   "222F",  "0   ",  "221B",   "2032",   "221D",   "0  ",  "2265",  "20D7",  "2033", "0  ", 
 "21D2",  "2204",   "22BB",  "0   ",  "00AC",   "2070",   "2080",   "0  ",  "2099",  "2A1D",  "2A4F", "0  ",
 "21D4",  "22A4",   "2227",  "0   ",  "22A2",   "2071",   "2081",   "0  ",  "1D62",  "2A3F",  "2A4E", "0  ",
 "2203",  "22A5",   "2228",  "0   ",  "22A8",   "02B2",   "2082",   "0  ",  "2C7C",  "2A40",  "2A72", "0  " }; 

//MathName[4][3][12][32] -> 4 pages x 3 options per key x 12 keys x 32 chars per description
const static char MathName[4][3][12][32] = 
{"alpha", "delta",   "eta",   "0", "kappa",              "nu",          "pi",            "0 ","sigma2",         "phi",          "omega",          "0",
 "beta",  "epsilon", "theta", "0", "lambda",             "xi",          "rho",           "0 ","tau",            "chi",          "Omega",          "0",
 "gamma", "zeta",    "iota",  "0", "mu",                 "omicron",     "sigma1",        "0 ","upsilon",        "psi",          "space",          "0",
 "Gamma", "Lambda", "Sigma",  "0", "Omega",              "union",       "not-element-of","0", "not-subsetof",   "dotproduct",   "parallel",       "0",
 "Delta", "Xi",     "Phi",    "0", "not-subset-or-equal","intersection","empty-set",     "0", "setdifference",  "crossproduct", "degree",         "0",
 "Theta", "Pi",     "Psi",    "0", "disjoint-union",     "element-of",  "subset-of",     "0", "subset-or-equal","plusminus",    "infinity",       "0",
 "sum",  "volume-integral","triple-integral", "0", "fourth-root", "n-ary-prod",    "notequal",     "0", "identicalto",   "combining-tilde", "combining-dot",  "0",
 "arrow","integral",       "line-integral",   "0", "square-root", "partial-deriv", "almostequal",  "0", "lessorequal",   "combining-bar",   "combining-2dot", "0",
 "nabla","double-integral","surface-integral","0", "cube-root",   "prime",         "proportional", "0", "greaterorequal","combining-arrow", "double-prime",   "0",
 "arrow-implies",          "there-does-not-exists", "xor",  "0", "not",    "sup-0", "sub-0",  "0", "sup-n", "join",                 "double-square-union",  "0", 
 "arrow-if-and-only-if",   "unconditionally-true",  "and",  "0", "proves", "sup-i", "sub-1",  "0", "sub-i", "amalgamation",         "double-square-intersection",  "0",   
 "there-exists",           "unconditionally-false", "or",   "0", "models", "sup-j", "sub-2",  "0", "sub-j", "intersection-with-dot","plus-sign-above-equals-sign", "0" }; 
 
///////////////////////////////////////////////////////////////////////////////////////////////////////
// 5 Small Config Buttons between 1 st and 3rd row Red Blue Green SkyBlue Gold

char padLabel[20][2] = {"A", "M", "S", "T", "B", "M", "K", "V", "N", "O", "A", "M", "S", "T", "B", "A", "M", "S", "T", "B"}; 

uint16_t padColor[20]   = {Violet, LGrey, SkyBlue, Pink, Violet, Red,    Blue,  Green4,  SkyBlue, Gold,  
                           Violet, LGrey, SkyBlue, Pink, Violet, Violet, LGrey, SkyBlue,  Pink,   Violet };
uint16_t padOutline[20] = {White, White, White, White, White, White, White, White,  White,   White, 
                           White, White, White, White, White, White, White, White,  White,   White};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Power Restart KeyPad    [Restart  Long Time] [Abort ] [PowerOff  Long Time]        [R-T] [Stp] [O-T] [ ]   
//                         [Restart Short Time] [  Cfg ] [PowerOff Short Time]        [R-t] [Cfg] [O-t] [ ]       
//                         [Restart  Immediate] [Logoff] [PowerOff   Imediate]        [Rst] [Log] [Off] [ ]
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
const static char PowerKeysLabel[12][4] = {"R-T",  "Stp",  "O-T", "R-C",
                                           "R-t",  "Cfg",  "O-t", "O-C",
                                           "Rst",  "Log",  "Off", "   " };
                                           
uint16_t PowerKeysColor[12]             = {Green4, Orange, Pink,  DGrey, 
                                           Green4, Black,  Pink,  DGrey,  
                                           Green2, Blue,   Red,   Black };
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Macro Timer KeyPad      [Repeat  Long Time] [      Stop    ] [Once  Long Time]      [R-T] [Stp] [O-T] [ ]   
//                         [Repeat Short Time] [      Cfg     ] [Once Short Time]      [R-t] [Cfg] [O-t] [ ]       
//                         [Repeat Clock Time] [Repeat x Times] [Once  at   Time]      [R-C] [Rxx] [O-C] [ ]
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
const static char MacroTimerLabel[12][4] = {"R-T",  "Stp",  "O-T", "RcT",
                                            "R-t",  "Cfg",  "O-t", "OcT",
                                            "R-C",  "Rep",  "O-C", "   " };

char TimerStr1[12][3] = { "TR", "tR", "TO", "tO", "CR", "CO", "Cr", "Co", "Rp", "Sp", "  ", "  "  }; // MacroTimer1 -> MacroTimer8 + Rep for optionindicator
char TimerStr2[12][4] = { "R-T","R-t","O-T","O-t","R-C","O-C","RcT","OcT","Rep","Stp","   ","   " }; // MacroTimer1 -> MacroTimer8 + Rep for status line
int TimerStrN[12]     = {  0,    9,    2,    6,    1,    11,   3,    7,    4,    8,    5,    11   }; // Xlat Macro Timer Button Number to Disp Str 
                                           
uint16_t MacroTimerColor[12]            = {Green4, Orange, Pink,  DGrey, 
                                           Green4, Black,  Pink,  DGrey,  
                                           Green2, Blue,   Red,   Black };                                           
                         
char ShutDwn[12][45] = { "C:\\Windows\\System32\\shutdown.exe /r /t 6000",
                         "C:\\Windows\\System32\\shutdown.exe /a",
                         "C:\\Windows\\System32\\shutdown.exe /s /t 6000", "   ",
                         "C:\\Windows\\System32\\shutdown.exe /r /t 30",   "Cfg",
                         "C:\\Windows\\System32\\shutdown.exe /s /t 30", "   ",
                         "C:\\Windows\\System32\\shutdown.exe /r /t 0",
                         "C:\\Windows\\System32\\shutdown.exe /l",
                         "C:\\Windows\\System32\\shutdown.exe /s /t 0",  "   " };  // = shutdown.exe /p
                         
const static char hex16[16][2] = {"0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F"}; 
const static char num16[17][3] = {"1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16"};
static const char* digits = "0123456789ABCDEF";       // dec 2 hex digits positional map
char Int2HexStr[64] = "  ";
                        
bool Rotate180 = false;          // Rotate screen 180 degrees so that USB cable is on other side

bool Math = false;               // Layer for Math KeyBrd
int  MathX = 0;                  // values 0 1 2 3 4 - 4 adds non-printing chars as numbers to KeyBrdByte
byte MathByte[200]= ""; // ={};  // Hold values to be sent
byte MathCode[200];              // Parallel keycodes
int  Math123 = 0;                // Such as Key [abc] = a or b or c - values 0,1,2
int  MathByteNum = 0;            // Size Math KeyBrdByte
char MathChr[38];                // Current Math description = mathhexnumber + description
char MathHexNum[5];              // Current Math Hex Number as ASCII without 0x

bool Fx = false;                 // F1-F12 current
bool Kbrd = false;               // Layer for KeyBrd
int  KeyBrdX = 0;                // values 0 1 2 3 4 - Should start on Page 4 or KeyBrdX = 3 - Macro Tools Page
byte KeyBrdByte[200]= ""; // ={};// Hold values to be sent
byte KeyCode[200];               // Parallel keycodes
int  KeyBrd123 = 0;              // Such as Key [abc] = a or b or c - values 0,1,2
int  KeyBrdF1F24 = 0;            // Single key cycles through all 24 Fnn keys
int  KeyBrdByteNum = 0;          // Size KeyBrdByte
int  KPNum = 0;                  // KeyPad Numbers 0,1 - 9 = 0x62,0x59 - 0x61 
int  BracketsNum = 0;            // Single key cycles through all 8 Brackets symbols
int  SymbolsNum = 0;             // Single key cycles through all 14 symbols
int  StarNum = 0;                // Single key cycles through all Starcodes such as *ca*
int  Numkeys123 = 0;             // Numkeys1 to Numkeys3 - 3 chars for each key such as 3 4 5 

int  PrevButton = 0;             // Used in case-button to check for double key press
char KBDisp[200]= "";   //  ={}; // Keybrd display macro buffer in status line
int  KBDispPos = 0;              // nect char position
bool KBType = false;             // If 1st byte>0x7F use keycode[0-5] else use keyPress
bool KeyRepeat = false;          // If button same a sprevious button pressed
bool KBrdActive = false;         // Let [ADD] know any char key has been pressed at least once before
bool KeyBrdDirect = false;       // Echo Keyboard keys to PC as they are typed
bool DoUpKey = false;            // Set true by SendBytes to let Upkey know valid macro can be saved

int  MacroM1M12[12] = { 0,0,0,0,0,0,0,0,0,0,0,0 };     // Value 0 = values assigned 1 = assigned values 2 = saved
int  MacroS1S12[12] = { 0,0,0,0,0,0,0,0,0,0,0,0 };
int  MacroT1T12[12] = { 0,0,0,0,0,0,0,0,0,0,0,0 };

int  MacroSizeM1M12[12] = { 0,0,0,0,0,0,0,0,0,0,0,0 }; // Byte size of macro of assigned + saved macro
int  MacroSizeS1S12[12] = { 0,0,0,0,0,0,0,0,0,0,0,0 }; 
int  MacroSizeT1T12[12] = { 0,0,0,0,0,0,0,0,0,0,0,0 };

byte  LinkM[12] = { 0,0,0,0,0,0,0,0,0,0,0,0 };          // 0 - 9 Number of linked macro's for each Key 1-12
byte  LinkS[12] = { 0,0,0,0,0,0,0,0,0,0,0,0 };
byte  LinkT[12] = { 0,0,0,0,0,0,0,0,0,0,0,0 };

bool  SaveReadLinks = true;                            // set via *lr* - later default true

#define macroTen  "Macro00","Macro00","Macro00","Macro00","Macro00","Macro00","Macro00","Macro00","Macro00","Macro00"
// [12] keys for each M S T [10] linked macrofilenames length 8 chars  0x00 Linked filenames always MacroNN NN = 01 - 99 
// Size = 12x10x8 = 960 bytes - Note that M,S,T filenames require 10 bytes for name+0x00
char LinkNameM[12][10][8] = {macroTen,macroTen,macroTen,macroTen,macroTen,macroTen,macroTen,macroTen,macroTen,macroTen,macroTen,macroTen};  
char LinkNameS[12][10][8] = {macroTen,macroTen,macroTen,macroTen,macroTen,macroTen,macroTen,macroTen,macroTen,macroTen,macroTen,macroTen};   
char LinkNameT[12][10][8] = {macroTen,macroTen,macroTen,macroTen,macroTen,macroTen,macroTen,macroTen,macroTen,macroTen,macroTen,macroTen};   

char TimerName[6][10] = {"Macro00","Macro00","Macro00","Macro00","Macro00","Macro00"};    

bool MouseKeys = false;          // Mouse Keyboard
bool NumKeys = false;            // Numeric Keyboard
bool PowerKeys = false;          // Bottom row on Cfg Layout 3 Logoff Off Restart
bool PowerKeysMenu = false;      // If true use GUI+X+u,i,r instead of GUI+R+shutdown - change with *po*
int  PowerClock = 0;              // 1 or 2 = Restart on PowerOff on Clock Time callback for Time triggered
bool MacroTimerDisp = false;     // Display Timer Keys if ON 
bool MacroTimer18  = false;      // One of the Timers1-4 has been set Once-of or Repeat Macro by time or by clock (need PicoW?)
bool MacroTimer1 = false;        // Repeat Time
bool Macrotimer2 = false;        // Repeat time
bool MacroTimer3 = false;        // Onceof Time
bool Macrotimer4 = false;        // Onceof time
bool MacroTimer5 = false;        // Repeat Clock Time
bool MacroTimer6 = false;        // Onceof Clock time
bool MacroTimer7 = false;        // Repeat Countdown Clock Time
bool MacroTimer8 = false;        // Onceof Countdown Clock time
int  MacroRepeat = 1;            // Repeat Onceof x times
byte MacroTimerArr1[2] = {0,0};  // Layout = 1,3,4 Key 1-12
byte MacroTimerArr2[2] = {0,0};  // Layout = 1,3,4 Key 1-12
byte MacroTimerArr3[2] = {0,0};  // Layout = 1,3,4 Key 1-12
byte MacroTimerArr4[2] = {0,0};  // Layout = 1,3,4 Key 1-12
byte MacroTimerArr5[2] = {0,0};  // Layout = 1,3,4 Key 1-12
byte MacroTimerArr6[2] = {0,0};  // Layout = 1,3,4 Key 1-12
byte MacroTimerArr7[2] = {0,0};  // Layout = 1,3,4 Key 1-12
byte MacroTimerArr8[2] = {0,0};  // Layout = 1,3,4 Key 1-12
bool PadKeys = false;            // 5 Small Config Buttons on RH side Red Blue Green
bool KeyFontBold = false;        // Button Font Bold/Normal labels
bool KeyFontColour = false;      // FontColour is White/Black
int  KeyFont = White;            // Colour White or Black
int Option1 = 0;                 // Source Macros number 0-11 and 0-99
int Option2 = 0;                 // Destination Macros number 0-11 and 0-99
int MST1 = 4;                    // Source Macro MST1 = 0 1 2 3 4  = M S T mAcro 4 = not defined keys - macro copy + link
int MST2 = 4;                    // Destination Macro MST2 = 0 1 2 3 4  = M S T mAcro 4 = not defined keys - macro copy + link
int NumMode = 0;                 // Used to indicate that [Num] key should define Source (1) or Destination (2) macro
//////////////////////////////////////////////////////////////////////////////////////////////////////
// Check if [Cfg] then [Home][UpArrow][PageUp]  [<][>]  [End][DwnArrow][PageDown] keys pressed
//            StrButton[button]     0  1  2  3  4  5  6  7  8  9  10 11          // 3 7 8 = V+ Vo V-
//                        CfgOK     x  x  x     x  x  x     x  x  x              // Not used with
static const byte CfgButton[12] = { 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0 };        // Cfg key
//////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                StrButton[button]                4  5  6     8  9  10                4  5  6     8  9  10
//  M1-M6 M7-M12 etc            c                  0  1  2     3  4  5                 6  7  8     9  10 11
static const byte StrButton[24] = { 0, 0, 0, 0, 0, 1, 2, 0, 3, 4, 5, 0, 0, 0, 0, 0, 6, 7, 8, 0, 9, 10, 11, 0 };
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Escape sequences \\ = backslash \? = ? \t = tab \b = backspace \" or \'  \x0A = hexadecimal number (byte)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
char str1[] = "https://www.raspberrypi.com/products/raspberry-pi-pico/"; // Strlen is chars+1
char str2[] = "https://github.com/raspberrypi\n\r";
char str3[] = "email.1@email.com";                             
char str4[] = "email.2@email.com";
char str5[] = "sfc /scannow\n";  
char str6[] = "\n\r"; 
char str7[] = "cmd\r";                             
char str8[] = "winver\n";  
char str9[] = "cmd";
char str10[] = "Dism /Online /Cleanup-Image /RestoreHealth"; 
char str11[] = "Dism.exe /online /Cleanup-Image /StartComponentCleanup";                             
char str12[] = "Dism.exe /online /Cleanup-Image /StartComponentCleanup /ResetBase"; 

char * str1to12[] = {str1,str2,str3,str4,str5,str6,str7,str8,str9,str10,str11,str12} ;

const int  StrSize =  200;     // Check if not byte used if mafe larger 
const int  ByteSize = 200;
const byte MaxBytes = StrSize;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
byte Str1[ByteSize],  Str2[ByteSize],  Str3[ByteSize],  Str4[ByteSize],   Str5[ByteSize],   Str6[ByteSize];
byte Str7[ByteSize],  Str8[ByteSize],  Str9[ByteSize],  Str10[ByteSize],  Str11[ByteSize],  Str12[ByteSize];
byte Ttr1[ByteSize],  Ttr2[ByteSize],  Ttr3[ByteSize],  Ttr4[ByteSize],   Ttr5[ByteSize],   Ttr6[ByteSize];
byte Ttr7[ByteSize],  Ttr8[ByteSize],  Ttr9[ByteSize],  Ttr10[ByteSize],  Ttr11[ByteSize],  Ttr12[ByteSize];
byte Mtr1[ByteSize],  Mtr2[ByteSize],  Mtr3[ByteSize],  Mtr4[ByteSize],   Mtr5[ByteSize],   Mtr6[ByteSize];
byte Mtr7[ByteSize],  Mtr8[ByteSize],  Mtr9[ByteSize],  Mtr10[ByteSize],  Mtr11[ByteSize],  Mtr12[ByteSize]; 

byte MacroBuff[ByteSize] = { 0 };
int  MacroBuffSize = ByteSize;

byte * Str1to12[] = {Str1,Str2,Str3,Str4,Str5,Str6,Str7,Str8,Str9,Str10,Str11,Str12} ;   
byte * Ttr1to12[] = {Ttr1,Ttr2,Ttr3,Ttr4,Ttr5,Ttr6,Ttr7,Ttr8,Ttr9,Ttr10,Ttr11,Ttr12} ;
byte * Mtr1to12[] = {Mtr1,Mtr2,Mtr3,Mtr4,Mtr5,Mtr6,Mtr7,Mtr8,Mtr9,Mtr10,Mtr11,Mtr12} ;

char inputString[StrSize] = "Send new text through serial port start with <x end with >\n where x = key number 1 to 6 or 0 for [M4]\n";  // String for [M4] Key
bool StrOK = false;          // String is complete - ended with 0x0A
bool ByteOK = false;         // Bytes received is complete - ended with 0x00
int  RecBytesLen = 0;        // Number of chars in RecBytes
int  RecBytesType = 1;       // Type 1, 2, 3 = sequence, simulataneous, media
byte RecBytes[MaxBytes];     // Raw bytes received must start < and end with > - can have length byte/int after < to include <> in data
byte NumBytes = 0;
bool NewData = false;
char AltNum[] = "1D6D1";

const int NumButtons = 17;             // Total number of buttons = 12 keys (3 rows x 4 columns) + 5 pads on right
TFT_eSPI_Button key[NumButtons];       // Create 12 keys + 5 config buttons for keypad

char datetime_buf[256];
char *datetime_str = &datetime_buf[0]; 
char alarm_buf[256];
char *alarm_str = &alarm_buf[0]; 
char power_buf[256];
char *power_str = &power_buf[0]; 
char timer_buf[256];
char *timer_str = &timer_buf[0]; 
// For rtc alarm set field = -1 then alarm ignore this - for example can then set alarm every 60 minutes by setting all other fileds -1 and min=00-59
// 0 is Sunday, so 5 is Friday - all byte8 length except year which is int16
datetime_t t     = { .year  = 2022, .month = 11, .day   = 03, .dotw  = 4, .hour  = 14,  .min   = 00, .sec   = 00 };  // Clock Date and Time
datetime_t alarm = { .year  = 2022, .month = 11, .day   = 04, .dotw  = 5, .hour  = 15,  .min   = 20, .sec   = 05 };  // Macro Clock Time Repeat+Oneshot
datetime_t power = { .year  = 2022, .month = 11, .day   = 05, .dotw  = 6, .hour  = 16,  .min   = 20, .sec   = 05 };  // Restart-Off Clock Timer
datetime_t timer = { .year  = 2022, .month = 11, .day   = 06, .dotw  = 0, .hour  = 17,  .min   = 20, .sec   = 05 };  // Macro Clock Timer Repeat+Oneshot
static volatile bool alarm_fired = false;  // Set in callback triggers Macro Send xOne xMany based on Clock Time
static volatile bool power_fired = false;  // Set in callback triggers Restart or PowerOff based on Clock Time
static volatile bool timer_fired = false;  // Set in callback triggers Restart or PowerOff based on Clock Timer
bool TimeSet = false;         // true if Clock Time has been set for example <tyymmddwhhmm> through serial port

///////////////////////////////////////
// Setup
///////////////////////////////////////
void setup() {
  
  // From Raspberry Pi Pico examples - seems to do nothing - assignments above still valid
  datetime_t t  = {0};  datetime_t alarm = {0};  datetime_t power = {0};  datetime_t timer = {0};
  rtc_init();
  rtc_set_datetime(&t);  // Use values above lese comms serial <tyymmddwhhmm> 221103w1200 12:00am 3 Nov 2022 w =0 Sunday 6 = Saturday
  
  pinMode(LED_BUILTIN, OUTPUT);   // Used for Capslock
  digitalWrite(LED_BUILTIN, LOW);
  
  Serial.begin(115200);

  if (!LittleFS.begin()) {LittleFS.format(); LittleFS.begin(); } 
  
  InitCfg(1);                     // Setup config values and strings/macros
   
  usb_hid.setPollInterval(2);
  usb_hid.setReportDescriptor(desc_hid_report, sizeof(desc_hid_report));
  usb_hid.setStringDescriptor("TinyUSB HID Composite");
  usb_hid.setReportCallback(NULL, hid_report_callback); // Activate report CapsScrollNum Lock indicators
  usb_hid.begin();                                      // Assume it does -> tusb_init();
  while( !TinyUSBDevice.mounted() ) delay(1);

  // Initialise the TFT screen TFT_eSPI/TFT_eSPI.h
  tft.init();
  if (Rotate180) tft.setRotation(1);
            else tft.setRotation(3);    // Set the display orientation to 1 or 3 (180 degrees different)
  touch_calibrate();                    // New Calibration Data obtained if DoCal = true
  
  tft.fillScreen(Black); 
  if (KeyFontBold) tft.setFreeFont(&FreeSansBold12pt7b);
              else tft.setFreeFont(&FreeSans12pt7b);       
  tft.setTextSize(KEY_TEXTSIZE);
 
  BackLightOn = true;      // TFT init will turn it on

  if (NormVal==0) digitalWrite(LCDBackLight, HIGH);    // Backlight Full ON
            else  analogWrite(LCDBackLight, NormVal);  // Backlight Brightness ON
   
  tft.setTouch(calData);   // void setTouch(uint16_t *data); in  TFT_eSPI/Extensions/Touch.h 
  
  if (SaveLayout) Layout = 2; else Layout = 1;  // Layout 1 (M1-M12 keys default) or 2 (Config)
  VolMuteCount = Layout;                        // Layout change - for on 1st start
  ConfigKeyCount = 0;                           // Start up
  ConfigButtons(0);                             // Draw Buttons and Labels 0 = All 3+5 rows

  NumKeysChange();                              // NumkeysX = Layout 0

  LastMillis = millis();                         // Initial start time
  NewData = KBrdActive = Kbrd = false;
  KeyBrdByteNum = KBDispPos = 0;                // Toggle On/Off with yellow pad in Kbrd mode
  KeyBrdX = 3;                                  // Start on Page 3 - Macro Tools Page
  KeyBrdDirect = PadKeys = false;               // 5 Small Pads RH side Red Blue Green SkyBlue Gold 
  KeyBrd123 = Numkeys123 = 0; 
}

/////////////////////////////
// Main Loop
/////////////////////////////
void loop() {
    
  if (MacroTimer18) CheckMacroTimers();   // Check Macro Timers 1-8 Oneshot Repeating Clocktime
  
  if (power_fired) { power_fired = false; if (PowerClock==1) { DoPowerKeys('r', PowerKeysMenu, 8);  }
                                          if (PowerClock==2) { DoPowerKeys('u', PowerKeysMenu, 10); } PowerClock=0; }
                     
  NowMillis = millis();   // get the current "time" (number of milliseconds since started)
  if ((NowMillis - LastMillis) >= TimePeriod)            //test whether the period has elapsed
     {if (DimVal==0) digitalWrite(LCDBackLight, LOW);    // Backlight Off
               else  analogWrite(LCDBackLight, DimVal);  // Backlight Dimmed
      LastMillis = NowMillis;                            // Start Timer again
      RepLast = RepNow = NowMillis;                      // Reset repeat key timer
      if (!MuteDisable) VolMuteCount = 0;                // VolMuteKey pressed Count = 0 if not L1-L4 keys active
      /*
      if (ConfigKeyCount>0)                       // If [Cfg]->[Kbd] then backlight times out but still shows kbd,
         {ConfigKeyCount = 0;                     // save etc keys even though its back to arrows home end etc
         ConfigButtons(1); }                      // [Cfg][End] or [Cfg][Hme] sequences active
      */
      if (!Kbrd) status("");                      // Clear the status line if KeyBrd not active
      BackLightOn = false;   }                    // Until keypress    
    
  ///////////////////////////////////////////////////////////////////////////////////////////////////////  
  // uint8_t  getTouch(uint16_t *x, uint16_t *y, uint16_t threshold = 600) in TFT_eSPI/Extensions/Touch.h
  // tft.setTouch(calData);              // void setTouch(uint16_t *data); // TFT_eSPI/Extensions/Touch.h 
  ///////////////////////////////////////////////////////////////////////////////////////////////////////
  bool pressed = tft.getTouch(&t_x, &t_y, 450);                              // True if valid key pressed
  
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Check if any key coordinate boxes contain the touch coordinates - no multitouch
  // void TFT_eSPI_Button::press(bool p) { laststate = currstate; currstate = p; }
  // Shorter: key[b].press((pressed && key[b].contains(t_x, t_y)));
  // boolean Adafruit_GFX_Button::contains(int16_t x, int16_t y) 
  //       { return ((x >= _x1) && (x < (_x1 + _w)) && (y >= _y1) && (y < (_y1 + _h))); }
  // justPressed  Query whether the button was pressed since last checked state.  Returns True if was not-pressed before, now is.
  // justReleased Query whether the button was released since last checked state. Returns True if was pressed before, now is not. 
  // isPressed    Query whether the button is currently pressed.                  Returns True if pressed 
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  for (uint8_t b = 0; b < NumButtons; b++) { if (pressed && key[b].contains(t_x, t_y)) key[b].press(true); else key[b].press(false); }                    
   
  // Check if any key changed state void press(bool p); bool isPressed() bool justPressed(); bool justReleased();
  RepNow = millis();   // get the current time
  for (uint8_t b = 0; b < NumButtons; b++) { if (key[b].justReleased()) {key[b].drawButton(false); RepLast = RepNow; }              // draw normal - code at release time
                                     if (key[b].justPressed())  {key[b].drawButton(true); { buttonpress(b); RepLast = millis(); }}  // draw invert - code at press time  
                                     else
                                     if (((RepNow - RepLast) >= RepTimePeriod) && (key[b].isPressed()))                             // code while the button is held
                                          {buttonpress(b); }  }                                                                     // Send the button number

  RecSerial();
  // if (NewData) showRecData();
  if (NewData) DoNewData();   // First char 0,1,2,3,4,5,6 store file, t, a, p clock alarm timer data, 7 to 9 non-ASCII data
  
} // main Loop

/////////////////////////////////////////////////////////////////////////////////////
// Check Macro Timers // TimerName[0-3] = Timer type Repeat T Repeat t Once T Once t
// Timers 1 - 4 are incremented by millis() count Timers 5 - 8 are 0/1 Off/On=Active
/////////////////////////////////////////////////////////////////////////////////////
void CheckMacroTimers()
{ unsigned long TimeNow = millis(); 

  LayoutPrev = Layout;
  
  if (MacroTimer1) { if ((TimeNow - TimeRepeatPrev) >= TimeRepeat)                                  // Repeating MacroTimer1 still ON
                        { TimeRepeatPrev = TimeNow; Layout = MacroTimerArr1[0]; MacroKeys(MacroTimerArr1[1]); optionsindicators(0);} }
                   
  if (Macrotimer2) { if ((TimeNow - timeRepeatPrev) >= timeRepeat)                                  // Repeating MacroTimer2 still ON 
                        { timeRepeatPrev = TimeNow; Layout = MacroTimerArr2[0]; MacroKeys(MacroTimerArr2[1]); optionsindicators(0);} }
    
  if (MacroTimer3) { if ((TimeNow - TimeOnceofPrev) >= TimeOnceof)                                  // Oneshot MacroTimer3 OFF
                        { MacroTimer3 = false; Layout = MacroTimerArr3[0];      MacroKeys(MacroTimerArr3[1]); optionsindicators(0);} }
                     
  if (Macrotimer4) { if ((TimeNow - timeOnceofPrev) >= timeOnceof)                                  // Oneshot MacroTimer4 OFF
                        { Macrotimer4 = false; Layout = MacroTimerArr4[0];      MacroKeys(MacroTimerArr4[1]); optionsindicators(0);} }

  if (alarm_fired) { if (MacroTimer5) { Layout = MacroTimerArr5[0]; MacroKeys(MacroTimerArr5[1]); }  // O-C s 10 and R-C s 8 
                     if (MacroTimer6) { Layout = MacroTimerArr6[0]; MacroKeys(MacroTimerArr6[1]); MacroTimer6 = false; }
                     alarm_fired = false; optionsindicators(0);}   // Repeating Clock MacroTimer5 ON - Oneshot Clock MacroTimer6 OFF
                     
  if (timer_fired) { if (MacroTimer7) { Layout = MacroTimerArr7[0]; MacroKeys(MacroTimerArr7[1]); }  //  RcT s 3 and OcT s 7 
                     if (MacroTimer8) { Layout = MacroTimerArr8[0]; MacroKeys(MacroTimerArr8[1]); MacroTimer8 = false; }
                     timer_fired = false; optionsindicators(0);}   // Repeating Clock MacroTimer7 ON - Oneshot Clock MacroTimer8 OFF
                               
  MacroTimer18 = MacroTimer8 || MacroTimer7 || MacroTimer6 || MacroTimer5 || Macrotimer4 || MacroTimer3 || Macrotimer2 || MacroTimer1; 
  
  if (Layout!=LayoutPrev) { Layout = LayoutPrev; }
  
  // optionsindicators();  // Causes T flicker if put here  
}
/////////////////////////////////
void GetTimeData(datetime_t *a)
/////////////////////////////////
{ if (RecBytes[1]==0x2d)  a->year  = -1; else { a->year  = 2000 + (RecBytes[1]-48)*10 + (RecBytes[2]-48);  }
  if (RecBytes[3]==0x2d)  a->month = -1; else { a->month = (RecBytes[3]-48)*10 + (RecBytes[4]-48); }
  if (RecBytes[5]==0x2d)  a->day   = -1; else { a->day   = (RecBytes[5]-48)*10 + (RecBytes[6]-48); }
  if (RecBytes[7]==0x2d)  a->dotw  = -1; else { a->dotw  = (RecBytes[7]-48); }
  if (RecBytes[8]==0x2d)  a->hour  = -1; else { a->hour  = (RecBytes[8]-48)*10 + (RecBytes[9]-48); }       
  if (RecBytes[10]==0x2d) a->min  = -1;  else { a->min   = (RecBytes[10]-48)*10 + (RecBytes[11]-48); } 
  a->sec = 0;   
}
/////////////////////////////////////////////
// Check for new character strings or macros
/////////////////////////////////////////////
void DoNewData()
{ int n = 0;
  char *ChrPtr;
  byte *BytePtr;
  bool Found = false;  bool tTime = false;  bool aTime = false;  bool pTime = false;  bool wTime = false;
  byte a, c = 0;
  String NameStr;
  int ASize;

  Found = NewData = StrOK = ByteOK = false;
  
  a = RecBytes[0] - 48;  // ASCII Number 0-9 subtract 48 
  tTime = (a==68);       // Was 0x74 = 't' Date-Time setting
  aTime = (a==49);       // Was 0x61 = 'a' Date-Time setting
  pTime = (a==64);       // Was 0x70 = 'p' Date-Time setting
  wTime = (a==71);       // Was 0x77 = 'w' Date-Time setting  
  Found = (a<7);

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //             0 12 34 56 7 89 01
  //            <t 22 11 03 4 14 51>
  // Use serial <t yy mm dd w hh mm> 22110341200 12:00am 3 Nov 2022 Thursday
  // datetime_t t = { .year  = 2022, .month = 11, .day   = 03, .dotw  = 4, .hour  = 14,  .min   = 00, .sec   = 00 };
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (tTime) {t.year  = 2000 + (RecBytes[1]-48)*10 + (RecBytes[2]-48); t.month = (RecBytes[3]-48)*10 + (RecBytes[4]-48); 
              t.day   = (RecBytes[5]-48)*10 + (RecBytes[6]-48);        t.dotw  = (RecBytes[7]-48);
              t.hour  = (RecBytes[8]-48)*10 + (RecBytes[9]-48);        t.min   = (RecBytes[10]-48)*10 + (RecBytes[11]-48); t.sec = 0; 
              rtc_set_datetime(&t); delay(keydelay2); TimeSet = true; status(datetime_str); return; }              
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (aTime) { GetTimeData(&alarm); status("Macro Timer received use [R-C][O-C]"); return; }
  if (wTime) { GetTimeData(&timer); status("Macro Timer received use [RcT][OcT]"); return; }
  if (pTime) { GetTimeData(&power); status("Power Timer received use [O-C][R-C]"); return; } 
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // These are currently the same type of alarm timer as aTime
  // Can be changed to Clock Time countdown timer: add_alarm_in_ms(2000, alarm_callback, NULL, false);
  //                                               add_repeating_timer_ms(500, repeating_timer_callback, NULL, &timer);
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////   
    
  if (Found){ if ((LayerAB==1)&&(a>0)) c = a + 5; else c = a - 1;    // S1-S6=>S7-S12 T1-T6=>T7=T12 M1-M6=>M7=M12

              if (Layout==3) {if (a>0) {StrOK = true;  NameStr = STRname[c]; BytePtr = Str1to12[c]; }   // Keys S1-S12
                                                 else {NameStr = "inputStr"; ChrPtr = inputString; } }  // Key [M4] normal text
              if (Layout==4) {if (a>0) {StrOK = true;  NameStr = TTRname[c]; BytePtr = Ttr1to12[c]; }   // Keys T1-T12
                                  else {ByteOK = true; return; }   }                                    // Key [M4] non-ASCII in RecBytes[] 
              if (Layout==1) {if (a>0) {ByteOK = true; NameStr = MTRname[c]; BytePtr = Mtr1to12[c];}    // Keys M1-M12
                                                 else {NameStr = "inputStr"; ChrPtr = inputString; } }  // Key [M4] normal text
              
              for (n=1; n<=NumBytes; n++) ChrPtr[n-1] = RecBytes[n];     
              for (n=1; n<=NumBytes; n++) BytePtr[n-1] = RecBytes[n]; }   // End if (Found)
              
   if ((a>6)&&(a<10)) {Found = true; n--; RecBytesType = a - 6; ByteOK = true;}        // (6<a<10) Non ASCII start string with char 7 - 9
   
   if ((Found)&&(StrOK)) {DoFileStrings(StrOK, NameStr, ChrPtr);   // Save the text/num strings 
                                  status(ChrPtr); }                // Display the new string       
     
   if ((Found)&&(ByteOK)) {ASize = DoFileBytes(ByteOK, NameStr, BytePtr, NumBytes); } // Save the numbers - check if ASize is ok                                        
}
////////////////////////////////////////////////////////////////////////////////////////////////////
static void alarm_callback(void)  // https://github.com/raspberrypi/pico-examples/blob/master/rtc/
////////////////////////////////////////////////////////////////////////////////////////////////////
// Do not call a non-SDK function from these callbacks - also keep as short as possible
///////////////////////////////////////////////////////////////////////////////////////////////////
{ //char datetime_buf[256]; char *datetime_str = &datetime_buf[0]; datetime_t t = {0};  
  //rtc_get_datetime(&t); datetime_to_str(datetime_str, sizeof(datetime_buf), &t);
  //printf("Alarm Fired At %s\n", datetime_str);
  // stdio_flush(); // if included fail compile
  alarm_fired = true; // Macro Timers 5 and 6
}
///////////////////////////////////
static void power_callback(void)  
///////////////////////////////////
{ 
  power_fired = true;  
}
/////////////////////////////////////
static void timer_callback(void)  
/////////////////////////////////////
{
  timer_fired = true;  // Macro Timers 7 and 8 
}
////////////////////////////////////////////////////////////////////////////////////
void RecSerial() // https://forum.arduino.cc/t/serial-input-basics-updated/382007/3
////////////////////////////////////////////////////////////////////////////////////
{   static boolean InProgress = false;
    static byte n = 0;
    byte StartMarker = 0x3C; 
    byte EndMarker = 0x3E;
    byte b;

    while ((Serial.available()>0) && (!NewData)) 
          { b = Serial.read();
            if (InProgress) { if (b!=EndMarker) { RecBytes[n] = b;
                                                   n++;
                                                   if (n >= MaxBytes) { n = MaxBytes - 1; } }
                              else { RecBytes[n] = '\0'; // terminate the string
                                     InProgress = false;
                                     NumBytes = n;   
                                     n = 0;
                                     NewData = true;
                                   }
                             }
            else if (b == StartMarker) { InProgress = true; }
          }
}
////////////////////////
void showRecData() 
////////////////////////
{   char RecStr[MaxBytes];
    Serial.print(NumBytes);
    Serial.print(" number HEX values ");
    for (byte n = 0; n <= NumBytes; n++) 
        {RecStr[n] = RecBytes[n];
         Serial.print(RecBytes[n], HEX);
         Serial.print(' '); }
    Serial.println();
    Serial.println(RecStr);
    //NewData = false;
}
////////////////////////
void showKeyData() 
////////////////////////
{   byte p, n, m, b;
    char c;
    //String NameStr;

   Serial.println();
   for ( m = 0; m<12; m++ )   // char LinkNameM[12][10][8] -> only Macro01-Macro99 allowed
         { Serial.print(m); Serial.print(" LinkM " ); Serial.print(LinkM[m]); Serial.print(" ");
          for ( n = 0; n < 10; n++) 
              { for (p=0; p<8; p++) {c = LinkNameM[m][n][p]; Serial.print(c); } Serial.print(" "); }
         Serial.println();  }
  
   Serial.println();
   Serial.print(" Buff 20bytes " ); Serial.print(MacroBuffSize); Serial.print(" ");
          for ( n = 0; n <= 20; n++) 
              { b =  MacroBuff[n]; Serial.print(b, HEX); Serial.print(" "); }
          Serial.println(); 

    Serial.println();
    for ( m = 0; m<12; m++ ) 
         { Serial.print(m); Serial.print(" Mtr 20bytes " );  Serial.print(MacroSizeM1M12[m]);  Serial.print(" Mtr Status " ); Serial.print(MacroM1M12[m]); Serial.print(" ");
          for ( n = 0; n < 20; n++) 
              { b =  Mtr1to12[m][n]; Serial.print(b, HEX); Serial.print(" "); }
          Serial.println(); }

   Serial.println();
   for ( m = 0; m<12; m++ ) 
         { Serial.print(m); Serial.print(" Str 20bytes Size " ); Serial.print(MacroSizeS1S12[m]); Serial.print(" Str Status " ); Serial.print(MacroS1S12[m]); Serial.print(" ");
          for ( n = 0; n < 20; n++) 
              { b =  Str1to12[m][n]; Serial.print(b, HEX); Serial.print(" "); }
          Serial.println(); }

   Serial.println();
   for ( m = 0; m<12; m++ ) 
         { Serial.print(m); Serial.print(" Ttr 20bytes Size " ); Serial.print(MacroSizeT1T12[m]); Serial.print(" Ttr Status "); Serial.print(MacroT1T12[m]); Serial.print(" ");
          for ( n = 0; n < 20; n++) 
              { b =  Ttr1to12[m][n]; Serial.print(b, HEX); Serial.print(" "); }
         Serial.println();  }        
         
 }
///////////////////////////////////////////////
void DoPowerKeys (char Cmd, bool Menu, int s) 
///////////////////////////////////////////////
{ uint8_t keycode[6] = { 0 };     // simultaneous keys pressed in here                  
  int n = 0;                      // s = button pressed

  if (s==3||s==7) {PowerKeys = false; rtc_set_alarm(&power, &power_callback); ConfigButtons(1); return;} // s 3 PowerClock=1 s 7 PowerClock=2
   
  if ((s>7)||(s==1)) { PowerKeys = false; // If immediate action Must do this before the actions below
                       status(" ");  ConfigButtons(1);  }
   
  if ((Menu)&&(s>0)) { keycode[0] = GuiL;             // or use HID_KEY_GUI_RIGHT
                       keycode[1] = HID_KEY_X;        // 'X' or 'x' not the same here
                       usb_hid.keyboardReport(HIDKbrd, 0, keycode);   delay(keydelay2); // GUI + X then U for shutdown options
                       usb_hid.keyboardRelease(HIDKbrd);              delay(keydelay2); // Confirm with ArrowLeft Enter 
                       usb_hid.keyboardPress(HIDKbrd, 'u');           delay(keydelay2); // After U it is i u r
                       usb_hid.keyboardRelease(HIDKbrd);              delay(keydelay1); 
                       usb_hid.keyboardPress(HIDKbrd, Cmd);           delay(keydelay2); // logoff switch off restart
                       usb_hid.keyboardRelease(HIDKbrd);              delay(keydelay1); return; }
              
          
  keycode[0] = GuiL;             // or use HID_KEY_GUI_RIGHT
  keycode[1] = HID_KEY_R;        // 'R' or 'r' not the same here
  usb_hid.keyboardReport(HIDKbrd, 0, keycode);   delay(keydelay2); // GUI + X then U for shutdown options
  usb_hid.keyboardRelease(HIDKbrd);              delay(keydelay2);  
  
  for (n=0;  n<strlen(ShutDwn[s]); n++) {usb_hid.keyboardPress(HIDKbrd, ShutDwn[s][n]); delay(keydelay2);
                                         usb_hid.keyboardRelease(HIDKbrd);              delay(keydelay2); } 
                            
  usb_hid.keyboardPress(HIDKbrd, '\r'); delay(keydelay2);           // Confirm with Enter
  usb_hid.keyboardRelease(HIDKbrd);

  if (PowerKeys) { PowerKeys = false; // Clear Power Keys for Timed action
                   status("Timer running");
                   ConfigButtons(1); }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Do Button Actions Layouts 1 to 4
// Three HID Keys action types: 
// (1) Keyboard Keys pressed in sequence - usb_hid.keyboardPress(RID_KEYBOARD, keyvar) - keyvar single variable - only ASCII type keys
//     see: https://github.com/adafruit/Adafruit_TinyUSB_Arduino/issues/50
// (2) Keyboard Keys pressed together    - usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode) - keycode 0 - 5 i.e. 6 keys pressed together
//     If modifier used i.e (0xEn keys become 0x0n) use tud_hid_keyboard_report(HIDKbrd, modifier, keycode); to press and send keycode[0]=0x00
//     tud_task(); tud_hid_keyboard_report(HIDKbrd, 0x00, keycode); to release - see Cut Copy Paste in M1 and Control Shift Escape in M6 below - 
//     also see https://forums.raspberrypi.com/viewtopic.php?t=311221
// (3) Keyboard Key 1 or more held + others typed in sequence - Alt + Numpad for Math sym,bols + special characters eg. Alt-Hold + 2, 3 - use
//     tud_hid_keyboard_report(HIDKbrd, modifier, keycode); do not usb_hid.keyboardReport
// (4) Media Keys pressed in sequence    - usb_hid.sendReport16(RID_CONSUMER_CONTROL, mediakeyvar) - mediakeyvar single variable
// The challenge with macro instructions is then to know how to deal with each type (1) to (4) or any combination of (1) to (4)
// Generally if non-test macro has Shift, Alt, GUI, Ctrl as the first key then it will be of type (2) or (3) (at least initially)
// If any other first key then is a normal keyboard key it will be of type (1) initially
// If any other first media key then is type (4) initially 
//
// typedef struct TU_ATTR_PACKED {
//  uint8_t modifier;   /**< Keyboard modifier (KEYBOARD_MODIFIER_* masks). */
//  uint8_t reserved;   /**< Reserved for OEM use, always set to 0. */
//  uint8_t keycode[6]; /**< Key codes of the currently pressed keys. */
//  } hid_keyboard_report_t;
//
#define ModCtrL 0x01 // 0xEn -> 0x0n for modifiers
#define ModCtrR 0x10 // Can just add (instead of or)
#define ModShfL 0x02
#define ModShfR 0x20
#define ModAltL 0x04
#define ModAltR 0x40
#define ModWinL 0x08 
#define ModWinR 0x80
#define ModGuiL 0x08 // Super Key Left
#define ModGuiR 0x80 // Super Key Right
//
//   Key modifiers as defined by the IEEE HID Spec:
//    LEFT  CONTROL          1    # 00000001 as binary
//    LEFT  SHIFT            2    # 00000010
//    LEFT  ALT              4    # 00000100
//    LEFT  CMD|WIN          8    # 00001000
//    RIGHT CONTROL          16   # 00010000
//    RIGHT SHIFT            32   # 00100000
//    RIGHT ALT              64   # 01000000
//    RIGHT CMD|WIN          128  # 10000000
//
//  Keyboard modifier codes bitmap
//  typedef enum {
//  KEYBOARD_MODIFIER_LEFTCTRL   = TU_BIT(0), ///< Left Control
//  KEYBOARD_MODIFIER_LEFTSHIFT  = TU_BIT(1), ///< Left Shift
//  KEYBOARD_MODIFIER_LEFTALT    = TU_BIT(2), ///< Left Alt
//  KEYBOARD_MODIFIER_LEFTGUI    = TU_BIT(3), ///< Left Window
//  KEYBOARD_MODIFIER_RIGHTCTRL  = TU_BIT(4), ///< Right Control
//  KEYBOARD_MODIFIER_RIGHTSHIFT = TU_BIT(5), ///< Right Shift
//  KEYBOARD_MODIFIER_RIGHTALT   = TU_BIT(6), ///< Right Alt
//  KEYBOARD_MODIFIER_RIGHTGUI   = TU_BIT(7)  ///< Right Window
//  }hid_keyboard_modifier_bm_t;
//                                         modifier reserved=0 keycode[6]
//  usb_hid.keyboardReport(RID_KEYBOARD=1, 0,                  keycode)
//
//  usb_hid.keyboardReport(KEYBOARD_MODIFIER_LEFTSHIFT | KEYBOARD_MODIFIER_LEFTALT, 0, 0); // press shift + alt
//  USB keyboards HID Report Descriptor:
// Bit:        7   6   5   4   3   2   1   0
//           +---+---+---+---+---+---+---+---+
// Byte 0    | RG| RA| RS| RC| LG| LA| LS| LC|  Modifier bits (LC=Left Control, LS= Left Shift, etc)
//           +---+---+---+---+---+---+---+---+
// Byte 1    |        Reserved byte          |
//           +---+---+---+---+---+---+---+---+
// Byte 2    |        Key 1                  |
//           +---+---+---+---+---+---+---+---+
// Byte 3    |        Key 2                  |
//           +---+---+---+---+---+---+---+---+
// Byte 4    |        Key 3                  |
//           +---+---+---+---+---+---+---+---+
// Byte 5    |        Key 4                  |
//           +---+---+---+---+---+---+---+---+
// Byte 6    |        Key 5                  |
//           +---+---+---+---+---+---+---+---+
// Byte 7    |        Key 6                  |
//           +---+---+---+---+---+---+---+---+
// https://stackoverflow.com/questions/66671427/multiple-modifiers-2-in-keyboard-input-report-for-custom-hid-keyboard
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MacroKeys(byte c)       
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////     
// Use while != 0x00 for String type macros because Size in MacroSizeM1M12[1-12] could be 0 and 
// MacroM1M12[1-12] could also be 0 if the macro is received via serial port or via *fm,s,t* 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{uint8_t keycode[6] = { 0 };      // simultaneous keys pressed in here
 uint8_t a, n, b; 
 
 //showKeyData();

 if (Layout==3) { if (Str1to12[c][0]==0x00) return false; // S1-S12 Assume NULL as first byte means no content  
                  if (Str1to12[c][0]<0x80) { n = 0; while (Str1to12[c][n]!=0) { usb_hid.keyboardPress(HIDKbrd, Str1to12[c][n]); delay(keydelay2);
                                                                                usb_hid.keyboardRelease(HIDKbrd);               delay(keydelay2); 
                                                                                n++; if (n>ByteSize) break; } return true; }
                  if (Str1to12[c][0]>0x7F) { for (n=0;  n<6; n++) {keycode[n] = Str1to12[c][n]; if (keycode[n]==0) break; }     // allow one 0x00 in keycode
                                                                                usb_hid.keyboardReport(HIDKbrd, 0, keycode);    delay(keydelay2); 
                                                                                usb_hid.keyboardRelease(HIDKbrd);               delay(keydelay2); 
                                                                                return true; }  }                               //if (Layout==3)

 if (Layout==4) { if (Ttr1to12[c][0]==0x00) return false; // T1-T12 Assume NULL as first byte means no content  
                  if (Ttr1to12[c][0]<0x80) { n = 0; while (Ttr1to12[c][n]!=0) { usb_hid.keyboardPress(HIDKbrd, Ttr1to12[c][n]); delay(keydelay2);
                                                                                usb_hid.keyboardRelease(HIDKbrd);               delay(keydelay2); 
                                                                                n++; if (n>ByteSize) break; } return true; }
                  if (Ttr1to12[c][0]>0x7F) { for (n=0;  n<6; n++) {keycode[n] = Ttr1to12[c][n]; if (keycode[n]==0) break; }     // allow one 0x00 in keycode
                                                                                usb_hid.keyboardReport(HIDKbrd, 0, keycode);    delay(keydelay2); 
                                                                                usb_hid.keyboardRelease(HIDKbrd);               delay(keydelay2); 
                                                                                return true; }  }                               // if (Layout==4)

 if (Layout==1) { if (Mtr1to12[c][0]==0x00) return false; // M1-M12 Assume NULL as first byte means no content                    
                  if (Mtr1to12[c][0]<0x80) { n = 0; while (Mtr1to12[c][n]!=0) { usb_hid.keyboardPress(HIDKbrd, Mtr1to12[c][n]); delay(keydelay2);
                                                                                usb_hid.keyboardRelease(HIDKbrd);               delay(keydelay2); 
                                                                                n++; if (n>ByteSize) break; } return true; }
                  if (Mtr1to12[c][0]>0x7F) { for (n=0;  n<6; n++) {keycode[n] = Mtr1to12[c][n]; if (keycode[n]==0) break; }     // allow one 0x00 in keycode
                                                                                usb_hid.keyboardReport(HIDKbrd, 0, keycode);    delay(keydelay2); 
                                                                                usb_hid.keyboardRelease(HIDKbrd);               delay(keydelay2); 
                                                                                return true; }  }                               // if (Layout==1) 
 return false;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MacroLinkM (byte c)  // Only 9 chained but can be up to 10
////////////////////////////////////////////////////////////////////////////////////////////////////////
// For a repeat macro function use LinkRepM[12] with each element = number of repeats say 1-1000
// Then do the repeat in a for loop here for each File fx
////////////////////////////////////////////////////////////////////////////////////////////////////////
{byte n;
 File f1 = LittleFS.open(LinkNameM[c][0], "r"); n=0; while (f1.available()) { Mtr1to12[c][n] = f1.read(); n++; } f1.close(); 
 MacroM1M12[c]==2; MacroSizeM1M12[c] = n-1; MacroKeys(c);
 File f2 = LittleFS.open(LinkNameM[c][1], "r"); n=0; while (f2.available()) { Mtr1to12[c][n] = f2.read(); n++; } f2.close(); 
 MacroM1M12[c]==2; MacroSizeM1M12[c] = n-1; MacroKeys(c); if (LinkM[c]==1) return true; 
 File f3 = LittleFS.open(LinkNameM[c][2], "r"); n=0; while (f3.available()) { Mtr1to12[c][n] = f3.read(); n++; } f3.close(); 
 MacroM1M12[c]==2; MacroSizeM1M12[c] = n-1; MacroKeys(c);  if (LinkM[c]==2) return true; 
 File f4 = LittleFS.open(LinkNameM[c][3], "r"); n=0; while (f4.available()) { Mtr1to12[c][n] = f4.read(); n++; } f4.close(); 
 MacroM1M12[c]==2; MacroSizeM1M12[c] = n-1; MacroKeys(c);  if (LinkM[c]==3) return true;
 File f5 = LittleFS.open(LinkNameM[c][4], "r"); n=0; while (f5.available()) { Mtr1to12[c][n] = f5.read(); n++; } f5.close(); 
 MacroM1M12[c]==2; MacroSizeM1M12[c] = n-1; MacroKeys(c);  if (LinkM[c]==4) return true;
 File f6 = LittleFS.open(LinkNameM[c][5], "r"); n=0; while (f6.available()) { Mtr1to12[c][n] = f6.read(); n++; } f6.close(); 
 MacroM1M12[c]==2; MacroSizeM1M12[c] = n-1; MacroKeys(c);  if (LinkM[c]==5) return true; 
 File f7 = LittleFS.open(LinkNameM[c][6], "r"); n=0; while (f7.available()) { Mtr1to12[c][n] = f7.read(); n++; } f7.close(); 
 MacroM1M12[c]==2; MacroSizeM1M12[c] = n-1; MacroKeys(c);  if (LinkM[c]==6) return true;
 File f8 = LittleFS.open(LinkNameM[c][7], "r"); n=0; while (f8.available()) { Mtr1to12[c][n] = f8.read(); n++; } f8.close(); 
 MacroM1M12[c]==2; MacroSizeM1M12[c] = n-1; MacroKeys(c);  if (LinkM[c]==7) return true;  
 File f9 = LittleFS.open(LinkNameM[c][8], "r"); n=0; while (f9.available()) { Mtr1to12[c][n] = f9.read(); n++; } f9.close(); 
 MacroM1M12[c]==2; MacroSizeM1M12[c] = n-1; MacroKeys(c);  if (LinkM[c]==8) return true;

 return false; 
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MacroLinkS (byte c)  // Only 5 chained but can be up to 10
////////////////////////////////////////////////////////////////////////////////////////////////////////
{byte n;
 File f1 = LittleFS.open(LinkNameS[c][0], "r"); n=0; while (f1.available()) { Str1to12[c][n] = f1.read(); n++; } f1.close(); 
 MacroS1S12[c]==2; MacroSizeS1S12[c] = n-1; MacroKeys(c);
 File f2 = LittleFS.open(LinkNameS[c][1], "r"); n=0; while (f2.available()) { Str1to12[c][n] = f2.read(); n++; } f2.close(); 
 MacroS1S12[c]==2; MacroSizeS1S12[c] = n-1; MacroKeys(c); if (LinkS[c]==1) return true; 
 File f3 = LittleFS.open(LinkNameS[c][2], "r"); n=0; while (f3.available()) { Str1to12[c][n] = f3.read(); n++; } f3.close(); 
 MacroS1S12[c]==2; MacroSizeS1S12[c] = n-1; MacroKeys(c);  if (LinkS[c]==2) return true; 
 File f4 = LittleFS.open(LinkNameS[c][3], "r"); n=0; while (f4.available()) { Str1to12[c][n] = f4.read(); n++; } f4.close(); 
 MacroS1S12[c]==2; MacroSizeS1S12[c] = n-1; MacroKeys(c);  if (LinkS[c]==3) return true;
 File f5 = LittleFS.open(LinkNameS[c][4], "r"); n=0; while (f5.available()) { Str1to12[c][n] = f5.read(); n++; } f5.close(); 
 MacroS1S12[c]==2; MacroSizeS1S12[c] = n-1; MacroKeys(c);  if (LinkS[c]==4) return true; 

 return false; 
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MacroLinkT (byte c)  // Only 5 chained but can be up to 10
////////////////////////////////////////////////////////////////////////////////////////////////////////
{byte n;
 File f1 = LittleFS.open(LinkNameT[c][0], "r"); n=0; while (f1.available()) { Ttr1to12[c][n] = f1.read(); n++; } f1.close(); 
 MacroS1S12[c]==2; MacroSizeT1T12[c] = n-1; MacroKeys(c);
 File f2 = LittleFS.open(LinkNameT[c][1], "r"); n=0; while (f2.available()) { Ttr1to12[c][n] = f2.read(); n++; } f2.close(); 
 MacroS1S12[c]==2; MacroSizeT1T12[c] = n-1; MacroKeys(c); if (LinkT[c]==1) return true; 
 File f3 = LittleFS.open(LinkNameT[c][2], "r"); n=0; while (f3.available()) { Ttr1to12[c][n] = f3.read(); n++; } f3.close(); 
 MacroS1S12[c]==2; MacroSizeT1T12[c] = n-1; MacroKeys(c);  if (LinkT[c]==2) return true; 
 File f4 = LittleFS.open(LinkNameT[c][3], "r"); n=0; while (f4.available()) { Ttr1to12[c][n] = f4.read(); n++; } f4.close(); 
 MacroS1S12[c]==2; MacroSizeT1T12[c] = n-1; MacroKeys(c);  if (LinkT[c]==3) return true;
 File f5 = LittleFS.open(LinkNameT[c][4], "r"); n=0; while (f5.available()) { Ttr1to12[c][n] = f5.read(); n++; } f5.close(); 
 MacroS1S12[c]==2; MacroSizeT1T12[c] = n-1; MacroKeys(c);  if (LinkT[c]==4) return true;

 return false; 
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PadKeysState(int Pbutton, bool Restore)  // Pads are now Pbuttons 1 to 5
{ if (Restore) { if (Pbutton==3) { VolDisable = VolDisablePrev; MuteDisable = MuteDisablePrev; }
                 if (Pbutton<3||Pbutton==4) Media = MediaPrev;                                 }                            
          else { if (Pbutton==3) { VolDisablePrev = VolDisable; MuteDisablePrev = MuteDisable;    }
                 if (Pbutton==1) { MediaPrev = Media; Kbrd = Media = NumKeys = MouseKeys = false; }
                 if (Pbutton==2) { MediaPrev = Media; Math = Media = NumKeys = MouseKeys = false; }
                 if (Pbutton==3) {                    Math = Kbrd  = NumKeys = MouseKeys = false; }                   
                 if (Pbutton==4) { MediaPrev = Media; Math = Media = Kbrd    = MouseKeys = false; }
                 if (Pbutton==5) { ;}  } // used as an option key
  ConfigButtons(1);
  if (Kbrd) status("[NXT]Page [ADD]Char [EXE]Macro"); else status(" ");
}
/////////////////////////////
void buttonpress(int button)
/////////////////////////////
{
  uint8_t keycode[6] = { 0 };     // simultaneous keys pressed in here
  uint8_t i, n, a, b, c;          // 
  uint8_t CfgOK ;                 // Do Cfg actions
  uint8_t modifiers = 0;          // Alt Shift Control Gui modifiers b7 RG| RA| RS| RC| LG| LA| LS| LC| b0
  byte *BPtr;
  
  if ( !usb_hid.ready() ) return; // assume it includes -> if (tud_hid_ready()) 
  
  /////////////////////////////////////////////////////////// Skip 1st any-key keypress which re-activates backlight
  LastMillis = millis();                                   // Reset Backlight Timer if any key pressed                                        
  if (!BackLightOn)                                        // Keypress only active if backlight on
     {if (NormVal==0) digitalWrite(LCDBackLight, HIGH);    // Backlight Full ON
                else  analogWrite(LCDBackLight, NormVal);  // Backlight Brightness ON
      BackLightOn = true;                                  //
      return; }                                            //
  
  // Macro Timers must either be executed (buttons 1 or 0,2,3,4,6,7,8,9,10) or cancelled (button 5 or 11)  
  if (MacroTimerDisp) { if (button==11) { if (MacroTimer18)  status("Macro Timer running in background");  
                                          if (!MacroTimer18) status("No Macro Timer running");
                                          MacroTimerDisp=false; ConfigButtons(1); return; } }
  // Power Keys must either be executed (buttons 1 or 0,2,3,4,6,7,8,9,10) or cancelled (button 5 or 11)
  if (PowerKeys) {if (button==11) {status("Power Keys exit"); PowerKeys=false; ConfigButtons(1); return;} }
  
  ///////////////////////////////////////////////////////////////////////////////
  // 5 Config Pads Red Blue Green SkyBlue Gold button > 11
  // Handle 1st then return - as if button = 0-11
  // Pad Keys switch off all other KeyBrds except restore MediaKeys if on before
  ///////////////////////////////////////////////////////////////////////////////
  if ((button>11)&&(Layout==2)) 
     { PadKeys = true; 
       if (button==12) { Math = !Math;           PadKeysState(button-11, !Math);      return; } // Pbutton = 1 to 5                       
       if (button==14) { Media = !Media;         PadKeysState(button-11, !Media);     return; }
       if (button==15) { NumKeys = !NumKeys;     PadKeysState(button-11, !NumKeys);   return; }       
       if (button==16) { if (Kbrd) {KeyBrdDirect = !KeyBrdDirect; optionsindicators(10); // 10 = space
                                    if (KeyBrdDirect) status("KeyBoard Direct ON"); else status("KeyBoard Direct OFF");}
                         if (NumKeys) { if (Numkeys123<2) Numkeys123++ ; else Numkeys123 = 0; NumKeysChange(); ConfigButtons(1); }
                         return; }       
       Kbrd = !Kbrd; SendBytesEnd(0); PadKeysState(button-11, !Kbrd); return;  }

  if ((button>11)&&(Layout!=2))
     {if (button==12 || button==16) { LayerAB = (LayerAB==0); PadKeys = true; LayerABStr[6] = 65 + LayerAB; status(LayerABStr); ConfigButtons(1); return; }
      Layout = button-11-(button==13); PadKeys = true; status(" "); ConfigButtons(1); return; }     
                                  
  n = 0;                          // for the while loops
  KeyRepeat = false;                                            
  if (PrevButton!=button) KeyBrd123 = 0; else KeyRepeat = true; // Start at a not b or c if other button previously used
  PrevButton = button;
    
  ///////////////////////////////////////////////////////////////////////////////////////////////////
  // StrButton[button]                 4  5  6     8  9  10                4  5  6     8  9   10
  //                c                  0  1  2     3  4  5                 6  7  8     9  10  11
  //byte StrButton[24] = { 0, 0, 0, 0, 0, 1, 2, 0, 3, 4, 5, 0, 0, 0, 0, 0, 6, 7, 8, 0, 9, 10, 11, 0 };
  c = StrButton[button + (LayerAB*12)];   // c = 0 - 11 map button 0-11 to macrokeys 0-11->1-12
  ///////////////////////////////////////////////////////////////////////////////////////////////////

  //if ((!MuteDisable)&&(VolMuteCount>0)&&(button!=7)) {VolMuteCount = 0; // Any other key resets Mute pressed count
  //                                    VolMuteMillisLast  = millis(); }  // Set Timer = now 5 seconds to select next Layout
 
  //////////////////////////////////////////////////////////////////////////////////////////////////////////
  // reset if not [Cfg] and [Home] [UpArrow] [PageUp] [<] [Cfg] [>] [End] [DwnArrow] [PageDown] key pressed
  // StrButton[button]      0  1  2  3  4  5  6  7  8  9  10 11
  //             CfgOK      x  x  x     x  x  x     x  x  x 
  // byte CfgButton[12] = { 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0 };
  CfgOK = CfgButton[button];
  if ((Layout==2) && (CfgOK==0) && (ConfigKeyCount>0)) {ConfigKeyCount = 0; status(" "); } 
  ////////////////////////////////////////////////////////////////////////////////////////////////////////// 
                                                           
  if (Layout!=2) {   // Layout 1 or 3 or 4   
  switch(button){
    case 0: ///////////////////////////////////////////////// Cut = control + x   
      keycode[0] = HID_KEY_X; 
      tud_hid_keyboard_report(HIDKbrd, ModCtrL, keycode); delay(keydelay); 
      tud_task();   // call before releasing the keys
      keycode[0] = 0x00;
      tud_hid_keyboard_report(HIDKbrd, 0x00, keycode);

      //keycode[0] = CtrL; keycode[1] = HID_KEY_X;  
      //usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay); 
      //usb_hid.keyboardRelease(HIDKbrd);  break;
    case 1: ///////////////////////////////////////////////// Copy = control + c
      keycode[0] = HID_KEY_C; 
      tud_hid_keyboard_report(HIDKbrd, ModCtrL, keycode); delay(keydelay); 
      tud_task();   // call before releasing the keys
      keycode[0] = 0x00;
      tud_hid_keyboard_report(HIDKbrd, 0x00, keycode);

      //keycode[0] = CtrL; keycode[1] = HID_KEY_C; 
      //usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay); 
      //usb_hid.keyboardRelease(HIDKbrd);  break;
    case 2: ////////////////////////////////////////////////// Paste = control + v
      keycode[0] = HID_KEY_V; 
      tud_hid_keyboard_report(HIDKbrd, ModCtrL, keycode); delay(keydelay); 
      tud_task();   // call before releasing the keys
      keycode[0] = 0x00;
      tud_hid_keyboard_report(HIDKbrd, 0x00, keycode);

      //keycode[0] = CtrL; keycode[1] = HID_KEY_V;  
      //usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay); 
      //usb_hid.keyboardRelease(HIDKbrd);  break;
    case 3: /////////////////////////////////////////////////// Volume Increase
      if (VolDisable) {if (DelBS) keycode[0] = BckS; else keycode[0] = DelK;  //Delete or BackSpace Key
                       usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay); 
                       usb_hid.keyboardRelease(HIDKbrd); break; }
    
      usb_hid.sendReport16(HIDCons, VolUp);  delay(keydelay);
      usb_hid.sendReport16(HIDCons, 0);      break;
      
    case 4: // M1 S1 T1 ///////////////////////////////////////////////// Alt + PrintScreen - some mini keyboards cannot do this           
         if (Layout==3) 
            { if (LinkS[c]==0) { if (MacroKeys(c)) break; }
              if (LinkS[c]>0)  { if (MacroLinkS(c)) break; } }    
         if (Layout==4) 
            { if (LinkT[c]==0) { if (MacroKeys(c)) break; }
              if (LinkT[c]>0)  { if (MacroLinkT(c)) break; } }

        if (Layout==1) 
            { if (LinkM[c]==0) { if (MacroKeys(c)) break; }
              if (LinkM[c]>0)  { if (MacroLinkM(c)) break; }     

              keycode[0] = AltL; 
              keycode[1] = PrtScr;
              keycode[2] = 0x00; // Need this why? 
              usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay);
              usb_hid.keyboardRelease(HIDKbrd);            delay(keydelay2);
              status("PrtScr Current Window");   }  break; // Layout==1
      
    case 5: // M2 S2 T2 ///////////////////////////////////////////////// Open admin powershell including UAC completion
         if (Layout==3) 
            { if (LinkS[c]==0) { if (MacroKeys(c)) break; }
              if (LinkS[c]>0)  { if (MacroLinkS(c)) break; } }    
         if (Layout==4) 
            { if (LinkT[c]==0) { if (MacroKeys(c)) break; }
              if (LinkT[c]>0)  { if (MacroLinkT(c)) break; } }

        if (Layout==1) 
            { if (LinkM[c]==0) { if (MacroKeys(c)) break; }
              if (LinkM[c]>0)  { if (MacroLinkM(c)) break; }

             keycode[0] = GuiL;                                               // or use HID_KEY_GUI_RIGHT
             keycode[1] = HID_KEY_X;
             usb_hid.keyboardReport(HIDKbrd, 0, keycode);   delay(keydelay2); // GUI + X then A admin powershell i normal powershell g Computer Management
             usb_hid.keyboardRelease(HIDKbrd);              delay(keydelay2); // Confirm with ArrowLeft Enter 
             usb_hid.keyboardPress(HIDKbrd, 'A');           delay(keydelay2); // Can also use Alt + "Y" too confirm
             usb_hid.keyboardRelease(HIDKbrd);              delay(keydelay3); // Can also open via Run GUI + R then Ctrl+Shft+Enter then Alt+Y
             keycode[0] = ArrLeft;
             keycode[1] = KeyEnter;
             usb_hid.keyboardReport(HIDKbrd, 0, keycode);   delay(keydelay2); 
             usb_hid.keyboardRelease(HIDKbrd);   } break; // Layout==1
      
    case 6: // M3 S3 T3 /////////////////////////////////////////////// Open Run window with last coommand visible and selected
         if (Layout==3) 
            { if (LinkS[c]==0) { if (MacroKeys(c)) break; }
              if (LinkS[c]>0)  { if (MacroLinkS(c)) break; } }    
         if (Layout==4) 
            { if (LinkT[c]==0) { if (MacroKeys(c)) break; }
              if (LinkT[c]>0)  { if (MacroLinkT(c)) break; } }

        if (Layout==1) 
            { if (LinkM[c]==0) { if (MacroKeys(c)) break; }
              if (LinkM[c]>0)  { if (MacroLinkM(c)) break; }

             keycode[0] = GuiL;                 // or use HID_KEY_GUI_RIGHT
             keycode[1] = HID_KEY_X;
             usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay);
             usb_hid.keyboardRelease(HIDKbrd);     }  break; // Layout==1
      
    case 7: //////////////////////////////////////////////// Volume Mute and change Layouts
      VolMuteCount++; if (VolMuteCount>4) VolMuteCount=1; Layout = VolMuteCount; ConfigButtons(1); delay(keydelay);    
      if (MuteDisable) break;  // No VolumeMute action toggle with [Cfg][Hme] sequence

      usb_hid.sendReport16(HIDCons, VolMute); delay(keydelay2);
      usb_hid.sendReport16(HIDCons, 0);       delay(keydelay2); break;
      
    case 8: // M4 S4 T4 ///////////////////////////////////////////// Send a text or macro - could overide by sending serial 0text or macro
         if (Layout==3) 
            { if (LinkS[c]==0) { if (MacroKeys(c)) break; }
              if (LinkS[c]>0)  { if (MacroLinkS(c)) break; } }    
         if (Layout==4) 
            { if (LinkT[c]==0) { if (MacroKeys(c)) break; }
              if (LinkT[c]>0)  { if (MacroLinkT(c)) break; } }

        if (Layout==1) 
            { if (LinkM[c]==0) { if (MacroKeys(c)) break; }
              if (LinkM[c]>0)  { if (MacroLinkM(c)) break; }
          
      ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      //                         <    0   CtrL 0xE0 + ShfL 0xE1 + Esc 0x29 >
      //                                     224         225        41
      // Macro 0x3C 0x30 0xE0 0xE1 0x29 0x3E <0 Control Shift Escape > with Layer 4 visible press [M4] will open the Task Manager
      ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      /*
      if (ByteOK)       { // <0777> shows 4 number HEX values 30 37 37 37 0     0777    RecBytes[1] 55 which is correct
                          // Must do this while pink layer is active
                          keycode[0] = RecBytes[1];     // 0xE0 224 RecBytes[0] = 7,8,9 ASCII
                          keycode[1] = RecBytes[2];     // 0xE1 225
                          keycode[2] = RecBytes[3];     // 0x29 41
                          keycode[3] = KeyEnter;
                          usb_hid.keyboardReport(HIDKbrd, 0, keycode);      delay(keydelay2);
                          usb_hid.keyboardRelease(HIDKbrd);                 delay(keydelay2); break;} 
      */
                          
    for (n=0;  n<strlen(inputString); n++) {usb_hid.keyboardPress(HIDKbrd, inputString[n]); delay(keydelay2);
                                            usb_hid.keyboardRelease(HIDKbrd);               delay(keydelay2); }   }  break; // Layout==1                                                        
    case 9: // M5 S5 T5 //////////////////////////////////////////// Open normal command prompt via Run window
         if (Layout==3) 
            { if (LinkS[c]==0) { if (MacroKeys(c)) break; }
              if (LinkS[c]>0)  { if (MacroLinkS(c)) break; } }    
         if (Layout==4) 
            { if (LinkT[c]==0) { if (MacroKeys(c)) break; }
              if (LinkT[c]>0)  { if (MacroLinkT(c)) break; } }

        if (Layout==1) 
            { if (LinkM[c]==0) { if (MacroKeys(c)) break; }
              if (LinkM[c]>0)  { if (MacroLinkM(c)) break; }
          
             keycode[0] = GuiL;             // Admin CMD/Powershell: GUI + R then cmd/powershell then instead of Enter use Ctrl+Shft+Enter then Alt+Y
             keycode[1] = HID_KEY_R;        // 
             usb_hid.keyboardReport(HIDKbrd, 0, keycode);      delay(keydelay2);
             usb_hid.keyboardRelease(HIDKbrd);                 delay(keydelay2);
      
            for (n=0;  n<ArraySz(str9); n++)                        // str7 Normal Cmd Prompt str8 winver
                {usb_hid.keyboardPress(HIDKbrd, str9[n]);     delay(keydelay2);
                 usb_hid.keyboardRelease(HIDKbrd);            delay(keydelay2); }
                 delay(keydelay3);
                 keycode[0] = CtrL;     // Instead of just [Enter] use [Control + Shift + Enter] => Run as admin
                 keycode[1] = ShfL;     // 
                 keycode[2] = KeyEnter; // 
                 usb_hid.keyboardReport(HIDKbrd, 0, keycode);      delay(keydelay2);
                 usb_hid.keyboardRelease(HIDKbrd);                 delay(keydelay2);
                 delay(keydelay3);
                 keycode[0] = AltL;     // Alt + Y on UAC window instead of ArrowLeft+Enter
                 keycode[1] = KeyYes;   // Y or y
                 keycode[2] = KeyEnter; //
                 usb_hid.keyboardReport(HIDKbrd, 0, keycode);      delay(keydelay2);
                 usb_hid.keyboardRelease(HIDKbrd);                 delay(keydelay2); }                      // Layout==1
          break; 
    case 10: // M6 S6 T6 //////////////////////////////////////// M6 text or Enter key LayerAB==1 b = 5; else b =0; 
         if (Layout==3) 
            { if (LinkS[c]==0) { if (MacroKeys(c)) break; }
              if (LinkS[c]>0)  { if (MacroLinkS(c)) break; } }    
         if (Layout==4) 
            { if (LinkT[c]==0) { if (MacroKeys(c)) break; }
              if (LinkT[c]>0)  { if (MacroLinkT(c)) break; } }

        if (Layout==1) 
            { if (LinkM[c]==0) { if (MacroKeys(c)) break; }
              if (LinkM[c]>0)  { if (MacroLinkM(c)) break; }
          

      // Send Alt (Hold) + Numpad numbers for Math symbols and special characters
      //////////////////////////////////////////////////////////////////////////////////
      // Working Alt + NumPad Special Key left arrow Symbol - NB Numlock must be ON
      //         Alt + 27 as keycode [0]=Alt-L [1]=KeyPad2 [2]=KeyPad4 [3]=0x00 works
      //         Alt + 8733 (infinity symbol) only does up to Alt + 873 (small d thingy)
      //////////////////////////////////////////////////////////////////////////////////
      // Can toggle Numlock on/off with HID_KEY_NUM_LOCK as keycode[0] (not keyPress)
      //////////////////////////////////////////////////////////////////////////////////
      //keycode[0] = HID_KEY_ALT_LEFT;                    // Working
      //keycode[1] = HID_KEY_KEYPAD_2;
      //keycode[2] = HID_KEY_KEYPAD_7;                    // Alt+Keypad 27 = <- symbol               
      //keycode[3] = 0x00;                                // Needed else adds a L/F
      //usb_hid.keyboardReport(HIDKbrd, 0, keycode);      delay(keydelay2);
      //usb_hid.keyboardRelease(HIDKbrd);                 break;
      /////////////////////////////////////////////////////////////////////////////////////////
      // 1D6D1 + Alt + X = small pi works in MS Word (1D6D1 = 120529)
      /////////////////////////////////////////////////////////////////////////////////////////
      //for (n=0; n<6; n++) {usb_hid.keyboardPress(HIDKbrd, AltNum[n]); delay(keydelay2);
      //                     usb_hid.keyboardRelease(HIDKbrd);          delay(keydelay2);}
      //keycode[0] = AltL;
      //keycode[1] = 0x1B; // char X from hid.h
      //keycode[2] = 0x00; 
      //usb_hid.keyboardReport(HIDKbrd, 0, keycode);      delay(keydelay2);
      //usb_hid.keyboardRelease(HIDKbrd);                 
      //break;          
      ////////////////////////////////////////////////////////////////////////////////////////
      // This turns sticky codes on and off
      // If used for Alt + keypad numbers make sure the option turn off sticky when 
      // 2 keys are pressed at one is off and lock modifier keys when prssed twice in a row
      // is ON. Then turn on sticky keys, press Alt twice then in word press numlock on, then 
      // type 8733 on the keypad, press Alt twice and the open infinity symbol will show
      /////////////////////////////////////////////////////////////////////////////////////////
      /*
      for (n=0; n<5; n++) { keycode[0] = HID_KEY_SHIFT_LEFT;             delay(keydelay2);
                            usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay2);
                            usb_hid.keyboardRelease(HIDKbrd);            delay(keydelay2); }
      usb_hid.keyboardPress(HIDKbrd, '\r'); delay(keydelay);
      usb_hid.keyboardRelease(HIDKbrd);     break;
      */
      // Open Task Manager ///////////////////////////////////////////////////////
      keycode[0] = Esc; // Thsi works and is shorter than the other way below
      tud_hid_keyboard_report(HIDKbrd, ModCtrL + ModShfL + 0x00, keycode); delay(keydelay); 
      tud_task();        // call before releasing the keys
      keycode[0] = 0x00;
      tud_hid_keyboard_report(HIDKbrd, 0x00, keycode); }   break; // Layout==1
  
      // Enter Key ///////////////////////////////////////////////////////////////                                                        
      //usb_hid.keyboardPress(HIDKbrd, '\r'); delay(keydelay);
      //usb_hid.keyboardRelease(HIDKbrd);     break; 
      
    case 11: ////////////////////////////////////////// Volume decrease or Return
      if (VolDisable) { usb_hid.keyboardPress(HIDKbrd, '\r'); delay(keydelay);
                        usb_hid.keyboardRelease(HIDKbrd); break; }
    
      usb_hid.sendReport16(HIDCons, VolDown); delay(keydelay);
      usb_hid.sendReport16(HIDCons, 0);       break; 
  }   // switch
  }   // if Layout=1 or Layout>2

  ///////////////////////////////////////////////////////////////////////////////////////////////
  // #define ArrUp    0x52  #define ArrDown  0x51  #define ArrLeft  0x50  #define ArrRight 0x4F
  // #define KeyHome  0x4A  #define KeyEnd   0x4D  #define PageUp   0x4B  #define PageDwn  0x4E
  ///////////////////////////////////////////////////////////////////////////////////////////////
  else //if (Layout==2)    
  switch(button){
    case 0: // HOME - Media Previous //////////////////////////////////////////////////////////////////
      if (PowerKeys) { DoPowerKeys ('T', PowerKeysMenu, button); break; }

      if (MacroTimerDisp) { MacroTimer1 = !MacroTimer1; MacroTimer18 = MacroTimer18 || MacroTimer1;
                        if (MacroTimer1) { TimeRepeatPrev = millis(); status("[R-T] Repeat Macro Timer started"); }
                                                                 else status("[R-T] Repeat Macro Timer stopped");
                        ConfigButtons(1); optionsindicators(button); break; }
    
      if (Math) {MakeMathStr(button); break; }
    
      if (Kbrd) {MakeStr(button); break; }
     
      if (ConfigKeyCount==1) {ConfigKeyCount--;
                              MuteDisable = !MuteDisable; 
                              //GetSysInfo(1); // Save state on Flash
                              if (MuteDisable) status("Volume Mute Key Disable"); else status("Volume Mute Key Enable");
                              ConfigButtons(1); break; }

      if (Media) {usb_hid.sendReport16(HIDCons, StopMed); delay(keydelay);
                  usb_hid.sendReport16(HIDCons, 0); break; }      

      if (NumKeys) { keycode[0] = BckS;     // Backspace
                     keycode[1] = 0x00;     // NULL
                     usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay2);
                     usb_hid.keyboardRelease(HIDKbrd);            delay(keydelay2); break;}

      keycode[0] = KeyHome;
      usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(HIDKbrd);  break;
      
    case 1: // ARROW UP ////////////////////////////////////////////////
    
      if (DoUpKey)      // Set by SendBytesaeic
         { if (MST2==0) { for (n=0; n<MacroBuffSize; n++) { Mtr1to12[Option2][n] = MacroBuff[n]; } status("Mx Macro OK"); MacroSizeM1M12[Option2] = MacroBuffSize;
                          MacroM1M12[Option2] = 2; BPtr = Mtr1to12[Option2]; DoFileBytes(1, MTRname[Option2], BPtr, MacroBuffSize); } 
           if (MST2==1) { for (n=0; n<MacroBuffSize; n++) { Str1to12[Option2][n] = MacroBuff[n]; } status("Sx Macro OK"); MacroSizeS1S12[Option2] = MacroBuffSize;
                          MacroS1S12[Option2] = 2; BPtr = Str1to12[Option2]; DoFileBytes(1, STRname[Option2], BPtr, MacroBuffSize);  } 
           if (MST2==2) { for (n=0; n<MacroBuffSize; n++) { Ttr1to12[Option2][n] = MacroBuff[n]; } status("Tx Macro OK"); MacroSizeT1T12[Option2] = MacroBuffSize; 
                          MacroT1T12[Option2] = 2; BPtr = Ttr1to12[Option2]; DoFileBytes(1, TTRname[Option2], BPtr, MacroBuffSize);  }
           DoUpKey = false; }
    
      if (PowerKeys) { DoPowerKeys ('a', PowerKeysMenu, button); break; }

      if (MacroTimerDisp) 
         { MacroTimer8 = MacroTimer7 = MacroTimer6 = MacroTimer5 = Macrotimer4 = MacroTimer3 = Macrotimer2 = MacroTimer1 = MacroTimerDisp = MacroTimer18 = false;
           status("All Macro Timers stopped"); ConfigButtons(1); optionsindicators(button); break; }
      
      if (Math) {MakeMathStr(button); break; }
      
      if (Kbrd) {MakeStr(button); break; }
            
      if (NumKeys) {usb_hid.keyboardPress(HIDKbrd, NumkeysX[button][0]); delay(keydelay);
                    usb_hid.keyboardRelease(HIDKbrd);              break;}     
    
      if (ConfigKeyCount==1) {ConfigKeyCount--;
                              DelBS = !DelBS; 
                              //GetSysInfo(1); // Save state on Flash
                              if (DelBS) status("Backspace ON"); else status("Delete ON");
                              ConfigButtons(1); break; }
                        
      keycode[0] = ArrUp;
      usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(HIDKbrd);  break;
      
    case 2: // PAGE UP /////////////////////////////////////////////////
      if (PowerKeys) { DoPowerKeys ('T', PowerKeysMenu, button); break; }

      if (MacroTimerDisp) { MacroTimer3 = !MacroTimer3; MacroTimer18 = MacroTimer18 || MacroTimer3;
                        if (MacroTimer3){ TimeOnceofPrev = millis(); status("[O-T] Oneshot Macro Timer started"); }
                                                                else status("[O-T] Oneshot Macro Timer stopped");
                        ConfigButtons(1); optionsindicators(button); break; }
    
      if (Math) {MakeMathStr(button); break; }
    
      if (Kbrd) {MakeStr(button); break;}
    
      if (ConfigKeyCount==1) {ConfigKeyCount--;
                              VolDisable = !VolDisable; 
                              //GetSysInfo(1); // Save state on Flash
                              if (VolDisable) status("Volume Change Keys Disable"); else status("Volume Change Keys Enable");
                              ConfigButtons(1); break; }                              
      
      if (NumKeys) {usb_hid.keyboardPress(HIDKbrd, NumkeysX[button][0]); delay(keydelay);
                    usb_hid.keyboardRelease(HIDKbrd);              break;}

      keycode[0] = PageUp;
      usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(HIDKbrd);  break;
      
    case 3: // V+ or Delete or Escape(Math) or Execute(Keybrd) //////////////////////////////////////
      if (PowerKeys) { PowerClock=1; status("Restart on Clock"); DoPowerKeys ('R', PowerKeysMenu, button); break; }

     if (MacroTimerDisp) { MacroTimer7 = !MacroTimer7; MacroTimer18 = MacroTimer18 || MacroTimer7;
                        if (MacroTimer7) { rtc_set_alarm(&timer, &timer_callback); status("[RcT] Repeat Macro Countdown Clock started");}
                                                                              else status("[RcT] Repeat Macro Countdown Clock stopped");
                        ConfigButtons(1); optionsindicators(button); break; }
    
      if (Math) {Math = false; status("Math exit");  MathX = 0; Math123 = 0; ConfigButtons(1); break;}
    
      if (Kbrd) {if (KeyBrdX==3) {MakeStr(button); break;}  // [EXE] now [Snd] Send
                 if (KeyBrdDirect)  {usb_hid.keyboardPress(HIDKbrd, KeyBrdByte[KeyBrdByteNum]); delay(keydelay);     
                                     usb_hid.keyboardRelease(HIDKbrd);                          delay(keydelay); break; }
                 Kbrd = false; status((char *)KBDisp);                    
                 if (KeyBrdByteNum>0) {SendBytes();}          // Execute macro + exit keybrd
                 SendBytesEnd(1); ConfigButtons(1); break;}   // Clear incase wrong symbols added or no char added
       
      if (NumKeys) {usb_hid.keyboardPress(HIDKbrd, NumkeysX[button][0]); delay(keydelay);
                    usb_hid.keyboardRelease(HIDKbrd);              break;}

      if (VolDisable) {if (DelBS) keycode[0] = BckS; else keycode[0] = DelK;  //Delete or BackSpace Key
                       usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay); 
                       usb_hid.keyboardRelease(HIDKbrd); break; }
        
      usb_hid.sendReport16(HIDCons, VolUp); delay(keydelay);
      usb_hid.sendReport16(HIDCons, 0);     break;
      
    case 4: // ARROW LEFT //////////////////////////////////////////////
      if (PowerKeys) { DoPowerKeys ('t', PowerKeysMenu, button); break; }

      if (MacroTimerDisp) { Macrotimer2 = !Macrotimer2; MacroTimer18 = MacroTimer18 || Macrotimer2;
                        if (Macrotimer2) { timeRepeatPrev = millis(); status("[R-t] Repeat Macro Timer started"); } 
                                                                 else status("[R-t] Repeat Macro Timer stopped");
                        ConfigButtons(1); optionsindicators(button); break; }
    
      if (Math) {MakeMathStr(button); break; }
    
      if (Kbrd) {MakeStr(button); break;}
      
      if (ConfigKeyCount==1) {ConfigKeyCount--; LayerAB = (LayerAB==0); LayerABStr[6] = 65 + LayerAB;
                              status(LayerABStr); ConfigButtons(1); break; }

      if (Media) {usb_hid.sendReport16(HIDCons, PrevMed); delay(keydelay);
                  usb_hid.sendReport16(HIDCons, 0); break; }      
    
      if (NumKeys) {usb_hid.keyboardPress(HIDKbrd, '\r'); delay(keydelay);
                    usb_hid.keyboardRelease(HIDKbrd);               break;}

      keycode[0] = ArrLeft;
      usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(HIDKbrd);  break;
      
    case 5: // CONFIG //////////////////////////////////////////////////
      if (Math) {MakeMathStr(button); break; }
      
      if (Kbrd) {MakeStr(button); break;}
      
      if (NumKeys) {usb_hid.keyboardPress(HIDKbrd, NumkeysX[button][0]); delay(keydelay);
                    usb_hid.keyboardRelease(HIDKbrd);              break;}

      GetSysInfo(0);             //  "Logoff PowerOff Restart         "
      if (ConfigKeyCount==1) { status("Config and Files Saved"); ConfigKeyCount = 0; }
                        else { status("L14 Bs Vol AB Sav Med Del Po S12"); ConfigKeyCount++ ; } 
      ConfigButtons(1);  break;
      
    case 6: // ARROW RIGHT /////////////////////////////////////////////
      if (PowerKeys) { DoPowerKeys('t', PowerKeysMenu, button); break; }

      if (MacroTimerDisp) { Macrotimer4 = !Macrotimer4; MacroTimer18 = MacroTimer18 || Macrotimer4;
                        if (Macrotimer4) { timeOnceofPrev = millis(); status("[O-t] Oneshot Macro Timer started"); }
                                                                 else status("[O-t] Oneshot Macro Timer stopped");
                        ConfigButtons(1); optionsindicators(button); break; }
      
      if (Math) {MakeMathStr(button); break; }
    
      if (Kbrd) {MakeStr(button); break;}
    
      // Power Restart Logoff Timer options
      if (ConfigKeyCount==1) {ConfigKeyCount--; MacroTimerDisp = !MacroTimerDisp;  
          if (MacroTimerDisp) status("[R-T]Repeat  Macro[Rep]eat  [O-T]Oneshot"); else status("MacroTimer Exit");
          ConfigButtons(1); break;}

      if (Media) {usb_hid.sendReport16(HIDCons, NxtMed); delay(keydelay);
                  usb_hid.sendReport16(HIDCons, 0); break; }
      
      if (NumKeys) {usb_hid.keyboardPress(HIDKbrd, NumkeysX[button][0]); delay(keydelay);
                    usb_hid.keyboardRelease(HIDKbrd);              break;}

      keycode[0] = ArrRight;
      usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(HIDKbrd);  break;
      
    case 7: // Vol Mute ///////////////////////////////////////////////////////////////
      if (PowerKeys) { PowerClock=2; status("PowerOff on Clock"); DoPowerKeys ('O', PowerKeysMenu, button); break; }

      if (MacroTimerDisp) { MacroTimer8 = !MacroTimer8; MacroTimer18 = MacroTimer18 || MacroTimer8;
                        if (MacroTimer8) {rtc_set_alarm(&timer, &timer_callback); status("[OcT] Oneshot Macro Countdown Clock started");  }                        
                                                                             else status("[OcT] Oneshot Macro Countdown Clock stopped");
                        ConfigButtons(1); optionsindicators(button); break; }
    
      if (Math) {if (MathX<3) MathX++; else MathX=0; 
          Math123=0; ConfigButtons(1); break;} // Changes 4 Math KeyBrds from Greek abc -> ABC -> Calculus -> Logic
        
      if (Kbrd) {if (KeyBrdX<4) KeyBrdX++; else KeyBrdX=0; 
          KeyBrd123=0; ConfigButtons(1); break;} // Changes 5 KeyBrds from abc - ABC - 012 - *^% - SHFT
    
      if (NumKeys) {usb_hid.keyboardPress(HIDKbrd, NumkeysX[button][0]); delay(keydelay);
                    usb_hid.keyboardRelease(HIDKbrd);              break;}

      VolMuteCount++; if (VolMuteCount>4) VolMuteCount=1; Layout = VolMuteCount; ConfigButtons(1);     
      if (MuteDisable) break;  // No VolumeMute action toggle with [Cfg][Hme] sequence

      usb_hid.sendReport16(HIDCons, VolMute); delay(keydelay);
      usb_hid.sendReport16(HIDCons, 0);       break;
      
    case 8: // END //////////////////////////////////////////////////////////////////////////////
      if (PowerKeys) { DoPowerKeys ('r', PowerKeysMenu, button); break;} 

      if (MacroTimerDisp) { MacroTimer5 = !MacroTimer5; MacroTimer18 = MacroTimer18 || MacroTimer5;
                        if (MacroTimer5) { rtc_set_alarm(&alarm, &alarm_callback); status("[R-C] Repeat Macro Clock started");}
                                                                              else status("[R-C] Repeat Macro Clock stopped");
                        ConfigButtons(1); optionsindicators(button); break; }

      if (Math) {MakeMathStr(button); break; }
      
      if (Kbrd) {MakeStr(button); break;}
    
      if (ConfigKeyCount==1) {ConfigKeyCount--;               
                              Media = !Media; if (Media) status("Media Keys ON"); else status("Media Keys OFF"); 
                              ConfigButtons(1); break; }
           
      if (Media) {usb_hid.sendReport16(HIDCons, PlayMed); delay(keydelay);
                  usb_hid.sendReport16(HIDCons, 0); break; }

      if (NumKeys) {usb_hid.keyboardPress(HIDKbrd, NumkeysX[button][0]); delay(keydelay);
                    usb_hid.keyboardRelease(HIDKbrd);              break;}

      keycode[0] = KeyEnd;
      usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(HIDKbrd);  break;
      
    case 9: // ARROW DOWN //////////////////////////////////////////////
      if (Math) {MakeMathStr(button); break; }
    
      if (Kbrd) {MakeStr(button); break;}

      // Power Restart Logoff Timer options
      if (ConfigKeyCount==1) {ConfigKeyCount--; PowerKeys = !PowerKeys;  
          if (PowerKeys) status("[Rst]art  [Log]off  Power[Off]"); else status(" ");
          ConfigButtons(1); break;}   
             
      if (PowerKeys) { DoPowerKeys ('i', PowerKeysMenu, button); break; }

      if (MacroTimerDisp) { status("Repeat Macro not yet implemented"); MacroTimerDisp = false; ConfigButtons(1); break; }
      
      if (NumKeys) {usb_hid.keyboardPress(HIDKbrd, NumkeysX[button][0]); delay(keydelay);
                    usb_hid.keyboardRelease(HIDKbrd);              break;}
     
      keycode[0] = ArrDown;
      usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(HIDKbrd);  break;
      
    case 10: // PAGE DOWN //////////////////////////////////////////////
      if (PowerKeys) { DoPowerKeys ('u', PowerKeysMenu, button); break; }

      if (MacroTimerDisp) { MacroTimer6 = !MacroTimer6; MacroTimer18 = MacroTimer18 || MacroTimer6;
                        if (MacroTimer6) {rtc_set_alarm(&alarm, &alarm_callback); status("[O-C] Oneshot Macro Clock started"); }                        
                                                                             else status("[O-C] Oneshot Macro Clock stopped");
                        ConfigButtons(1); optionsindicators(button); break; }

      if (Math) {MakeMathStr(button); break; }
      
      if (Kbrd) {MakeStr(button); break;}
      
      if (ConfigKeyCount==1) {ConfigKeyCount--; SaveLayout = !SaveLayout; GetSysInfo(1); // Save the state
                              if (SaveLayout) status("Startup Layout L2"); else status("Startup Layout L1"); ConfigButtons(1); break;}

      if (NumKeys) {usb_hid.keyboardPress(HIDKbrd, NumkeysX[button][0]); delay(keydelay);
                    usb_hid.keyboardRelease(HIDKbrd);              break;}

      keycode[0] = PageDwn;
      usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay); 
      usb_hid.keyboardRelease(HIDKbrd);  break;
      
    case 11: // V- or Enter or Add /////////////////////////////////////////////////////////////////////////////////////////////////////
      if (Math) { SendMath(); status("Math symbol sent"); MathByteNum=0; break; }  
    
      if (Kbrd) {if (KeyBrdX==3) {MakeStr(button); break;}  // [ADD] now [Sav] Save
                if (!KBrdActive) {status("Press char/mod first"); break;} // No char key or modifier keys has been pressed at least once
                 /*if (KeyRepeat) {if (KeyBrdX>2) break;
                                 KeyBrdByte[KeyBrdByteNum] = KeyBrdByte[KeyBrdByteNum-1]; KBDispPos--; KeyBrdByteNum--;
                                 MakeStr(PrevButton);}  */
                 if (KeyBrdX==4) {KBDispPos = KBDispPos + 2;  if (KeyBrdByte[0]>0x7F) KBType = true; } // can be 2nd or 3rd?
                 if (Fx) {KBDispPos = KBDispPos + 2; }
                 KBDispPos++; KeyBrdByteNum++; break; }  
    
      if (NumKeys) {usb_hid.keyboardPress(HIDKbrd, NumkeysX[button][0]); delay(keydelay);
                    usb_hid.keyboardRelease(HIDKbrd);              break;}

      if (VolDisable) { usb_hid.keyboardPress(HIDKbrd, '\r'); delay(keydelay);
                        usb_hid.keyboardRelease(HIDKbrd); break; }

      usb_hid.sendReport16(HIDCons, VolDown); delay(keydelay);
      usb_hid.sendReport16(HIDCons, 0);       break;
  }   // Layout=2 button switch
}     // buttonpress

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Draw the keys ( 3 times 4 loops to create 12) Button Label 10 chars max
// The button number is the current column added to the current row times  the number of buttons per row. 
// First the row is multiplied by 4 and then the col is added.
// The first button is 0. Example: col = 2 (3rd column), row = 1 (2nd row) =>: 1 * 4 = 4 --> 4 + 2 = 6 (7th button).
// TFT_eSPI/Extensions/Button.h           x-middle,  y-middle,   width,      height,     outline,          fill,   text
// void initButton(TFT_eSPI *gfx, int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t outline, uint16_t fill,
//                                uint16_t textcolor, char *label, uint8_t textsize);
// void drawButton(bool inverted = false, String long_name = "");
// Compute the squared distance (d2) from the button center to the current touch coordinates.
// d2 = (bx - tx)*(bx - tx) + (by - ty)*(by - ty);
// If d2 is less than the button radius squared, then that button was selected.
// You will find this function, which does not do any multiplies, in the Adafruit graphics library:
// boolean Adafruit_GFX_Button::contains(int16_t x, int16_t y) 
//          {return ((x >= _x1) && (x < (_x1 + _w)) &&  (y >= _y1) && (y < (_y1 + _h))); }}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ConfigButtons(uint8_t rowcount) {  // rowcount=0 all 4 rows rowcount=2 last row only  rowcount=1 no background redraw 
  int n, m, b, p;
  uint8_t rows = 3;    // 3 rows of labels
  uint8_t row, col;    // keys
  uint16_t PadB, PadC; // 5 Pads Red Blue Green SkyBlue Yellow
  /////////////////////////////////////////////////////////////////////////////////////////////
  // Four Layouts possible:  Layout=1 b=0   Layout=2 b=1   Layout=3 b=2   Layout=4 b=3
  /////////////////////////////////////////////////////////////////////////////////////////////
  b = Layout - 1;                              //  L=1/b=0  L=2/b=1  L=3/b=2 L=4/b=3 
    
  if (rowcount==0) tft.fillScreen(BackgroundColor[b]);      // All four layouts Black background
  if (rowcount==1) rowcount = 0;                            // No background refresh
  
  uint16_t OutlineColor = ButtonOutlineColor[b];
  
  if (MuteDisable) b = b + 4;                   // b = 4,5,6,7
  if (VolDisable)  b = b + 8;                   // b = 8,9,10,11 
  // (VolDisable) && (MuteDisable) b = b + 12   // b = 12,13,14,15
  
  for (n=0; n<12; n++) for (m=0; m<3; m++) keyLabel[n][m] = Labels[LayerAB][b][n][m]; // label always = 4 chars \0 terminate stringlen 1,2,3
  for (n=0; n<12; n++) keyColor[n] = Colours[b][n];

  b = Layout - 1;                               // reset b to 0 - 3
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //  If Layout 2 [Cfg] Key pressed new options in different colour:
  //   "Hme", "Up",   "PgU", "Del/V+",  "<",    "Cfg", ">",    "V0/L14",  "End", "Dwn", "PgD", "V-/Ret"
  //   "L14", "BsD",  "Vol", "   ",     "A-B",  "Sav", "Med",  "   ",     "Del", "Kbd",  "S12", "   " 
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // also see for loop comma operator use here: https://forum.arduino.cc/t/arduino-does-not-understand-a-ranged-for-loop/164312/10
  int cR[9] = { 0, 1, 2, 4, 5, 6, 8, 9, 10 };    
  if (ConfigKeyCount>0) {for (m=0; m<9; m++) { n = cR[m]; strcpy(keyLabel[n], CfgLabel[n]); keyColor[n]=CfgColour;} } 
  
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //                 "Hme", "Up",  "PgU", "Del/V+", "<",    "Cfg",  ">",   "V0/L14",  "End", "Dwn",  "PgD", "V-/Ret"
  // MediaLabel[x][] "Stp", "   ", "   ", "   ",    "Prv",  "   ",  "Nxt", "   ",     "P-P", "   ",  "  ",  "   " 
  // MediaLabel[x][] "|||", "   ", "   ", "   ",    "|<",   "   ",  ">|",  "   ",     ">||", "   ",  "  ",  "   " 
  // keyLabel[x][]     0                              4               6                8                                           
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  int mR[4] = { 0, 4, 6, 8 };
  if ((Media)&&(b==1)&&(ConfigKeyCount==0)) {for (m=0; m<4; m++) {n = mR[m]; strcpy(keyLabel[n], MediaLabel[n]); }
       keyColor[0] = keyColor[4] = keyColor[6] = keyColor[8] = MediaColour; }
                 
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // PowerKeys
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if ((PowerKeys)&&(b==1)&&(ConfigKeyCount==0)) {for (m=0; m<12; m++) {strcpy(keyLabel[m], PowerKeysLabel[m]); keyColor[m] = PowerKeysColor[m]; } }

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Macro Timers
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if ((MacroTimerDisp)&&(b==1)&&(ConfigKeyCount==0)) {for (m=0; m<12; m++) {strcpy(keyLabel[m],  MacroTimerLabel[m]); keyColor[m] = MacroTimerColor[m]; } }

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (Math) { for (m=0; m<12; m++) {strcpy(keyLabel[m], MathLabel[MathX][m]);}
              for (m=0; m<11; m++) {keyColor[m] = MathColour1;} keyColor[3]=keyColor[7]=keyColor[11]=MathColour2; goto KeyBrdsDone; } // I can do it once

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (Kbrd) {for (m=0; m<12; m++) {strcpy(keyLabel[m], KbrdLabel[KeyBrdX][m]);}
             for (m=0; m<12; m++) {keyColor[m] = CfgColour;}; goto KeyBrdsDone; }                                                     // twice

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (MouseKeys) { for (m=0; m<12; m++) {strcpy(keyLabel[m], Mousekeys1[m]);}
                   for (m=0; m<12; m++) {keyColor[m] = Pink;}; goto KeyBrdsDone; }                                                    // thrice
                  
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (NumKeys)   { for (m=0; m<12; m++) {strcpy(keyLabel[m], NumkeysX[m]);}
                   for (m=1; m<12; m++) {keyColor[m] = DGrey;} keyColor[0]=Blue;keyColor[4]=Red; }
                   
  KeyBrdsDone:

  b = Layout - 1;                               // reset b to 0 - 3
              
  for (row = 0 + rowcount; row < rows; row++) {   // 3 rows 0,1,2 => b = 0+(0*4)=0 1+(0*4)=1 ... 0+(1*4)=4 ....
    for (col = 0; col < 4; col++) {               // 4 columns 0,1,2,3
        b = col + row * 4;
        key[b].initButton(&tft, KEY_X + col * (KEY_W + KEY_SPACING_X), KEY_Y + row * (KEY_H + KEY_SPACING_Y), 
                                               KEY_W, KEY_H, OutlineColor, keyColor[b], KeyFont, keyLabel[b], KEY_TEXTSIZE);
        key[b].drawButton();  }  }

   ////////////////////////////////////////////////////////////////////////////////
   // 5 Small Config Buttons between 1 st and 3rd row Red Blue Green SkyBlue Gold
   ////////////////////////////////////////////////////////////////////////////////
   b = Layout - 1;                     // reset b to 0 - 3 
   for (row = 0; row < 5; row++) 
       {p = row+b*5; key[row+12].initButton(&tft, PAD_X, PAD_Y + row * PAD_SPACING_Y, 
                                                  PAD_W, PAD_H, White, padColor[p], KeyFont, padLabel[p], KEY_TEXTSIZE);
        key[row+12].drawButton();  }             
             
  if (Change) indicators();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Output report callback for LED indicator such as Caplocks
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void hid_report_callback(uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
  (void) report_id;
  (void) bufsize;
  bool CapsLockPrev = CapsLock;            
  bool NumLockPrev = NumLock;             
  bool ScrollLockPrev = ScrollLock; 
  
  /*********************************************************
  KEYBOARD_LED_NUMLOCK    = TU_BIT(0), ///< Num Lock LED
  KEYBOARD_LED_CAPSLOCK   = TU_BIT(1), ///< Caps Lock LED
  KEYBOARD_LED_SCROLLLOCK = TU_BIT(2), ///< Scroll Lock LED
  KEYBOARD_LED_COMPOSE    = TU_BIT(3), ///< Composition Mode
  KEYBOARD_LED_KANA       = TU_BIT(4)  ///< Kana mode
  **********************************************************/

  // LED indicator is output report with only 1 byte length
  if ( report_type != HID_REPORT_TYPE_OUTPUT ) return;

  // The LED bit map:  Kana (4) | Compose (3) | ScrollLock (2) | CapsLock (1) | Numlock (0)
  uint8_t ledIndicator = buffer[0];

  // turn on LED if capslock is set
  digitalWrite(LED_BUILTIN, ledIndicator & KEYBOARD_LED_CAPSLOCK);
  
  CapsLock   = (ledIndicator & KEYBOARD_LED_CAPSLOCK); 
  NumLock    = (ledIndicator & KEYBOARD_LED_NUMLOCK);
  ScrollLock = (ledIndicator & KEYBOARD_LED_SCROLLLOCK);
 
  Change = ((CapsLock!=CapsLockPrev)||(NumLock!=NumLockPrev)||(ScrollLock!=ScrollLockPrev));

  if (Change) indicators();
   
  if ((Change)&&(!BackLightOn))  {if (NormVal==0) digitalWrite(LCDBackLight, HIGH);     // Backlight Full ON
                                             else  analogWrite(LCDBackLight, NormVal);  // Backlight Brightness ON
                                  BackLightOn = true; }
}
///////////////////////////
// Number Pad Page Select
///////////////////////////
void NumKeysChange()
{ for (int m=0; m<12; m++) {if (Numkeys123==0) strcpy(NumkeysX[m], Numkeys1[m]);
                            if (Numkeys123==1) strcpy(NumkeysX[m], Numkeys2[m]);
                            if (Numkeys123==2) strcpy(NumkeysX[m], Numkeys3[m]); }
}
//////////////////////
void SaveLinks()
//////////////////////
{ int n = 0, m = 0;

  File f1 = LittleFS.open("LinkM", "w"); f1.write(LinkM, 12); f1.close(); 
  File f2 = LittleFS.open("LinkNameM", "w"); for (n=0; n<12; n++) for (m=0; m<10; m++) f2.write(LinkNameM[n][m], 8); f2.close();    
  File f3 = LittleFS.open("LinkS", "w"); f3.write(LinkS, 12); f3.close(); 
  File f4 = LittleFS.open("LinkNameS", "w"); for (n=0; n<12; n++) for (m=0; m<10; m++) f4.write(LinkNameS[n][m], 8); f4.close(); 
  File f5 = LittleFS.open("LinkT", "w"); f5.write(LinkT, 12); f5.close(); 
  File f6 = LittleFS.open("LinkNameT", "w"); for (n=0; n<12; n++) for (m=0; m<10; m++) f6.write(LinkNameT[n][m], 8); f6.close(); 
}
/////////////////
void ReadLinks()
//////////////////
{ int m = 0, n = 0;
    
  File f1 = LittleFS.open("LinkM", "r"); f1.read(LinkM, 12); f1.close();
  File f2 = LittleFS.open("LinkNameM", "r"); for (n=0; n<12; n++) for (m=0; m<10; m++) f2.readBytes((char *)LinkNameM[n][m], 8); f2.close();  
  File f3 = LittleFS.open("LinkS", "r"); f3.read(LinkS, 12); f3.close();
  File f4 = LittleFS.open("LinkNameS", "r"); for (n=0; n<12; n++) for (m=0; m<10; m++) f4.readBytes((char *)LinkNameS[n][m], 8); f4.close();   
  File f5 = LittleFS.open("LinkT", "r"); f5.read(LinkT, 12); f5.close();
  File f6 = LittleFS.open("LinkNameT", "r"); for (n=0; n<12; n++) for (m=0; m<10; m++) f6.readBytes((char *)LinkNameT[n][m], 8); f6.close(); 
}
////////////////////////////////
void InitCfg(bool Option)
///////////////////////////////
{ char *ChrPtr;
  String NameStr;
  byte b, *BPtr;
  int ASize =0, m = 0, n = 0;
  uint16_t ByteLen = 0;
  
  if (Option) {
  if (LittleFS.exists("MuteDisable"))       MuteDisable   = false;  else MuteDisable   = true;    // L1-L4 enabled = default VolMute Key disabled 
  if (LittleFS.exists("VolDisable"))        VolDisable    = true;   else VolDisable    = false;   // V+ V- disabled Delete-Return enabled
  if (LittleFS.exists("Layout"))            SaveLayout    = true;   else SaveLayout    = false;   // Selected Saved Layout restored
  if (LittleFS.exists("LayerB"))            LayerAB       = 1;      else LayerAB       = 0;       // Layer A 1-6 or B 7-12 restored
  if (LittleFS.exists("MediaKeys"))         Media         = true;   else Media         = false;   // Media Keys in Layout 2 restored
  if (LittleFS.exists("DeleteBackspace"))   DelBS         = true;   else DelBS         = false;   // Toggle Delete or Backspace key
  if (LittleFS.exists("DoCal"))             DoCal         = true;   else DoCal         = false;   // 1 -> Run calibration on start
  if (LittleFS.exists("Rotate180"))         Rotate180     = true;   else Rotate180     = false;   // Set the display orientation to 1 or 3 (180 degrees different)
  if (LittleFS.exists("KeyFontBold"))       KeyFontBold   = true;   else KeyFontBold   = false;   // Button Font Bold/Normal labels
  if (LittleFS.exists("KeyFontColour"))    {KeyFontColour = true;   KeyFont = Black; }   
                                     else  {KeyFontColour = false;  KeyFont = White; }            // Button Font Bold/Normal labels
  if (LittleFS.exists("SRLinks"))           SaveReadLinks = true;   else SaveReadLinks = false; } // Save and Read Macro Links

  for (n=0; n<12; n++) 
      {NameStr = STRname[n]; BPtr = Str1to12[n]; MacroS1S12[n] = 0;               // ByteSize is max - ok DoFileBytes read until eof
       if (LittleFS.exists(NameStr)) { MacroSizeS1S12[n] = DoFileBytes(0, NameStr, BPtr, ByteSize);  MacroS1S12[n] = 2; } } // 0 = read 1 = write
           
  for (n=0; n<12; n++) 
      {NameStr = TTRname[n]; BPtr = Ttr1to12[n];  MacroT1T12[n] = 0;              // ByteSize is max - ok DoFileBytes read until eof    
       if (LittleFS.exists(NameStr)) { MacroSizeT1T12[n] = DoFileBytes(0, NameStr, BPtr, ByteSize); MacroT1T12[n] = 2; } }
           
  for (n=0; n<12; n++) 
      {NameStr = MTRname[n]; BPtr = Mtr1to12[n];  MacroM1M12[n] = 0;              // ByteSize is max - ok DoFileBytes read until eof         
       if (LittleFS.exists(NameStr)) { MacroSizeM1M12[n] = DoFileBytes(0, NameStr, BPtr, ByteSize); MacroM1M12[n] = 2; }  } 
  
  if (LittleFS.exists("inputStr")) {ChrPtr = inputString; DoFileStrings(StrOK, "inputStr", ChrPtr);}

  if (LittleFS.exists("LCDBlankTime")) {ReadLCDBlank();}

  if (LittleFS.exists("LCDDimmer")) {ReadLCDDim();}

  if (LittleFS.exists("LCDBrightness")) {ReadLCDBright();}

  if (LittleFS.exists("tRestart"))  { timeRestart  = ReadPowerKeys(0);}
  if (LittleFS.exists("TRestart"))  { TimeRestart  = ReadPowerKeys(1);}
  if (LittleFS.exists("tPowerOff")) { timePowerOff = ReadPowerKeys(2);}
  if (LittleFS.exists("TPowerOff")) { TimePowerOff = ReadPowerKeys(3);}

  if (LittleFS.exists("tRepeat")) { timeRepeat  = ReadMacroTimers(0);}
  if (LittleFS.exists("TRepeat")) { TimeRestart = ReadMacroTimers(1);}
  if (LittleFS.exists("tOnce"))   { timeOnceof  = ReadMacroTimers(2);}
  if (LittleFS.exists("TOnce"))   { TimeOnceof  = ReadMacroTimers(3);}

  if (SaveReadLinks) ReadLinks();  // stored by SaveLinks() in GetSysinfo
  
  //showKeyData();
}

///////////////////////////////////////////////////////////////////////////////////////
uint16_t DoFileBytes(bool DoWrite, String STRf,  byte *BytePtr, uint16_t ByteArrayLen)
///////////////////////////////////////////////////////////////////////////////////////
{ uint16_t ByteLen = 0;
  int n;
  byte b;
      
  if (!DoWrite)                            // read data
     //BytePtr[0] = 0x00;                    // For test in buttons() M1 - M3
     {File f = LittleFS.open(STRf, "r");
      n = 0;
      while (f.available())
            {b = f.read();
             BytePtr[n++] = b; }
      //if (n >= ByteArrayLen) break; }
      ByteLen = f.size();
      f.close(); }
     
  if (DoWrite)                             // write data
     {File f = LittleFS.open(STRf, "w");
      f.write(&BytePtr[0], ByteArrayLen);  // write the whole array to the file
      f.print('\0');                       // f.println();
      ByteLen = f.size();
      f.close();    }

  return ByteLen;
}

//////////////////////////////////////////////////////////////////
// case 1:  ChrPtr = Ttr1; NameStr = TTR1f; 
// if (Found) DoFileStrings(StrOK, NameStr,   ChrPtr); 
//            DoFileStrings(StrOK, TTR1f, ChrPtr); 
//////////////////////////////////////////////////////////////////
void DoFileStrings(bool DoWrite, String STRf,  char *ChrPtr)
{ uint16_t StrLen;
      
  if (!DoWrite)                               // read data
     {File f = LittleFS.open(STRf, "r");
     //StrLen = f.available(); 
     //StrLen = strlen(Str1); 
     StrLen = f.size();
     f.readBytes((char *)ChrPtr, StrLen);
     f.close(); }
       
  if (DoWrite)  // write data
     {File f = LittleFS.open(STRf, "w");
      StrLen = strlen(ChrPtr);               //Sent string <5This is S5> then STRf=StrData5 StrLen=10 which is correct
      f.print(ChrPtr);                       // if byte[] not char[]?
      f.print('\0');
      f.close();  }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LittleFS supports directories. Path + filename can only have a maximum of 31 characters 
// Dir dir = LittleFS.openDir("/");
// while (dir.next()) {
//        Serial.print(dir.fileName());
//        File f = dir.openFile("r");
//        Serial.println(f.size()); }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ListFiles()
{ int n = 1;
  File root = LittleFS.open("/", "r");
  File file = root.openNextFile();
  while(file){
      Serial.print(n);
      Serial.print(" ");
      Serial.print(file.name());
      Serial.print(" ");
      Serial.println(file.size());
      n++;
      file = root.openNextFile();
  }
}
////////////////////////////////////////////////////////////
void DeleteFiles()  // delete text/number macros and config
////////////////////////////////////////////////////////////
{ VolDisable  = true;
  MuteDisable = true;
  if (LittleFS.format()); 
     {LittleFS.begin();
      LittleFS.remove("MuteDisable");      // Must be changed via Cfg - self-restored
      LittleFS.remove("VolDisable");       // Must be changed via Cfg - self-restored
      InitCfg(0);
      Serial.println("Files + Config deleted");}
}
////////////////////////////////////////////////
void DisplayClocks(bool DisplayOption)
////////////////////////////////////////////////
{ Serial.println();
  Serial.print("Date and Time <t>: ");
  rtc_get_datetime(&t);
  datetime_to_str(datetime_str, sizeof(datetime_buf), &t);
  Serial.print(datetime_str);
  if (DisplayOption) { status("Date and Time <t>: "); delay(2000); status(datetime_str); delay(3000); }
  Serial.println();

  Serial.print("Alarm Macro [R-C][O-C] <a>: ");
  // rtc_get_datetime(&alarm); // If included this sets alarm = date time
  datetime_to_str(alarm_str, sizeof(alarm_buf), &alarm);
  Serial.print(alarm_str);
  if (DisplayOption) { status("Alarm Macro [R-C][O-C] <a>"); delay(2000); status(alarm_str); delay(3000); }
  Serial.println();

  Serial.print("Alarm Macro [RcT][OcT] <w>: ");
  datetime_to_str(timer_str, sizeof(timer_buf), &alarm);
  Serial.print(timer_str);
  if (DisplayOption) { status("Alarm Macro [RcT][OcT] <w>: "); delay(2000); status(timer_str); delay(3000); }
  Serial.println();

  Serial.print("Alarm Power [R-C][O-C] <p>: ");
  datetime_to_str(power_str, sizeof(power_buf), &alarm);
  Serial.print(power_str);
  if (DisplayOption) { status("Alarm Power [R-C][O-C] <p>: "); delay(2000); status(power_str); delay(3000); }
  Serial.println();  
}
////////////////////////////////////////////////
void GetSysInfo(int Action) // Also save config
//////////////////////////////////////////////// 
{ char *ChrPtr;
  byte *BPtr;
  uint32_t  Bt;
  int StrLen, n = 0, m = 0, b = 0;
  int fHeap = rp2040.getFreeHeap();
  int uHeap = rp2040.getUsedHeap();
  int tHeap = rp2040.getTotalHeap();
  int fCPU  = rp2040.f_cpu();
  
  if (Action==1) {if (SaveLayout) { char LayoutF[2]  = "1"; ChrPtr = LayoutF; DoFileStrings(1, "Layout",  ChrPtr); }
                  else LittleFS.remove("Layout"); return;}
  
  if (VolDisable)  { char VolD[2]   = "1"; ChrPtr = VolD;  DoFileStrings(true, "VolDisable",   ChrPtr); }
          else LittleFS.remove("VolDisable");
      
  if (!MuteDisable) { char MuteD[2]  = "1"; ChrPtr = MuteD; DoFileStrings(true, "MuteDisable",  ChrPtr); }
         else LittleFS.remove("MuteDisable"); 

  if (LayerAB==1) { char LayerD[2]  = "1"; ChrPtr = LayerD; DoFileStrings(true, "LayerB",  ChrPtr); }
         else LittleFS.remove("LayerB"); 

  if (Media) { char MediaD[2]  = "1"; ChrPtr = MediaD; DoFileStrings(true, "MediaKeys",  ChrPtr); }
         else LittleFS.remove("MediaKeys");

  if (DelBS) { char DelBSD[2]  = "1"; ChrPtr = DelBSD; DoFileStrings(true, "DeleteBackspace",  ChrPtr); }
         else LittleFS.remove("DeleteBackspace");

  if (SaveReadLinks) { char SRLinks[2]  = "1"; ChrPtr = SRLinks; DoFileStrings(true, "SRLinks",  ChrPtr); }
         else LittleFS.remove("SRLinks");       
  
  Serial.println("Version: VolumeMacro 51 GPL3 Nov 2022");
  Serial.printf("CPU MHz: %d\n\r", fCPU);
  Serial.printf("FreeHeap: %d\n\r", fHeap);
  Serial.printf("UsedHeap: %d\n\r", uHeap);
  Serial.printf("TotalHeap: %d\n\r", tHeap);
  Serial.printf("Core temperature: %2.1fC\n\r", analogReadTemp());
  
  Serial.println("Files (Number Name Size):");
  ListFiles();
  
  DisplayClocks(false);
  
  Serial.println();
  ReadLCDBlank();
  Serial.printf("LCD Blank Timeout: %d seconds\n\r", TimePeriod/1000);     // make sure this is never 0 

  ReadLCDDim(); b = (DimVal*100)/255;
  Serial.printf("LCD Dimmed: %d percent (%d/255)\n", b, DimVal);  

  ReadLCDBright(); b = (NormVal*100)/255; if (b==0) b = 100;
  Serial.printf("LCD Brightness: %d percent (%d/255)\n", b, NormVal);
      
  Serial.printf("Restart Time  (seconds): %d %d\n", timeRestart,  TimeRestart);
  Serial.printf("PowerOff Time (seconds): %d %d\n", timePowerOff, TimePowerOff);

  Serial.print("Calibration Data: "); for (int i = 0; i < 5; i++) { Serial.print(calData[i]); if (i < 4) Serial.print(", "); }

  if (SaveReadLinks) SaveLinks();  // Restored by ReadLinks() in InitCfg - makes the [Cfg] [Sav] key respond slower
  
  //showKeyData();   
}
//////////////////////
void ReadLCDBlank() 
//////////////////////
{ unsigned long int current = 0;             // https://stackoverflow.com/questions/62816850/arduino-read-integer-from-txt
  File f = LittleFS.open("LCDBlankTime", "r");  
  while (f.available()) { int c = f.read(); if ('0' <= c && c <= '9') { current = current * 10 + (c - '0'); }   } f.close(); 
  if (current>10000) TimePeriod = current; else TimePeriod = 30000;             // 10 seconds minimum Blank time - never 0!
}
//////////////////////
void ReadLCDBright() 
//////////////////////
{ int current = 0;             
  File f = LittleFS.open("LCDBrightness", "r");  
  while (f.available()) { int c = f.read(); if ('0' <= c && c <= '9') { current = current * 10 + (c - '0'); }   } f.close(); 
  if (current<255) NormVal = current; else NormVal = 0;  // 50% else maximum on                   
}
//////////////////////
void ReadLCDDim() 
//////////////////////
{ int current = 0;             
  File f = LittleFS.open("LCDDimmer", "r");  
  while (f.available()) { int c = f.read(); if ('0' <= c && c <= '9') { current = current * 10 + (c - '0'); }   } f.close(); 
  if (current<129) DimVal = current; else DimVal = 0;                         // 50% max eles OFF
}
/////////////////////////////////////////
unsigned long ReadPowerKeys(int Option)
/////////////////////////////////////////
{ int current = 0; 
  char PArray[4][10] = {"tRestart", "TRestart", "tPowerOff", "TPowerOff"};
  File f = LittleFS.open(PArray[Option], "r"); 
  while (f.available()) { int c = f.read(); if ('0' <= c && c <= '9') { current = current * 10 + (c - '0'); }   } f.close(); 
  return current; 
}
///////////////////////////////////////////
void WritePowerKeys(unsigned PVal, int Option) 
///////////////////////////////////////////
{ char PArray[4][10] = {"tRestart", "TRestart", "tPowerOff", "TPowerOff"};
   //                     012345678901234567890123456    012345678901234567890123456
  char RPArr[4][28]  = { "Restart t           seconds", "Restart T           seconds",
                         "PowerOff t          seconds", "PowerOff T          seconds" }; 
    
  File f = LittleFS.open(PArray[Option], "w"); 
  if (f) {f.print(PVal); f.close(); }

  int i = 6;                                                                    // 3 digits
  while (i!=0) { RPArr[Option][i+12] = (PVal%10) + 48; PVal = PVal/10; i--; };  // modulus (%) and division (/) operations 
  status(RPArr[Option]); SendBytesEnd(1);  
}

  ///////MUST UPDATE//////////////////////////////////////////////////////////////////////////////////////////////////////////
  //                                012345678901234567890123456789012345                   01234567890123456789012345678901
  // if (ConfigKeyCount==1) status("Config and Files Saved");               else   status("L14 Bs Vol AB Sav Med Del Po S12"); 
  // if (Rotate180)         status("Restart 180 degrees rotate calibrate");
  // if (PowerKeys)         status("[Rst]art  [Log]off  Power[Off]");       else   status(" ");
  // if (PowerKeys)         status("Timer running");
  // if (MuteDisable)       status("Volume Mute Key Disable");              else   status("Volume Mute Key Enable");  
  // if (VolDisable)        status("Volume Change Keys Disable");           else   status("Volume Change Keys Enable"); 
  // if (DelBS)             status("Backspace ON");                         else   status("Delete ON");
  // if (Kbrd)              status("[NXT]Page [ADD]Char [EXE]Macro");       else   status(" ");
  // if (!KBrdActive)       status("Press char/mod first");  
  // if (SaveLayout)        status("Startup Layout L2");                    else   status("Startup Layout L1");
  // if (Media)             status("Media Keys ON");                        else   status("Media Keys OFF"); 
  // if (LayerAB==0)        status("Layer B");                              else   status("Layer A"); 
  // if (DoCal)             status("Calibrate ON - send *ca* OFF");         else   status("Calibrate OFF - send *ca* ON");
  // if (KeyFontBold)       status("Key Font Bold is ON");                  else   status("Key Font Bold is OFF");
  // if (KeyBrdDirect)      status("KeyBoard Direct ON");                   else   status("KeyBoard Direct OFF");
  // if (RemOK)             status("Source macro deleted");                 else   status("Source macro not found");  
  // if (NumMode==1)        status("Source Macro Number"); }                else   status("Destination Macro Number"); }
  // if (Math)              status("Math exit");  
  // if (Math)              status("Math symbol sent");
  // 
  //                        status((char *)MathChr); status((char *)KBDisp); 
  //                        status(BlankArr); status(DimArr); status(RPArr[Option]);
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////
// Display mini status bar
// STATUS_X 150 // Centred on this 320/2=160
// STATUS_Y 222 // bottom
/////////////////////////////////////////////////
void status(const char *msg) 
{ if (ScrSizeType==1) tft.setTextPadding(480);                       // 480 for 480x320
  if (ScrSizeType==0) tft.setTextPadding(320);                       // 320 for 320x240
  //tft.setCursor(STATUS_X, STATUS_Y);
  tft.setTextColor(SkyBlue1, Black);
  if (Kbrd) tft.setTextColor(Yellow, Black);
  if (Math) tft.setTextColor(White, Black);
  if (PowerKeys) tft.setTextColor(Orange, Black);
  if (ScrSizeType==1) tft.setFreeFont(&FreeSansBold9pt7b);
  if (ScrSizeType==0) tft.setFreeFont(&FreeSans9pt7b);
  tft.setTextDatum(1);
   
  tft.drawString(msg, STATUS_X, STATUS_Y);
  if (KeyFontBold) tft.setFreeFont(&FreeSansBold12pt7b);
              else tft.setFreeFont(&FreeSans12pt7b);
}
////////////////////////////////////
void FillKeysStr(int SelectLayout)
////////////////////////////////////
{ int n, m;
  if (SelectLayout==3||SelectLayout==0)   // Keys S1-S12
  for (n=0; n<12; n++) for (m=0; m<=strlen(str1to12[n]); m++) Str1to12[n][m] = str1to12[n][m];
  if (SelectLayout==4||SelectLayout==0)   // Keys T1-T12
  for (n=0; n<12; n++) for (m=0; m<=strlen(str1to12[n]); m++) Ttr1to12[n][m] = str1to12[n][m];
  if (SelectLayout==1||SelectLayout==0)   // Keys M1-M12
  for (n=0; n<12; n++) for (m=0; m<=strlen(str1to12[n]); m++) Mtr1to12[n][m] = str1to12[n][m];
}
/////////////////////////////////////////
unsigned long ReadMacroTimers(int Option)
/////////////////////////////////////////
{ long unsigned int current = 0; 
  char TArray[4][10] = {"tRepeat", "TRepeat", "tOnce", "TOnce"};
  File f = LittleFS.open(TArray[Option], "r"); 
  while (f.available()) { long unsigned int c = f.read(); if ('0' <= c && c <= '9') { current = current * 10 + (c - '0'); }   } f.close(); 
  return current; 
}
//////////////////////////////////////////////////////////////
void WriteMacroTimers(unsigned long TVal, int Option, byte b) 
//////////////////////////////////////////////////////////////
{ int n;
  byte a;
  char TArray[4][10] = {"tRepeat", "TRepeat", "tOnce", "TOnce"};
  //                      01234567890123456789    0123456789012345689
  char TPArr[4][21]  = { "Repeat  t           ", "Repeat  T          ",
                         "Onceof  t           ", "Onceof  T          " };                          
     
  File f = LittleFS.open(TArray[Option], "w"); 
  if (f) {f.print(TVal); f.close(); }
  a = 1;
  if (Option==1||Option==3) { n = 12; while (a!=0x00) { a = mTArrayStr[b][n-12]; TPArr[Option][n] = a;  n++; } }  
  if (Option==2||Option==4) { n = 12; while (a!=0x00) { a = mtArrayStr[b][n-12]; TPArr[Option][n] = a;  n++; } }
  //status((char *)mTArrayStr[b]); 
  status(TPArr[Option]); SendBytesEnd(1); 
}
/////////////////////////
bool SendBytesStarCodes()
///////////////////////// 
{ unsigned long t, z;
  uint8_t a, b, n, i, d;
  bool UnLink = false;

  if (KeyBrdByte[1]==0x66&&KeyBrdByte[2]==0x6F)     // *fo* = Key Label Bold Normal Font
     {KeyFontBold = !KeyFontBold; 
      if (KeyFontBold) { status("Key Font Bold is ON"); 
                         File f = LittleFS.open("KeyFontBold", "w"); 
                         if (f) {f.print("1"); f.close(); } 
                         tft.setFreeFont(&FreeSansBold12pt7b); }
                  else { status("Key Font Bold is OFF"); 
                         LittleFS.remove("KeyFontBold"); 
                         tft.setFreeFont(&FreeSans12pt7b); } 
      ConfigButtons(1); SendBytesEnd(1); return true; }

  if (KeyBrdByte[1]==0x66&&KeyBrdByte[2]==0x63)     // *fc* = Key Label Font Colour Black White
     {KeyFontColour = !KeyFontColour; 
      if (KeyFontColour) { status("Key Font Colour is Black"); 
                         File f = LittleFS.open("KeyFontColour", "w"); 
                         if (f) {f.print("1"); f.close(); } 
                         KeyFont = Black; }
                  else { status("Key Font Colour is White"); 
                         LittleFS.remove("KeyFontColour"); 
                         KeyFont = White; } 
      ConfigButtons(1); SendBytesEnd(1); return true; }
    
      
   if (KeyBrdByte[1]==0x64&&KeyBrdByte[2]==0x65)    // "*de*" = delete config and macro files
      {status("Files deleted"); DeleteFiles(); ConfigButtons(1); SendBytesEnd(1); return true;  }
      
  if (KeyBrdByte[1]==0x63&&KeyBrdByte[2]==0x74)    // "*ct*" = display clocks 4x 1 second delay
      {DisplayClocks(true); return true;  }
  
  if (KeyBrdByte[1]==0x6B&&KeyBrdByte[2]==0x62)     // *kb* = num keyboard turn on
     {NumKeys = true; ConfigButtons(1); SendBytesEnd(1); status(" "); return true; }

  if (KeyBrdByte[1]==0x75&&KeyBrdByte[2]==0x61)     // *ua* = unlink all macros
     { for (n=0; n<12; n++) { LinkM[n] = 0; LinkS[n] = 0; LinkT[n] = 0; }
       status("All Macros unlinked"); return true; } 

  if (KeyBrdByte[1]==0x75&&KeyBrdByte[2]==0x6c)     // *ul* = unlink current Source Key Num
     { UnLink = true;
       if (MST1==0) { if (LinkM[Option1]>0) LinkM[Option1] = 0; else UnLink = false; }
       if (MST1==1) { if (LinkS[Option1]>0) LinkS[Option1] = 0; else UnLink = false; }
       if (MST1==2) { if (LinkT[Option1]>0) LinkT[Option1] = 0; else UnLink = false; } 
       if (UnLink) status("Macros unlinked"); else status("No link found"); return true; }      

  if (KeyBrdByte[1]==0x63&&KeyBrdByte[2]==0x6D)     // 63 6D *cm* = copy macros 
     {CopyMacro(); SendBytesEnd(0); return true; }  

  if (KeyBrdByte[1]==0x72&&KeyBrdByte[2]==0x6F)     // *ro* = rotate 180 degrees turn on
     {Rotate180 = !Rotate180;                       // Toggle On/Off
     if (Rotate180) 
        {File f = LittleFS.open("Rotate180", "w"); 
         if (f) {f.print(Rotate180); f.close(); } }
     if (!Rotate180) LittleFS.remove("Rotate180");      // no else here must be crystal-clear
     status("Restart 180 degrees rotate calibrate"); 
     LittleFS.remove("TouchCalData"); return true; }    // this forces re-calibration as well     

  if (KeyBrdByte[1]==0x63&&KeyBrdByte[2]==0x61)         // *ca* = Calibration On/Off 
     {DoCal = !DoCal; if (DoCal) { status("Calibrate ON - send *ca* OFF"); 
                                   File f = LittleFS.open("DoCal", "w"); if (f) {f.print(DoCal); f.close(); } return true; } 
                            else { status("Calibrate OFF - send *ca* ON"); LittleFS.remove("DoCal"); return true; }  }      
  
  if (KeyBrdByte[1]==0x6c&&KeyBrdByte[2]==0x72)         // *lr* = Save and Read Links from Flash
     {SaveReadLinks = !SaveReadLinks; if (SaveReadLinks) status("Links Save Restore ON"); else status("Links Save Restore OFF"); return true; } 
      
  if (KeyBrdByte[1]==0x6c&&KeyBrdByte[2]==0x64)         // *ld* = Data Links and Macros sent to serial port
     {showKeyData(); status("Links Data Sent"); return true; } 
 
  if (KeyBrdByte[1]==0x66&&KeyBrdByte[2]==0x73)         // *fs* = Fill S keys with predefined coded strings
     {FillKeysStr(3); status("[S1-S12] filled"); return true; }
  if (KeyBrdByte[1]==0x66&&KeyBrdByte[2]==0x74)         // *ft* = Fill T keys with predefined coded strings
     {FillKeysStr(4); status("[T1-T12] filled");  return true; }
  if (KeyBrdByte[1]==0x66&&KeyBrdByte[2]==0x6d)         // *fm* = Fill M keys with predefined coded strings
     {FillKeysStr(1); status("[M1-M12] filled"); return true; }
  if (KeyBrdByte[1]==0x66&&KeyBrdByte[2]==0x61)         // *fa* = Fill M S T keys with predefined coded strings
     {FillKeysStr(0); status("[All 1-12] filled");  return true; }       
  
  if (KeyBrdByte[1]==0x6D&&KeyBrdByte[2]==0x6B)         // *mk* = Mouse keys turn on - not implemented
     {MouseKeys = true; ConfigButtons(1); SendBytesEnd(1); status(" "); return true; }

  if (KeyBrdByte[1]==0x70&&KeyBrdByte[2]==0x6F)         // *po* = Power Keys Menu Option 
     {PowerKeysMenu = !PowerKeysMenu; if (PowerKeysMenu) status("PowerKeys Menu ON"); else status("PowerKeys Menu OFF"); return true; }

  b = (KeyBrdByte[4]-48);    // b = 0 - 9
  if (b >9) return false;    // do nothing if not
                      
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // z = TimePeriod/1000;  i = 6; while (i!=0) { BlankArr[i+12] = (z%10) + 48; z = z/10; i--; }   // modulus (%) and division (/) operations 
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  if (KeyBrdByte[1]==0x74&&KeyBrdByte[2]==0x62)                        // *tb*num = blank lcd timemout *tb*0,1-9 
      {t = TimePeriod; TimePeriod=tbArray[b];                          // Use tbArrayStr to convert number to string
       z = TimePeriod/1000;  i = 6;                                    // 6 digits
       while (i!=0) { BlankArr[i+12] = (z%10) + 48; z = z/10; i--; }   // modulus (%) and division (/) operations 
       File f = LittleFS.open("LCDBlankTime", "w");
       if (f) {f.print(TimePeriod); f.close(); }                       // Stored as 6 ASCII number bytes such as 49 53 48 48 48 48 = 150000 = 150 sec
       Serial.println(TimePeriod);
       SendBytesEnd(1); status(BlankArr); return true; }  
                                                                                                               
  if (KeyBrdByte[1]==0x64&&KeyBrdByte[2]==0x62)                        // *db*num = dim backlight pwm  *db*0,1-9
      {DimVal=dbArr[b];                                                // 
       for (i=0; i<2; i++) DimArr[i+16] = BBArr[b][i];                 //
       File f = LittleFS.open("LCDDimmer", "w");
       if (f) {f.print(DimVal); f.close(); }                           // Stored as 1-3 ASCII number bytes 48 48 56 = 008 = 8/255 %
       Serial.println(DimVal);
       SendBytesEnd(1); status(DimArr); return true; }      

  if (KeyBrdByte[1]==0x62&&KeyBrdByte[2]==0x62)                        // *bb*num = normal backlight pwm  *bb*0,1-9 0 8/255 - 128/255 % PWM
      {NormVal=bbArr[b];                                               // 0, 8/255 - 128/255 % PWM
      if (NormVal==0) digitalWrite(LCDBackLight, HIGH);                // Backlight Full ON
                else  analogWrite(LCDBackLight, NormVal);              // Backlight Brightness ON
       for (i=0; i<3; i++) NormArr[i+15] = BBArr[b][i];                // 
       File f = LittleFS.open("LCDBrightness", "w");
       if (f) {f.print(NormVal); f.close(); }                          // Stored as 1-3 ASCII number bytes 48 48 56 = 008 = 8/255 %
       Serial.println(NormVal);
       SendBytesEnd(1); status(NormArr); return true; }      
       
  // Power Keys  //
  
   if (KeyBrdByte[1]==0x6F)                                            // o
      { if (KeyBrdByte[2]==0x74)                                       // *ot*num 
           {t = timePowerOff; timePowerOff=b*30;                       // 1 = 30 seconds to 9 = 270 seconds
           if (timePowerOff>600) timePowerOff = t;                     // 10 minutes max
           if (timePowerOff==0) timePowerOff = 600;                    // 10 minutes
           WritePowerKeys(timePowerOff, 2);}                           // "tRestart", "TRestart", "tPowerOff", "TPowerOff"
        if (KeyBrdByte[2]==0x54)                                       // *oT*num 
           {t = TimePowerOff; TimePowerOff=b*1000;                     // 1 = 1000 seconds to 9 = 9000 seconds (150 minutes)
           if (TimePowerOff>60000) TimePowerOff = t;                   // restore to current time
           if (TimePowerOff==0) TimePowerOff = 60000;                  // 1000 minutes max (16.7 hours)
           WritePowerKeys(TimePowerOff, 3) ;} return true; }           // "tRestart", "TRestart", "tPowerOff", "TPowerOff"
      
   if (KeyBrdByte[1]==0x72)                                            // r
      { if (KeyBrdByte[2]==0x74)                                       // *rt*num 
           {t = timeRestart; timeRestart=b*30;                         // 1 = 30 seconds to 9 = 270 seconds
           if (timeRestart>600) timeRestart = t;                       // 10 minutes max
           if (timeRestart==0) timeRestart = 600;                      // 10 minutes
           WritePowerKeys(timeRestart, 0); return true; }              // "tRestart", "TRestart", "tPowerOff", "TPowerOff"
       if (KeyBrdByte[1]==0x72&&KeyBrdByte[2]==0x54)                   // *rT*num 
          {t = TimeRestart; TimeRestart=b*1000;                        // 1 = 1000 seconds to 9 = 9000 seconds (150 minutes)
          if (TimeRestart>60000) TimeRestart = t;                      // restore to current time
          if (TimeRestart==0) TimeRestart = 60000;                     // 1000 minutes max (16.7 hours)
          WritePowerKeys(TimeRestart, 1); return true;} }              // "tRestart", "TRestart", "tPowerOff", "TPowerOff" 
             
   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Macro Timers 
   // Only set MacroTime and MacroTimer1-4 in button code if MacroTimer key has been pressed after setting an option here
   // Associate Macro with Timer in KeyBrd Macro Tools Page with Key [Tmr] if 1 2 3 4 have been added to display bottom then 
   // Source Number Dest Number is associated with (1)(2) Repeat,(3)(4) Once of,(5) Clock or (8) pure repeat
   // 32 bits (4 bytes)  0 to 4,294,967,295 -> maximum value about 4000 x 10800000
   //                                    3 hours     30    60    90 sec  2 min   3 min   5 min  10 min   30 min  1 hour
   //  long unsigned int mtArray[10] = {10800000,  30000, 60000, 90000, 120000, 180000, 300000, 600000, 1800000, 3600000 };
   // x 100                              300 hrs     2 hrs    3 hrs    5 hrs     6 hrs     10 hrs    12 hrs   24 hrs     48 hrs     1 week
   //  long unsigned int mTArray[10] = {10800000,  7200000, 10800000, 18000000, 21600000, 36000000, 43200000, 86400000, 172800000, 604800000 };
   //                        012345678901234567890123456
   //  char mTArr[]       = "Oneshot Time               ";
   //  char rTArr[]       = "Repeat Time                ";
   //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   if (KeyBrdByte[1]==0x6d)                                            // m = macro onceof timers
      { if (KeyBrdByte[2]==0x74)                                       // *mt*num 
           {t = timeOnceof; timeOnceof = mtArray[b];         
           if (timeOnceof>mtArray[0]) timeOnceof = t;                  //  
           if (timeOnceof==0) timeOnceof = t;                          // no change
           WriteMacroTimers(timeOnceof, 2, b);}                        // "tRepeat", "TRepeat", "tOnce", "TOnce"
        if (KeyBrdByte[2]==0x54)                                       // *mT*num 
           {t = TimeOnceof; TimeOnceof = mTArray[b];                   //  
           if (TimeOnceof>mTArray[0]) TimeOnceof = t;                  // restore to current time
           if (TimeOnceof==0) TimeOnceof = t;                          //  
           WriteMacroTimers(TimeOnceof, 3, b) ;} return true; }        // "tRepeat", "TRepeat", "tOnce", "TOnce"
      
   if (KeyBrdByte[1]==0x6e)                                            // n = macro repeat timers
      { if (KeyBrdByte[2]==0x74)                                       // *nt*num 
           {t = timeRepeat; timeRepeat = mtArray[b];                   // 
           if (timeRepeat>mtArray[0]) timeRepeat = t;                  //  
           if (timeRepeat==0) timeRepeat = t;                          //  
           WriteMacroTimers(timeRepeat, 0, b); return true; }          // "tRepeat", "TRepeat", "tOnce", "TOnce"
       if (KeyBrdByte[1]==0x72&&KeyBrdByte[2]==0x54)                   // *nT*num 
          {t = TimeRepeat; TimeRepeat= mTArray[b];                     //  
          if (TimeRepeat>mTArray[0]) TimeRepeat = t;                   // restore to current time
          if (TimeRepeat==0) TimeRepeat = 60000;                       //  
          WriteMacroTimers(TimeRepeat, 1, b); return true;} }          // "tRepeat", "TRepeat", "tOnce", "TOnce"   
   return false;
}

//////////////////////////////////////////////////////////////////////////////////
void SendBytesEnd(bool All)
//////////////////////////////////////////////////////////////////////////////////
{ int n;
  KeyBrdByteNum = 0; KBDispPos = 0;  KeyBrdX = 3; KeyBrd123 = 0;
  for (n = 0; n < 200; n++) { KeyBrdByte[n] = 0x00; KBDisp[n] = '\0'; }
  if (All) {Kbrd = false;} 
  KBType = false; Fx = false; KBrdActive = false; 
  optionsindicators(0); 
}
/////////////////////////////////////////////////////////////////////////////////////////
void SendBytes()    
////////////////////////////////////////////////////////////////////////////////////////        
// Send and Save KeyBrd Macro Editor macros 01 - 99,00
// Destination is Option 2 i.e. [Dst] = rightmost number in KeyBrd 
// M S T A is also known but only saved to M S T when [Up] key pressed (only 12 keys max)
// If destination is A then saved to Macro00-99 in here (100 keys max if A selected)
//
// Example 1: Set up MM 01 01 as SD SrcNum DestNum - during same session press [CTR][SHF][Esc][EXE][UP] - save to M01
//            Can check in Terminal File list has Macro01 4bytes MtrData1 4bytes (only autosave for 01)
//            Press [M1] and TaskMan opens
// Example 2: Set up MM 01 02 - press [GUI][r][EXE][Up] - save to M02 - filelist only has Macro02 3bytes
//            Must therefore copy it to M02
//            Set up AM 02 02 - then [Cpy] - file list now has MtrData2 4 (added 0x00) press [M2] open run command window
// Example 3: Set up AS 02 01 - then [Cpy] - file list now has StrData1 4 press [S1] open run command window
// Example 4: Set up AS 01 02 - then [Cpy] - file list now has StrData2 5 press [S2] open TaskMan
//
// Watch out for Caplock or Shift states during [EXE] phase
// They do not affect the characters during [ADD] phase
// This is why there are separate lower case and uppercase pages
// These will change a -> A or A -> a and Numbers to symbols such as 9 -> ( and 0 -> )
////////////////////////////////////////////////////////////////////////////////////////
{ uint8_t keycode[6] = { 0 }; // simultaneous keys pressed in here
  uint8_t a, b, n, i;
  //                 01234567890123456789012
  char status1[] = {"Press [Up] Save macro00"} ;   // saved as macro 00-99
  String NameStr;
  
  // Check for special commands Start with * eg *ab*n n = 0-9
  if (KeyBrdByte[0]==0x2A) { if (SendBytesStarCodes()) return; else {status("*Code incorrect"); return; } }  

  NameStr = KBMacro[Option2];   // Option2 KeyBrd Destination = 7 will save to Macro08 -> +1 08 is displayed on KeyBrd
  if (Option2<10) {status1[22] = num16[Option2][0];} else {status1[21] = (Option2/10)-48; status1[22] = num16[Option2][1];}  // Display Option2 nn 
    
  // sequential via keyPress 
  if ((!Fx)&&(KeyBrdByte[0]<0x80))                                              // F1-F24 must be sent with keycode not keyPress
     { for (n = 0; n < KeyBrdByteNum; n++) if (KeyBrdByte[n]<0x80)              // When 1st char is normal alphanumeric all must be <0x80    
           {usb_hid.keyboardPress(HIDKbrd, KeyBrdByte[n]); delay(keydelay);     // Converted via end of hid.h from ascii to hid
            usb_hid.keyboardRelease(HIDKbrd);              delay(keydelay); 
            MacroBuff[n] = KeyBrdByte[n];}
       File f = LittleFS.open(NameStr, "w");
       if (f) {f.write(KeyBrdByte, KeyBrdByteNum); f.print('\0'); f.close(); }         
       status(status1); MacroBuffSize = KeyBrdByteNum; DoUpKey = true; }

  // simultaneous via keycode 
  for (n=0; n<6; n++) keycode[6] = 0x00;       
  if ((Fx)||(KeyBrdByte[0]>0x7F)&&(KeyBrdByteNum<6))                            // Fnn = 3A-45 + 68-73 hex
     { for (n = 0; n < KeyBrdByteNum; n++) {keycode[n] = KeyBrdByte[n]; MacroBuff[n] = KeyBrdByte[n]; }  
           usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay);
           usb_hid.keyboardRelease(HIDKbrd);            delay(keydelay);  
       File f = LittleFS.open(NameStr, "w");
       if (f) {f.write(KeyBrdByte, KeyBrdByteNum); f.print('\0'); f.close(); }  
       status(status1); MacroBuffSize = KeyBrdByteNum; DoUpKey = true; }
  
  // Note also saves: DoFileBytes(1, M,S,T TRname[Option2], BPtr, MacroBuffSize[Option2]) in buttons() case [Up] 

   /*
      for (n = 0; n < KeyBrdByteNum; n++) 
          {Serial.print(KeyBrdByte[n], HEX);
           Serial.print(' '); }
      Serial.println(" ");
      for (n = 0; n < 6; n++)   
          {Serial.print(keycode[n], HEX);
           Serial.print(' '); }
   */            
SendBytesEnd(1); //clean-up
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// int LinkM[12] LinkS[12] LinkT[12  
// 0 - 9 Number of linked macro's for each Key 1-12
// char LinkNameM[12][10][9] = { 0 };  
// File names on Flash of linked macro's -> Macro01, Macro02
// [12] keys for each M S T [10] linked macrofilenames length 8 chars  0x00
// Linked filenames always MacroNN NN = 01 - 99
// 
// Chained linking is always additive - it does not ovewrite previous link but adds an additional link
// To overwrite first remove links using *ul* on source macro
//
// Linking Macros Examples:
// Example 1: Program M1 with a Open Run windows [GUI][r] and [M2] with a notepad+C/R
//            Set up MM 01 02 - Press Link [Lnk], then press [M1] it opens the run window, then runs notepad
// Example 2: Program M1 with a Open Run windows [GUI][r] and [M4] with a [Ctr+Shft+Esc
//            Set up MM 04 01 - Press Link [Lnk], then press [M1] it open TaskMan, then opens the run window.
// Example 3: Program M1 M2 and M4 as in example 7 and example 8 - open Run window, notepad+C/R and open TaskMan
//            Set up MM 01 02 - Press Link [Lnk], then press [M1] it opens run window and notepad. Then add
//            a 3rd link Set up MM 01 04, press [Lnk] - then press [M1] it opens notepad via run window and then TaskMan
// Note 1: For example 3 before linking there must be three files names Macro01 (3 bytes), Macro02 (9 bytes), and 
//         Macro04 (4 bytes) - press [Cfg] to an open serial monitoe to check. There are advanced linking options when
//         numbers have been added [ADD] but for the three examples use [Lnk] on a clean display i.e. it will only 
//         display "Destination Macro Number", or "Source Macro" or "Destinatio Macro" before pressing [Lnk].
// Note 2: To unlink send *ul* with the Macro Key to be unlinked visible as the Source Macro such a Mx mm xx.
// Note 3: Linked Macro Data will be lost after a power cycle or reset.
// Note 4: Use Source = Destination then press [Lnk] - if the intention is to repeat the same macro more than once. 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool LinkMacro()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Two stage 1. Link two MacroNum1 to MacroNum2 - use only Option1 Option2 not SD - Note 8 char filename space in LinkNameM
//           2. Assoc linked macro to M S T key - use SD Source M S T [Add] 1-12 
// To unlink use *ul* with the Source visible (make the Source = Destination such as MM 05 05)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////             
{ byte b = 0;
  bool LinkOK = true;
  // bool UnLink = false;
  // char Name1[8] = "Macro00";

  // if ((MST1==MST2)&&(Option1==Option2)) UnLink = true;
    
  if (KeyBrdByteNum>=1) b = KeyBrdByte[0]-49;
  if (KeyBrdByteNum>1)  b = b*10 + KeyBrdByte[1]-49;

  if (MST1==0) { // if (UnLink) { LinkM[b] = 0; status("Macro's unlinked"); return UnLink; } // Do linked macro Mxx if (LinkM[1-12]>0) do MacroLink(1-12)  
                 if (LinkM[b]==0) strcpy(LinkNameM[b][LinkM[b]],   KBMacro[Option1]); 
                                  strcpy(LinkNameM[b][LinkM[b]+1], KBMacro[Option2]); LinkM[b]++; }

  if (MST1==1) { // if (UnLink) { LinkS[b] = 0; status("Macro's unlinked"); return UnLink; }  
                 if (LinkS[b]==0) strcpy(LinkNameS[b][LinkS[b]],   KBMacro[Option1]); 
                                  strcpy(LinkNameS[b][LinkS[b]+1], KBMacro[Option2]); LinkS[b]++; }

  if (MST1==2) { // if (UnLink) { LinkT[b] = 0; status("Macro's unlinked"); return UnLink; } 
                 if (LinkT[b]==0) strcpy(LinkNameT[b][LinkS[b]],   KBMacro[Option1]); 
                                  strcpy(LinkNameT[b][LinkS[b]+1], KBMacro[Option2]); LinkT[b]++; }
   
  if (LinkOK) status("Macro's linked"); else status("Macro's not found");  
  
  return LinkOK;  // MST1 = 4 undefined  
}
/////////////////////////////////////////////////////////////
bool RenameMacro() // LittleFS.rename(pathFrom, pathTo) 
/////////////////////////////////////////////////////////////
// Must restart for new filemacro to be copied to M S T 1-12
/////////////////////////////////////////////////////////////
{ String NameStr1, NameStr2;
  bool RenOK = false;

  if (MST1==3) NameStr1 = KBMacro[Option1]; if (MST1==0) NameStr1 = MTRname[Option1];
  if (MST1==1) NameStr1 = STRname[Option1]; if (MST1==2) NameStr1 = TTRname[Option1];

  if (MST2==3) NameStr2 = KBMacro[Option2]; if (MST2==0) NameStr2 = MTRname[Option2];
  if (MST2==1) NameStr2 = STRname[Option2]; if (MST2==2) NameStr2 = TTRname[Option2];

  RenOK = LittleFS.rename(NameStr1, NameStr2);    // Rename 1 to 2 for M S T files and macro00-99
  
  if (RenOK) status("Macro renamed Src -> Dst"); else status("Rename Macro's not found");
  return RenOK;  // MST1 = 4 undefined  
}
////////////////////
bool RemoveMacro()
////////////////////
{ String NameStr;
  bool RemOK = false;
  if (MST1==3) { NameStr = KBMacro[Option1]; RemOK = LittleFS.remove(NameStr); }    // Delete Source File macro00-99
  if (MST1==0) { NameStr = MTRname[Option1]; RemOK = LittleFS.remove(NameStr); }    // Delete Read Source File M1 - M12               
  if (MST1==1) { NameStr = STRname[Option1]; RemOK = LittleFS.remove(NameStr); }    // Delete Read Source File S1 - S12  
  if (MST1==2) { NameStr = TTRname[Option1]; RemOK = LittleFS.remove(NameStr); }    // Delete Read Source File T1 - T12 
  
  if (RemOK) status("Source macro deleted"); else status("Source macro not found");
  return RemOK;  // MST1 = 4 undefined  
}
/////////////////////////////////////////////////////////////////////
// To copy Macro1-99 to Keys M1-12,S1-12,T1-12 
// Compose *cm*nnXmm via [ADD] where nn = 01-99 X = M S T mm = 01-12
//              012345678
// KeyBrdByte = *cm*nnXmm  = KBDisp
// OR just use *cm* will then copy using current settings displayed
// via MST1, MST2, Option1, Option2
// byte MacroBuff[ByteSize];
// int  MacroBuffSize = ByteSize;
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool SendMacro()  
/////////////////////////////////////////////////////////////////////////////////////////////////////
// Different from [EXE] - no [Up]key save option - already saved to a file
// Cannot list macros here without a file - for example those loaded using *fm,s,t*
// Because DoFileBytes(0, NameStr will not find file NameStr
// Works on macros loaded using serial port - they are saved to a file such as <3This is Key S3>
/////////////////////////////////////////////////////////////////////////////////////////////////////
{ uint8_t keycode[6] = { 0 };      // simultaneous keys pressed in here
  String NameStr;
  byte *BPtr;
  int n;
  int MSTLayout[5] = {1,3,4,2,2};                                   // Convert MST to Layout if Layout = 2
 
  Layout = MSTLayout[MST1];                                         // Use [Src][Num] Macro detail in KeyBrd

  if (Layout!=2) { MacroKeys(Option1); Layout = 2; return true; }   // Send M S T Option1                                                                         

  Layout = 2;                                                       // M S T keys = Layouts 1, 3, 4, will return if executed
 
  NameStr = KBMacro[Option1];         // A - Use files Macro01 - Macro99 
  BPtr = MacroBuff; MacroBuffSize = DoFileBytes(0, NameStr, BPtr, ByteSize); if (MacroBuffSize==0) { status("Macro's not found"); return false; }
                  
  if (MacroBuff[0]<0x80) { n = 0; while (MacroBuff[n]!=0) {usb_hid.keyboardPress(HIDKbrd, MacroBuff[n]);  delay(keydelay2);
                                                           usb_hid.keyboardRelease(HIDKbrd);              delay(keydelay2); 
                                                           n++; if (n>ByteSize) break; } return true; }
  if (MacroBuff[0]>0x7F) { for (n=0;  n<6; n++) {keycode[n] = MacroBuff[n]; if (keycode[n]==0) break; }    // allow one 0x00 in keycode
                                                 usb_hid.keyboardReport(HIDKbrd, 0, keycode);              delay(keydelay2); 
                                                 usb_hid.keyboardRelease(HIDKbrd);                         delay(keydelay2); 
                                                 return true; }                                            // if (Layout==2) 
 return false;
}
///////////////////////////////////////////////////////////////////// 
void CopyMacro()    // Source Macro must already be saved to a file
/////////////////////////////////////////////////////////////////////
//                             012345678901234
{ char KeyBrd5status[16] =   {"Macro nn -> Xnn"};
  int Xlat1[5] = {6,7,12,13,14};
  int MST = 4; // Later assigned to MST1 or MST2
  String NameStr;
  byte *BPtr;
  int n;
  
  MST = MacroBuffSize = 0;
 
  if (KeyBrdByteNum>4) { MST = KeyBrdByte[6]-77; if (MST>0) MST = MST-5; if (MST>2) return; // MST = 0 1 2 3 4  = M S T A notdef - macrokeys
                         for (int n=0; n<5; n++)  KeyBrd5status[Xlat1[n]]  = KeyBrdByte[n+4];
                         Option1 = (KeyBrdByte[4]-48)*10 + KeyBrdByte[5]-49;           // Option1 = 0 - 11 = macro 1 - 992
                         Option2 = (KeyBrdByte[7]-48)*10 + KeyBrdByte[8]-49;           // Option2 = 0 - 11 keys M S T = 1 - 12 
                         status((char *)KeyBrd5status); }
  
  if (MST1==3) { NameStr = KBMacro[Option1]; }   // A - Use files Macro01 - Macro99                         
  if (MST1==0) { NameStr = MTRname[Option1]; }   // M - Use files MtrData1012
  if (MST1==1) { NameStr = STRname[Option1]; }   // S
  if (MST1==2) { NameStr = TTRname[Option1]; }   // T

  BPtr = MacroBuff; MacroBuffSize = DoFileBytes(0, NameStr, BPtr, ByteSize);   // Read source macro from Flash
  if (MacroBuffSize==0) { status("Macro's not found"); return; }         

  if (MST2==0) { for (n=0; n<MacroBuffSize; n++) { Mtr1to12[Option2][n] = MacroBuff[n]; } MacroM1M12[Option2] = 2; BPtr = Mtr1to12[Option2];  
                      MacroSizeM1M12[Option2] = MacroBuffSize; DoFileBytes(1, MTRname[Option2], BPtr, MacroBuffSize); }  // Save Destination File

  if (MST2==1) { for (n=0; n<MacroBuffSize; n++) { Str1to12[Option2][n] = MacroBuff[n]; } MacroS1S12[Option2] = 2; BPtr = Str1to12[Option2];  
                      MacroSizeS1S12[Option2] = MacroBuffSize; DoFileBytes(1, STRname[Option2], BPtr, MacroBuffSize); }  // Save Destination File  

  if (MST2==2) { for (n=0; n<MacroBuffSize; n++) { Ttr1to12[Option2][n] = MacroBuff[n]; } MacroT1T12[Option2] = 2; BPtr = Ttr1to12[Option2];
                      MacroSizeT1T12[Option2] = MacroBuffSize; DoFileBytes(1, TTRname[Option2], BPtr, MacroBuffSize); }  // Save Destination File

  if ((MST!=4)&&(MacroBuffSize>0)) status("Macro Copied check file list"); else status("Macro's not found");
                 
}
///////////////////////////////////////////////////////////////////////////////////////////
// MST1 MST2  used to define source and destination macro for copy and/or linking/chaining
// Source = A (mAcro00-mAcro99) or Keys M S T 1-12 for copy or A for linking
// Destination = Keys M S T 1-12 for copy or A for linking
///////////////////////////////////////////////////////////////////////////////////////////
int GetMST(byte b)                       // MST = 0 1 2 3 4  = M S T A undef keys
/////////////////////////////////////////////////////////////////////////////////////
{ 
  int MST = 4; // Later assigned MST1 or MST2
  // MST = b-62; if (MST!=3) MST = MST-12; if (MST>0) MST = MST-6;  
  if (b==0x4D) MST = 0; // 0x4D=77 Source = M Keys 1-12 
  if (b==0x53) MST = 1; // Source = S Keys 1-12 
  if (b==0x54) MST = 2; // Source = T Keys 1-12 
  if (b==0x41) MST = 3; // Source = A = mAcro01-macro99 
  return MST;
}
///////////////////////////////////////////////////////////////////// 
void DoSourceMacro()    
/////////////////////////////////////////////////////////////////////
{ byte a, b;
   if (KeyBrdByteNum==0) {MST1++; if (MST1>3) MST1=0; }  // Do not use MST1 = 4 = undefined as an option
   
  if (KeyBrdByteNum>0)
     {a = KeyBrdByte[0]; b = a - 48;      
      if (KeyBrdByteNum==3)             { Option1 = (KeyBrdByte[1]-48)*10 + KeyBrdByte[2]-49; MST1 = GetMST(a); }
      if ((KeyBrdByteNum==2)&&(0<b<10))   Option1 = b*10 + KeyBrdByte[1]-49;
      if ((KeyBrdByteNum==2)&&(b>9))    { Option1 = KeyBrdByte[1]-49;                         MST1 = GetMST(a); }
      if ((KeyBrdByteNum==1)&&(0<b<10))   Option1 = b; 
      KeyBrdByteNum=0; }                  // Else [Src][Dst][Num] ignored
    
 NumMode = 1;
 optionsindicators(0);
}
///////////////////////////////////////////////////////////////////// 
void DoDestMacro()    
/////////////////////////////////////////////////////////////////////
{ byte a, b;
   if (KeyBrdByteNum==0) {MST2++; if (MST2>3) MST2=0; }  // Do not use MST2 = 4 = undefined as an option
 
  if (KeyBrdByteNum>0)
     {a = KeyBrdByte[0]; b = a - 48;      // Option2 = 0 - 11 = macro 1 - 99  
      if (KeyBrdByteNum==3)             { Option2 = (KeyBrdByte[1]-48)*10 + KeyBrdByte[2]-49; MST2 = GetMST(a); }
      if ((KeyBrdByteNum==2)&&(0<b<10))   Option2 = b*10 + KeyBrdByte[1]-49;
      if ((KeyBrdByteNum==2)&&(b>9))    { Option2 = KeyBrdByte[1]-49;                         MST2 = GetMST(a); }
      if ((KeyBrdByteNum==1)&&(0<b<10))   Option2 = b; 
      KeyBrdByteNum=0; }                  // Else [Src][Dst][Num] ignored
          
 NumMode = 2;
 optionsindicators(0);
}
///////////////////////////////////////////////////////////////////// 
void DoNumMacro()    
/////////////////////////////////////////////////////////////////////
{ byte a, b;
  int Option12; // Assigned to Option1/2
  
  if (KeyBrdByteNum==0) {if (NumMode==1) { Option1++; 
                             if (MST1<3)  if (Option1>11) Option1=0;
                             if (MST1==3) if (Option1>98) Option1=0; 
                             status("Source Macro Number");  }  
                         if (NumMode==2) { Option2++; 
                             if (MST2<3)  if (Option2>11) Option2=0;
                             if (MST2==3) if (Option2>98) Option2=0; 
                             status("Destination Macro Number");} }
  
  if (KeyBrdByteNum>0)
     {a = KeyBrdByte[0]; b = a - 48;      
      if (KeyBrdByteNum==3)             { return; }
      if ((KeyBrdByteNum==2)&&(0<b<10))   Option12 = b*10 + KeyBrdByte[1]-49;
      if ((KeyBrdByteNum==2)&&(b>9))    { return; }
      if ((KeyBrdByteNum==1)&&(0<b<10))   Option12 = b;    
      if (NumMode==1) { Option1 = Option12; status("Source Macro Number"); }
                  else{ Option2 = Option12; status("Destination Macro Number"); }
      KeyBrdByteNum=0; }                  // Else [Src][Dst][Num] ignored      

 optionsindicators(0);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool GetTimerMacro ()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// char TimerName[8][10] = {"Macro00","Macro00","Macro00","Macro00","Macro00","Macro00","Macro00","Macro00"}; 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
{ byte a, b, c, n;
  //                       012345678901234567890123456
  char TimerDisp[28] =   {"Macro X nn -> Timer   (R-T)"};
  int  XLat1[6]     = {'A','M','x','S','T','X'};
  char XLat2[12][3] = {"01","02","03","04","05","06","07","08","09","10","11","12" };
  char XLat3[8][4]  = {"R-T","R-t","O-T","O-t","R-C","O-C","RcT","OcT" };
  b = 0; // b = 1,2,3,4,5,6,7,8 = TimerName[0-7] = Timer type Repeat T Repeat t Once T Once t Repeat C Once C RcT OcT (Clock Countdown Timers) 
  if (KeyBrdByteNum>0) { a = KeyBrdByte[0]; b = a - 48; }  // add 1-8 and a ='1'-'8' b=1-8
  
  //for (n=0; n<8; n++) TimerName[n][0] = 0x00;
   
  if (MST1==0) { strcpy(TimerName[b-1],  MTRname[Option1]); c = 1; } // Needs 10 byte filename
  if (MST1==1) { strcpy(TimerName[b-1],  STRname[Option1]); c = 3; } 
  if (MST1==2) { strcpy(TimerName[b-1],  TTRname[Option1]); c = 4; } 
  if (MST1==3) { strcpy(TimerName[b-1],  KBMacro[Option1]); c = 0; } // Needs 8 byte filename
  
  if (MST1>3)  { status("Select Source Macro for Timer"); return false; } 
  if (b>8||b==0||KeyBrdByteNum==0) { status("Need 1-8 option added"); return false; } 
  
  if (b==1) { MacroTimerArr1[0] = c; MacroTimerArr1[1] = Option1; } // [0] = Layout 1,3,4 [1] = Key 1-12
  if (b==2) { MacroTimerArr2[0] = c; MacroTimerArr2[1] = Option1; }
  if (b==3) { MacroTimerArr3[0] = c; MacroTimerArr3[1] = Option1; }
  if (b==4) { MacroTimerArr4[0] = c; MacroTimerArr4[1] = Option1; }
  if (b==5) { MacroTimerArr5[0] = c; MacroTimerArr5[1] = Option1; }
  if (b==6) { MacroTimerArr6[0] = c; MacroTimerArr6[1] = Option1; }
  if (b==7) { MacroTimerArr7[0] = c; MacroTimerArr7[1] = Option1; }
  if (b==8) { MacroTimerArr8[0] = c; MacroTimerArr8[1] = Option1; }

  TimerDisp[6]  = XLat1[c]; TimerDisp[20] = a; TimerDisp[8] = XLat2[Option1][0]; TimerDisp[9] = XLat2[Option1][1];
  TimerDisp[23] = XLat3[b-1][0]; TimerDisp[25] = XLat3[b-1][2];
  status((char *)TimerDisp);
  
  return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
void ListMacro()    // Source https://stackoverflow.com/questions/5100718/integer-to-hex-string-in-c
/////////////////////////////////////////////////////////////////////////////////////////////////////
// Cannot list macros here without a file - for example those loaded using *fm,s,t*
// Because DoFileBytes(0, NameStr) will not find file NameStr
// Works on macros loaded using serial port - they are saved to a file such as <3This is Key S3>
/////////////////////////////////////////////////////////////////////////////////////////////////////
{ String NameStr;
  int i, value;
  byte *BPtr;
  byte a, b;
  int m, n;  
  for (n=0; n<48; n++) Int2HexStr[n] = 0x20; // fill with spaces not NULL
      
  if (MST1==3) { NameStr = KBMacro[Option1]; }   // A - Use files Macro01 - Macro99                         
  if (MST1==0) { NameStr = MTRname[Option1]; }   // M - Use files MtrData1012
  if (MST1==1) { NameStr = STRname[Option1]; }   // S
  if (MST1==2) { NameStr = TTRname[Option1]; }   // T

  BPtr = MacroBuff; MacroBuffSize = DoFileBytes(0, NameStr, BPtr, ByteSize); 
  if (MacroBuffSize==0) { status("Macro's not found"); return; }              // See notes above must have existing file
          
  n = 0; m=0; 
  if (ScrSizeType==1) a = 8; else a = 5;                                // 480x240 or 320x240
  while (n<a) { value = MacroBuff[n];                                   // Contents in hex
                Int2HexStr[n+m]   = digits[(value >> 4) & 0x0F];   
                Int2HexStr[n+m+1] = digits[value & 0x0F]; n++; m=m+3; }  

  m = m + 3; 
  value = MacroBuffSize;                                               // Size in hex
  Int2HexStr[n+m]   = digits[(value >> 4) & 0x0F];   
  Int2HexStr[n+m+1] = digits[value & 0x0F]; n++; m=m+3; 

   b = 0x20; i = 0;
   while (b!=0) { b = NameStr[i]; Int2HexStr[n+m+i] = b; i++; }
                
  status((char *)Int2HexStr); // Should print up to first NULL
  
}

////////////////////////
void MakeStr(int button)
////////////////////////
{   int i, n;
    byte a, b, c;
    a = 0;
    Fx = false;     // Only true for one MakeStr type keypress at a time

    b = KeyBrdByte[KeyBrdByteNum] = KbrdLabel[KeyBrdX][button][KeyBrd123];  // b is current key label character or modifier
    
    KBrdActive = true; // Let [ADD] know any char key has been pressed at least once before

    if (KBType)                                                                          // Needed if Modifier key pressed first
       {if (KeyBrdX==0) if (0x60<b<0x7B) {a = b - 93;}                                   // ((b>0x60) && (b<0x7B)) Xlate a-x
        if (KeyBrdX==1) if (0x40<b<0x5B) {a = b - 61;}                                   // ((b>0x40) && (b<0x5B)) Xlate A-X but leave DelK = 0x4C
        if (KeyBrdX==2) if (0x2F<b<0x3A) {if (b==0x30) a = b - 9; else a = b - 19; } }   // ((b>0x2F) && (b<0x3A)) Xlate 0-9 to 1-0  

    if (KeyBrdX==2) {if (button==10) {a = KeyBrdFx[KeyBrdF1F24]; Fx = true;}             // Xlate F1-F24 - see if (a>0) below where it is used 
                     if (button==9)  {a = b = KeyBrdBrackets[BracketsNum][0]; BracketsNum++; if (BracketsNum==8) BracketsNum=0; }
                     if (button==8)  {a = b = KeyBrdSymbols[SymbolsNum][0]; SymbolsNum++; if (SymbolsNum==17) SymbolsNum=0; } 
                     if (button==6)  { for (n=0; n<4; n++) { b = StarCode[StarNum][n]; KeyBrdByte[n] = b; KBDisp[n] = b; } 
                                       KeyBrdByteNum = 4; KBDispPos = 4; status((char *)KBDisp); 
                                       StarNum++; if (StarNum==27) StarNum = 0;      return; } }
              
    if (KeyBrdX==3) {if (button==0)   { ListMacro();                                  return; } 
                     if (button==1)   { RenameMacro(); InitCfg(0);                    return; }     
                     if (button==2)   { RemoveMacro(); InitCfg(0);                    return; }   
                     if (button==3)   { if (SendMacro()) status("Macro Sent");        return; }  
                     if (button==4)   { GetTimerMacro();                              return; } 
                     if (button==5)   { CopyMacro(); InitCfg(0);                      return; }    
                     if (button==6)   { LinkMacro();                                  return; }    
                     if (button==8)   { DoSourceMacro(); status("Source Macro");      return; }  
                     if (button==9)   { DoDestMacro();   status("Destination Macro"); return; }
                     if (button==10)  { DoNumMacro();                                 return; }
                     if (button==11)  { if (SaveMacro()) status("Macro Saved");       return; } }                     
                     
    if (KeyBrdX==4) {KeyBrdByte[KeyBrdByteNum] = KeyBrdNum[KeyBrd123][button];           // Get special code such as ShiftLeft etc
        for (n = 0; n <= KeyBrdByteNum; n++) {Serial.print(KeyBrdByte[n], HEX); Serial.print(' '); } Serial.println(); }
           
    // n = 0;
    if (KeyBrdX<4) 
       {KBDisp[KBDispPos] = b; KBDisp[KBDispPos+1] = ' '; KBDisp[KBDispPos+2] = ' '; 
        if (a>0) KeyBrdByte[KeyBrdByteNum] = a; 
        if (Fx) { for (n=0; n<3; n++) {KBDisp[KBDispPos+n] = KeyBrdFxChr[KeyBrdF1F24][n]; }  KeyBrdF1F24++; if (KeyBrdF1F24>23) KeyBrdF1F24=0; } } // Fx true 1 MakeStr()
        
    if (KeyBrdX==4) for (n=0; n<3; n++) KBDisp[KBDispPos+n] = KeyBrdSpc[KeyBrd123][button][n];
    
    for (i = 0; i <= KBDispPos+n; i++) {Serial.print(KBDisp[i]);  Serial.print(' '); } Serial.println();
    for (int n = 0; n <= KeyBrdByteNum; n++) {Serial.print(KeyBrdByte[n], HEX); Serial.print(' '); } Serial.println();    

    status((char *)KBDisp);

    if (KeyBrd123<2) KeyBrd123++; else KeyBrd123=0; // chars per key 0 1 2 repeat
}
/////////////////////////////////////////////////////////////////////////////////////////////// 
bool SaveMacro()    
///////////////////////////////////////////////////////////////////////////////////////////////
// Saves current Source Macro when it has no file on flash - if already a file just return
// When Key receives data from Serial port or via *fm,s,t* then MacroSizeM1M12[1-12]
// Size = 0 and MacroM1M12[1-12] Status = 0
// When Key receives data from KeyBrd Macro Editor then Size = number of bytes and Status = 2
// for both String type (1st byte <0x80 and Modifier type (1st byte > 0x7FF)
///////////////////////////////////////////////////////////////////////////////////////////////
{ int m, n;        
  byte * BPtr;
  
  n = 0;
  if (MST1==0) { if (LittleFS.exists(MTRname[Option1])) { status("[M] Macro already saved"); return false; }
                 for (n=0; n<ByteSize; n++) { if (Mtr1to12[Option1][n]==0x00) break;  } MacroM1M12[Option1] = 2; BPtr = Mtr1to12[Option1];  
                      MacroSizeM1M12[Option1] = n; DoFileBytes(1, MTRname[Option1], BPtr, n); }  // Save Destination File

  if (MST1==1)  { if (LittleFS.exists(STRname[Option1])) { status("[S] Macro already saved"); return false; }
                  for (n=0; n<ByteSize; n++) { if (Str1to12[Option1][n]==0x00) break; } MacroS1S12[Option1] = 2; BPtr = Str1to12[Option1];  
                      MacroSizeS1S12[Option1] = n; DoFileBytes(1, STRname[Option1], BPtr, n); }  // Save Destination File 

  if (MST1==2) { if (LittleFS.exists(TTRname[Option1])) { status("[T] Macro already saved"); return false; }
                 for (n=0; n<ByteSize; n++) { if (Ttr1to12[Option1][n]==0x00) break;  } MacroT1T12[Option1] = 2; BPtr = Ttr1to12[Option1];  
                      MacroSizeT1T12[Option1] = n; DoFileBytes(1, TTRname[Option1], BPtr, n); }  // Save Destination File 
 
  if (n>1) return true; else return false;        
}

/////////////////////////////
void MakeMathStr(int button)
/////////////////////////////
{   int i, n;
    for (i=0; i<4;  i++) {MathHexNum[i] = MathNum[MathX][Math123][button][i];     // [KeyBrd 0-3=MathX][Symbol 0,1,2 = Math123][button 1-11]
                          MathChr[i] = MathHexNum[i]; }                           // MathNum = 4 hex chars + 0x00 -> use i<4 not i<5                          
    MathChr[i] = Spc;                         
    for (n=0; n<32; n++) {MathChr[n+i+1] = MathName[MathX][Math123][button][n]; if (MathChr[n+i+1]==0) break; }          
                          
    for (n=0; n<5; n++) {Serial.print(MathHexNum[n]); Serial.print(' '); } Serial.println(MathChr);   
    status((char *)MathChr);

    if (Math123<2) Math123++; else Math123=0; 
}

////////////////////////
void SendMath()
////////////////////////
{   int n;
    uint8_t keycode[6] = { 0 };     // simultaneous keys pressed in here    
    /////////////////////////////////////////////////////////////////////////////////////////
    // 1D6D1 + Alt + X = small pi works in MS Word and LibreOffice (1D6D1 = 120529)
    // Watch out for auto-correction if the symbol is the first in a sentence/line.
    /////////////////////////////////////////////////////////////////////////////////////////
    for (n=0; n<4; n++) { usb_hid.keyboardPress(HIDKbrd, MathHexNum[n]); delay(keydelay2);   // MathNum = 4 hex chars + 0x00 -> use i<4 not i<5
                          usb_hid.keyboardRelease(HIDKbrd);              delay(keydelay2);}
                          
      for (n=0; n<6; n++) keycode[6] = 0x00;                      
      keycode[0] = AltL;
      keycode[1] = 0x1B; // char X from hid.h       
      usb_hid.keyboardReport(HIDKbrd, 0, keycode);      delay(100);
      usb_hid.keyboardRelease(HIDKbrd);                 delay(keydelay2);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Options Indicators status 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// #define OPT_X 105   // Centred on this - Sppace in RH side about 16 pixels
// #define OPT_Y 88    // 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// num16[17][3] = {"1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16"};
//                 button =   0     1     2     3     4     5     6     7     8     9     10    11
// char TimerStr1[12][3] = { "TR", "tR", "TO", "tO", "CR", "CO", "Cr", "Co", "Rp", "Sp", "  ", "  "  }; // MacroTimer1 -> MacroTimer8 + Rep for optionindicator
// char TimerStr2[12][4] = { "R-T","R-t","O-T","O-t","R-C","O-C","RcT","OcT","Rep","Stp","   ","   " }; // MacroTimer1 -> MacroTimer8 + Rep for status line
// int TimerStrN[12]     = {  0,    9,    2,    6,    1,    11,   3,    7,    4,    8,    5,    11   }; // Xlat Macro Timer Button Number to Disp Str 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void optionsindicators(int Option) {
  int ColArr[5] = {Cyan, Yellow, Green, White, Purple};
  char MSTArr[5][2] = {"M","S","T","A","x"};
  char SrcDst[3]  = {"xx"};
  char NumStr1[3] = {"00"};
  char NumStr2[3] = {"00"};
  char TimerDisp[3] = {"  "};
  int b;
  
  SrcDst[0]    = MSTArr[MST1][0]; SrcDst[1] = MSTArr[MST2][0];
  NumStr1[0]   = (Option1+1)/10 + 48; NumStr1[1] = (Option1+1)%10 + 48;
  NumStr2[0]   = (Option2+1)/10 + 48; NumStr2[1] = (Option2+1)%10 + 48; 

  if (Option<=11) {  b = TimerStrN[Option]; TimerDisp[0] = TimerStr1[b][0]; TimerDisp[1] = TimerStr1[b][1]; }
  MacroTimer18 = MacroTimer8 || MacroTimer7 || MacroTimer6 || MacroTimer5 || Macrotimer4 || MacroTimer3 || Macrotimer2 || MacroTimer1; 
  
  tft.setTextPadding(32);  // 28 too small to  erase previous letters
  tft.setTextColor(ColArr[MST1], Black);
  if (ScrSizeType==1) tft.setFreeFont(&FreeSansBold9pt7b);
  if (ScrSizeType==0) tft.setFreeFont(&FreeSans9pt7b);
  tft.setTextDatum(1);
  if (KeyBrdDirect&&Kbrd)      tft.drawString("D", OPT_X, OPT_Y+182);           else { tft.drawString(" ", OPT_X, OPT_Y+182);     } // 3rd line has 4 places
  if (MacroTimer18)            tft.drawString(TimerDisp, OPT_X+110, OPT_Y+182); else { tft.drawString(" ", OPT_X+110, OPT_Y+182); } 
  if (Kbrd)                    tft.drawString(SrcDst, OPT_X, OPT_Y);            else { tft.drawString(" ", OPT_X, OPT_Y);         }
  //if (KeyBrdDirect)            tft.drawString("D", OPT_X, OPT_Y);             else { tft.drawString(SrcDst, OPT_X, OPT_Y);      }
  //if (!Kbrd) if (MacroTimer18) tft.drawString("T", OPT_X, OPT_Y);             else { tft.drawString(" ", OPT_X, OPT_Y);         } // Must come after the previous line
  if ((Option1<100)&&(Kbrd))   tft.drawString(NumStr1, OPT_X+110, OPT_Y);       else { tft.drawString("  ", OPT_X+110, OPT_Y); }
  if ((Option2<100)&&(Kbrd))   tft.drawString(NumStr2, OPT_X+220, OPT_Y);       else { tft.drawString("  ", OPT_X+220, OPT_Y); }
  if (KeyFontBold) tft.setFreeFont(&FreeSansBold12pt7b);
              else tft.setFreeFont(&FreeSans12pt7b);   
}
////////////////////////////////////////////////////////////////////////////
// #define IND_X 105  // Centred on this - Space in RH side about 16 pixels
// #define IND_Y 180  // 
// IndXPos[7] = {0, 0, 78, 156, 0, 78, 156};
////////////////////////////////////////////////////////////////////////////

void indicators() 
{ if (ScrSizeType==1) tft.setTextPadding(18);
  if (ScrSizeType==0) tft.setTextPadding(14);
  
  tft.setTextColor(Green, Black);
  
  if (ScrSizeType==1) tft.setFreeFont(&FreeSansBold12pt7b);
  if (ScrSizeType==0) tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setTextDatum(1);
  if (CapsLock)    tft.drawString("C", IND_X, IND_Y);      else tft.drawString(" ", IND_X, IND_Y);
  if (NumLock)     tft.drawString("N", IND_X+110, IND_Y);  else tft.drawString(" ", IND_X+110, IND_Y);
  if (ScrollLock)  tft.drawString("S", IND_X+220, IND_Y);  else tft.drawString(" ", IND_X+220, IND_Y);
  
  if (KeyFontBold) tft.setFreeFont(&FreeSansBold12pt7b);
              else tft.setFreeFont(&FreeSans12pt7b);   
}  

////////////////////////////
void touch_calibrate()
///////////////////////////
{ LittleFS.begin();
    
  if (LittleFS.exists(CalFile))        
     {if ( DoCal)                
         { LittleFS.remove(CalFile);}             // Delete if we want to re-calibrate
      else {File f = LittleFS.open(CalFile, "r"); // check if calibration file exists and size is correct
            if (f) {if (f.readBytes((char *)calData, 14) == 14) calDataOK = 1;  f.close(); }  }  }

  if (calDataOK && !DoCal)         // calibration data valid
     {tft.setTouch(calData);}    
     else { tft.fillScreen(Black); // data not valid so recalibrate
            tft.setCursor(20, 0);
            tft.setTextFont(2);
            tft.setTextSize(1);
            tft.setTextColor(White, Black);
            tft.println("Touch corners as indicated");
            tft.setTextFont(1);
            tft.println();
            if (DoCal) 
               { tft.setTextColor(Red, Black);
                 tft.println("Set DoCal to false to stop this running again!"); }
            tft.calibrateTouch(calData, SkyBlue, Black, 15);
            tft.setTextColor(Green, Black);
            tft.println("Calibration complete!");
            File f = LittleFS.open(CalFile, "w");  // store data
            if (f) { f.write((const unsigned char *)calData, 14); f.close();  }  }
}