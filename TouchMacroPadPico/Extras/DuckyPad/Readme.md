# Ducky Script Example with the Pico Touch Macropad

<img src="images/notepad-hello-world.png" width="16" height="16"/> <img src="images/duckyPad.png" width="16" height="16"/> How to do a [**well known duckyScript example**](notepad-hello-world.png) - Open notepad, type hello, wait 5 seonds, and then increase the text size 20 times - [**duckyScript**](https://github.com/dekuNukem/duckyPad/blob/master/duckyscript_info.md) needs about 80-90 key-presses as does this example <img src="macro.gif" width="16" height="16"/>

A screen recording of the macrolink in actio is [**here**](macro.gif). The files used a50, a51, a52, a53 amd K01Link and M01Link are also included
and can be copied to the TouchLCD SDCard to test.

```
There are two ways to do this example Option 1 and Option 2. Both need the four macros a50, a51, a52, and a53 to be already defined either 
by using the Macroeditor or sending it via a serial port to the LCD from a PC. a50 is GUI r (Open Run box), a51 is the text "notepad" + Enter, a52 is
Ctrl + increase zoom in notepad, a53 is the text "Hello". To define the four macro refer to the bottom of this page: Creating Macros a50 to a53.

A repeat rnn and Rnn with r = repeat 1-99 and R - repeat 100 to 9999 similar to the existing delay dnn and Dnn with d seconds 1-99 and D minutes 1-99,
is planned for the next release, which will make the macrolink string used below less awkward: a50a51a53d05r20a52

Option 1: Use the PC serial port to send the [K2] macrolink string to the LCD;
------------------------------------------------------------------------------ 
Make sure that the A-D indicator is orange - either use [Cfg][A-D] or press Pad [m] when in Layout 1 (M keys).
Change to Layout L2. Press the [k] pad to open the macro editor and press [*CmT] ubntil *sd* shows. Press [NXT]4x press [JKL]2x
then press [add][exe] when *sd*K shows - display will show "SDCard Files Set K". Press pad [k] to exit the Macroeditor then goto
Layer 3 (S Keys) and send the text <2a50a51a53d05a52a52a52a52a52a52a52a52a52a52a52a52a52a52a52a52a52a52a52> from a PC serial port
to the LCD. The display will show "K02 link". Make one of the top-row keys [K2] by pressing [Cfg] then [Key]3x display will show
"Select Layout 1 Key 1". Then press the bottom option Pad [o] until "Key 2" is displayed in the status bar - there are 33 options.
Press [Sav] then goto Layer 1 (M Keys) and the first top key will be K2. Pressing [K2] with A-D in orange, will then open notepad, 
type hello, wait 5 seconds, then increase the text 20 times. A-D must enable the SDCard because the file K01Link that pressing key
[K1] runs, is only on the SDCard. If a copy is also in FlashMemory A-D can be white as well.


Option 2: Create the macrolink string using the Macroeditor:
------------------------------------------------------------
Make sure that the A-D indicator is orange - this means the macrostring will be saved on the SDCard and not FlasMemory. Enter the 
Macroeditor and select destination macro M 01 - press [Src] then [Dst] then [Num] until "Target M 01" shows. Then compose a text string "a50a51a53d05a52a52a52a52a52a52a52a52a52a52a52a52a52a52a52a52a52a52a52" using [ADD] after each letter or number. Then press the [Lnk] key. 
This will create a link file named M01Link containing the text string above on the SDCard. 

Pressing key [M1] (when A-D is orange), will now run notepad, then type "hello" and increase the text size three times. 


Creating Macros a50 to a53
--------------------------
Everytime after creating the macro you must press [EXE] which will execute the macro (and
exit the Macroeditor), and pressing [Up] will then save it on the SDCard as file a + nn.
Press [k] Pad to again enter the Macroeditor for the next macro.

Step 1: Assign the open run command to macrofile a50 - Gui + "r"
Select destination macro A 50 - press [Src] then [Dst] then [Num] until "Target A 50" shows
Press [NXT] press [GUI] press [ADD] press [NXT] press [pqr]3x press [ADD] press [EXE] 
press [Up] to save the macro - in shorter form it is: 
[Src][Dst][Num]50x[NXT][GUI][ADD][NXT][pqr]3x[ADD][EXE][Up] 

Note that [Num]50x means you do not need to press [Num] 50 times, you can just hold in key [Num]
it will rapidly count up to 50.

Step 2: Assign text notepad + Enter to macrofile a51 - "notepad" + Enter
Select destination macro A 51 - press [Dst] then [Num] until "Target A 51" shows
[NXT]3x[mno]2x[ADD][mno]3x[ADD][stu]2x[ADD][def]2x[ADD][pqr][ADD][abc][ADD][def][ADD]
[NXT]4x[CRF][ADD][EXE][Up]

Step 3: Assign increase text size to macrofile a52 - Control + "+"
Select destination macro A 52 - press [Num] until "Target A 52" shows
[NXT]2x[CTR][ADD][NXT]3x[9+-]2x[ADD][EXE][Up]

Step 4: Assign type hello to macrofile a53 - "hello"
Select destination macro A 53 - press [Num] until "Target A 53" shows
[NXT]3x[ghi]2x[ADD][def]2x[ADD][jkl]3x[ADD][jkl]3x[ADD][def]2x[ADD][EXE][Up]


``` 


