# Ducky Script Example with the Pico Touch Macropad

<img src="images/notepad-hello-world.png" width="16" height="16"/> <img src="images/duckyPad.png" width="16" height="16"/> How to do a [**well known duckyScript example**](notepad-hello-world.png) - Open notepad, type hello and increase the text size 3 times - [**duckyScript**](https://github.com/dekuNukem/duckyPad/blob/master/duckyscript_info.md) needs about 80-90 key-presses as does the example below.

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


