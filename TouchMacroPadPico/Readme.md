# Pico Touch Macropad

The [**RPi Pico**](https://www.raspberrypi.org/products/raspberry-pi-pico/) is used as a **Touch LCD Macropad** with 72 strings or 72 user-programmable macro-actions, or pre-coded macros selectable from five banks of 24 each, as well as another 99 programmable macrofiles, each of which can be chained 10x. There are more than 50 different pages and layouts for actions such as Media-keys (Volume Mute Play-Stop Next-Previous Bass-Treble), inserting special and math-symbols, a Restart-Shutdown Timer and Time(Clock) Config, Timer-and-Time-executed repeat-and-oneshot macros, a macro composition and linking editor, and a Mouse-button, Scoll-wheel and cursor movement keypad, an enhanced 3 page numberpad, displaying sensor data and music track playing data etc. 

Where available the SDCard slot can also be used for saving and reading files. Keys \[S1\] to \[S24\] Send SDCard files as keyboard characters to the PC - the size of the files/strings are only limited by the SDCard size. Change between three SDCard file-sets of 24 files each, with \*sd\*n where n=1,2,3. 

<img src="MacropadPCSensorData/images/PowerShellSession.jpg" width="16" height="16"/> In addition it is [**configured to display sensor data from a PC**](MacropadPCSensorData) using [**HWInfo's gadget registry data**](https://github.com/Timthreetwelve/HWiNFO-VSB-Viewer) sent to the macropad via a [**powershell script**](MacropadPCSensorData/PowerShellSession.txt) - note that the sensor data is accessed directly and not using the VSB Viewer.  

<img src="MacropadFoobarPlaying/images/PowerShellSession.jpg" width="16" height="16"/> It is also [**configured to display music playing from Foobar2000**](MacropadFoobarPlaying) using the Now Playing Simple foobar2000 plugin, which is sent to the macropad via a [**powershell script**](MacropadFoobarPlaying/PowerShellSession.txt).

<img src="SetDateTime/images/SetDateTime1.png" width="16" height="16"/> [**Date Time Display**](SetDateTime) This is an alternative Date Time which is only displayed, and not used to set the Pico system time-date. The procedure is explained in detail in the SetDateTime section.

<img src="handposition.png" width="40" height="30"/> The Touch LCD is used more conveniently when placed upright rather than lying flat, and then using your thumb-tip to press the 5 option pads, and the larger key pads. The single-touch characteristic of resistive touch overlays is in this case serendipitous. Place one or two fingers on top or behind, the LCD to stabilise it when using your thumb to press the keys. This orientation is then similar to the current use of both thumbs to manipulate smartphones touch screens from the side, as opposed to using other fingers from the touchscreen front. 

Re-configuration of this touch screen for left-handed use is not a major exercise - it already allows for 180-degree screen rotation through a \*code and only the key labels must then be corrected from being upside down, and the keys moved from bottom to top - but for both left and right-handed users the frequent use of the other hand-thumb with smartphones, means that both thumbs have already been trained for efficient touch screens use.

Usability guidelines for touch macro pads are difficult to find, but [**these Material Design Layout Guidelines**](https://m2.material.io/design/layout/understanding-layout.html#layout-anatomy) are in part, applicable. What is likely is that operating a touch-macropad with only a rigid finger motion has become unnatural when compared to the side-swipe-and-press thumb-use.

The difference in clarity between the TFT LCD (the Arduino-type 4"), and the other three IPS type LCDs (3.5" and 2.8"), are strikingly obvious, and the latter three are preferred even though the larger 4" LCD panel is easier to use.

1. [**ILI9486 3.5" 480x320 SPI Raspberry Pi connector Touch Macro-keys LCD**](TouchLCDili9486RPi)
2. [**ILI9486 4" 480x320 SPI Arduino Uno connector Touch Macro-keys LCD**](TouchLCDili9486Pico)
3. [**ST7789 2.8" 320x240 SPI Pico connector Touch Macro-keys LCD**](TouchLCDst7789Pico)
4. [**ILI9488 3.5" 480x320 SPI Pico connector Touch Macro-keys LCD**](TouchLCDili9488Pico)

<img src="https://github.com/TobiasVanDyk/Pico-MCU-from-Raspberry-Pi/TouchMacroPadPico/images/ada.gif" width="16" height="16"/> [**Adafruit**](https://www.adafruit.com/) has made a stunningly useful and beautiful [**Touch Desk**](https://learn.adafruit.com/touch-deck-diy-tft-customized-control-pad?view=all), which shares a similar layout approach to what I used - their design dates back to early 2021. 

<p align="left">
<img src="images/ada1.jpg" height="100" /> 
<img src="images/ada2.jpg" height="100" /> 
<img src="images/ada3.jpg" height="100" />
</p>

**Intellikeys:** Some the simpler IntelliKeys layouts will be included in the future such as [**this**](IntelliKeys/Arrows_Classic_overlay.jpg) or [**this**](IntelliKeys/Numbers_Classic_overlay.jpg) - as explained [**here**](IntelliKeys/IntelliKeysIntelliTools.pdf) ([**Spectronics source last updated in 2000**](https://www.spectronics.com.au/article/intellikeys-and-intellitools-programs-solutions-for-everyone)). Also refer to the [**original 1992 article**](IntelliKeys/IntelliKeys-the-Smart-Keyboard.pdf) - Source: Johns Hopkins APL Technical Digest, Volume 13, Number 4 (1992). Also see the [**new initiative by Adafruit**](https://github.com/adafruit/Adafruit_IntelliKeys).

<img src="notepad-hello-world.png" width="16" height="16"/> <img src="images/duckyPad.png" width="16" height="16"/> How to do a [**well known duckyScript example**](notepad-hello-world.png) - Open notepad, type hello and increase the text size 3 times - [**duckyScript**](https://github.com/dekuNukem/duckyPad/blob/master/duckyscript_info.md) needs about 80-90 key-presses as does the example below.

``` 
Change to Layout L2. Press the [k] pad to open the macro editor.

Step 1: Assign the open run command to key [M1] - Gui + "r"
Select destination macro M 01 - press [Dst] then [Num] 
Press [NXT] press [GUI] press [ADD] press [NXT] press [pqr]3x press [ADD] press [EXE] 
press [Up] to save the macro - in shorter form from now on: 
[Dst][Num][NXT][GUI][ADD][NXT][pqr]3x[ADD][EXE][Up]

Step 2: Assign type notepad + Enter to key [M2] - "notepad" + Enter
Select destination macro M 02 - press [Num]
[Num][NXT][mno]2x[ADD][mno]3x[ADD][stu]2x[ADD][def]2x[ADD][pqr][ADD][abc][ADD][def][ADD]
[NXT]4x[CRF][ADD][EXE][Up]

Step 3: Assign type hello to key [M3] - "hello"
Select destination macro M 03 - press [Num]
[Num][NXT][ghi]2x[ADD][def]2x[ADD][jkl]3x[ADD][jkl]3x[ADD][def]2x[ADD][EXE][Up]

Step 4: Assign increase text size to key [M4] - Control + "+"
Select destination macro M 04 - press [Num]
[Num][NXT][CTR][ADD][NXT]3x[9+-]2x[ADD][EXE][Up]

Exit the Macro editor (press the [k] pad), then test all four macro keys [M1] to [M4] by 
pressing them in sequence. It should open notepad type hello and increase the text size once

Step 5: Link all four macros to key [M1] (or any other key):
Enter the macro keyboard and select M 01 as the source macro (press [Src] then [Num] until 
M 01 shows and select M 02 as the destination macro - press [Dst] then [Num] until M 02 shows

Then press [Lnk] - keys [M1] and [M2] are now linked - when you press [M1] it will open the 
run dialog and then open notepad. Then also add macro M 03 to the link list - press [Num] until
it shows M 03 as the destination (the source macro should still be M 01) and press [Lnk] - Key
[M1] will now run notepad and type "hello". Now also add macro 04 - press [Num] for destination
macro M 04 then press [Lnk] - pressing key [M1] will now run notepad and type "hello" and 
increase the text size by one. 

Press [Lnk] again - pressing key [M1] will now run notepad and type "hello" and increase the 
text size by two. Press [Lnk] again -pressing key [M1] will now run notepad and type "hello"
and increase the text size by three.

Key [M1] is now linked to keys [M2][M3] and 3x[M4] - note that you can also link key [M1] to 
macro files instead of macro keys.
``` 

<p align="left">
<img src="images/white.jpg" width="20" /> 
<img src="handposition.png" height="100" />  
<img src="images/s2.jpg" height="100" /> 
<img src="images/s3.jpg" height="100" /> 
</p>
 
<p align="left">
<img src="images/white.jpg" width="20" /> 
<img src="images/mth1.jpg" height="80" /> 
<img src="images/num0.jpg" height="80" /> 
<img src="images/num2.jpg" height="80" /> 
<img src="images/tim1.jpg" height="80" /> 
</p>

<p align="left">
<img src="images/white.jpg" width="20" /> 
<img src="images/cfg1.jpg" height="80" /> 
<img src="images/kbd1.jpg" height="80" /> 
<img src="images/mac3.jpg" height="80" /> 
<img src="images/med1.jpg" height="80" /> 
</p>

<img src="Win11Icons.jpg" width="16" height="16"/> Note a suggestion for possibly [**better file/folder operations**](Better-File-Operations.txt) in [**Windows 11**](https://answers.microsoft.com/en-us/windows/forum/all/windows-11-file-explorer-right-click-menu-was/abed6378-4c45-4c1c-9cb1-fa6097ca4253?page=1) amongst the task support, familiarity, and other usability problems, in Windows 11 when compared to Windows 10.