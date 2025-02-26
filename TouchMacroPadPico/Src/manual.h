/* manual.h
-----------------------------------------------------------------------------------------------------------------------
Layout 1 - M Keys - [M1]-[M24] - Cycle through Layout 1 to 4 press [L1-L4] or long-press [Vo] 
-----------------------------------------------------------------------------------------------------------------------
[Cut CtrlX] [Copy CtrlC] [Paste CtrlV] [VolUp  Delete]       [  Cut ] [  Cpy ] [  Pst ] [V+][ Del ] [A-D]
[ TaskMan ] [  Adm CMD ] [    Run    ] [VolMute L1-L4]       [M1 M19] [M2 M20] [M3 M21] [Vo][L1-L4] [MST]
[ PC Info ] [  Adm PS  ] [ Taskbar 2 ] [VolDwn  Enter]       [M4 M22] [M5 M23] [M6 M24] [V-][ Ret ] [D-A]
         Caps          Num         Scroll                            C        N        S

[Cut][Copy][Paste][Delete][Enter] are 8 keys - Layout 1 has one group of 3 (+2) keys and Layouts 3 and 4 another 
group of 3 (+2) keys - that can be each assigned from 54 options:
1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17  18  19 20  21  22 - 27  28 - 51  52  53  54 
Del Bks Tab aTb Ins Esc PrS aPr Ret Snp Osk UnD ReD caD Cut Cpy Pst Tsk Run wX CPi Ts1 Ts6  K1 - K24 Num Cap Scr

1 Delete 2 Backspace 3 Tab  4 AltTab 5 Insert 6 Esc 7 PScr 8 AltPScr 9 Return 10 Snipping 11 OnScrKeyBrd
12 UnDo 13 ReDo 14 CtrAltDel 15 Cut 16 Copy 17 Paste 18 TaskMan 19 Run 20 GuiX 21 Copilot 22-27 Tskbar1-6
28-51 K1-K24 NumLock 52 CapsLock 53 ScrollLock 54. Keys K1-K24 can be associated with a macrostring definition: 
Keys K1 to K24 read the content of files K01Link to K24Link (selectable from the SDCard and/or flash memory), 
which contains a list of 3-letter file names containing macro definitions and/or text strings, such as a01-a99, 
m01-m24, s01-s24, t01-t24 or any other 3-letter name, to be executed in sequence, with separately defined 
repetitions and delays. 

If K1-K24 are pressed with no KxxLink file on either the SDCard or Flash, and a macro (max 3 bytes), has been 
defined for the Kxx key (by using *cm*nnKxx with nn file a01-a99 that will be copied to Key Kxx), then the short 
macro is executed - refer to Example 2 below for a detailed description on how to switch the 24 Kx keys from 
executing a link file to executing a short macro. Delete all the K01-K24 short macro defines (not the KxxLinks), 
by using *cm*nnkxx where nn and xx can be anything - use k00 for convenience.

For example, the file K01Link contains the text a50a51a52. Assign this file which is hard-linked to key K1 to one 
of the top row 3 keys and when pressed it will execute the contents of three macro and/or text files: a50: GUI r, 
a51: "notepad", and a52: Ctrl + increase size. Select which K01Link file pressing K1 will execute by selecting the 
SDCard or FlasMem through the 8-cycle A-D Layer - when white it uses the Flashmemory, and when orange the SDCard. 
This text file list of files to execute can also be used for all 72 keys M1-M24, S1-S24, T1-T24, or they can 
execute the individual macros, or text strings, selectively.

Another example, K01Link contains r03a51d10L00K24 - this will print "notepad" three times, wait 10 seconds, then 
load and execute a new linkfile K24Link on the same storage medium (Flash or SDCard) as the file K01Link.

Repeat and Delay are controlled by the characters r, R, and d, D respectively, L0n at the end of a linkfile loads 
another linkfile - the name follows after L0n - can continue ad infinitum.

 Repeat rnn = repeat 01=1 to 99=99 times the next macro    - r03a01 repeat a01 3x
 Repeat Rnn = repeat 01=100 to 9900 times the next macro   - R03a01 repeat a01 300x
 Repeat RRn = repeat 1=10000 to 90000 times the next macro - RR3a01 repeat a01 30000x
 Use rnna01 = repeat a01 nn times and not a01rnn as in DuckyScript
  
 Delay ddn - delay 0=100 to 9=900 milliseconds - dd3a01 delay 300 milliseconds before a01 executed
 Delay dnn - delay 01=1  to 99=99 seconds      - d03a01 delay 3 seconds before a01 executed
 Delay Dnn - delay 01=1  to 99=99 minutes      - D03a01 delay 3 minutes before a01 executed
 Delay DDn - delay 1=1   to 9=9 hours          - DD3a01 delay 3 hours before a01 executed

 Load Link File LxnXXX Lxn n=0 Do not change current SDCard/Flash storage to load linkfile XXXLink 
 XXX = a-z, A-Z, 0-9       n=1 Use Flash to load linkfile XnnLink
 in any combination        n=2 Use SDCard to load linkfile XnnLink 
                           n=3 Flip current storage to load linkfile XnnLink 
 If LayerAxD changed       x=0 L0n: Do not restore LayerAxD on completion
 no/restore original       x=1 L1n: Restore LayerAxD on completion
           
The 8 keys Cut-Cpy-Pst-Del-Ret-cX-cC-cV, can also be programmed through starcodes x1, x2, x3 Layout 1, x4, x5,
x6 for Layout 3 and 4, and x7 and x8 for the [Del] and [Ret] keys. They can use any of the 54 actions for x1 - x8.
Program as *xn*number n=1-8, number = 1-54.

Press [Cfg] once, and then press [Key] once or twice, to select 54 options for either the [Del] or the [Ret] 
key which are then selected by pressing the red bottom Pad [o] which will cycle through the 54 options for both the 
[Del]ete key top right and the [Ret]urn key bottom right. Pressing the [Key] key 3x, 4x or 5x times will select the 
Layer 1 [Cut], second [Cpy], or third [Pst] key respectively which can then be changed by pressing the bottom Pad 
i.e. option [o]. Pressing the [Key] key, 6x, 7x, or 8x will select this top-row 3 keys in Layouts 3 or 4, which
can then be changed to one of the 54 options using the bottom Pad key. The status line will show which key is chosen
and which option is selected. To save the choice press the [Sav] key just below it, else it will reset to the 
default [Del], [Ret], [Cut,Copy,Paste] keys after the next reboot. If the [Sav] key is not pressed the new 
choice(s) will be valid for current session only. To reset the keys to their default options, use the *x1*0 code 
i.e. by pressing the second Pad [k], and then press [*Cm] until *x1* shows, then press [012] once, then press 
[ADD][EXE].

The key M1-M24 macros listed above are default macros - use the macro editor to configure new macros or copy, 
rename and/or make linkfiles for new or existing macros. Some of the default macros run items on the pinned on the 
taskbar, and are indicated by GuiL + Key0 - Key9 etc in macroBanks.h WinMacro content.

The keys in Layouts 1, 3, and 4 (M, S, and T 1-24 each) are linked to either a lowercase (m01-m23. s01-s24, t02-t24)
or uppercase (M01-M24, S01-S24, T01-T24), group of 24 files containg macros and/or text strings. Use the [Cf][Opt] 
keys to toggle between Upper and Lower case names or use the [*Cm] key to send *up*. 

The 8-cycle [A-D] key in Layout 2: If the A-D indicator shows a white A, B. C, or D then the Layouts 
Mx Sx Tx use FlashMem macrofiles or linkfiles, or the actions assigned with *fm* *fs* *ft* strings. If the indicator
is orange then then the Layouts Mx Sx Tx use the SDCard files or the Coded functions. To save the A-D (orange or 
white) choices press [Sav] else the currently saved choice will be used after the next reboot. Note that the A-D page
chosen using the Pad keys [a] or [d] are not saved between a reboot - you must use [Cfg][A-D][Sav] sequence to save 
both White/Orange[A-D] state.

Connecting a PC serial monitor to the TouchPad and pressing [Cfg] twice i.e. press [Cfg][Sav] will list all the files
on both the FlasMem and SDCard filesystems. Delete a file by constructing its name in the Macro-editor, and then 
press [Rmv] remove.

The default sequence in choosing what to do when the keys M1-M24, S1-S24, T1-T24, K1-K24 are pressed are:

if S Keys Page 3: Keys S1-S24
   if Orange A-D Do SDCard text files    - if ok exit
   if White  A-D Do FlashMem Macro-definitions - if ok exit else Do FlashMem Macro-Link actions - if ok exit
   if Orange A-D Do SDCard Macro-Link actions  - if ok exit else Do SDCard Macro-definitions    - if ok exit              
   Do S-keys actions as defined in Bank123     - exit
                  
if T or M Keys Pages 1 or 4: Keys M1-M24 or T1-T24
   if White  A-D Do FlashMem Macro-definitions - if ok exit else Do FlashMem Macro-Link actions - if ok exit
   if Orange A-D Do SDCard Macro-Link actions  - if ok exit else Do SDCard Macro-definitions    - if ok exit              
   Do M,T-keys actions as defined in Bank123   - exit

if K Keys: Keys K1-K24
   if Orange A-D Do SDCard MacroLink actions - if ok exit 
   if White  A-D Do Flash  MacroLink actions - if ok exit 
   If defined do (1 - FlashMem or 2 - Hard-coded) BSD K1-K24 Macros - exit

This sequence can be replaced by an instructionlist for keys M,S,T - refer to (A) in the starcodes section 
where using *im,s,t*numberlist are used to specify the sequence in choosing a macro execution.
   
Keys K1-K24 execute Linkfiles K01Link-K24Linkbyte macros, if none then execute BSD macros if not 0x00 - i.e 
K01-K24 will execute the defines in macroBanks.h BsDCode1-BsDCode1 28-51. If both definitions exist then
pressing a key K1-K24 will first execute its linkfile contents (K01Link-K24Link), and then the short BSD macro
will also be executed.

Keys [M2] and [M5] show additional options based on Layer state A-D: They run an admin Command Prompt and 
admin Powershell respectively, only in Layer A, and do M-key Bank123 actions if not in Layer A. 

SDCard Filenames accessed by S keys [S1]-[S24] and selectable by [Cfg][Opt] or with *sd*n are:
----------------------------------------------------------------------------------------------------------------------
 SDNum 1-3    S0001.txt-S0024.txt A1.txt-A6,txt - C1.txt-C6.txt 01-24  *sd*n=1-3 Can be edited in sdcard.h 
 SDNum 4-9    U-Z + 01-24 = U01-U24 - Z01-Z24                          *sd*n=4-9 or *sd*U-X or *sd*u-g 
 SDnum 10-12  MST mst 01-24 = M01-M24 S01-S24 T01-T24                  *sd*m,s,t *sd*M,S,T  
 SDNum 13-19  A-G a-g 01-24 = A01-A24 - G01-D24                        *sd*A-G   *sd*a-g )
 SDNum 20-21  K,k  K=K01Link-K24Link k = K01-K24                       *sd*K,k
 
3-Char filenames *sd*4-9 (UVWXYZ U01-U24) *sd*mst (MST M01-M24) *sd*abcdef (A01-A24) can be used in CnnLink files
Upper/lowercase filenames - lowercase only through *sd*n n=1-9, U-X u-x mst MST A-G a-g - 4-9 = U,u-Z,z
*sd* no other parameters: SDFleList sent to Serial Port when using Pad [n] Upper/Lowercase Name or Pad [o] Select Set
-----------------------------------------------------------------------------------------------------------------------
Layout 2A - Config - Cycle through Layout 1 to 4 press [L1-L4] or longpress [Vo] if Mute is the current option for [L2]
-----------------------------------------------------------------------------------------------------------------------
[Hme  Stop    Option] [UpArr   Key] [PgeUp            Vol] [VolUp  Delete]        [Hme] [Up ] [Pgu] [V+][Del]
[ArrL Prev       A-D] [Config Save] [ArrR  Nxt Vo-Mute1L4] [VolMute L1-L4]        [  <] [Cfg] [>  ] [Vo][L14]
[End  PlayP MacroTim] [DwnArr POff] [PgeDwn         Media] [VolDwn Return]        [End] [Dwn] [PgD] [V-][Ret]
Macro: Source Num      Target Num                                                  S-nn  T-nn      
                    Caps          Num               Scroll                             C     N     S

-----------------------------------------------------------------------------------------------------------------------
Layout 2B - 96 nKeys n01 to n96 - These keys and files they use, named by default [n01] to [n96] are not macros, 
textstrings, or linkfiles. They redirect or point to another file(name) optionally via its path - i.e. folder(s) and 
name, of any length less than 30 characters, on the same storage (Flash or SDCard), to be executed. For example if 
file n01 contains the text /passwords/BoA.txt then pressing key n01 will send the text from the file BoA.txt in folder
/passwords to the current focussed, opened application such as Chrome or notepad.

Note that the nKeys first character (default n), can be changed with with *0n*char - for example *0n*p will change the 
keys, and filenames used to p01 - p96. Any character or digit can be used - but not all will yield valid filenames.

Example: in the KBrd set the source as N01, construct the text s11, and press [Sav] then the file n01 will be saved on 
either the SDCard (Source is brown) or Flash (Source is white) and has the contents "s11". If n01 is pressed it will
execute the macro or text in file s11 if it exists on the same storage medium as n01. If the contents of n01 is the 
text "s11Link" it will attempt the execute the link list in a file s11link.

Also note that the number of pages can be set with *Code *0p*pages with pages = 1 to 8.
-----------------------------------------------------------------------------------------------------------------------
[n01] [n02] [n03] [n04]       [n13] [n14] [n15] [n16]                 [n85] [n86] [n87] [n88]  [s]
[n05] [n06] [n07] [n08]       [n17] [n18] [n19] [n20]   ----------->  [n89] [n90] [n91] [n92]  [k]
[n09] [n10] [n11] [n12]       [n21] [n22] [n23] [n24]   5 more pages  [n93] [n94] [n95] [n96]  [m]
    Caps  Num  Scroll              C     N     S                           C     N     S       [n]
Note that n is changable to 0-9, aA - xX with *0n*char           Use Pad[o] to switch pages -> [o]

Other indicators:
[] Red/Green     K 1-3 u-z      w l r          White-FlashMen Orange -SDCard
Serial-On/Off    SDCard-Set      OS                    Layer A-D 
[]Multicolor
Indicator for MacroBank Selected in Layouts 1 3 4 - Orange,Green,Blue,Yellow,White 

Layout 2 (Config) has five additional small pad-buttons on the right side (from top to bottom):
[s] Symbols and Special characters such as Math and Greek Symbols on/off 
[k] Macro Composition Keypad on/off.
[m] Mouse Keypad on/off.
[n] n-Key mode: nxx files (such as n01, n34 etc, on both Flash and SDCard), contains the name of a file to be executed
with a filename length <30. The filename can include / char => /paintmacro/paint001.txt will execute file paint001.txt
in the folder /paintmacro. If the file name stored in file nxx ends with 'Link' it is processed as a linkfile (list of 
files, repeats, delays etc.) Use the Option Pad [o] to change to the next page - 8 pages with n01-n12, n13-n24, up to
n85-n96 are available. They can be stored on both the Flash memory or the SDCard. Switch between the n-Key Mode and 
the Number-Pad mode by using *09*. The n01-n96 files are therefore neither macros, textstrings, or linkfiles. They only
point to another file which has a a path - i.e. folder and name, of any length less than 30 characters, on the same 
storage (Flash or SDCard), to be executed.
[n] Grey Number Pad:  Number-Keypad on/off - then use red Pad [o] to scroll through NumberPad pages.
                      Macro Mode - Pressing grey Pad 4 [n] toggles KeyBoard Direct to PC Mode On/Off - show "d" in  
                      Macro Editor instead of Src/Dst macros. This sends single enries to PC when [EXE] pressed.
                      SDCard Select Set Mode: Switch Upper/Lower-case filenames for SDCard Filesets 1-21
[o] Red Options Pad: Config Mode - Toggle Capslock and Numlock on/off in combinations
                     Macro Mode  - 4-Cycle combinations of Source and Destination SDCard (Orange) or Flash (White) 
                     NumPad Mode - Switch between 3 NumPad pages.
                     [Opt] Mode  - Macro Upper/Lower case files, [L1-4][VolMute]Long-Press On/Off, StartupLayout L1-L4,
                                   Select M S T MacroBanks 1-5, Select SDCard File Set 1-21, Send SD+Flash File lists 
                     [Key] Mode  - Select 24 options for [Del], [Ret], [Cut,Copy,Paste] keys
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
Note that if Macro Upper Case Files is ON [Cfg][Opt] then the Pad labels are in Uppercase [M][S][T]

Note: If the same Pad Key is pressed in its own Layout - for example press pad S when in the [S1] - [S24] keys layout,
it will then ignore any assigned strings and use the Coded programmed action. Toggle it on/off by pressing the 
Pad once or twice. This applies for all three keysets M, S and T. The A,B,C,D indicator will change from white to 
orange when this option is On, and back to white when Off. For the set of S keys only: If the SDCard option is enabled,
the orange A-D indicator will result in files stored on the SDCard being used when pressing [S1] to [S24] - select
which files with [Cf][Opt]5x then Red Pad [o] or use *sd*n. Note that even sending *sd*K, and then pressing [S1] 
with orange A will print the content of the plaintext LinkFile K01Link, pressing [S7] orange B, will show the contents 
file K07Link if these files exist on the SDCard.
-----------------------------------------------------------------------------------------------------------------------
Layout 3 - S Keys - [S1]-[S24] - Cycle through Layout 1 to 4 press [L1-L4]  
Layout 4 - T Keys - [T1]-[T24] - Cycle through Layout 1 to 4 press [L1-L4] 
-----------------------------------------------------------------------------------------------------------------------
[Cut Ctrl+X][Copy Ctrl+C][Paste Ctrl+V][VolUp DelBsTI]  [ cX  ] [ cC  ] [ cV  ] [V+][DB-TI][A-D]
[Macro 1/19][Macro 2/20 ][Macro 3/21  ][VolMute L1-L4]  [ST  1] [ST  2] [ST  3] [Vo][L1-L4][MST]
[Macro 4/22][Macro  5/23][Macro 6/24  ][VolMute Enter]  [ST 22] [ST 23] [ST 24] [V-][ Ret ][D-A]
          Caps          Num         Scroll                     C       N       S

-----------------------------------------------------------------------------------------------------------------------
Layouts 1, 3 and 4, four Layers A B C D each with six keys: A = M1-M6, S1-S6, T1-T6 to D = M19-M24, S19-S24, T19-T24
-----------------------------------------------------------------------------------------------------------------------
To cycle through the Layouts press [L1-L4]. Long-press [L1-L4] to toggle [Vo][L1-L4]. Also use the three Pads 
[m][s][t] to move to Layers L1 L3 L4 and use the top or bottom pads [a][d] to move through the four layers a,b,c,d.

When the [L1-L4] or [Vo] key is held in for more than 1 second it will change between the two key states [Vo][L1-L4]
in any of the Layouts 1-4. Note that when in Layout 2 Cfg it will is easier to switch to the [Vo] key using the
[Cfg][Vo] keys.

Press Config Key (new set of config keys change colour):
[Vm ] [ArrowRight] - VolumeMute -> L1, L2, L3, L4 -> V1, V2, V3, V4 - press again repeat to restore Vo
[Key] [ArrowUp]  ] - Select from 24 key actions (as below) for [Del], [Ret], [Cut,Copy,Paste] keys - press 1x to 8x 
[Vol] [PageUp]   ] - Vol+ -> Delete and Vol- -> Enter - repeat to restore V+ V-
[A-D] [ArrowLeft ] - Layouts 1, 3, 4, has 8 cycles: change to Layer A B C D white (Text) and orange (SDCard/Coded)
[Med] [PageDown  ] - Change Layout 2 to Media Controls Previous-Next-PlayPause-Stop - use *e1-4* to configure. Note
                     that mediaplayers may not immediately respond to rapid Play -> Pause -> Play keypresses - allow
                     a second or two between these commands.
[mCT] [End       ] - Macro Timer Trigger Repeat and Oneshot Countdown or Clock Timers
[Opt] [Home      ] - Toggle [L1-4]{Vo]Long-Press On/Off, Select Startup Layout L1-L4,
                     Select MacroBanks 1-5 for M,S,T, Select SDCard File Set 1-21
[Sav] [Config    ] - Info and File List to Serial Monitor and Text/Macro and Config files saved
[ROf] [ArrowDwn]   - Restart-PowerOff-Logoff - Bottom row [Rst][Log][Off] - cancel by pressing 
      [Cfg][ROf]   - Includes long or short Timer options as well and Countdown and Clock Timers

Select Key Actions [Del] and [Ret] keys:
1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17  18  19 20  21  22 - 27  28 - 51  52  53  54
Del Bks Tab aTb Ins Esc PrS aPr Ret Snp Osk UnD ReD caD Cut Cpy Pst Tsk Run wX CPi Ts1 Ts6  K1 - K24 Num Cap Scr
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

Page 4: Macro Tools                                 Page 5: Modifiers                            Large Files
[Src] Macro Source M S T or A K N                   [ALT] = [ Alt-L ][  Alt-R ][ PrintScr] [EXE]    
[Dst] Macro Target (Destination) M S T A K N        [SHF] = [Shift-L][Shift-R ][ Delete  ] 
[Num] Macro Number M S T = 1-24 A K = 1-99 N = 1-36 [CTR] = [Contr-L][Contrl-R][BackSpace]          
[Cpy] Copy [Src][Num] to [Dst][Num] Macro           [GUI] = [ Win-L ][ Win-R  ][  NULL   ] [NXT]    No
[Rmv] Remove [Src][Num] Macro or constructed name   [TEI] = [  Tab  ][ Escape ][ Insert  ]          Yes
[Ren] Rename [Src][Num] Macro                       [CRF] = [  C/R  ][   L/F  ][  Return ]          Yes
[Lnk] Make a Linkfile from a list of files such as  [LHR] = [Arrow-L][  Home  ][ Arrow-R ] [ADD] 
      m01a55s12a01 constructed in the Editor        [UED] = [ArrowUp][  End   ][Arrow-Dwn]          No
[Lst] List first 8 bytes contents of source macro   [UND] = [PageUp ][ Numlock][ PageDwn ]          No
[Snd] Send constructed macro or if none, send Source Macro to PC to execute 
[Sav] Save entered data to the Source file without execution. Can therefore be used to save *Codes to a file
      to be executed the same as a list of other macros - see note 8 below.
[Tmr] Macro Timer options (One-shot or Repeat) must have [ADD]ed a number 1-8 before

Note 1: Rename and Remove Macro works for large files > 200 bytes, but Copy, List, Send Macro only for small 
files < 200 bytes

Note 2: m,s,t macros with numbers 25-99 can be entered and saved as well - set the source or target to a, then press 
[Num] to increase the displayed number to any number between 25-99, then press either ] or [Dst] for m, s, or t, and 
the constructed macro can then be executed [EXE] and saved [Up], or to only save with no execution, press the 
Macroeditor [Sav] key.
-----------------------------------------------------------------------------------------------------------------------
Note 1: The following convention is used - the Macro Destination [Dst] is also referred to as the Target Macro here 
in some cases. Pressing the red Pad [0] will cycle through 5 possible combinations of Source and Destination for SDCard
Orange) or Flash (White) storage - for example display shows: White Source M 01  Orange Target A 50

Composed Macro --> Destination (Composed with the Macro Editor) mM,sS,tT 01-24 or aA,kK 01-99 or nN 01-36
Link Macro     --> Destination (Set target = T04, enter a01a02d01r06a03, press [Lnk], then press [T4] to run it)
Unlink Macro   --> Source
Rename Macro   --> Source -> Destination (both on same storage) or enter names with = inbetween: Oldname=Newname
Remove Macro   --> Source or enter name - if "k00" entered K1-K24 BSD 3-byte macro-defines cleared (not files)  
Send Macro     --> Source
Copy Macro     --> Source -> Destination or use Name1=Name2 to copy to/from any combination of SDCard/Flash
                   If the Destination is Knn then the Source file content is copied to the K keys Macrobank nn
Timer Macro    --> Source
List Macro     --> Source or enter name
Save Macro     --> Source name, entered data or else macro content saved to Flash/SDCard.
                   Also used to save all 24 macros/strings (same as using *sm,s,t*) - load with *fs,t,m*
                   Also used to save *Codes to a file that can be executed the same as other macros
 
Note 2: To choose between Macro A or K = 1 to 99 does not require 99 key-presses - just hold the [Num] key down for 
key-repeat It cycles through 1-99 in about 10 seconds.

Note 3: If a shorter string replaces a longer string end the shorter string with a NULL char - press [GUI]3x and then 
press [ADD] at the end of shorter string. For example Key [S2] contains "notepad" which is 7 chars. To replace it with
*188# select [S2] as the destination, press [Dst] then [Num] until 02 shows, then press [*=/], press [ADD], press [*=/]
and [ADD] again (** is not treated as special command but as the chr *), then press [012]2x[ADD], [678]3x[ADD], again 
[678]3x[ADD], [Sym]2x[ADD], [NXT]2x, [GUI]3x[ADD], then press [EXE] and then the [Up] key to save.

Note 4: Delete a file by either entering its name in the Macroeditor for example K12Link, or selecting it as the 
Source before pressing [Rmv]. Make sure that the A-D indicator is the correct color - if the file is on the 
SDCard it must be orange, if FlashMemory it must be white.

Note 5: The [Snd] Send Macro either send the macro composed in the Macro Editor, or if nothing has been entered the
destination macro is sent to the PC.

Note 6: Rename Macro Select the Src and Dst - both must be on the same storage - or enter the 2 names with an = 
inbetween: Oldname=Newname. Each name must be a maximum of 30 characters.

Note 7: Saving text macros in bulk. There are various combinations of saving for example S-key textfiles, once loaded, 
to either Flash or the SDCard, and using either Upper/Lower-case filenames for Flash (2 different filesets S01-S24 and 
s01-s24 will then exist on Flash), or Upper/Lower-case on the SDCard (1 fileset sS01-sS24 will exist S01 is the same 
file as S01 on the SDCard). Saving to the SDCard is much faster than saving to Flash. For example: Load Skeys with the
textfile in strings24.h - press [*Cm] for *fs* press [EXE]. Save these to SDCard (not Flash): In the Macroeditor press
red Pad [o] until both source and target are brown (it does not matter what the source or target is) Then use [*Cm] 
to execute *ss* [EXE]. Files s01-s24 (or S01-S24 if Uppercase is on - but they are the same files as the lower-case
files for SDCard) will be on the SDcard. Exit the Macroeditor and in Layer 3 press [S1] which will send the content 
of file s01/S01 on the SDCard namely "1", provide the A-D indicator is brown. If it is white pressing [S1] will open
the Windows TaskManager - see macroBanks.h WinMacro[0] = Ctr+Shf+Esc, pressing [S2] will open the WinX Menu which is
Win+x or WinMacro[1]. Note that is the SDCard has SDCard files 1-21 on it, then these will be sent first when pressing
[S1], disable them by selecting SDCard - disabled with [Cfg][Opt]Pad[o]. Saving the Skeys files on Flash will mean that
pressing [S1] with A-D white, will send "1", and if A-D is brown, the SDCard set selected 1-21 [Cfg][Opt], on the 
SDCard, will be executed/sent. Therefore to save text loaded with *fm,s,t* to the SDCard not Flash - use the red Pad[0]
to make the Source and Destination brown.

Note 8: [Sav] key in Macroeditor: [Sav] key saves entered data without execution to the Source file m,s,t, 1-24 a01-a99
It can be used to save *Code for example *fo* (bold fonts) to a file such as t05 when key [T5] pressed will toggle bold 
fonts on/off. Therefore all *Codes can now be included in list of macros to be executed. For example can have a list of
just three macros: The first two macros opens Photoshop - GUI + r  and "C:\Program Files\Adobe\Photoshop\Photoshop.exe\n"
 - and the third macro *at*2 switch to the Photoshop specific Macrobank number 2 for keyset T1-24. This can then be 
triggered by pressing any key such as [T1] or [K11], that runs the XnnLink file containing these three macros. When 
saving a *Code in the Macroeditor the last character must be a NULL or 0x00, i.e. to save *fc* to key [T1] make t01 the
source, then construct *fc* with the [ADD] key, and at the end do [GUI]3x[Add], to add the Null character at the end.
Then press [Sav] while still in the Macroeditor - it will not execute the *Code action just save it as a macro. To 
execute *fo* press key [T1].
                                                                          
The keyboard has 5 pages - most with 9 triple function keys, and 3 control keys [EXE] [NXT] [ADD]. For example page 1 
has keys [abc], [def], to [y,z,space]. To select a or b or c press the abc key once, twice or thrice - to add it to a 
new macro press the ADD key else press another character-symbol modifier key or press [NXT] for the next keyboard page. 

Press Pad 2 again to leave the keyboard or press [EXE] to send the macro to the PC and save to the (Source-Destination)
or SD Macro Key as indicated by the second number 1 to 24. Change the target number by pressing the [Dst] then the [Num]
key. The macro is saved when the Up-Arrow key is pressed after the [EXE] key. Refer to the examples below on how to copy
commands between the KeyBrd macros (a01-a99 files), 3 sets of 24 keys M, S, T files s01-s24, m01-m24, t01-t24, and the
36 N-key files n01-n36.

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

Macros sent to the PC from the built-in keyboard can also be saved to file aX X = 01-99. The aX macros are automatically
saved - no need to press [Up] after pressing [EXE]. They will be saved to the SDCard (orange Target) or Flash (white 
Destination). To copy them as an MST macro setup Ax MSTx X,x = 01-99, 01-24 in ant orange-white combination but make sure
the storage medium with the source file is chosen, then press [Cpy], or enter aX=MSTx and press [Cpy].  

Note: The [Cpy] key on page 4 is the most direct way to copy the [Src][Num] to [Dst][Num] Macro. The alternative is
the starcode *cm* = [Cpy] macro source->dest and it can include K keys. Note: If *cm* can copy to K1-K24 definitions 
max 3 byte macros. 
      
Example 1: Set up M01 M04 as SrcNum DstNum - then press [CTR][SHF][TEI]2x[EXE][UP] - press [Up] to save to key Target
          (Destination) key [M4]. Press [M4] and the (Windows) Task-Manager opens (Ctrl+Shft+Esc).
Example 2: Set up M01 M01 - press [GUI][r][EXE][Up] - save to key [M1] - file list shows two files a01 and m01 - both
           3 bytes length - setup Source M01 then press [Lst] it shows E3 15 00 which are in hexadecimal [GUI][r][NULL]
           Copy M01 to Key [S4] - Set up M01 S04 then  press [Cpy]. Press [M4] to test and setup SrcNum S04 then press
           [Lst] - E3 15 00 (one 0x00 added if last byte is not 0x00). Press key [S4] to open the run window.
Example 3: Set up A01 S01 - then [Cpy] - file list has s01 4 press [S1] open run command window
Example 4: Set up A04 S02 - then [Cpy] - file list has s02 5 press [S2] open TaskMan
Example 5: Set up S04 M04 - press [GUI][r][EXE][Up] - press [S4] to open run window
           Set up M04 S02 - then [Cpy] - press [S2] open run windows 
Example 6: Set up A04 T07 - Rename a04 (must exist) to t07 - press [Ren] - then press [T7]

Linking Macro Examples: Linking is done through a textfile list of 3-letter filenames in sequence. For example the file
M01Link on the SDCard has text a50a51a52a51a52a52a52 Pressing key [M1] with an orange A-D, will then execute macro files
a50(GUI r), a51("notepad") a52(Ctrl +) a51("notepad") a52(Ctrl +) a52(Ctrl +) a52(Ctrl +), A shorter version of the 
string is a50a51d01a52a51r03a52 - delay 1 second to give the OS time to open notepad then type notepad, then increase 
size 3x

This applies to all three keysets M, S and T but M is used in the example below.

Keys M1-M24 read the content of files M01Link-M24Link on SDCard/FlashMem which contains: 3-letter filesname on flash
memory such as a01-a99 (read in groups of 3 bytes = filename) Create file M01Link using the Macro Editor: Setup m01
as Target, then create a50a51a52 using [ADD]. Then press [Lnk] and file M01Link will be created on the SDCard if
A-D is orange or in FlashMem if A-D is white 

Example 1: Program [M1] with a Open Run windows [GUI][r] - it is saved as file m01
           Program [M2] with a notepad+C/R - it is saved as file m02
           Program [M3] with hello+C/R - it is saved as file m03
           Program [M4] with [CTR][SHF][Esc] Open Taskman - it is saved as file m04
           
           Then enter m01m02d01m03d01m04 (d01=delay 1 second to give the operating system time to open notepad before
           typing hello) Setup M05 as the Source and then press Link [Lnk] - M05Link is displayed in the status bar.
           Then press [M5] it opens the run window, and then opens notepad, then enter text "hello" in Notepad, and
           also opens the TaskManager.

Example 2: Use the macroeditor to copy file M05Link created in Example 1, from Flash storage to the SDcard: Enter 
           M05Link=K24Link with the Source white and the Target brown (the source and target letters and numbers 
           can be anything) - and press [Cpy]. Then program a link for key [T4] with the content L02K24 - enter 
           L02K24 in the Macroeditor with the Target white and press [Lnk]. A file T01Link will be created on Flash
           storage with the content L02K24. Then press key T4 (A-D must be white), and it will run the content of 
           a linkfile named K24Link on the SDCard.

Example 3: Using the K1-K24 keys for both KxxLink macros and short 3byte BSD macros. Start with an existing set of
           three files a50, a51, a52 on flash storage - read below for more details. 
           
           First replace [Del] with [K3] by using [Cfg][Key][redPad]press until Key 3 shows then press [Sav]. File 
           a50 contains GUI r 0x00 and is on the Flash memory. In the macro editor press [Src] till "A" shows -
           it is not necessary to press [Num] = 50 in this case. Then press [*cm] until *cm* shows then add 50K03 
           to it until *cm*50K03 is displayed. Then press [EXE] and the status bar displays: "Macro 50 -> K03". 
           Press [K3] which will open the Run box. As a further check execute *ld* which will send to the serial 
           port a list of the K Keys BSD defines where K1-K24 are 27 - 51. It lists: 29 E3 15 00 which is GUI r 0x00
           for K3 = BSD number 29. 
           
           Now we want [K3] to execute a link file K03Link instead of the BSD define as used above. We have one file 
           already - a50 but we need two other files a51 = "notepad+C/R" and a52 = "Hello". Create these two files 
           in the macroeditor and check the filelist using *lf*. 
           
           As an additional exercise we are then going to copy the three files from flash to SDCard as a30, a31 and 
           a32. In the Macroeditor setup SrcDst = aa A50(white) A30(orange). (Press the red Pad key to change the 
           colours of the Source and Destination numbers.) Then press [Cpy]. Increase the numbers to A51 and A31 
           and again press [Cpy]. Then do it for A52 and A32. Check the filelist for files a31, a32, a33 on the 
           SDCard (Note that the file size should increase by one because 0x00 was added to the end.) Now press 
           [Cfg][Opt] until SDCard Files 1-21 is shown. Then press the red Pad [o] until "SDCard Set K" - uppercase K
           - shows. Make sure the A-D indicator is orange - if it is white press key [A-D] until it is orange - then 
           press [L2] or [V2] for Layout 3  - the S1-S6 keys - and then from a serial terminal send the string
           <3a30a31d01r05a32> the status bar will show K03Link. Pressing [K3] will now open notepad and type hello 5x.

           The Macroeditor [Cpy] can also be used to directly define the short BSD macros for the k keys. For example 
           setup the Destination as K01, then enter GUI Tab and press [EXE] - a file K01 will be created with a 
           GUI+Tab action. Then setup Source K01 and Target K02 and press [Cpy]. Instead of copying file K01 to K02
           it will copy the content (GUI Tab) of file K01 to the short macro definition for key K1.

           Instead of using a serial terminal to create KnnLink files a shorter method is: Setup Destination K05 
           and construct a30a31d01r05a32 in the Macroeditor. Then press [Lnk] and the file K05Link will be created
           that will be run when pressing key K5.

           Note that pressing a key K1-K24 will first execute its linkfile contents (K01Link-K24Link), and then 
           even if a short BSD macro has been defined (i.e. it is not 0x00 or NULL), it will not be executed. 
                
F1-F24 keys are all one key [Fnn] and are sent as keycodes (simultaneous) and not keypress types - to send [F3] open
the macro keyboard then press [NXT]2x[Fnn]3x[ADD][EXE]. Press [Up] to assign it to a [Dst][Num] key.

The Keyboard has a Direct (to PC) Mode - use the grey Pad [n] to toggle it On/Off. A Blue "D" indicator will show if
it is on. Any character selected (shows in status bar), will be sent to the PC by pressing [EXE] - [ADD] is not 
necessary. If a character or more than one characters have been [ADD]ed they will only be sent after Direct Mode is 
switched off.
-----------------------------------------------------------------------------------------------------------------------
The Keyboard can be used to change various options by sending *option*value commands. If a number(s) is needed after
the second * then pressing [ADD] is required. The [*Cm] key can also be used and then pressing the [Add] key after
the last * is not necessary. All *Codes return to same *Code with [*Cm] if [EXE] was pressed. *Codes are incremented 
to the next starcode if no [EXE} pressed. The main codes are listed below:

(1) Macro Timer Keys time values - default Short Time = 30 seconds and default Long Time 10 minutes. To change Timer
    values send Repeat Macro Timer *mt*num or *mT*num or Oneshot Macro Timer *nt*num or *nT*num where num 0 is
    3 hours t time and 300 hours T time mode. 1 = 30 seconds (2 hours for T) to 9 = 1 hour (48 hours for T). Associate 
    a Macro with a Timer in the KeyBrd Macro Tools Page using key [Tmr] after 1 to 8 have been [ADD]ed to information
    line at the bottom. Then the Source Number Macro will be linked to (1) and (2) Repeat-Timer T and t, (3) and (4)
    Oneshot-Timer T and t, (5) Repeat on Clock Time, (6) Oneshot on Clock Time, (7) Repeat Countdown Timer, 
    (8) Oneshot Countdown timer. Ensure that the matching key on the timer keybrd is pressed i.e. if you used 2 then 
    press key [R-t] else the timer will not fire. For example: Set up an Key[M01] macro of [Gui]r, and check that it is 
    saved on FlashMem as file M01. Then select Source macro M01, press [012]3x[ADD][NXT][Tmr] exit the Macro KeyBrd,
    press [Cfg][Mct][R-t]. The Run box will then keep opening repeatedly about every 10-30 seconds - pause by pressing
    [R-t] again, restart press [R-t], or cancel by pressing the yellow button [Stop]. Add a star * to the added number
    1-8 i.e 1*, 2* etc. use macrolink files XnnLink instaed of macro files Xnn. Use the option Pad [o] to select where
    (SDCard or Flas) the macro file is stored.
(2) LCD blank timeout - Send the macro *tb*num with the built-in keyboard where num = 0,1-9 - *tb*0 = 120 hours, 
    *tb*1 = 30 seconds.  
(3) LCD blank dim value - Send the macro *db*num with the built-in keyboard where num = 0,1-9 - *db*0 = no dimming
    just on/off after blank timeout, and 1-9 = 3 to 30 % PWM instead of blank. 
(4) LCD screen brightness - Send the macro *bb*num with the built-in keyboard where num = 0,1-9 - *bb*0 = full
    brightness and 1-9 = 3 to 50 % PWM brightness.
(5) Power Keys Menu/Command Option - Send the macro *po* with the built-in keyboard to toggle the Power Keys Menu
    option - use the OS Menu (such as GUI+X + U + i,u,r or the run command (such as GUI+R + shutdown + options) command.
    If the Power Menu is active then only the [Stp], [Cfg] and the [Black] keys are useful.
(6) Power Keys time values - Default Short Time = 30 seconds and Default Long Time 600 seconds (10 minutes). To change 
    Time values send PowerOff *ot*num or *oT*num or Restart *rt*num or *rT*num where num 0 = 6000 second (100 minutes)
    1 = 30 second (x100 for T) to 9 = 300 second (x100 for T).
(7) Calibration Enable/Disable - Send the starcode *ca* with the Macroeditor to set/clear re-calibration option after 
    a restart to ON or OFF. 
(8) LCD Orientation 180 degree change - Send the macro *ro* with the built-in keyboard - the USB cable will then be on
    the opposite side. This forces a re-calibration on restart. 
(9) Key Font Bold/Normal change - Send the macro *fo* with the built-in keyboard to toggle the labels on the buttons 
    from normal to bold.
(a) Key Font White/Black change - Send the macro *fc* with the built-in keyboard to toggle the labels on the buttons 
    from white to black.
(b) Macro Copy - Copy a01-a99 to M,S,T K keys. Can use *cm* if the SrcNum DstNum is set up - see the four examples 
    above. Else compose *cm*nnXmm via [ADD] where: nn = a01-a99 X = Keys M S T K mm = 01-24. Keys K1-K24 are max 3
    byte macros - if Knn pressed will run this definition, if no definition will do KnnLink file. See Example 2 
    above for more detail.
(c) Macro Unlink - unlink *ul* with the Macro Key to be unlinked visible as the Source Macro such a Src nn Dst mm. 
    (*ua* unlink all macros - not yet implemented)
(d) To fill M S T 1-24 with hard-coded text string examples send *fm* *fs* *ft* or *fa* (all three) commands. For the
    S keys strings24.h is used and for the T keys stringt24.h is used.
(e) To overwrite and save to Flash M S T keys 1-24 send *sm* *ss* *st* or *sa* (all 3 sets M S T of 24 keys) commands.
    Note that after using *fs,m,t,a* the macros are not automatically saved - it is also necessary to do *s,m,t,a* to
    save them (and to afterwards list the first part of their content with [Lst]). 
(f) Use *df* to delete all SDCard files - this includes the copy of the calibration file.
(g) To delete all Flash macro and config files use "*de*" - calibration file is saved to SDCard, then restored - no
    re-calibrate needed on restart. Use *ca* to set/clear re-calibration on start. Remove individual macro files 
    with the [Rmv] or rename with [Ren] keys.
(i) *ct* display four time clocks with a 1 second delay - Time, Macro[R-C][O-C], Macro[Rct][OcT] Power[R-C][O-C].
(j) *br* = toggle brightness controls up/down replace volume up/dwn for Layouts 1, 3, 4 (not in Layout 2 Cfg). The
    brightness slider usually only has an effect when used in notebook computers not desktops.
(k) *tt* *ta* *tp* *tw* Use *tx*yymmddwhhmm -> *tx*22110341200 12:00am 3 Nov 2022 Thursday where x = t,a,p,w
    t = Main Time/Clock a  = Macro Clock Repeat-Oneshot [R-C][O-C] p = Macro Clock Countdown [RcT][OcT] 
    w = Power Clock [O-C][R-C]. if using [*Cm] only add the numbers yymmddwhhmm w = weekday 0 = Sunday 6 = Saturday  
(l) *xn*number n = 0,1 - 8, 9 number = 1-54. Top row 3 keys cX-Cut, cC-Copy, cV-Paste: Programmable as x1, x2, x3 
    Layout 1, and x4, x5, x6 for Layout 3 and 4. *x0*0 clear all 8 keys, *xn*0 clear all 6 top-row keys. 54 options 
    are: Del Bks Tab aTb Ins Esc PrS aPr Ret Snp Osk UnD ReD caD Cut Cpy Pst Tsk Run wX CPi Ts1 Ts6  K1 - K24 Num 
    Cap Scroll. Use *x7*n=1-54 for [Del]ete key changes and *x8*n=1-54 for [Ret]urn key changes. For example *x1*3 - 1st 
    top-row key in Layout 1 (M) will change from [Cut] to [Tab]. (Press [*Cm] until *x1* shows then press [345] 
    once, press [ADD] press [EXE]) To reset use *x1*0 will reset all six x1-x6 to Cut, Copy, Paste. Another example
    *x4*6 - 1st top-row key in Layouts 2 and 3 [S] and [T] keys, will change from [Cut] to [Esc]. To program all 8 
    keys at once use *x9*m1m2m3s1s2s3d1r1 = all 8 values 1-6 7,8 with [Del] Key value = d1, and [Ret] Key value = r1.
    Can also use the serial port and send for example <*x9*0102030405060719> - then the 8 keys will be Del Bks Tab 
    aTb Ins Esc and [Del] = PrS and [Ret] = Run. To reset keys use the following guide:
    <*x9*0> reset all 8 keys    
    <*x8*0> reset Del       
    <*x7*0> reset Ret      
    <*x4*0> reset cX cC cV  
    <*x1*0> reset Cut Copy Paste  
    <*x0*>  clear all 8 keys   
    <*x0*0> reset all 8 keys  
(m) Mouse Cursor and Scroll movement: *m1*nn Scroll amount 1-10 (default 1) and *m2*nn Cursor move amount 1-99 
    (default 5). For example *m2*20 will change the cursor jump movement to 20 from the 5 default.          
(n) Media Key Options: Normally pressing the media key [Cfg] then [Med], only displays the four Media controls 
    Play-Pause, Next, Previous and Stop. To add the Volume Up and Down and the Mute keys press [Cfg][Vol] or 
    [Cfg][Vm]. By using *e1* to *e4* codes the action of the media key [Med] can be changed to display standard 
    (1) four Media Control Keys, (2) add the Volume Up/Down keys to the 4 control keys, (3) add the Mute key to 
    (1) and (2), (4) add the Tone control keys. To disable this additional behaviour use the *e0* command. Note 
    that when the media options (2-4) are enabled using *e2* to *e4*, pressing the media key off, will cancel all 
    the other media keys as well. To save the media keys press [Cfg] then [Sav] then after a reboot the setting e0 
    to e4 is saved i.e. if the [Med] is pressed after a powerup, and the e2 setting was previously activated, then 
    both the Volume and Media control keys will show. The single key options [Vol], [Vm] for Volume and Mute are
    still active when the *en* options are activated.
(o) Operating System Option: Use *os*[EXE] to toggle between Windows and Linux. Currently only used for different 
    Restart or PowerOff or Logout options for the [ROf] key. These commands are working in Debian 11.7 but Linux 
    Mint 21.1 and Ubuntu 20.04 steal the focus from the Terminal to the Desktop search box. Note that using these 
    will show passwords as plaintext in the history. Edit ShutDwn1 string and change PASSWORD and USER.
(p) Serial monitoring (via USB) on/off using *se*. A Red/Green square [LED] in the top-left will indicate the state
    (Off/On). Switch the serial on if macros or other text strings are sent to the Touchpad such as DateTime, 
    Foobar2000 Track Info, or HWInfo System Sensors. The state is saved.
(q) Skip first key pressed in LCD in dimmed state - used to wake LCD - toggle on/off with *ks* - the state is saved.
(r) Use SDCard files: When in S (Layer L3), press [s] Pad so that A,B,C,D change from white (FlashMem) to orange 
    (SDCard). Keys [S1] to [S24] then send text SDCard files as keyboard characters to PC - the size of the strings
    are only limited by the SDCard capacity provided the NKeys functions are used - i.e. either use the nKeys such
    as [n01], [p96], or if MST keys use the nKeys [Mxx] [Sxx] [Txx] for large text files. Change between 21 SDCard
    file-sets of 24 files each with *sd*n where n=1-9, m.s,t a-g, k,K. For example *sd*2[ADD][EXE] enables the 
    second set of files. The folder SDCardFiles has a set of example files. Change back to regular [S1]-[S24] key
    strings by pressing [s] Pad again to a white A-D. Use *sd*0 - i.e. select *sd* with [*Cm] then press 0[ADD][EXE]
    to disable the SDCard text files function - re-enable with *sd*n. If a new SDCard is inserted it may require 
    pressing the hardware reset button of the TouchLCD once. SDCard filenames are listed above in manual.h or in 
    sdcard.h Use *sd* no other parameters then SDFleList sent to Serial Port when using Pads [n] Upper/Lowercase 
    Name or Pad [o] Select Set.
(s) Bank123 options: Use *am,s,t*number namely *am*n or *as*n or *at*n with n=1,2,3,4,5 for Keys M S T. For n = 1,2,3
    choose between 3 different sets of 24 macro actions each, for each set of 24 keys M,S,T or choose 24 different
    Run or CMD commands with n = 4,5. Note that pressing WinKeyLeft = [GuiL] + [number key] such as [GUI1], [GUI2] 
    etc will run the 1st, 2nd etc.items pinned to the taskbar.
    n=1 Combination of 6 Notepad Shortcuts + 6 Photoshop Shortcuts + 6 Firefox Shortcuts + 6 MSWord Special Characters
    n=2 24 Windows shortcuts: TskMan  StrtRC Run PCInfo Calc PwerSh FMan TskView WakePC APrtScr MouseRClk CtrAltDel 
                              NewFldr Prop   OSK GUI9   GUI6 GUI7   GUI8 GUI0    GUI=   GUIF1   GUI4      GUI5
    n=3 Same set of 24 actions that can be used for top row set of 4 keys and the [Del [Ret] keys:
    Del Bks Tab aTb Ins Esc PrS aPr Ret Snp Osk Num Cap Scr Cut Cpy Pst Tsk Run wX CPi Ts1 Ts2 Ts3
    n=4 Set of 24 Windows Run commands
    n=5 Set of 24 Windows Powershell commands
    [LED] Indicator for MacroBank Selected in Layouts 1 3 4 = Orange,Green,Blue,Yellow,White
(t) *r0* Reset LCD - reboot after 2 seconds delay. 
    *r1* toggle action - enable/disable Reset-Once-On-Start
    *r2* Reboot the RP2040 into USB UF2 upload mode - can cancel press HW Reset within 5 seconds
    *r3* Enable double-tap hardware reset button, will then resstart to the UF2 USB bootloader (only for one session)
(u) *bl* toggle Black Key function on Powerkeys page - switch backlight on/off - enable/disable with *bl* To use 
    this first send *bl*[EXE] code then in Layer2 press [Cfg][ROf], and then press the Black Key (press twice the very 
    first time), to the switch backlight off - press on the same place of blank screen to switch the screen on again. 
    For PC's who supply USB power after switchoff this could be used to blank the LCD  screen after pressing one of the
    Power Keys for switch off. If disabled with *bl* pressing the black key will just exit the Power Keys screen.
(v) Key Held Duration: *kr*num with num = 1-9 or 100-900 msec key press wait duration before repeat.
(w) Key Held Enable: *ke* Enable/Disable Volume Mute Processing if [Vo][L1-L4] key is long-pressed
(x) *lf* Send SDCard and FlashMemory Filelist to serial port.
(y) *up* or use the [Cf][Opt] keys to toggle between Upper and Lower case macro file names.
(z) *im,s,t*numberlist Macro instruction list numberlist = maximum 12 single characters 0-9 or a-d. Can add less
    than 12 characters after *im,s,t* but last character added must be a zero 0. To see the instruction list use
    *ld* when a serial terminal is connected to the TouchLCD.
    Instruction List: Only execute 0-9 or a-d, exit if 0, can be in any order or length (set to 12 currently): 
    1=SDCardTextFiles 2=Flash+M-L 3=Flash+L-M 4=SDCard+M-L 5=SDCard+L-M 6=Do1 7=Do2 8=Do3 9=Bank123 0=Exit 
    a=Flash+M b=Flash+L c=SDCard+M d=SDCard+L
    For example 1 2 5 9 0   is the same as Layout=3 previously
                2 5 9 0     is the same as Layout=4 previously 
                2 5 6 7 9 0 is the same as Layout=1 previously
    Use *im* *is* *it* to reset to default i.e. when no numbers added after *im*, *is*, *it* 
    Use *ix* to select between the new instruction-list or the old coded section 
(A) *09* toggles between NumPad and nKey n01-n96 mode when Pad [n] is pressed in the Config page. Use Pad [o] to move
    between the 1-8 pages of each mode. 
(B) *0n*char The nKeys first character (default n), can be changed with with *0n*char - for example *0n*p will change
    the keys, and filenames used to p01 - p96. Any character or digit can be used - but not all will yield valid 
    filenames.  
(C) *0p*pages withe pages = 1 to 8 set the number of pages for nKeys or the NumPad 
(D) *0s*list-of-10-characters - these are displayd when pressing [Cfg][Opt] then select nKeys character woth Pad[o]
    The default is 'n','o','p','q','r','s','t','m','a','k' 
(E) *0R* Enable/Disable the resistor colour-coded number pad - plain colours used when off 
(F) Rename *rn*oldname=newname For example *rn*s02=a02[EXE] will rename file s02 (if it exists), to a02. Can also use 
    this to rename folders for example rename *Code/old=/new will rename folder old to folder new. Use [EXE] to
    complete the *rn*x=y command, using [Snd] will send *rn*x=y as text, and [Sav] will save *rn*x=y as text in the 
    current Source file such as a01. Do not use [Ren] to complete the rename action - use [EXE].       
-----------------------------------------------------------------------------------------------------------------------
Symbols-SpecialChar-Math-Greek-Algebra Keyboard: 
Press Pad [s]. This is a triple-key macro keyboard with 4 pages and 4 x 9 x 3 = 108 Special characters, Math/Algebra, and
Greek symbols. It is sent to the PC as (hex) Unicode + [ALT] + [x] and can be directly entered into MSWord and in 
LibreOffice (also on Linux). The three control keys are [EXE] - send the symbol to the PC, [NXT] - next page of symbols, 
and [ESC] go back to the main Config layout - or press the small blue pad again to leave the Symbols keyboard. Watch out 
for MSWord insisting on capitalising the first letter of a sentence - which will change the first math symbol in a sentence
or line after a second symbol or letter or enter is typed. 

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
Power Restart KeyPad [Restart Timer] [ Stop   ] [Power Timer] [Restart Clock  ]     [R-T] [Stp] [O-T] [R-C]   
                     [Restart timer] [Cfg-Exit] [Power timer] [Power   Clock  ]     [R-t] [Cfg] [O-t] [O-C]       
                     [Restart   Now] [Logoff  ] [Power   Now] [Backlight OnOff]     [Rst] [Log] [Off] [B-L]
-----------------------------------------------------------------------------------------------------------------------
Macro Timers KeyPad  [Repeat Timer] [  Stop  ] [Onceof Timer] [Repeat   CTimer]     [R-T] [Stp] [O-T] [RcT]   
                     [Repeat timer] [Cfg-Exit] [Onceof timer] [Oneshot  CTimer]     [R-t] [Cfg] [O-t] [OcT]       
                     [Repeat Clock] [Repeat x] [Onceof Clock] [Backlight OnOff]     [R-C] [Rep] [O-C] [B-L]
  
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

Both single macros from M, S and T 1-24 and linked macros can be used for the timers - if a linked macro is used add
a number 1* to 8* instead of 1 to 8. The Timers are programmed as Time-Fire-Time-Fire. There will be an option later 
to change this to Fire-Time-Fire-Time for the Repeat timers. The two real-time (using the Pico's HW RTC or Clock)
timers are configured by first setting the Clock Time by sending the string <tyymmddwhhmm> -> <t22110341439> is Thursday
3 Nov 2022 at 14h30. Then set the alarm time by sending the string <ayymmddwhhmm> -> <a22110601439> is Sunday 6 Nov 
2022 at 14h30. To send a repeat macro every 1 minute send <a-1-1-1--1-1> (the double -- is for the day of week not
significant), and associate with it 5 [R-C]. The clock time and alarm time are sent to a serial terminal and displayed
in the status bar by pressing [Cgf] twice. Can use a *code *tx*yymmddwhhmm to send all the clock values else send these
either manually using a serial terminal or use a Proccessing script, or a scheduled task powershell script. Note that 
the Pico has a HW RTC but does not have a dedicated battery backup for its HW RTC.

The Repeat-only mode (i.e send macro fixed number of times with a delay or no delay, is not implemented as yet.

Text Strings: 
Large Text file processing for nKeys: Can handle very large text strings strings, preferably stored on SDCard using 
nKeys n,o,p,q,r. If nKeys = m,s,t then large strings also enabled for M S T keys 01 - 96. Large strings tested up to
64kB. Keys MST 01-24 are best used for text<200 char and MST 25-96 for large text or use all NKeys NOPQR etc 01-96 
for large text. For example copy two large (>10kB) files L1 and S12 on SDCard. Then program nKey n01 with content L1.
Use KeyBrd editor with n01 as source (in brown not white), and add L1[Sav]. Test by pressing key nKey [n01]. Change 
nKey letter to S with [Cfg][Opt]PadKey[o] then press nKey [S12] - tested both keys both with notepad as the focus.

Send new text strings up to 200 characters to keys S1/T1 - S12/T24 via USB serial. Start string with <1 to <6 
followed by the string assigned to S1/T1-S24/T24 - end string with > If current Layout is L3 then S1 to S6 
changed, Layout L4 T1 to T6 changed, Layout L1 then M1 to M6 changed If current Layer is A then M1/S1/T1-M6/S6/T6 
changed, if Layer B M7/S7/T7-M12/S12/T12 changed etc. To store the string sent on SDCard rather than Flash make sure
that the "A" to "D" colour is orange and not white and that Layer 1, 3, or 4 is showing [L1][L3][L4]. The sent 
string will then be stored to the current SDCard file selected set 1 to 5 and to the filename selected by the first
number after the starting < character. Very large text files should be directly copied from a PC to the SDCard.

Example 1: Indicator A-D is orange and Layer 3 [S]keys [S7] to [S12] is visible. SDCard set 1 is active (set with 
*sd*1[ADD][EXE] or use [Cfg][Opt]+Pad[o]). Then use a serial terminal to send <1This is a string for key [S1].>. 
This string will then be stored in the SDCard file S0007.txt. When key [S7] is pressed with an orange A-D visible then
this text will be printed. If the second SDCard set is active then the same procedure as above will store the file as
B1.txt.
Example 2: A white "C" is showing and Layer 3 [S]keys [S13] to [S18] is visible. Then use a serial terminal to send 
<1This is a string for key [S1].>. This string will then be assigned to key [S13]. When key [S13] is pressed then
this text will be printed. 
Example 3: Send the macro 0x3C 0x34 0xE0 0xE1 0x29 0x3E (which is <4 Control Shift Escape >), 
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
    files. macros and settings but no re-calibration is needed at the first start-up.
(2) Press either the white button on the Pico MCU baord and hold it in, then press the reset button once, and then only
    release the white button. Or unplug and re-plug the USB cable whilst holding and then release the Pico white button.
    The file manager should show a new storage device named RPI-RP2. Drag and drop any of the code.UF2 files to this 
    device. It will restart after a second or two. If this still does not reset the keypad then instead of the code.UF2 
    file drag and drop the file flash_nuke.uf2, wait a few seconds and then drag the code.UF2 file to the device.
-----------------------------------------------------------------------------------------------------------------------
On First Start: 
Do a four-arrow corner calibration - press at the TIP of each arrow just ONCE. If you make a mistake and press the same
corner twice it is likely that you will need a reset with the nuke.uf2 file (also provided here), because the LCD will 
not pick up the correct corner keys being touched.  

The default LCD settings are full brightness and when off full blank. Change these by pressing the second Pad on the 
right (k) or (gray), and then press then press [*Cm] until *bb* shows in status line at the LCD bottom. Then press the
[678] key twice (7 shows), press [ADD] and then press [EXE]. The normal use LCD Brightness is then set to 40 percent. 
Do the same for the LCD blank setting - press [*Cm] until *db* shows, then press the [345] key once (3 shows), then 
press [ADD] and [EXE] - this sets the blank LCD to 10 percent brightness when in sleep mode.

You can also increase the elapsed time period before the display dims - change it from the default 30 seconds to 300
seconds by selecting *tb* using the [*Cm] key, then press the [678] key once (6), and then press [ADD] and [EXE].

When the LCD has dimmed or blanked a first keypress is ignored - as it is used to restore the LCD to its selected 
brightness. Use [*Cm] *ks* [EXE] to enable or disable this key-skip behaviour.

The Volume Up and Down keys [V+] and [V-], are on by the default on a first start. To switch the Volume keys off 
press [Cfg] in Layout 2, then press [Vol]. Press the [Sav] key to save this Volume-off state - else it will be on 
again at the next switch-on. 

As a replacement for the Volume [V+] key choose from a set of 54 options (Del Bks Tab aTb Ins Esc PrS aPr Ret Snp 
Osk UnD ReD Scr Cut Cpy Pst Tsk Run wX CPi Ts1 Ts6  K1 - K24 Num Cap). With the Volume key off, press [Cfg] and then 
[Key] once for [Del] key options, and twice for [Ret] key options. Press the bottom Pad [o] to select from the other 
54 options. Press the [Sav] key to save the option chosen.


*/
