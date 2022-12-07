# Pico MCU from Raspberry Pi

<p align="left">
<img src="Raspberry-Pi-Pico-Pinout.png" width="300" /> 
</p>



The [**RPi Pico**](https://www.raspberrypi.org/products/raspberry-pi-pico/) is programmed as:
A Touch Macropad with 72 + 99 macrokeys, each of which can be chained 10x, and layouts for mediakeys, math-symbols, Restart-Shutdown config, Timer-executed macros, and a macro composition and linking editor etc. The Touch LCD is used most conveniently when placed upright rather than flat, and using your thumb-tip to press the 5 option pads. Place two fingers on top of the LCD to stabilise it when using your thumb to press the keys.
1. [**ILI9486 3.5" 480x320 SPI Raspberry Pi connector Touch Macro-keys LCD**](https://github.com/TobiasVanDyk/Pico-MCU-from-Raspberry-Pi/tree/main/TouchLCDili9486RPi)
2. [**ILI9486 4" 480x320 SPI Arduino Uno connector Touch Macro-keys LCD**](https://github.com/TobiasVanDyk/Pico-MCU-from-Raspberry-Pi/tree/main/TouchLCDili9486Pico)
3. [**ST7789 2.8" 320x240 SPI Pico connector Touch Macro-keys LCD**](https://github.com/TobiasVanDyk/Pico-MCU-from-Raspberry-Pi/tree/main/TouchLCDst7789Pico)
4. [**ILI9488 3.5" 480x320 SPI Pico connector Touch Macro-keys LCD**](https://github.com/TobiasVanDyk/Pico-MCU-from-Raspberry-Pi/tree/main/TouchLCDili9488Pico)

How to do a [**well known duckyscript example**](notepad-hello-world.png) - Open notepad, type hello and increase the text size 3 times.

``` 
Change to Layout L2. Press the [k] pad to open the macro editor.

Step 1: Assign the open run command to key [M1] - Gui + "r"
Select destination macro M 01 - press [Dst] then [Num] 
Press [NXT] press [GUI] press [ADD] press [NXT] press [pqr]3x press [EXE] press [Up] to save the 
macro - in shorter form from now on: [Dst][Num][NXT][GUI][ADD][NXT][pqr]3x[ADD][EXE][UP]

Step 2: Assign type notepad + Enter to key [M2] - "notepad" + Enter
Select destination macro M 02 - press [Num]
[Num][NXT][mno]2x[ADD][mno]3x[ADD][stu]2x[ADD][def]2x[ADD][pqr][ADD][abc][ADD][def][ADD]
[NXT]4x[CRF][ADD][EXE][Up]

Step 2: Assign type hello to key [M3] - "hello"
Select destination macro M 03 - press [Num]
[Num][NXT][ghi]2x[ADD][def]2x[ADD][jkl]3x[ADD][jkl]3x[ADD][def]2x[ADD][EXE][Up]

Step 3: Assign increase text size to key [M4] - Control + "+"
Select destination macro M 04 - press [Num]
[Num][NXT][CTR][ADD][NXT]3x[9+-]2x[ADD][EXE][Up]

Exit the Macro editor (press the [k] pad then test all four macro keys [M1] to [M4] by 
pressing them in sequence. It should open notepad type hello and increase the text size once

Step 4: Link all four macros to key [M1] (or any other key):
Enter the macro keyboard and select M 01 as the sourec macro (press [Src] then [Num] until 
M 01 shows and select M 02 as the destination macro - press [Dst] then [Num] until M 02 shows

Then press [Lnk] - keys [M1] and [M2] are now linked - when you press [M1] it will open the 
run dialog and then open notepad Add micro m 03 to the link list - press [Num] until it shows 
M 03 as the destination (the source macro should still be M 01) and press [Lnk] - Key [M1[ 
will now run notepad and type "hello". Add macro 04 - press [Num] for destination macro M 04 
then press [Lnk] - pressing key [M1] will now run notepad and type "hello" and increase the 
text size by one. Press [Lnk] again - pressing key [M1] will now run notepad and type "hello"
and increase the text size by two. Press [Lnk] again -pressing key [M1] will now run notepad
and type "hello" and increase the text size by three.

Key [M1] is now linked to keys [M2][M3] and 3x[M4]
``` 

<p align="left">
<img src="images/white.jpg" width="20" /> 
<img src="images/s2.jpg" height="100" /> 
<img src="images/s3.jpg" height="100" /> 
</p>

6. [**LCD ST7789 240x135 SPI Pico connector display driver**](https://github.com/TobiasVanDyk/Pico-MCU-from-Raspberry-Pi/tree/main/LCDst7789Pico)
7. [**VS1053 SDCard SPI MP3 music player**](https://github.com/TobiasVanDyk/Pico-MCU-from-Raspberry-Pi/tree/main/Vs1053Pico)
8. [**RP2040Zero**](https://github.com/TobiasVanDyk/Pico-MCU-from-Raspberry-Pi/tree/main/RP2040ZeroPico) Waveshare Mini Pico Neopixel Demo
9. [**i2s Audio-USB DAC**](https://github.com/TobiasVanDyk/Pico-MCU-from-Raspberry-Pi/tree/main/DacPico) Pimoroni (PCM5100A) and Waveshare (PCM5101A)
10. [**OLED SSD1306 i2c display driver**](https://github.com/TobiasVanDyk/Pico-MCU-from-Raspberry-Pi/tree/main/OledPico)



### Other Recent Related Topics
* [**How to set up Pi Pico (W) 1.40 SDK on Windows 10x64**](Install-PicoSDK-in-Windows10x64-July2022.pdf), using current (July 2022) versions of all components. 
* [**Pico W**](https://www.raspberrypi.com/news/raspberry-pi-pico-w-your-6-iot-platform/) and later [**Bluetooth**](https://forums.raspberrypi.com/viewtopic.php?t=336764#p2015678) as well. [**Pico SDK version 1.40 updated for Pico W**](https://github.com/raspberrypi/pico-sdk/releases/tag/1.4.0), [**Pico Examples updated for Pico W**](https://github.com/raspberrypi/pico-examples) 30 June 2022.
* [**Pico SDK version 1.3.1 has been released**](https://github.com/raspberrypi/pico-sdk/releases/tag/1.3.1) 19 May 2022.
* June 2022 [**Tiny PCM5102A USB Audio PicoDSP board**](https://github.com/DatanoiseTV/PicoDSP-Hardware)
* Combine your Pi400 and the RPi Pico - [**Pi 400 Pico Plate**](https://github.com/Wren6991/Pi400-Pico-Plate).
 
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






