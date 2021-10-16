# Pico MCU Audio USB DAC

Instructions for building a new USB-Audio device using the Waveshare Pico Audio Hat with a PCM101A DAC

https://www.waveshare.com/wiki/Pico-Audio
https://www.waveshare.com/pico-audio.htm


### 1 Using Raspberry Pi Os (32 bit)

Note that this use the old Raspberry Pi USB development files - a next step will be to use the tinyusb examples to build the Pico USB-Audio device as in https://github.com/hathach/tinyusb

sudo apt update
sudo apt install cmake gcc-arm-none-eabi build-essential
sudo reboot

1. Start with following installed directories under /home/pi/pico
pico-playground
pico-examples
pico-extras
pico-sdk
Pico_Audio

1.1  
sudo apt update
cd ~
mkdir pico
cd pico
git clone -b master https://github.com/raspberrypi/pico-sdk.git
export PICO_SDK_PATH=/home/pi/pico/pico-sdk
cd pico-sdk
git submodule update --init
cd ..
git clone -b master https://github.com/raspberrypi/pico-examples.git
git clone -b master https://github.com/raspberrypi/pico-extras.git
git clone -b master https://github.com/raspberrypi/pico-playground.git

1.2 Check that tinyUSB is installed under pico-sdk after the submodule update.

2. Install and build Waveshare code:
(Note pre-built usb_sound_card.uf2 already included inWaveshare code which can be dragged to Pico when in MSD (mass storage mode) - will yield a functoning 44.1kHz/48kHz 16 bit USB-Audio device.)
2.1
sudo apt-get install p7zip-full
cd ~
sudo wget  https://www.waveshare.com/w/upload/7/7c/Pico_Audio.7z 
7z x Pico_Audio.7z -o./Pico_Audio
Move Pico_Audio to /home/pi/pico - there should be two folder under /home/pi/pico/Pico_Audio/Pico-Audio and /home/pi/pico/Pico_Audio/Pico-Audio/usb_sound_card
cd ~/pico/Pico_Audio/Pico-Audio
cd build
export PICO_SDK_PATH=../../../pico-sdk
cmake ..
make -j4
This will build a sine wave i2s text program when its audio_firmware.uf2 is dragged to Pico - there is also a pre-built uf2 included for this

3. Build a new usb_sound_card.uf2
3.1 Edit pico-extras/src/rp2_common/pico_audio_i2s/include/pico/audio_i2s.h 
Change: 

#ifndef PICO_AUDIO_I2S_DATA_PIN
//#warning PICO_AUDIO_I2S_DATA_PIN should be defined when using AUDIO_I2S
// OLD VALUE #define PICO_AUDIO_I2S_DATA_PIN 28
#define PICO_AUDIO_I2S_DATA_PIN 26
#endif

#ifndef PICO_AUDIO_I2S_CLOCK_PIN_BASE
//#warning PICO_AUDIO_I2S_CLOCK_PIN_BASE should be defined when using AUDIO_I2S
// OLD VALUE #define PICO_AUDIO_I2S_CLOCK_PIN_BASE 26
#define PICO_AUDIO_I2S_CLOCK_PIN_BASE 27
#endif

See the Waveshare example code audio_data.h for the sine wave example and the Pico Audio Waveshare Wiki.
Without this change the USB-device will appear to be functional but sound will be produced.
//DIN 	GPIO26 	Audio data input
//BCk 	GPIO27 	Audio data bit clock input
//LRCK 	GPIO28 	Audio data word clock input 

#define PICO_AUDIO_DATA_PIN 26
#define PICO_AUDIO_CLOCK_PIN_BASE 27

3.2 Build the usb_sound_card
cd ~/pico/pico-playground
mkdir ./build
cd build
export PICO_SDK_PATH=../../../pico-sdk
cmake ..
cd /home/pi/pico/pico-playground/apps/usb_sound_card/
make -j4

Look for the uf2 file under the build folder pico/pico-playground/build/app/usb_sound_card. Rename the new usb_sound_card.uf2 to for example usb_sound_card2.uf2, then drag it to the Pico when in MSD mode

There should then be a functional 44.1kHz/48kHz 16 bit USB-Audio device

### 2 Using Linux Mint 1.93 (32 bit) Virtual Machine with a Windows Host

Based in part on:
https://circuitdigest.com/microcontroller-projects/how-to-program-raspberry-pi-pico-using-c
https://graspingtech.com/upgrade-cmake/

sudo apt update
sudo apt install git cmake gcc-arm-none-eabi gcc g++ libstdc++-arm-none-eabi-newlib
sudo apt install automake autoconf build-essential texinfo libtool libftdi-dev libusb-1.0-0-dev

Note this has cmake 3.10 installed (cmake --version) i.e. must compile a new version > 3.12 required) from source:

wget https://github.com/Kitware/CMake/releases/download/v3.21.3/cmake-3.21.3.tar.gz
tar -zxvf cmake-3.21.3.tar.gz
cd cmake-3.21.3
./bootstrap
make 
sudo make install 

mkdir ~/pico
cd pico
git clone -b master https://github.com/raspberrypi/pico-sdk.git
cd pico-sdk
git submodule update --init
cd ..
git clone -b master https://github.com/raspberrypi/pico-examples.git
git clone -b master https://github.com/raspberrypi/pico-extras.git
cd pico-extras
git submodule update --init
cd ..
git clone -b master https://github.com/raspberrypi/pico-playground.git

Then follow the instructions as in 3.1 and 3.2 above

As alternative VM's which  do not require a cmake build, use Linux Mint 20.2 x64 has cmake 3.16 installed, or use
Debian 11.1 x86 has cmake 3.18 installed
 

### 3 Using Windows 8.1 x86

1. Install Windows PowerShell 5.1 (via the Windows Management Framework 5.1) from https://www.microsoft.com/en-us/download/details.aspx?id=54616
2. Download and run pico-setup-windows-0.3.3-x86.exe (or newer), from https://github.com/ndabas/pico-setup-windows/releases
I changed the install directory to C:\Pico. This will downlod about 1.8GB for Visual Studio BuildTools.
3. Run pico-setup.cmd as admin - this will install all the git folders, and compile all the pico-examples - look in the pico-examples/build subfolders for all the uf2 files.
4. Run pico-env.cmd as admin - the output is:
C:\Pico>pico-env.cmd
PICO_sdk_PATH=C:\Pico\pico-sdk
PICO_examples_PATH=C:\Pico\pico-examples
PICO_extras_PATH=C:\Pico\pico-extras
PICO_playground_PATH=C:\Pico\pico-playground
OPENOCD_SCRIPTS=C:\Pico\tools\openocd-picoprobe\scripts

5. Build the usb_sound_card
Remeber to edit the 3.1 pico-extras\src\rp2_common\pico_audio_i2s\include\pico\audio_i2s.h file
#define PICO_AUDIO_I2S_DATA_PIN 26
#define PICO_AUDIO_I2S_CLOCK_PIN_BASE 27

Right-click on the Developer Command Prompt for Pico shortcut in the Pico folder and run as admin.
cd pico-playground
mkdir build
cd build
cmake -G "NMake Makefiles" ..
cd apps\usb_sound_card
nmake

Look for the uf2 file under the build folder Pico\pico-playground\build\app\usb_sound_card. Rename the new usb_sound_card.uf2 to for example usb_sound_card2.uf2, then drag it to the Pico when in MSD mode

There should then be a functional 44.1kHz/48kHz 16 bit USB-Audio device


