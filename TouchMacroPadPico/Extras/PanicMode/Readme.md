## Panic mode reset. 

*If for any reason your keypad becomes unresponsive or behaves strangely reset it as follows:*

(1) If the Config button [Cfg] still works press it once and then press the [Del]ete key at the left-hand bottom. Then unplug or reset the keypad. This will delete all files. macros and settings and you should have a re-calibration st start-up.

(2) Press the white button at the bottom and hold it in, then press the black reset button at the back and release it (or alternatively unplug and re-plug the USB cable instead of the reset button), then only release the white button at the bottom. The file manager should show a new storage device named RPI-RP2.   Drag and drop any of the code.UF2 files to this device. It will restart after a second or two. If this still does not reset the keypad then instead of the code.UF2 file drag and drop the file flash_nuke.uf2, wait a few seconds and then drag the code.UF2 file to the device.

(3) Note that recent firmware versions are compiled with the early backlight-on commented out in setup(). This means that if flash_nuke.uf2 is used to wipe the flash memory the calibration arrows will not be visible. You can either press in the 4 corners (upper left, lower left, upper right, lower right) on the blank screen, or you can remove the comment for the BL switch-on as shown below, or use an older version of the firmware to calibrate, then load the new firmware over it. (The reason for this is because some PC USB ports supply power that keeps the backlight on even if the PC is switched off, when the commented line below is active.)
