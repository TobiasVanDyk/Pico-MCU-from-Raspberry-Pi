////////////////
// stringRSL.h
////////////////
char ShutDwn2[12][60] = { "sudo shutdown -r +10",             // PiOS/Linux PW = password USER = username
                          "sudo shutdown -c",                 // Use longer delays if some characters are missing
                          "sudo shutdown -P +10", "   ",      
                          "shutdown -r +1",  "Cfg",
                          "shutdown -P +1",  "   ",
                          "sudo shutdown -r now",
                          "sudo sudo pkill -SIGKILL -u USER",
                          "sudo shutdown -P now", "   " };  
                         
char ShutDwn1[12][60] = { "echo PW | sudo -S shutdown -r +10",             // Linux PW = password USER = username
                          "echo PW | sudo -S shutdown -c",                 // echo PW | sudo -S sudo or use echo PW | sudo -S
                          "echo PW | sudo -S shutdown -P +10", "   ",      // Use longer delays if some characters are missing
                          "echo PW | sudo -S shutdown -r +1",  "Cfg",
                          "echo PW | sudo -S shutdown -P +1",  "   ",
                          "echo PW | sudo -S shutdown -r now",
                          "echo PW | sudo -S sudo pkill -SIGKILL -u USER",
                          "echo PW | sudo -S shutdown -P now", "   " };  

char ShutDwn0[12][60] = { "C:\\Windows\\System32\\shutdown.exe /r /t 6000",          // Windows
                          "C:\\Windows\\System32\\shutdown.exe /a",
                          "C:\\Windows\\System32\\shutdown.exe /s /t 6000", "   ",
                          "C:\\Windows\\System32\\shutdown.exe /r /t 30",   "Cfg",
                          "C:\\Windows\\System32\\shutdown.exe /s /t 30",   "   ",
                          "C:\\Windows\\System32\\shutdown.exe /r /t 0",
                          "C:\\Windows\\System32\\shutdown.exe /l",
                          "C:\\Windows\\System32\\shutdown.exe /s /t 0",    "   " };  // = shutdown.exe /p       

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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Alternative Linux/Debian/Ubuntu/Mint tested Restart/PoerOff/Logout/Cancel routines
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
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
