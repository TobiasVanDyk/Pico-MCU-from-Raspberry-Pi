# Ducky Script Example with the Pico Touch Macropad

<img src="images/notepad-hello-world.png" width="16" height="16"/> <img src="images/duckyPad.png" width="16" height="16"/> How to do a [**well known duckyScript example**](notepad-hello-world.png) - Open notepad, type hello and increase the text size 3 times - [**duckyScript**](https://github.com/dekuNukem/duckyPad/blob/master/duckyscript_info.md) needs about 80-90 key-presses as does the example below.

``` 
Change to Layout L2. Press the [k] pad to open the macro editor and press [NXT].

Step 1: Assign the open run command to macrofile a01 - Gui + "r"
Select destination macro M 01 - press [Dst] then [Num] 
Press [NXT] press [GUI] press [ADD] press [NXT] press [pqr]3x press [ADD] press [EXE] 
press [Up] to save the macro - in shorter form from now on: 
[Dst][Num][NXT][GUI][ADD][NXT][pqr]3x[ADD][EXE][Up]

Step 2: Assign text notepad + Enter to macrofile a02 - "notepad" + Enter
Select destination macro M 02 - press [Num]
[Num][NXT][mno]2x[ADD][mno]3x[ADD][stu]2x[ADD][def]2x[ADD][pqr][ADD][abc][ADD][def][ADD]
[NXT]4x[CRF][ADD][EXE][Up]

Step 3: Assign type hello to macrofile a03 - "hello"
Select destination macro M 03 - press [Num]
[Num][NXT][ghi]2x[ADD][def]2x[ADD][jkl]3x[ADD][jkl]3x[ADD][def]2x[ADD][EXE][Up]

Step 4: Assign increase text size to macrofile a04 - Control + "+"
Select destination macro M 04 - press [Num]
[Num][NXT][CTR][ADD][NXT]3x[9+-]2x[ADD][EXE][Up]

Exit the Macro editor (press the [k] pad again), then test all four macro keys [M1] to [M4] by 
pressing them in sequence. It should open notepad type hello and increase the text size once.
Note: If there are any Link files associated with the keys [M1] to [M4] i.e. files named for
example M01Link, these will be executed rather than the macro file m01. Delete the Link file
in the Macro-editor by constructing its name M01Link, and then pres [Rmv].

Step 5: Link all four macros to a key [M5] (or any other key):
Enter the macro keyboard and select M 01 as the destination macro (press [Dst] then [Num] until 
M 05 shows.

Step 6: Compose a text string "a01a02a03a04a04a04" using [ADD] after each letter. Then press the
[Lnk] key. This will create a link file named M05Link containing the text string above. Make 
sure the A-D indicator is the same colour (white or orange i.e. FlasMem or SDCard) as when the 
macrofiles A01, a02, a03, and a04 were created.

Pressing key [M5] will now run notepad, then type "hello" and increase the text size three times. 

Key [M5] is now linked to the file M05Link.

``` 


