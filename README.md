# Pico MCU from Raspberry Pi
There are two main approaches currently for programming the [**RPi Pico**](https://www.raspberrypi.org/products/raspberry-pi-pico/) - using a python interpreter installed on the Pico as firmware (for example Micropython or Adafruit's [**CircuitPython**](https://circuitpython.org/board/raspberry_pi_pico/), or using a [**C++ based SDK**](https://github.com/raspberrypi/pico-sdk). Using the command and Visual Studio Code build methods as in [**Working with the Raspberry Pi Pico with Windows and C/C++**](https://www.element14.com/community/community/raspberry-pi/blog/2021/01/24/working-with-the-raspberry-pi-pico-with-windows), on Windows 8.1 (32 bit), proved to be reasonably easy - details are given below in section 3. The much shorter python programming tool-chain, works well under both of the main Raspberry Pi Os flavours (64 bit and kernel 5.10, and 32 bit with kernel 5.4). It seems certain that soon the Arduino IDE will also support the RPi Pico, as well as their own [**Arduino Nano Connect RP2040-based board**](https://blog.arduino.cc/2021/01/20/welcome-raspberry-pi-to-the-world-of-microcontrollers/), and also new RP2040 Pico boards from [**Sparkfun**](https://www.sparkfun.com/rp2040) and [**Adafruit**](https://www.adafruit.com/category/875).

Below is a short description on how to setup and use the RPi Pico micropython toolchain (Section 1 and 2), and using the CMake SDK with both command and Visual Studio Code build-methods (Section 3), under windows 8.1 (and 10). 


### 1. Installing the Raspberry Pi Pico under Windows 8.1 and 10

Windows 10 (version 20H2),  installs the necessary COM-Port driver [**usbser.sys**](images/win10Pico2.jpg) automatically - with a new USB Serial device as COM port number x (refer to the four images [**win10Pico1**](images/win10Pico1.jpg) to [**win10Pico4**](images/win10Pico4.jpg) in the images subfolder). Unexpectedly, Windows 8.1 (64 bit) seems to be unable to find a suitable serial driver for the RPi Pico MCU, and shows it as an unknown serial device (yellow exclamation mark), in the windows device manager. 

Trying to install the [**modified windows 7 atmel_devices_cdc.inf driver**](pico-serial.inf) as described here: [**Windows 7 and XP Serial Port Drivers**](https://www.raspberrypi.org/forums/viewtopic.php?f=146&t=300053), is unsuccessful for Window 8.1 64-bit, as the driver is unsigned, but worked well for Windows 8.1 32-bit as shown below:

<p align="left">
<img src="images/win81a.jpg" width="280" />  
<img src="images/win81b.jpg" width="220" /> 
<img src="images/win81c.jpg" width="280" /> 
<br>

[**Another solution is presented here**](https://picockpit.com/raspberry-pi/raspberry-pi-pico-and-micropython-on-windows/), by creating a generic serial driver using the [**Akeo Zadig USB Tool**](https://zadig.akeo.ie/). This installs the Pico as a generic USB serial CDC device. As shown below, its use is a simple two step procedure: Plug in the Pico, then select the Board CDC (Interface 0) and the USB Serial (CDC) in the other field, and lastly install the driver:
<br>
<p align="left">
<img src="images/image6a.png" width="400" />  
<br>

The result is a new com port (6 in this case) in the device manager:
<br>
<p align="left">
<img src="images/image1b.jpg" width="500" />  
<br>

After a reset of the Pico to a mass storage device, to be used for uploading of the .uf2 firmware, is done as shown below:
<br>
<p align="left">
<img src="images/image4a.jpg" width="500" />  
<br>


### 2. Installing Micropython and the Thonny Python IDE, for the Raspberry Pi Pico under Windows 8.1

A reset button was connected between the RPi Pico pin 30 (RUN) and pin 28 (GND) as shown below - this avoids the requirement of a a disconnection of the USB cable to set the Pico in boot-select load mode. 
<br>
<p align="left">
<img src="images/picoboard1.jpg" width="600" />  
<br>


After the Pico is plugged into a windows 8.1 USB port, press and hold both the onboard Boot-Select button and the (external) Reset button, and then first release the reset button followed by the bootsel button. A new storage drive RPI-RP2 will then be visible. Drag the most current [**Pico Micropython firmware**](https://www.raspberrypi.org/documentation/pico/getting-started/) (pico_micropython_20210121.uf2), to this drive which will then reboot the Pico and it will re-appear as a COM device.

Keep the Pico plugged in as a COM device and then install [**Thonny**](https://thonny.org/) using its default settings (i.e. as standard, and using the included Python 3.x bundle and to the C:\Users\\%User%\AppData\Local\Programs\Thonny, location. Open Thonny, and select at the bottom right the Pico Micropython interpreter, Thonny will then connect to the Pico as shown below:
<br>
<p align="left">
<img src="images/image1a.jpg" width="500" />  
<br>

Open the File menu and all the python files currently on the Pico is shown:
<br>
<p align="left">
<img src="images/image2a.jpg" width="500" />  
<br>


In this case small OLED (128x64 i2c SSD1306) test program [**ssd1306.py**](ssd1306.py), which is in part based on [**this Github**](https://github.com/blippy/rpi/tree/master/1306 ), and [**this discussion**](https://mcturra2000.wordpress.com/2021/01/25/raspberrypi-pico-working-oled/). 

For convenience the SSD1306 driver is written as a sub-class of the Micropython framebuffer object as discussed [**here**](http://docs.micropython.org/en/latest/library/framebuf.html#constructors).

The connection between the OLED and the Pico is: 

| OLED | GND | VCC | SCL | SDA |
|:-----|:----|:----|:----|:----|
| PICO | GND | 3V3 | GP5 | GP4 |

<br>
<p align="left">
<img src="images/image3a.jpg" width="500" />  
<br>
  
### 3. Installing the C/C++ SDK and Visual Studio Code for the Raspberry Pi Pico under Windows 8.1

This is based on the command and Visual Studio Code build methods as in [**Working with the Raspberry Pi Pico with Windows and C/C++**](https://www.element14.com/community/community/raspberry-pi/blog/2021/01/24/working-with-the-raspberry-pi-pico-with-windows), on Windows 8.1 (32 bit), and was completed in about two hours - details are as given below. 

<p align="left">
<img src="images/win81-1.png" width="300" />  
<img src="images/win81-2.jpg" width="500" /> 
<br>
  
<p align="left">
<img src="images/win81-3.png" width="240" />
<img src="images/win81-4.jpg" width="240" />  
<img src="images/win81-5.jpg" width="200" />   
<br>
