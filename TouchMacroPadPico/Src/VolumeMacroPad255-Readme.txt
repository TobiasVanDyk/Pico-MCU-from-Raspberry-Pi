Compiled with Pico SDK 2,10, Arduino Pico 4.4.4 and included Adafruit_TinyUSB_Arduino 3.4.2, and TFTeSPI 2.5.43
3.5inch Touch Display Module for Raspberry Pi Pico included SDCard module https://www.waveshare.com/pico-restouch-lcd-3.5.htm
-------------------------------------------------------------------------------------------------------------------------------------------------Using library Adafruit_TinyUSB_Arduino at version 3.4.2 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\4.4.4\libraries\Adafruit_TinyUSB_Arduino 
Using library SPI at version 1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\4.4.4\libraries\SPI 
Using library TFT_eSPI at version 2.5.43 in folder: C:\Users\Tobias\Documents\Arduino\libraries\TFT_eSPI 
Using library LittleFS at version 0.1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\4.4.4\libraries\LittleFS 
Using library SDFS at version 0.1.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\4.4.4\libraries\SDFS 
Using library SdFat at version 2.3.0 in folder: C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\4.4.4\libraries\SdFat 
"C:\\Users\\Tobias\\AppData\\Local\\Arduino15\\packages\\rp2040\\tools\\pqt-gcc\\4.0.1-8ec9d6f/bin/arm-none-eabi-size" -A "I:\\Data\\Win10\\Arduino/VolumeMacroPad255.ino.elf"
Sketch uses 240876 bytes (23%) of program storage space. Maximum is 1044480 bytes.
Global variables use 38708 bytes (14%) of dynamic memory, leaving 223436 bytes for local variables. Maximum is 262144 bytes.
C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\tools\pqt-python3\1.0.1-base-3a57aed-1/python3 -I C:\Users\Tobias\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\4.4.4/tools/uf2conv.py --serial COM7 --family RP2040 --deploy I:\Data\Win10\Arduino/VolumeMacroPad255.ino.uf2 
Resetting COM7
Converting to uf2, output size: 528896, start address: 0x2000
Scanning for RP2040 devices
Flashing D: (RPI-RP2)
Wrote 528896 bytes to D:/NEW.UF2
-------------------------------------------------------------------------------------------------------------------------------------------------

To install new version of Arduino Pico first delete it from boards manager, then delete the folder 
C:\Users\Name\AppData\Local\Arduino15\packages\rp2040 then close and reopen Arduino IDE and then add the new Arduino Pico Board again.

NB: Use 2MB Flash option with 1MB Sketch 1 MB FS 
    Use USB Stack Adafruit TinyUSB
    Use 125 MHz CPU when higher SPI read is used:  #define SPI_FREQUENCY      35000000  // 50MHz causes volume artifacts
                                                   #define SPI_READ_FREQUENCY 15000000  // 20 MHz also ok

New changes:
1. Changed KeyBrd [EXE] and [Sav] to cater for nKeys other than char 'n' - other macro-keyboard-editor functions still need to be tested.
2. Added Undo Redo Ctr+Z Ctr+Y to Macrobank choices
3. Rename *rn*oldname=newname with oldname newname file or /folder names. To test using the keybrd editor choose *rn* with the [*Cm] key, then
use [ADD] tp construct *rn*s02=a02 and press [EXE]. File s01 will be renamed to a02. Can also use this to rename folders for example rename *Code/old=/new will rename folder old to folder new. Use [EXE] to complete the *rn*x=y command, using [Snd] will send *rn*x=y as text, and 
[Sav] will save *rn*x=y as text in the current Source file such as a01. Do not use [Ren] to complete the rename action - use [EXE].
4. Replaced Numpad with n-Keys n01-n96 on 8 pages, 12 keys per page. These only contain the path and name of a file to be executed. The n01-n96 files are therefore neither macros, textstrings, linkfiles, nor *codes. They only point to another file which has a a path - i.e. folder and file name, of any length less than 30 characters, on the same storage (Flash or SDCard) as the current nKeys set, to be executed as either a macro, textfile, linkfile, or *code. Switch between NumPad and nKey mode with *09*. Change the nKeys first character n with *0n*char - for example *0n*p will change the keys, and filenames used to p01 - p96. Use *Code *0s*list-of-10-char to change a shortlist using [Cfg][Opt] and Pad[o] to select the nKeys first char from a list of 10 - the default is 'n','o','p','q','r','s','t','m','a','k'. Note that when the nKey first char is m,s,t,a,k then the nKeys list will interpret these as macros or textstrings or linkfiles or *codes, and not as a filepath/filename. 
5. A large number of other changes were made to the source code compared to the previous version such as extra cycle in [Cf][Key] (BsDNum>=BSDMax 
and XNum[y]>=BSDMax) and VarStatus and OptStatus cycle corrections .

Previous changes
Added *im,s,t*numberlist Macro instruction list numberlist = maximum 12 single characters 0-9 or a-d. Can add less
than 12 characters after *im,s,t* but last character added must be a zero 0. To see the instruction list use *ld* 
when a serial terminal is connected to the TouchLCD.
Instruction List: Only execute 0-9 or a-d, exit if 0, can be in any order or length (set to 12 currently): 

    1=SDCardTextFiles 2=Flash+M-L 3=Flash+L-M 4=SDCard+M-L 5=SDCard+L-M 6=Do1 7=Do2 8=Do3 9=Bank123 0=Exit 
    a=Flash+M b=Flash+L c=SDCard+M d=SDCard+L

For example 1 2 5 9 0   is the same as Layout=3 below
            2 5 9 0     is the same as Layout=4 below 
            2 5 6 7 9 0 is the same as Layout=1 below  

Use *im* *is* *it* to reset to default i.e. no numbers added
Use *ix* to select between the new instruction-list or old coded section - on 1st use use *ix* to switch new list-based on

Slightly slower SPI Read and write speeds - User_Setup-New.zip
Updated Arduino-Pico 3.9.4
Fixed Rotate180 *ro* by moving InitCfg(1) to earlier in setp()
SDFS begin moved and delay added
Modified logic when when S and T keys filled with string values - is now 1st option keys Tx and 2nd option (after SD strings) Sx keys.
Fixed Reset Once notification - some SDCard brands may need to have this enabled or press HW Reset once on cold start - will investigate
Add run macrolink files as well as macrofiles from macro timer screen. See manual examples.
Fixed Linkmacro and Macro sequence success test
Improved Renamemacro
Add K to Source and Destination options (now M S T A K), See Example 3 i manual for details how this makes KnnLink and K keys 
short macro defines much easier.
Fixed – wrong selection - always bank 0, should be 0, 1, 5 - when Macrobanks selected in DoMacroButtons.
Fixed Macrobanks selection with [Opt] key and Pad [o] 
Change macrobanks with *am,s,t*1-5 now works again.
1. Macroeditor [Sav] key now saves entered data without execution to the Source file m,s,t, 1-24 a01-a99
It can be used to save *Codes for example *fo* (bold fonts) to a file such as t05 when key [T5] pressed will toggle bold fonts on/off
2. All *Codes can now be included in list of macros to be executed. For example can have a list of three macros: The first two macros 
opens Photoshop - GUI + r  and "C:\Program Files\Adobe\Photoshop\Photoshop.exe\n" - and the third macro *at*2 switch to the Photoshop 
specific Macrobank number 2 for keyset T1-T24. This can then be triggered by pressing a key such as [T1] or [K11], that runs the 
XnnLink file containing these three macros.
Add new options to LxnXXX x=0,1 0: Keep new value (SDCard/Flash Orange,White) 1: Restore old value
Replaced SD.h with SDFS.h
Stage 1 of consolidating various config files into a single file. The files NoKeySkip, CheckSerial, KeyHeldEnable, BLOnOff, Rotate180, KeyFontBold,
and ResetOnceEnable can be deleted from Flash memory if they are present - use the Macroeditor, type in the filename and press [Rmv].
Add L0nXnn to Linkfile content to load a new linkfile XnnLink with L0n where n = 3 different SDCard/Flash storage load options
2nd last Pad [n] used to toggle SDCard Fileset upper/lowercase filenames.
On macro create, save, or copy, a terminating 0x00 is only added if the last byte is not 0x00 already
NameStr + NameStr1 + NameStr2 re-use
*de* Delete Flash files will now make a copy of Calibration Data on SDCard and restore it to Flash after Flash has been formatted.
Add SourceName=TargetName i.e. entered names option to MacroEditor [Cpy] copy - can use any combination of to/from SDCard/Flash
Fixed missing Media Play/Pause
Delete all SDCard files with *df*
Rename Macros can now use entered Oldname=Newname. Remove macros can use enterd name if "k00" entered all 24 BSD 3byte macros are cleared.
Pad [0] (Red Pad) in Macro Editor will cycle through 4 possible combinations of Source and Destination for SDCard (Orange)
or Flash (White) storage - for example display shows: White Source M 01  Orange Target A 50 - then [Cpy] will copy file a01
from SDCard to file a50 on Flash. 
Pad [n] (Grey Pad) toggles KeyBoard Direct to PC Mode On/Off when in MacroEditor - show "D" in Macro Editor.
Add K keys K01-24 to Copy Macro destination - 3 byte BSD defines saved and read. If these are defined (not 0x00), then they are 
only executed after the K01Link-K24Link files - first if on SDCard, if not then on Flash - are executed.
Arduino Pico 3.9.1 - 255 char LittleFS (Flash) filenames -> (26+26+10)^255 = 1.15e457 Upper-and-LowerCase Char+Numbers Filenames 
1. Starcodes can be sent via a PC serial port or a Powershell command to the Touchpad. For example sending <*x9*0102030405060719> will
set all 8 programmable keys to new actions and labels - namely the 8 keys will be Del Bks Tab aTb Ins Esc and [Del] = PrS and 
[Ret] = Run. Using a Powershell get-process, one can then set these keys according to which application is open.
2. *xn*m has been expanded to include *x9*list and *x0*0. For example *x9*m1m2m3s1s2s3d1r1 = all 8 programmable key values 1-6 7,8 with [Del] Key value
 = d1, and [Ret] Key value = r1. *x0*0 reset all 8 keys to their default.
3. Macro Keys M S and T can now toggle between upper and lowercase filenames (meaningful only for flash storage) Use the [Cf][Opt] keys
to toggle between Upper and Lower case names or use *ul*. This adds another 3x24 (M01-M24, S01-S24, T01-T24) + 99 (A01-A99 filenames)
i.e. another 171 macros that can be stored and accessed. Note that the Flash Filesystem is case-senstive i.e. s01 and S01 are two
seperate files but the SDCard is case-preserving but not case-sensitive.
4. [Cfg][Opt] has option to send SDCard+Flash Filelist to PC Serial Port
Updated SDCard Filenames generation for upper/lowercase filenames - lowercase only through *sd*n n=1-9, U-X u-x mst MST A-G a-g
Updated Arduin-Pico
Corrected LinkMacro which wrongly used Source Number and Destination Letter
Large number of changes see List of replacement.txt
Simplified buttonpress()
Reverted back to version 174 but added fixes from version 175
Add *lf* Send SDCard and FlashMemory Filelist to serial port.
Fixed SDCard or FlashMem macro same name switch with A-D white-orange
New color "LED" indicator for Layouts M S T MacroBank 1-5 = Orange Green Blue Yellow White
If long-press [L1]-[L4] for VolumeMute toggle is Enabled Label changes to [V1]-[V4]
Expanded SDCard File to 21 sets 1,2,3, U-Z, M,S,T, A-G, k, K
New [Opt] in [Cfg] Layout: 4 Categories: (1) Toggle [L1-4]{Vo]Long-Press On/Off, (2) Select StartupLayout L1-L4, 
                                         (3) Select MacroBanks 1-5 for M,S,T using Pad[0], (4) Select SDCard File Set 1-21                           
Serial indicator moved to left as square red/green "LED", replaced by SDCard Set 1-3, U-Z, A-G, kK
Note: Can replace glcdfont.c in TFT_eSPI\Fonts with the version included here for a 100% filled block char for the LED symbol
Replaced all String uses with char []
Fixed wrong macro name display after construction in macro editor
Fixed SDCard Filename construction
Added current SDcard Set indicator - shared with Scroll-Lock
Previous changes
Added r2 and r3 to *rn* reboot types:
*r0* Reset LCD - reboot after 2 seconds delay. 
*r1* toggle action - enable/disable Reset-Once-On-Start
*r2* Reboot the RP2040 into USB UF2 upload mode - can cancel press HW Reset within 5 seconds
*r3* Enable double-tap hardware reset button, will then resstart to the UF2 USB bootloader (only for one session)

Extended K1-K6 keys to K1-K24
Extended delay and repeat options:
Repeat rnn = repeat 01=1 to 99=99 times the next macro    - r03a01 repeat a01 3x
Repeat Rnn = repeat 01=100 to 9900 times the next macro   - R03a01 repeat a01 300x
Repeat RRn = repeat 1=10000 to 90000 times the next macro - RR3a01 repeat a01 30000x
Use rnna01 = repeat a01 nn times and not a01rnn as in DuckyScript
 
Delay ddn - delay 0=100 to 9=900 milliseconds - dd3a01 delay 300 milliseconds before a01 executed
Delay dnn - delay 01=1  to 99=99 seconds      - d03a01 delay 3 seconds before a01 executed
Delay Dnn - delay 01=1  to 99=99 minutes      - D03a01 delay 3 minutes before a01 executed
Delay DDn - delay 1=1   to 9=9 hours          - DD3a01 delay 3 hours before a01 executed

Add rnn and Rnn option to macrolink string. rnn repeat nn times, D repeat 100 x nn i.e. a macrostring r06a53 will run
macro a53 6 times
Add dnn and Dnn option to macrolink string. dnn delay nn seconds, D delay nn minutes i.e. a macrostring a01d01a02 will run
macro a01, the delay 1 second, then run macro 2.
Major change in the macro linking implementation. Executing a series of macros is now done by following a plaintext list of 
3-letter filenames in sequence. This list is stored in files named MSTKnnList. For example the file M01Link contains the 
text a50a51a52n. When pressed it will execute the contents of three macro and/or text-files: a50: GUI r, a51: "notepad", 
a52: Ctrl + increase size. In addition select which of two possible M01Link files pressing M1 will execute by selecting
the SDCard or FlasMem as storage target through the 8-cycle A-D Layer indicator.
Use [Key] then Pad [o] keys to select 33 options for [Del], [Ret], [Cut,Copy,Paste] keys - press 1x to 8x. Include 6 K1-K6
keys which are linked to files K01Link to K06Link
Save/Restore A-D SDCard/Coded or Text/Macro Strings (White/Orange A-D state) must use [Cfg][A-D][Sav] sequence to save
Use [Var] then Pad [o] keys to select 24 options for [Del] and [Ret] key 
New [Cfg] settings page layout
Configuration starts on *Cm page
Fix Serial S and KeyboardDirect D priority
A-D indicator changes colour from white to orange instead of "X" replacing "A" "B" "C" "D"
Pressing the [Cfg][A-D] now cycles through 8 options - four A-D (white) and four A-D (orange) - press [Sav] to save option
Last four of the 24 preset selectable options changed to CoPilot (Gui+Shift+F23), Taskbar 1, 2 or 3
Change press [Cfg] once then [Var] repeatedly then [Sav] to save new key option - replaced by options pad [o]
Change [Cfg] auto-save - must now press [Cfg][Sav] before changes are saved
Added *x7*n=1-24 and *x8*n=1-24 for [Del]ete and [Ret]urn key options
Reverted calibration backlight changes
Change [Ret]urn key 24 options same as [Del]ete key [Var] with A-D or for [Del] key, or [Var] with X showing for [Ret] key
Fixes for *xn*number options
Add *ke* Enable/Disable Volume Mute Processing if key held in - on first use run *ke* once to enable long-press for [Vo] VolMute
Add *kr*num with num = 1-9 or 100-900 msec key press wait duration before repeat
Change [Vo] Volume Mute behaviour - need long press (1 second) to toggle [L1-L4] and [Vo]
*r0* Reset LCD - reboot after 2 seconds delay. 
SDCard files can be updated via PC serial port - see manual.h "Text Strings" for instructions.
For all *Codes return to same *Code with [*Cm] if [EXE] was pressed. *Codes are incremented to the next starcode if no [EXE}
StarCodes handling improved via switch-case
sdcard.h made an include file
Black Key on Powerkeys page switch backlight on/off enable/disable with *bl*
coloursDef.h adjusted for Waveshare RPi TouchLCD 3.5 inch Type C
Added SDCard Read macro functions similar to the Waveshare ST7789 Pico-LCD board.
Align setup() to be similar to ST7789
Revert to setup() as in VolumeMacroPad130 - check if less freezing on cold start
Add option to enable/disable Reset-Once-On-Start with *r1* option (toggle)
Changes in USB-Serial initialisation and position during start sequence
Enabled a once-reset at start
mathKeys.h and macroBanks.h as include files
Two admin powershell and command functions for M1 and M5
Replaced button (reserved word) with Button
Use *am*n or *as*n or *at*n with n=1,2,3,4,5 for Keys M S T to choose between 3 different sets of 24 macro actions each, for each
set of 24 keys M,S,T Or choose between 2 sets of 24 differenr Run or CMD commands with n = 4,5.
n=1 Combination of 6 Notepad Shortcuts + 6 Photoshop Shortcuts + 6 Firefox Shortcuts + 6 MSWord Special Characters
n=2 24 Windows shortcuts  TskMan  StrtRC Run PCInfo Calc PwerSh FMan TskView WakePC APrtScr MseRClk CtrAltDel 
                          NewFldr Prop   OSK GUI9   GUI6 GUI7   GUI8 GUI0    GUI=   GUIF1   GUI4    GUI5
n=3 Same set of 24 action that can be used for top row set of 4 keys
n=4 Set of 24 Windows Run commands
n=5 Set of 24 Windows Powershell commands
Standard set of 24 windows macro definitions for keys [S1] to [S24] if "X" option on
TaskMan StrtRClk Run PCInfo Calc PwerSh FMan TaskView WakePC AltPrtScr MseRClkMenu CtrAltDel NewFldr Prop OSK GUI9 6 7 8 0 F1 = 4 5     
                                      ( GUI 4 Restart 5 Shutdown 6 Edge 7 Outlook 8 Word 9 Firefox 0 Minimize = Magnifier F1 Help )
Easier to check X option - assign strings in stringt24.h to T group with *ft*, then press the [t] Pad 
option - X will show - then press the keys below with either Notepad, Photoshop, Firefox or Word opened 
respectively. Switch back to T assigned strings by pressing Pad [t] again while still on the T keys page.
New Pre-programmed Keys M1-M24 S1-S24 T1-T24 - Activated with "X" Option
If the same Pad Key is pressed in its own Layout - for example press Option pad S when in the [S1] - [S24] keys layout,
it will then ignore any assigned strings and use the coded programmed action. Toggle it on/off by pressing the Pad once
or twice. This functions for all three keygroups M, S and T. The A,B,C,D indicator will change to "X" when this option is 
On, and back to A - D when it is Off.
Small correction to const static char StarCode for *e0* to *e4*
New include files coloursDef.h and keysDef.h for Key and Colour definitions, stringRSL.h and manual.h for Reboot, Shutdown, Logoff 
definitions and a User Guide.
Choose with *Code if first key pressed in LCD in dimmed state skipped i.e. used to wake LCD - toggle on/off with *ks*
NB corrected twice: for (n=0; n<6; n++) keycode[6] = 0x00; -> for (n=0; n<6; n++) keycode[n] = 0x00; 
Serial monitoring (via USB) on/off using *se*. A Red/Green "S" will indicate the state (Off/On). Switch the serial on if macros or other
text strings are sent to the Touchpad such as DateTime, Foobar Track Info, or HWInfo System Sensors. This setting is saved.
Operating System Option: Use *os*[EXE] to toggle between Windows, Linux, and Raspberry Pi OS. Currently only used for different Restart
or PowerOff or Logout options for the [ROf] key. These commands are working in Debian 11.7 but PiOS, Mint 21.1 and Ubuntu 22 sometimes
steal the focus from the Terminal to the Desktop search box. An alternative using Alt+F2 (Runbox) is presented at the end of the code 
section. Note that using these will show passwords as plaintext in the history. For Linux edit ShutDwn1 string and change PASSWORD and
USER.
There are now two string header files strings24.h for the S keys and stringt24.h for the T keys. Load these using *fs* *ft* and 
save them to flash using *ss* and *st*.
Date Time Display: This is alternative Date Time which is only displayed not used to set the Pico system time-date.
The procedure is explained in detail in the SetDateTime section.
Can now choose Layout 1,2,3 or 4 as the startup Layer - press [Cfg][1S4] repeatedly.
Fixed issue with A,B,C,D layer letter display
PC Music Playing Data: The Music Playing data read from Foobar2000's Now Playing Simple foobar2000 plugin, can be sent
to the touchpad and displayed on the LCD statusbar. The procedure is explained in detail in the MacropadFoobarPlaying section.
PC Sensor Data: The sensor data read from HWInfo's Gadget Regisry data can be sent to the touchpad and displayed on the
LCD statusbar. The procedure is explained in detail in the MacropadPCSensorData section.
Media Key Options: Previously pressing the media key [Cfg] then [Med], only displays the four Media controls Play-Pause, Next, 
Previous and Stop. To add the Volume Up and Down and the Mute keys press [Cfg][Vol] or [Cfg][1L4]. 
Using the four new *e1* to *e4* codes the action of this media key can be changed to display the (1) four Media Control Keys, 
(2) add the Volume Up/Down keys to the 4 control keys, (3) add the Mute key to (1) and (2), (4) add the Tone control keys. 
To disable this additional behaviour use the *e0* command. 
Change and save Mouse Cursor and Scroll movement: *m1*nn Scroll amount 1-10 (default 1) and *m2*nn Cursor move amount 1-99 (default 5). 
For example *m2*20 will change the cursor jump movement to 20 from the 5 default.
Mouse Keyboard:
When in Layer 2 - press middle pad [u] to toggle Mouse KeyBoard on/off - these are 12 mouse buttons,
scroll, and cursor movement keys:
[LBt]   [Up]  [RBt]   [Sc+]    [Left-Button]   [Cursor-Up]    [Right-Button]   [Scroll-Up]
[ < ]  [MBt]  [ > ]   [L2x]    [Cursor-Left] [Middle-Button]  [Cursor-Right] [Double-Left-Click]
[Bck]  [Dwn]  [Fwd]   [Sc-]    [Back-Button]  [Cursor-Down]  [Forward-Button]  [Scroll-Dwn]
Note that a Mouse Right-click can be also programmed as a Shift and F10.
Colour Scheme Changes
Added option for double ** to process strings starting with *
Added comment to end a shorter string that replaces a longer string with NULL char - press [GUI]3x and [ADD] at end of shorter string




