# Ducky Script Example with the Pico Touch Macropad

<img src="images/notepad-hello-world.png" width="16" height="16"/> <img src="images/duckyPad.png" width="16" height="16"/> How to do a [**well known duckyScript example**](notepad-hello-world.png) - Open notepad, type hello and increase the text size 3 times - [**duckyScript**](https://github.com/dekuNukem/duckyPad/blob/master/duckyscript_info.md) needs about 80-90 key-presses as does the example below.

``` 
Make sure that the A-D indicator is orange - either use [Cfg][A-D] or press Pad [m] when in Layout 1 (M keys).
Change to Layout L2. Press the [k] pad to open the macro editor and press [NXT].

Step 1: Assign the open run command to macrofile a01 - Gui + "r"
Select destination macro A 01 - press [Dst] then [Num] 
Press [NXT] press [GUI] press [ADD] press [NXT] press [pqr]3x press [ADD] press [EXE] 
press [Up] to save the macro - in shorter form from now on: 
[Dst][Num][NXT][GUI][ADD][NXT][pqr]3x[ADD][EXE][Up]

Step 2: Assign text notepad + Enter to macrofile a02 - "notepad" + Enter
Select destination macro A 02 - press [Num]
[Num][NXT][mno]2x[ADD][mno]3x[ADD][stu]2x[ADD][def]2x[ADD][pqr][ADD][abc][ADD][def][ADD]
[NXT]4x[CRF][ADD][EXE][Up]

Step 3: Assign type hello to macrofile a03 - "hello"
Select destination macro A 03 - press [Num]
[Num][NXT][ghi]2x[ADD][def]2x[ADD][jkl]3x[ADD][jkl]3x[ADD][def]2x[ADD][EXE][Up]

Step 4: Assign increase text size to macrofile a04 - Control + "+"
Select destination macro A 04 - press [Num]
[Num][NXT][CTR][ADD][NXT]3x[9+-]2x[ADD][EXE][Up]

Step 5: Link all four macros to a key [M1] (or any other key):
Enter the macro keyboard and select M 01 as the destination macro (press [Dst] then [Num] until 
M 05 shows.

Step 6: Compose a text string "a01a02a03a04a04a04" using [ADD] after each letter. Then press the
[Lnk] key. This will create a link file named M01Link containing the text string above. Make 
sure the A-D indicator is the same colour (white or orange i.e. FlasMem or SDCard) as when the 
macrofiles a01, a02, a03, and a04 were created.

Pressing key [M1] will now run notepad, then type "hello" and increase the text size three times. 

Alternatively to [M1] add Key K1 to the top-row keys: Use the MacroEditor to send *sd*K to the 
LCD. Exit the editor and press [Cfg] then [Key], and keep pressing [Key] until Layout 1 Key 1
is displayed. Then select using the bottom Pad [o] K1 from the 33 options. Send a serial text
string <1a01a02a03a04a04a04> to the LCD when A-D is orange and you are in Layout 3 (S keys). 
Pressing the new top righthand key [K1] should now run the linked macro.

``` 


