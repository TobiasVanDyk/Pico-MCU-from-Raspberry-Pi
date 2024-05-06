# Pico Touch Macropad Manual with SDCard included
```

-----------------------------------------------------------------------------------------------------------------------
Layout 1 - M Keys - [M1]-[M24] - Cycle through Layout 1 to 4 press [L1-L4] or [Vo] 
-----------------------------------------------------------------------------------------------------------------------
[Cut CtrlX] [Copy CtrlC] [Paste CtrlV] [VolUp Various]       [  Cut ] [  Cpy ] [  Pst ] [V+][ Var ] [A-B]
[ TaskMan ] [  Adm CMD ] [    Run    ] [VolMute L1-L4]       [M1 M19] [M2 M20] [M3 M21] [Vo][L1-L4] [MST]
[ PC Info ] [  Adm PS  ] [ Taskbar 2 ] [VolDwn  Enter]       [M4 M22] [M5 M23] [M6 M24] [V-][ Ret ] [B-A]
         Caps          Num         Scroll                            C        N        S

[Var]ious 24 options for key 4 [Del]ete and key 12 [Ret]urn and keys 1,2,3 [cX Cut][cC Cpy][cV Pst]:
1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17  18  19 20  21  22  23  24
Del Bks Tab aTb Ins Esc PrS aPr Ret Snp Osk Num Cap Scr Cut Cpy Pst Tsk Run wX CPi Ts1 Ts2 Ts3
i.e.  1 Delete 2 Backspace 3 Tab  4 AltTab 5 Insert 6 Esc 7 PScr 8 AltPScr 9 Return 10 Snipping 11 OnScrKeyBrd
12 NumLock 13 CapsLock 14 ScrollLock 15 Cut 16 Copy 17 Paste 18 TaskMan 19 Run 20 GuiX 21 Copilot 22-24 Tskbar1-3
           
Top row 3 keys cX-Cut, cC-Copy, cV-Paste: Programmable as x1, x2, x3 Layout 1, and x4, x5, x6 for Layout 3 and 4. 
Can use any of the [Various] actions for x1 - x6. Program as *xn*number n=1-6, number = 1-24

Note: Press [Cfg] once, and then press [Var] to select either the [De] or the [Ret] key 24 options which are selected
by pressing the red bottom Pad [o] which will cycle through the 24 options for both the [Del]ete key top right and the
[Ret]urn key bottom right. The status line will show which key is chosen and which option is selected. To save the
choice press the [Sav] key just below it, else it will reset to the default [Del] and [Ret] after the next reboot.

Note: The key M1-M24 macros listed above are default macros - use the macro editor to configure new macros or copy, 
rename and/or chain/link new or existing macros. Some of the default macros run items on the pinned on the taskbar,
and are indicated by GuiL + Key0 - Key9 etc in macroBanks.h WinMacro content.

Mote: The the 8-cycle [A-D] key in Layout 2: If the A-D indicator shows a white A, B. C, or D then the Layouts 
Mx Sx Tx use the assigned with *fm* *fs* *ft* strings. If the indicator is orange then then the Layouts Mx Sx Tx use
the SDCard files or the Coded functions. To save the A-D (orange or white) choices press [Sav] else the currently saved
choice will be used after the next reboot. Note that the A-D page chosen using the Pad keys [a] or [d] are not saved
between a reboot - you must use [Cfg][A-D][Sav] sequence to save both White/Orange[A-D] state.
-----------------------------------------------------------------------------------------------------------------------
Layout 2 - Config - Cycle through Layout 1 to 4 press [L1-L4] or [Vo] 
-----------------------------------------------------------------------------------------------------------------------
[Hme  Stop StartL1L4] [UpArr   Var] [PgeUp         Vol] [VolUp  Delete]        [Hme] [Up ] [Pgu] [V+][Del]
[ArrL Prev       A-D] [Config Save] [ArrR  Nxt Mute1L4] [VolMute L1-L4]        [  <] [Cfg] [>  ] [Vo][L14]
[End  PlayP MacroTim] [DwnArr POff] [PgeDwn      Media] [VolDwn Return]        [End] [Dwn] [PgD] [V-][Ret]
Macro: Source Num      Target Num                                              S-nn  T-nn      
                    Caps          Num               Scroll                          C     N     S

Layout 2 (Config) has five additional small pad-buttons on the right side (from top to bottom):
[m] Math KeyPad on/off 
[k] Macro Composition Keypad on/off.
[e] Mouse Keypad on/off.
[n] Number Keypad on/off.
[o] Options Pad: Config Mode   - Toggle Capslock and Numlock on/off in combinations
                 Macro Mode    - Direct Mode On/Off (Blue "D" indicator).
                 NumPad Mode   - Switch between 3 NumPad pages.
                 [Var]key Mode - Select 24 options for [Del and [Ret] keys
-----------------------------------------------------------------------------------------------------------------------
Layout 2 - Full Media Mode - Play Controls On - Volume Controls On - Tone Controls On 
-----------------------------------------------------------------------------------------------------------------------
[     Stop     ] [ Bass Up ] [ Treble Up ] [Volume Up  ]        [|||] [B+ ] [T+ ] [V+]
[Previous Track] [ Config  ] [ Next Track] [Volume Mute]        [ |<] [Cfg] [ >|] [Vo]
[  Play-Pause  ] [Bass Down] [Treble Down] [Volume Down]        [>||] [B- ] [T- ] [V-]
                                         
               Caps        Num          Scroll                       C     N     S  

Note: Bass and Treble Controls only works with SoundBlaster Audigy and X-Fi Audio Cards

The 5 Pads has the following functions in Layouts 1, 3, and 4 (M S T Keys):
[a][d]    Toggle Layers A B C or D with M1-M6->M19-M24, S1-S6->S19-S24, T1-T6->T19-T24
[m][s][t] Move to Keys M, S or T Layout Pages
Note: If the same Pad Key is pressed in its own Layout - for example press pad S when in the [S1] - [S24] keys layout,
it will then ignore any assigned strings and use the Coded programmed action. Toggle it on/off by pressing the 
Pad once or twice. This applies for all three keysets M, S and T. The A,B,C,D indicator will change from white to 
orange when this option is On, and back to white when Off. For the set of S keys only: If the SDCard option is enabled,
the orange A-D indicator will result in files stored on the SDCard being used when pressing [S1] to [S24].
-----------------------------------------------------------------------------------------------------------------------
Layout 3 - S Keys - [S1]-[S24] - Cycle through Layout 1 to 4 press [L1-L4] or [Vo] 
Layout 4 - T Keys - [T1]-[T24] - Cycle through Layout 1 to 4 press [L1-L4] or [Vo] 
-----------------------------------------------------------------------------------------------------------------------
[Cut Ctrl+X][Copy Ctrl+C][Paste Ctrl+V][VolUp DelBsTI]  [ cX  ] [ cC  ] [ cV  ] [V+][DB-TI][A-B]
[Macro 1/19][Macro 2/20 ][Macro 3/21  ][VolMute L1-L4]  [ST  1] [ST  2] [ST  3] [Vo][L1-L4][MST]
[Macro 4/22][Macro  5/23][Macro 6/24  ][VolMute Enter]  [ST 22] [ST 23] [ST 24] [V-][ Ret ][B-A]
          Caps          Num         Scroll                     C       N       S

-----------------------------------------------------------------------------------------------------------------------
Layouts 1, 3 and 4, four Layers A B C D each with six keys: A = M1-M6, S1-S6, T1-T6 to D = M19-M24, S19-S24, T19-T24
-----------------------------------------------------------------------------------------------------------------------
To cycle through the Layouts press [L1-L4]. Long-press [L1-L4] to toggle [Vo][L1-L4]. Also use the three Pads 
[m][s][t] to move to Layers [L1 L3 L4] and use the top or bottom pads [a][d] to move through the four layers a,b,c,d.

When the [L1-L4] or [Vo] key is held in for more than 1 second it will change between the two key states [Vo][L1=L4]
in any of the Layouts 1-4. Note that when in Layout 2 Cfg it will is easier to switch to the [Vo] key using the
[Cfg][Vo] key.

Press Config Key (new set of config keys change colour):
Vo  [ArrowRight] - VolumeMute -> L1, L2, L3, L4 - repeat to restore Vo
Var [ArrowUp]  ] - Select from 24 key actions (as below) for [Del] and [Ret] keys - press once or twice. 
Vol [PageUp]   ] - Vol+ -> Delete and Vol- -> Enter - repeat to restore V+ V-
A-D][ArrowLeft ] - Layouts 1, 3, 4, has 8 cycles: change to Layer A B C D white (Text) and orange (SDCard/Coded)
Med [PageDown  ] - Change Layout 2 to Media Controls Previous-Next-PlayPause-Stop
mCT [End       ] - Macro Timer Trigger Repeat and Oneshot Countdown or Clock Timers
Sta [Home      ] - Start with Layout 1 to Layout 4 on powerup - press [Cfg][Sta] repeatedly
Sav [Config    ] - Info and File List to Serial Monitor and Text/Macro and Config files saved
ROf [ArrowDwn]   - Restart-PowerOff-Logoff - Bottom row [Rst][Log][Off] - cancel by pressing 
    [Cfg][ROf]   - Includes long or short Timer options as well and Countdown and Clock Timers

Select Key Actions [Del] and [Ret] keys:
1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17  18  19 20  21  22  23  24
Del Bks Tab aTb Ins Esc PrS aPr Ret Snp Osk Num Cap Scr Cut Cpy Pst Tsk Run wX CPi Ts1 Ts2 Ts3
-----------------------------------------------------------------------------------------------------------------------
Mouse Keyboard: When in Layer 2 - press middle pad [u] to toggle Mouse KeyBoard on/off - these are mouse buttons, 
                scroll, and cursor movement keys:

[LBt] [Up ] [RBt]  [Sc+]         [Left-Button] [  Cursor-Up  ]  [ Right-Button ] [   Scroll-Up     ]
[ < ] [MBt] [ > ]  [L2x]         [Cursor-Left] [Middel-Button]  [ Cursor-Right ] [Double-Left-Click]
[Bck] [Dwn] [Fwd]  [Sc-]         [Back-Button] [ Cursor-Down ]  [Forward-Button] [  Scroll-Down    ]

Note that a Mouse Right-click can be also programmed as a Shift and F10.

-----------------------------------------------------------------------------------------------------------------------
Macro Composition Keyboard:

Keyboard Page 1          Page 2         Page 3              Page 4               Page 5
[abc][def][ghi][EXE]  [ABC]-[XY_]  [012][345][678]   [Lst][Ren][Rmv][Snd]   [ALT][SHF][CTR] 
[jkl][mno][pqr][NXT]   Uppercase   [9+-][/=*][*Cm]   [Snd][Cpy][Lnk][NXT]   [GUI][TEI][CRF] 
[stu][vwx][yz ][ADD]     Page 1    [Sym][Brc][Fnn]   [Src][Dst][Num][Sav]   [LHR][UED][UND]  
                                                      Snn  Tnn
                                                    Source Target 
                                           Macro Selection: M01-M24 S01=S24 T01-T24 A01-A99
Page 1: [xy ] = x y space
Page 2: [XY_] = X Y underscore  Page 1 and 2 + Caplock reverse characters
Page 3: [Fnn] F1-F24  [Sym] 17 symbols 
        [Brc] 8 bracket symbols  
        [*Cm] Insert from 45* Star codes - do not press [ADD] if required press Number[ADD][EXE]

Page 4: Macro Tools                                 Page 5: Modifiers 
[Src] Macro Source M S T or A(ll)                   [ALT] = [ Alt-L ][  Alt-R ][ PrintScr] [EXE]
[Dst] Macro Target (Destination) M S T              [SHF] = [Shift-L][Shift-R ][ Delete  ] 
[Num] Macro Number M S T = 1-12 A = 1-99            [CTR] = [Contr-L][Contrl-R][BackSpace]  
[Cpy] Copy [Src][Num] to [Dst][Num] Macro           [GUI] = [ Win-L ][ Win-R  ][  NULL   ] [NXT]
[Rmv] Remove [Src][Num] Macro                       [TEI] = [  Tab  ][ Escape ][ Insert  ]  
[Ren] Rename current[Src][Num] Macro                [CRF] = [  C/R  ][   L/F  ][  Return ]   
[Lnk] Link [Src][Num] and [Dst][Num] Macro          [LHR] = [Arrow-L][  Home  ][ Arrow-R ] [ADD] 
      Then optionally assoc with [Src]NumDisplayed  [UED] = [ArrowUp][  End   ][Arrow-Dwn]  
[Lst] List first 8 bytes contents of source macro   [UND] = [PageUp ][ Numlock][ PageDwn ]
[Snd] Send Source Macro directly 
[Sav] Save Source Macro to Flash (overwrite if already saved)
[Tmr] Macro Timer options (One-shot or Repeat)

-----------------------------------------------------------------------------------------------------------------------
Note: The Macro Destination [Dst] is also referred to as the Target Macro here in some cases.

Note: To choose between Macro A = 1 to 99 does not require 99 key-presses - just hold the [Num] key down for key-repeat
It cycles through 1-99 in about 5 seconds.

Note: If a shorter string replaces a longer string end the shorter string with a NULL char - press [GUI]3x and then 
press [ADD] at the end of shorter string. For example Key [S2] contains "notepad" which is 7 chars. To replace it with
*188# select [S2] as the destination, press [Dst] then [Num] until 02 shows, then press [*=/], press [ADD], press [*=/]
and [ADD] again (** is not treated as special command but as the chr *), then press [012]2x[ADD], [678]3x[ADD], again 
[678]3x[ADD], [Sym]2x[ADD], [NXT]2x, [GUI]3x[ADD], then press [EXE] and then the [Up] key to save.
                                                                          
The keyboard has 5 pages - most with 9 triple function keys, and 3 control keys [EXE] [NXT] [ADD]. For example page 1 
has keys [abc], [def], to [y,z,space]. To select a or b or c press the abc key once, twice or thrice - to add it to a 
new macro press the ADD key else press another character-symbol modifier key or press [NXT] for the next keyboard page. 

Press Pad 2 again to leave the keyboard or press [EXE] to send the macro to the PC and save to the (Source-Destination)
or SD Macro Key as indicated by the second number 1 to 24. Change the target number by pressing the [Dst] then the [Num]
key. The macro is saved when the Up-Arrow key is pressed after the [EXE] key. Refer to the examples below on how to copy
commands between the KeyBrd macros (a01-a99 files), and the 3 sets of 24 keys M, S, T files s01-s24, m01-m24, t01-t24.

The macros on page 5 are modifiers (simultaneously pressed keys) such as Control + Alt + Delete + GUI (maximum of 6 keys
in macro). To send this sequence press [CTR][ADD][ALT][ADD][SHF]x3 (3 times for delete)[ADD][EXE]. 

To open the windows snipping tool press [GUI][ADD][SHF][ADD][NXT][s][ADD][EXE] - the snipping tool will open. This can 
then be saved to [M2] by pressing the [Up] button. 

To send a sequence such as [WinKey] + "r" press [GUI][ADD][NXT][pqr]x3[ADD][EXE]. To assign Alt + PrintScr to key [M2]
press [NXT]4x[ALT][ADD][ALT]3x[ADD][EXE} and [Up] to save. Alt + PScr is then executed when pressing [M2]. 

Another example: [GUI][ADD][NXT]1x[r][EXE][Up] then press [n][ADD][o][ADD][t][ADD][e][ADD][p][ADD][a][ADD][d][ADD] 
[NXT]3x[CRF][ADD][NXT][h][ADD][e][ADD][l][ADD][l][ADD][e][ADD][EXE][Up]. After this press just two keys [M2][M1] to 
open notepad and insert the text "hello". To make the text larger [CTR][ADD][NXT]3x[9+-]2x to make the text larger.

Toggle the sticky keys: Press [SHF][ADD] 5 times then press [EXE] and save to key [M2]. (For a complete macro add 
[C/R] [ADD] key at the end of this sequence.) 

The option to use the combined modifier bit instead of a modifier byte, is used in the top row Cut-Copy-Paste keys, and
also for some of the pre-programmed examples for the M1-M24 keys. 

Macros sent to the PC from the built-in keyboard will be saved to file aX X = 01-99, and if selected, assigned to key
[MX]. Press [Up] after sending the macros to the PC to assign them to [MX]key. 

To set it up at first (displays red xx = Src Num Dst Num) press [Src] once then [Dst] - it will now show M01 M01 i.e.
the source and destination macro is M1 and M1. 

Note: The [Cpy] key on page 4 is now the most direct way to copy the [Src][Num] to [Dst][Num] Macro. The [*Cm] key 
      cycles through all available star codes and insert it directly (no [ADD] required except if the *code* is 
      followed by numbers then press [ADD] after inserting the number). For example press [*Cm] until *bb* is 
      displayed then press then press [678][ADD][EXE] to set the LCD Brightness to 25 percent.
      
Example 1: Set up M01 M04 as SrcNum DstNum - then press [CTR][SHF][TEI]2x[EXE][UP] - press [Up] to save to key Target
          (Destination) key [M4]. Press [M4] and the (Windows) Task-Manager opens (Ctrl+Shft+Esc).
Example 2: Set up M01 M01 - press [GUI][r][EXE][Up] - save to key [M1] - file list shows two files a01 and m01 - both
           3 bytes length - setup Source M01 then press [Lst] it shows E3 15 00 which are in hexadecimal [GUI][r][NULL]
           Two copies were made as only the aXX files are used for linking. Copy M01 to Key [S4] - Set up A01 S04 then 
           press [Cpy]. Press [M4] to test and setup SrcNum S04 then press [Lst] - E3 15 00 00 (one 0x00 added). Press 
key [S4] to open the run window.
Example 3: Set up A01 S01 - then [Cpy] - file list has s01 4 press [S1] open run command window
Example 4: Set up A04 S02 - then [Cpy] - file list has s02 5 press [S2] open TaskMan
Example 5: Set up S04 M04 - press [GUI][r][EXE][Up] - press [S4] to open run window
           Set up M04 S02 - then [Cpy] - press [S2] open run windows 
Example 6: Set up A04 T07 - Rename a04 (must exist) to t07 - press [Ren] - then press [T7]

Linking Macro Examples: M macros are associated with file-macros a01-a24 
                        S macros are associated with file-macros a25-a48
                        T macros are associated with file-macros a49-a72 -> linking T20+T21 link files a68+a69

Example 1: Program [M1] with a Open Run windows [GUI][r] 
           Program [M2] with a notepad+C/R
           Program [M3] with hello+C/R
           Program [M4] with [CTR][SHF][Esc] Open Taskman
           
           Then set up SrcNum DstNum M01 M02 - Press Link [Lnk], then press [M1] it opens the run window, and then opens
           notepad. Set up M01 M03 - Press Link [Lnk], then press [M1] it opens the run window, then runs notepad, then
           enter text "hello" in Notepad. Again repeat above line - it should write hello twice when pressing key [M1]
           
           If no number is added via [012][345][678][9][ADD] then keys [M1][S1][T1] are used as the default with the Src
           determining whether it is M, S, or T and the DstNumber determining which macro number a01-a99 are added. If 
           a number is added for example 5, then those SrcNumbers [M5][S5][T5] are used as the key to run the macro (or
           linked macros).
           
Example 2: Program M1 and M4 (GUI+r and Ctr+Shf+Esc) i.e. M01 M04. Then go to the numbers page and add 5 i.e. press 
           [345[3x[ADD] then goto the Macro Tools page where M01 M04 is still visible and press [Lnk]. Key [M5] (not 
           keys [M1] or [M4]), will then open the Run window, and then also open the TaskMan. 
Example 3: Setup Source S04 and Target M02, and do not add any numbers - press [Lnk] - will then link files a04 and a02
           to key [S1].
Example 4: Setup Source M04 Target M02, and do not add any numbers - press [Lnk] - will then link files a02 and a02 to
           key [M1] i..e pressing key [M1] will run the macro in file a02 twice. Repeat this again, and the number of 
           times will increase by one each time. 
                
Note 1: To unlink send *ul* with the Macro Key to be unlinked visible as the Source Macro such a Mx mm xx.
Note 2: Linked Macro Data will be lost after a power cycle or reset unless the save-restore option is turned on by 
        executing an *lr* command - then also press the black [Cfg] button twice to save the option to flash. An *ld* 
        will send a Link and Macro datadump to the serial port. 
Note 3: Pressing [Cpy] is the same as *cm* [EXE]. Macro-linking has been implemented for all 72 M S T 1-24 keys.
Note 4: All keys have the same repeat period of 0.5 seconds - holding a key down after this time has elapsed will 
        trigger a fairly rapid repeat.

F1-F24 keys are all one key [Fnn] and are sent as keycodes (simultaneous) and not keypress types - to send [F3] open
the macro keyboard then press [NXT]2x[Fnn]3x[ADD][EXE]. Press [Up] to assign it to a [Dst][Num] key.

The Keyboard has a Direct (to PC) Mode - use the Yellow Options Pad on the bottom right, to switch Direct Mode On/Off.
A Blue "D" indicator will show if it is on. Any character selected (shows in status bar), will be sent to the PC by 
pressing [EXE] - [ADD] is not necessary. If a character or more than one characters have been [ADD]ed they will only 
be sent after Direct Mode is switched off.
-----------------------------------------------------------------------------------------------------------------------
The Keyboard can be used to change various options by sending *option*value commands. The [*Cm] key can also be used 
and then pressing the [Add] key after the last * is not necessary. If a number is needed after the second * then
pressing [ADD] is required. All *Codes return to same *Code with [*Cm] if [EXE] was pressed. *Codes are incremented 
to the next starcode if no [EXE} pressed. The main codes are listed below:

(1) Macro Timer Keys time values - default Short Time = 30 seconds and default Long Time 10 minutes. To change Timer
    values send Repeat Macro Timer *mt*num or *mT*num or Oneshot Macro Timer *nt*num or *nT*num where num 0 is
    3 hours t time and 300 hours T time mode. 1 = 30 seconds (2 hours for T) to 9 = 1 hour (48 hours for T). Associate 
    a Macro with a Timer in the KeyBrd Macro Tools Page using key [Tmr] after 1 to 8 have been [ADD]ed to information
    line at the bottom. Then the Source Number Macro will be linked to (1) and (2) Repeat-Timer T and t, (3) and (4)
    Oneshot-Timer T and t, (5) Repeat on Clock Tiem, (6) Oneshot on Clock Time, (7) Repeat Countdown Timer, 
    (8) Oneshot Countdown timer. Ensure that the matching key on the timer keybrd is pressed i.e. if you used 2 then 
    press key [R-t] else the timer will not fire. For a first test use R-t - i.e. select Source macro M01, select 2 
    and press [ADD],[Tmr], exit the Macro KeyBrd, press [Cfg][Mct][R-t].
(2) LCD blank timeout - Send the macro *tb*num with the built-in keyboard where num = 0,1-9 - *tb*0 = 120 hours, 
    *tb*1 = 30 seconds.  
(3) LCD blank dim value - Send the macro *db*num with the built-in keyboard where num = 0,1-9 - *db*0 = no dimming
    - just on/off after blank timeout, and 1-9 = 3 to 30 % PWM instead of blank. 
(4) LCD screen brightness - Send the macro *bb*num with the built-in keyboard where num = 0,1-9 - *bb*0 = full
    brightness and 1-9 = 3 to 50 % PWM brightness.
(5) Power Keys Menu/Command Option - Send the macro *po* with the built-in keyboard to toggle the Power Keys Menu
    option - use the OS Menu (such as GUI+X + U + i,u,r or the run command (such as GUI+R + shutdown + options) command.
    If the Power Menu is active then only the bottom three keys [Rst][Log][Off] and the black Escape key are functional.
(6) Power Keys time values - Default Short Time = 30 seconds and Default Long Time 600 seconds (10 minutes). To change 
    Time values send PowerOff *ot*num or *oT*num or Restart *rt*num or *rT*num where num 0 = 6000 second (100 minutes)
    1 = 30 second (x100 for T) to 9 = 300 second (x100 for T).
(7) Calibration Enable/Disable - Send the macro *ca* with the built-in keyboard to set/clear re-calibration option after 
    a restart to ON or OFF. 
(8) LCD Orientation 180 degree change - Send the macro *ro* with the built-in keyboard - the USB cable will then be on
    the opposite side. This forces a re-calibration on restart. 
(9) Key Font Bold/Normal change - Send the macro *fo* with the built-in keyboard to toggle the labels on the buttons from
    normal to bold.
(a) Key Font White/Black change - Send the macro *fc* with the built-in keyboard to toggle the labels on the buttons from
    white to black.
(b) Macro Copy - Copy a01-a99 to M,S,T keys. Can use *cm* if the SrcNum DstNum is set up - see the four examples 
    above. Else compose *cm*nnXmm via [ADD] where: nn = a01-a99 X = Keys M S T mm = 01-24
(c) Macro Unlink - unlink *ul* with the Macro Key to be unlinked visible as the Source Macro such a Src nn Dst mm. *ua*
    = unlink all macros.
(d) Macro Link Save-Restore On-Off - Linked Macro Data will be lost after a power cycle or reset unless the save-restore 
    option is turned on by executing *lr* command - then also press the black [Cfg] button twice to save the values to 
    flash. An *ld* will send a Link and Macro datadump to the serial port. A save setting for the linked macros do make
    the [Cfg] key slower as well as pressing the [Sav] key - it takes about 1-2 seconds to respond.
(e) To fill M S T 1-24 with hard-coded text string examples send *fm* *fs* *ft* or *fa* (all three) commands. For the
    S keys strings24.h is used and for the T keys stringt24.h is used.
(f) To overwrite and save to Flash M S T keys 1-24 send *sm* *ss* *st* or *sa* (all 3 sets M S T of 24 keys) commands.
    Note that after using *fs,m,t,a* the macros are not automatically saved - it is also necessary to do *sS,m,t,a* to
    save them (and to list their contents in part).
(g) To delete all macro and config files use "*de*" - will re-calibrate on restart. Remove macro files with the [Rmv] 
    or rename with [Ren] keys
(i) *ct* display four time clocks with a 1 second delay - Time, Macro[R-C][O-C], Macro[Rct][OcT] Power[R-C][O-C].
(j) *0R* Enable/Disable the resistor colour-coded number pad.
(k) *br* = toggle brightness controls up/down replace volume up/dwn for Layouts 1, 3, 4 (not in Layout 2 Cfg). The
    brightness slider usually only has an effect when used in notebook computers not desktops.
(l) *tt* *ta* *tp* *tw* Use *tx*yymmddwhhmm -> *tx*22110341200 12:00am 3 Nov 2022 Thursday where x = t,a,p,w
    t = Main Time/Clock a  = Macro Clock Repeat-Oneshot [R-C][O-C] p = Macro Clock Countdown [RcT][OcT] 
    w = Power Clock [O-C][R-C]. if using [*Cm] only add the numbers yymmddwhhmm w = weekday 0 = Sunday 6 = Saturday  
(m) *xn*number n=1-8, number = 1-24. Top row 3 keys cX-Cut, cC-Copy, cV-Paste: Programmable as x1, x2, x3 Layout 1, 
    and x4, x5, x6 for Layout 3 and 4. 24 options are: Del Bks Tab aTb Ins Esc PrS aPr Ret Snp Osk Num Cap Scr Cut
    Cpy Pst Tsk Run wX CPi Ts1 Ts2 Ts3. For example *x1*3 - 1st top-row key in Layout 1 (M) will change from [Cut]
    to [Tab]. (Press [*Cm] until *x1* shows then press [345] once, press [ADD] press [EXE]) To reset use *x1*0 
    will reset all six x1-x6 to Cut, Copy, Paste. Or *x4*6 - 1st top-row key in Layouts 2 and 3 [S] and [T] keys,
    will change from [Cut] to [Esc]. To reset use *x1*0 - will reset all six x1-x6 to the default Cut, Copy,and 
    Paste. Use *x7*n=1-24 for [Del]etekey changes and *x8*n=1-24 for [Ret]urn key changes.
(n) Mouse Cursor and Scroll movement: *m1*nn Scroll amount 1-10 (default 1) and *m2*nn Cursor move amount 1-99 
    (default 5). For example *m2*20 will change the cursor jump movement to 20 from the 5 default.          
(o) Media Key Options: Normally pressing the media key [Cfg] then [Med], only displays the four Media controls 
    Play-Pause, Next, Previous and Stop. To add the Volume Up and Down and the Mute keys press [Cfg][Vol] or 
    [Cfg][Vo]. By using *e1* to *e4* codes the action of this media key can be changed to display the (1) four
    Media Control Keys, (2) add the Volume Up/Down keys to the 4 control keys, (3) add the Mute key to (1) and (2), 
    (4) add the Tone control keys. To disable this additional behaviour use the *e0* command. Note that when 
    the media options (2-4) are enabled using *e2* to *e4*, pressing the media key off, will cancel all the
    other media keys as well. To save the media keys press [Cfg] then [Sav] then after a reboot the setting e0 to
    e4 is saved i.e. if the [Med] is pressed after a powerup, and the e2 setting was previously activated, then 
    both the Volume and Media control keys will show. The single key options [Vol], [Vo] for Volume and Mute are
    still active when the *en* options are activated.
(p) Operating System Option: Use *os*[EXE] to toggle between Windows and Linux. Currently only used for different 
    Restart or PowerOff or Logout options for the [ROf] key. These commands are working in Debian 11.7 but Linux 
    Mint 21.1 and Ubuntu 20.04 steal the focus from the Terminal to the Desktop search box. Note that using these 
    will show passwords as plaintext in the history. Edit ShutDwn1 string and change PASSWORD and USER.
(q) Serial monitoring (via USB) on/off using *se*. A Red/Green "S" will indicate the state (Off/On). Switch the 
    serial on if macros or other text strings are sent to the Touchpad such as DateTime, Foobar Track Info, or 
    HWInfo System Sensors. The state is saved.
(r) Skip first key pressed in LCD in dimmed state - used to wake LCD - toggle on/off with *ks* - the state is saved.
(s) Use SDCard files: When in S (Layer L3), press [s] Pad so that X shows instead of A,B,C,D. Keys [S1] to [S24] then
    send stored SDCard file as keyboard characters to PC - the size of the files/strings are only limited by the SDCard 
    size. Change between three SDCard file-sets of 24 files each with *sd*n where n=1,2,3. For example *sd*2[ADD][EXE]
    enables second set of files. The folder SDCardFiles has a set of example files. Change back to regular [S1]-[S24] 
    key strings by pressing [s] Pad again so that X does not show but A-D. Use *sd*0 - i.e. select 8sd* with [*Cm] then
    press 0[ADD][EXE] - to disable the SDCard files function. Re-enable with *sd*1,2,3. If a new SDCard is inserted it 
    may require pressing the hardware reset button of the TouchLCD once.
(t) Use *am,s,t*number namely *am*n or *as*n or *at*n with n=1,2,3,4,5 for Keys M S T. For n = 1,2,3 choose between 3 
    different sets of 24 macro actions each, for each set of 24 keys M,S,T or choose 24 differenr Run or CMD commands 
    with n = 4,5. Note that pressing WinKeyLeft = [GuiL] + [number key] such as [GUI1], [GUI2] etc will run the 1st, 
    2nd etc. items pinned to the taskbar.
    n=1 Combination of 6 Notepad Shortcuts + 6 Photoshop Shortcuts + 6 Firefox Shortcuts + 6 MSWord Special Characters
    n=2 24 Windows shortcuts: TskMan  StrtRC Run PCInfo Calc PwerSh FMan TskView WakePC APrtScr MouseRClk CtrAltDel 
                              NewFldr Prop   OSK GUI9   GUI6 GUI7   GUI8 GUI0    GUI=   GUIF1   GUI4      GUI5
    n=3 Same set of 24 actions that can be used for top row set of 4 keys and the [Del [Ret] keys:
    Del Bks Tab aTb Ins Esc PrS aPr Ret Snp Osk Num Cap Scr Cut Cpy Pst Tsk Run wX CPi Ts1 Ts2 Ts3
    n=4 Set of 24 Windows Run commands
    n=5 Set of 24 Windows Powershell commands
(u) *r0* Reset LCD - reboot after 2 seconds delay. 
    *r1* toggle action - enable/disable Reset-Once-On-Start
(v) *bl* toggle Black Key function on Powerkeys page - switch backlight on/off - enable/disable with *bl* To use 
    this first send *bl*[EXE] code then in Layer2 press [Cfg][ROf], and then press the Black Key (press twice the very 
    first time), to the switch backlight off - press on the same place of blank screen to switch the screen on again. 
    For PC's who supply USB power after switchoff this could be used to blank the LCD  screen after pressing one of the
    Power Keys for switch off. If disabled with *bl* pressing the black key will just exit the Power Keys screen.
(w) Key Held Duration: *kr*num with num = 1-9 or 100-900 msec key press wait duration before repeat.
(x) Key Held Enable: *ke* Enable/Disable Volume Mute Processing if [Vo][L1-L4 key is long-pressed
-----------------------------------------------------------------------------------------------------------------------
Math-Greek-Algebra Keyboard:
This is a triple-key macro keyboard with 4 pages and 4 x 9 x 3 = 108 math and Greek algebra
symbols. It is sent to the PC as (hex) Unicode + [ALT] + [x] and can be directly entered into MSWord and in LibreOffice
(also on Linux). The three control keys are [EXE] - send the symbol to the PC, [NXT] - next page of symbols, and [ESC]
go back to the main Config layout - or press the small blue pad again to leave the Math keyboard. Watch out for MSWord 
insisting on capitalising the first letter of a sentence - which will change the first math symbol in a sentence/line 
after a second symbol or letter or enter is typed. 

The [M6] key section in the code has a few examples of using Alt + Number Keypad for Maths and special symbols or 
characters. There are 2 ways to do it - the first is to type the character's Unicode (hex without the 0x or decimal) into 
MSWord and then press [ALT] + [x]. The second method is to hold the Alt key down, then type in a special code number using
the number keypad and then release the Alt key. There are examples of entering the open infinity symbol and the small pi 
symbol in the [M6] key section. 
-----------------------------------------------------------------------------------------------------------------------
Numeric Keypad    [ BackSpc] [ 7 Spc aA ] [ 8 % bB ] [ 9 xX cC ]  Press 4th Pad - toggle the Number Keypad on/off.
                  [ Return ] [ 4  ,  dD ] [ 5 . eE ] [ 6 =  fF ]  Press 5th Pad - switch Num Pages 1-3 - CapsLock a-A
                  [ 0  + ( ] [ 1  -   ) ] [ 2 *  [ ] [ 3 /   ] ]  *0R* Enable/Disable the resistor colour-code.
-----------------------------------------------------------------------------------------------------------------------
Power Restart KeyPad [Restart Timer] [ Stop ] [Power Timer] [Restart Clock]     [R-T] [Stp] [O-T] [R-C]   
                     [Restart timer] [ Cfg  ] [Power timer] [Power   Clock]     [R-t] [Cfg] [O-t] [O-C]       
                     [Restart   Now] [Logoff] [Power   Now] [    Exit     ]     [Rst] [Log] [Off] [   ]
-----------------------------------------------------------------------------------------------------------------------
Macro Timers KeyPad  [Repeat Timer] [  Stop  ] [Onceof Timer] [Repeat  CTimer]  [R-T] [Stp] [O-T] [RcT]   
                     [Repeat timer] [   Cfg  ] [Onceof timer] [Oneshot CTimer]  [R-t] [Cfg] [O-t] [OcT]       
                     [Repeat Clock] [Repeat x] [Onceof Clock] [    Exit      ]  [R-C] [Rep] [O-C] [   ]
  
To cancel a timed shutdown press the [Stop] key. To exit the Macro Timer or the Power Restart Keypad press any of the
[black] keys or press [Cfg][McT] again. This is also a way to check if any of the timers are running - press [McT] then
press one of the black keys to exit - the display should indicate if a Macro Timer is running or not.
-----------------------------------------------------------------------------------------------------------------------
Macro Timer Example:
Program key [M2] with the text hello. Then again from the KeyBrd press [Src] and [Num] until it shows M and 02 as source.
[ADD] a number 1 to 8 - i.e. choose one of the 8 types of timers - for example press [012]3x[ADD]. Then press [NXT][Tmr] 
It will show a message that Macro M02 has been set up for Repeat Timer R-t. If 1 or 3 or 4 had been added then the timer 
used would be Repeat t, Oneshot T, and Oneshot t where t = short time , and T = Long Time. Exit the KeyBrd and press the 
black [Cfg] then [McT] and then make sure to press the same button that have been set up - in this case [R-t]. Open 
notepad and wait for the repeating hello to appear. Note numbers 1-4 are for the countdown timers and numbers 5-8 are used
for the clock timers such as R-C or O-C as discussed below.

Change the timer values with *mt*x *mT*x *nt*x *nT*x where x = 0-9 - you can use the [*Cm] key and [ADD] one number 0-9
then press [EXE].

Currently only single macros from M, S and T 1-24 can be used for the timers - if a chained (linked) macro is used 
it will run the last macro used by the chain. The Timers are programmed as Time-Fire-Time-Fire. There will be an option
later to change this to Fire-Time-Fire-Time for the Repeat timers. The two real-time (using the Pico's HW RTC or Clock)
timers are configured by first setting the Clock Time by sending the string <tyymmddwhhmm> -> <t22110341439> is Thursday
3 Nov 2022 at 14h30. Then set the alarm time by sending the string <ayymmddwhhmm> -> <a22110601439> is Sunday 6 Nov 
2022 at 14h30. To send a repeat macro every 1 minute send <a-1-1-1--1-1> (the double -- is for the day of week not
significant), and associate with it 5 [R-C]. The clock time and alarm time are sent to a serial terminal and displayed
in the status bar by pressing [Cgf] twice. Can use a *code *tx*yymmddwhhmm to send all the clock values else send these
either manually using a serial terminal or use a Proccessing script, or a scheduled task powershell script. Note that 
the Pico has a HW RTC but does not have a dedicated battery backup for its HW RTC.

The Repeat-only mode (i.e send macro fixed number of times with a delay or no delay, is not implemented as yet.

Text Strings: 
Send new text strings up to 200 characters to keys S1/T1 - S12/T24 via USBserial
Start string with <1 to <6 followed by the string assigned to S1/T1-S24/T24 - end string with >
If current Layout is L3 then S1 to S6 changed, Layout L4 T1 to T6 changed, Layout L1 then M1 to M6 changed
If current Layer is A then M1/S1/T1-M6/S6/T6 changed, if Layer B M7/S7/T7-M12/S12/T12 changed etc. 
To store the string sent on SDCard rather than Flash make sure that the "A" to "D" colour is orange and not white
and that Layer 1, 3, or 4 is showing [L1][L3][L4]. The sent string will then be stored to the current SDCard file 
selected set 1 to 5 and to the filename selected by the first number after the starting < character.

Example 1.1: Indicator A-D is orange and Layer 3 [S]keys [S7] to [S12] is visible. SDCard set 2 is active (set with 
*sd*2[ADD][EXE]). Then use a serial terminal to send <1This is a string for key [S1].>. This string will then 
be stored in the SDCard file S0007.txt. When key [S7] is pressed with an orange A-D visible then this text will be 
printed. 
Example 1.2: A white "C" is showing and Layer 3 [S]keys [S13] to [S18] is visible. Then use a serial terminal to send 
<1This is a string for key [S1].>. This string will then be assigned to key [S13]. When key [S13] is pressed then
this text will be printed. 
Example 2: Send the macro 0x3C 0x34 0xE0 0xE1 0x29 0x3E (which is <4 Control Shift Escape >), 
with Layer 4 visible, then pressing [M4] will open the Task Manager.

PC Sensor Data: The sensor data read from HWInfo's Gadget Regisry data can be sent to the touchpad and displayed on the
LCD statusbar. The procedure is explained in detail in the MacropadPCSensorData section.

PC Music Playing Data: The Music Playing data read from Foobar2000's Now Playing Simple foobar2000 plugin, can be sent
to the touchpad and displayed on the LCD statusbar. The procedure is explained in detail in the MacropadFoobarPlaying 
section.

Date Time Display This is an alternative Date Time which is only displayed, and not used to set the Pico system 
time-date. The procedure is explained in detail in the SetDateTime section. This uses <T > and the system time date 
uses <t >. 

-----------------------------------------------------------------------------------------------------------------------
Panic mode reset. If for any reason your keypad becomes unresponsive or behaves strangely reset it as follows:

(1) If the keys still send a *de* command using the second Pad. Then unplug or reset the keypad. This will delete all 
    files. macros and settings and you will need a re-calibration at start-up.
(2) Press either the white button on the Pico MCU baord and hold it in, then press the reset button once, and then only
    release the white button. Or unplug and re-plug the USB cable whilst holding and then release the Pico white button.
    The file manager should show a new storage device named RPI-RP2. Drag and drop any of the code.UF2 files to this 
    device. It will restart after a second or two. If this still does not reset the keypad then instead of the code.UF2 
    file drag and drop the file flash_nuke.uf2, wait a few seconds and then drag the code.UF2 file to the device.
-----------------------------------------------------------------------------------------------------------------------
On First Start: 
You will have to do a four-arrow corner calibration - press at the TIP of each arrow just ONCE. If you make a mistake 
and press the same corner twice it is likely that you will need a full reset with the nuke.uf2 file as described above
because the LCD will not pick up the correct buttons being touched.  

The default LCD settings are full brightness and when off full blank. Change these by pressing the second Pad on the 
right (k) or (gray), and then press [NXT]4x (four times), then press [*Cm] once and *bb* shows in staus line at the 
bottom. Then press the [678] key once (6 shows), press [ADD] and then press [EXE]. The normal use LCD Brightness is 
then set to 25 percent. Do the same for the LCD blank setting - press [*Cm] until *db* shows, then press the [345] 
key once (3 shows), then press [ADD] and [EXE] - this sets the blank LCD to 10 percent brightness when in sleep mode.

You can also increase the elapsed time period before the display dims - change it from the default 30 seconds to 300
seconds by selecting *tb* using the [*Cm] key, then press the [678] key once (6), and then press [ADD] and [EXE].
When the LCD has dimmed or blanked a first keypress is ignored - it is used to restore the LCD to its selected 
brightness. Use *ks* to toggle this key-skip behaviour off/on.

The Volume Up and Down keys [V+] and [V-], are on by the default on a first start. To switch the Volume keys off press
[Cfg] in Layout 2, then press [Vol]. Press the [Cfg] key again twice to [Sav]e this Volume-off state - else it will be 
on again at the next switch-on. As a replacement for the Volume [V+] key choose from a set of 24 options (Del Bks Tab
aTb Ins Esc PrS aPr Ret Snp Osk Num Cap Scr Cut Cpy Pst Tsk Run wX CPi Ts1 Ts2 Ts3). With the Volume key off, press 
[Cfg] and then [Var] once for [Del] key optios, and twice for [Ret] key options. Use the bottom Pad [o] to select from
the other 24 options. Press the [Sav] key to save the option chosen.

Linked macros are not saved by default (but restoring saved linked-macro configuration is always on). To turn it on 
(and off) execute *lr* command via the KeyBrd on the Cfg Page. When on it slows the response when pressing the [Cfg] and 
[Sav] key - it may be acceptable to turn the links save off (*lr*) once links have been programmed.




```


