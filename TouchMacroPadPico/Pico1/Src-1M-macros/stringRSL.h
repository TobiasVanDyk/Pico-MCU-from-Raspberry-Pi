//////////////////////////////////////////////
// stringRSL.h
//////////////////////////////////////////////
// Todo: Use Windows method for Linux and Pi
//       Timer2Str() DoPowerKeys() 
/////////////////////////////////////////////
char ShutDwn0[60];                                                     // Holds combined string 1 + 2 + char(TimerArr)
char ShutDwnWin1[36]   =  { "C:\\Windows\\System32\\shutdown.exe " };  // Windows string 1
char ShutDwnWin2[12][9] = { " /r /t ",  " /a",  " /s /t ", " ",        // Windows strings 2
                            " /r /t ",  "Cfg",  " /s /t ", " ",        // Same layout as 12 keys 0-11
                            " /r /t 0", " /l ", " /s /t 0"," "  };                           


char ShutDwn2[12][30] = { "sudo shutdown -r +10",             // PiOS/Linux USER = username
                          "sudo shutdown -c",                 // Use longer delays if some characters are missing
                          "sudo shutdown -P +10", "   ",      
                          "shutdown -r +1",  "Cfg",
                          "shutdown -P +1",  "   ",
                          "sudo shutdown -r now",
                          "sudo pkill -SIGKILL -u USER",
                          "sudo shutdown -P now", "   " };  

char ShutDwn1[60];                                               // Holds combined string 1 + 2 + char(TimerArr)
char ShutDwnLin1[30]     = { "echo PW | sudo -S " };             // Linux string 1 combine with strings below                       
char ShutDwnLin2[12][25] = { "shutdown -r +10",                  // Linux PW = password USER = username
                             "shutdown -c",                      // echo PW | sudo -S sudo or use echo PW | sudo -S
                             "shutdown -P +10", "   ",           // Use longer delays if some characters are missing
                             "shutdown -r +1",  "Cfg",
                             "shutdown -P +1",  "   ",
                             "shutdown -r now",
                             "pkill -SIGKILL -u USER",
                             "shutdown -P now", "   " };  
                          

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Alternative Linux/Debian/Ubuntu/Mint tested Restart/PowerOff/Logout/Cancel routines
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*  
6 Ways to Restart Linux from Terminal

Reboot Linux

1. Using the reboot Command
sudo reboot
To force an immediate reboot, run the reboot command with -f option: sudo reboot -f

2. Using the shutdown Command
sudo shutdown -r now
With the shutdown command, we can schedule a reboot after a specific time by specifying the minutes sudo shutdown -r +120

If you’ve scheduled a reboot but changed your mind, you can cancel it easily with the -c option: sudo shutdown -c

3. Using the systemctl Command
sudo systemctl reboot

4. Using the init Command
In older Linux distributions that use SysVinit, the system can be restarted using the following command: sudo init 6
When you run init 6, the system switches to runlevel 6, which means it prepares for a reboot. 
It stops all running processes, unmounts file systems, and then restarts the system. 
While this command still works on some systems, we recommend using the systemctl reboot command for newer Linux versions using systemd.

5. Using the telinit Command
The telinit 6 command restarts Linux by instructing init (or systemd on modern systems) to switch to runlevel 6, which triggers a reboot:
sudo telinit 6

This command ensures the system restarts in a controlled manner by following the proper shutdown process.

6. Using REISUB (Magic SysRq Key)

If your Linux system becomes unresponsive, you can safely restart it using the R, E, I, S, U, B key sequence
This method sends specific commands to the kernel using the SysRq (System Request) key. 
To restart your Linux system using this method, hold the keyboard keys Alt + SysRq (Print Screen) and press the following keys one by one in order:

    R: Puts the keyboard into raw mode, taking control away from applications.
    E: Sends the terminate signal to all processes.
    I: Sends the kill signal to all processes.
    S: Forces a disk sync, ensuring all data is written to disk.
    U: Remounts all filesystems as read-only, preventing corruption.
    B: Immediately reboots the system.

    Ctrl+Alt+End   = systemctrl poweroff
    Ctrl+Alt+Pause = systemctrl suspend
    Ctrl+Alt+Home  = systemctrl reboot   
   
   if (OptionOS[0]==1) {keycode[0] = AltL;   // Alt+F2 then gnome-terminal + Enter     
                     keycode[1] = F02;
                     keycode[2] = 0x00; 
                     usb_hid.keyboardReport(HIDKbrd, 0, keycode);   delay(keydelay);  
                     usb_hid.keyboardRelease(HIDKbrd);              delay(keydelay); 

                     char Term[] = "gnome-terminal\r";
                     for (n=0;  n<strlen(Term); n++) {usb_hid.keyboardPress(HIDKbrd, Term[n]); delay(keydelay);
                                                      usb_hid.keyboardRelease(HIDKbrd);        delay(keydelay); }
                         
                     for (n=0;  n<strlen(ShutDwn1[s]); n++) {usb_hid.keyboardPress(HIDKbrd, ShutDwn1[s][n]); delay(keydelay);
                                                             usb_hid.keyboardRelease(HIDKbrd);               delay(keydelay); } }
 
   if (OptionOS[0]==1) { keycode[0] = HID_KEY_T; 
                      tud_hid_keyboard_report(HIDKbrd, 5, keycode); delay(keydelay); // 5 => ModAltL + ModCtrL
                      tud_task();   // call before releasing the keys
                      keycode[0] = 0x00;
                      tud_hid_keyboard_report(HIDKbrd, 0x00, keycode);  
                      delay(keydelay3);   
                      for (n=0;  n<strlen(ShutDwn1[s]); n++) {usb_hid.keyboardPress(HIDKbrd, ShutDwn1[s][n]); delay(keydelay);
                                                              usb_hid.keyboardRelease(HIDKbrd);               delay(keydelay); } }                          
           
   if (OptionOS[0]==1) {keycode[0] = CtrL;               // Ctr+Alt+T open terminal 
                     keycode[1] = AltL;               // Works in Debian 11.7 without loosing focus to desktop search box
                     keycode[2] = HID_KEY_T;          // Ubuntu 22 and Mint 21 (sometimes) loose focus to desktop search box
                     keycode[3] = 0x00; 
                     usb_hid.keyboardReport(HIDKbrd, 0, keycode);   delay(keydelay);  
                     usb_hid.keyboardRelease(HIDKbrd);              delay(keydelay); 
                     delay(keydelay3);   
                     for (n=0;  n<strlen(ShutDwn1[s]); n++) {usb_hid.keyboardPress(HIDKbrd, ShutDwn1[s][n]); delay(keydelay);
                                                             usb_hid.keyboardRelease(HIDKbrd);               delay(keydelay); } } 
*/
////////////////////////////////////////////////////////////////////////////////////
// Alternative Raspberry Pi OS (LXDE) Restart Shutdown Logout via Run Box in Menu
////////////////////////////////////////////////////////////////////////////////////
/*
  if (OptionOS[0]==2) { //keycode[0] = AltL;             // PiOS: Open Run Box
                     //keycode[1] = F02; 
                     //keycode[2] = 0x00; 
                     //usb_hid.keyboardReport(HIDKbrd, 0, keycode);        delay(keydelay); 
                     //usb_hid.keyboardRelease(HIDKbrd);                   delay(keydelay);
                    
                     keycode[0] = GuiL;             // PiOS: Open Menu
                     keycode[1] = 0x00; 
                     usb_hid.keyboardReport(HIDKbrd, 0, keycode);        delay(keydelay); 
                     usb_hid.keyboardRelease(HIDKbrd);                   delay(keydelay); 
                     keycode[0] = ArrUp;            // Up to [Shutdown Restart Logout] options
                     keycode[1] = 0x00; 
                     usb_hid.keyboardReport(HIDKbrd, 0, keycode);        delay(keydelay); // GUI + 2 x Up Runbox
                     usb_hid.keyboardRelease(HIDKbrd);                   delay(keydelay); 
                     keycode[0] = ArrUp;            // Up to [Shutdown Restart Logout] options
                     keycode[1] = 0x00; 
                     usb_hid.keyboardReport(HIDKbrd, 0, keycode);        delay(keydelay); 
                     usb_hid.keyboardRelease(HIDKbrd);                   delay(keydelay); 
                     usb_hid.keyboardPress(HIDKbrd, '\r');               delay(keydelay); // Open Runbox with Enter 
                     usb_hid.keyboardRelease(HIDKbrd);                   delay(keydelay);  
                      
                     delay(keydelay3);   
                     for (n=0;  n<strlen(ShutDwn2[s]); n++) {usb_hid.keyboardPress(HIDKbrd, ShutDwn2[s][n]); delay(keydelay);
                                                             usb_hid.keyboardRelease(HIDKbrd);               delay(keydelay); } }                     
*/                                         
