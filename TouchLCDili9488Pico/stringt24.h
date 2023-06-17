////////////////
// stringt24.h
////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Mainly often used Raspberry Pi commands
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
char ttr1[] = "sudo apt update";
char ttr2[] = "sudo apt full-upgrade -y";
char ttr3[] = "sudo apt autoremove";                            
char ttr4[] = "sudo apt autoclean";
char ttr5[] = "sudo apt clean";  
char ttr6[] = "sudo nano /boot/config.txt"; 
char ttr7[] = "sudo nano /boot/cmdline.txt";                             
char ttr8[] = "sudo nano /etc/fstab";  
char ttr9[] = "sudo hdparm -t /dev/sda1";
char ttr10[] = "sudo fstrim / --verbose"; 
char ttr11[] = "sudo dd if=/dev/mmcblk0p1 bs=1M | gzip -c  > /mnt/data/Backup/bootsd1.gz";                             
char ttr12[] = "sudo chown pi:pi /mnt/data"; 
char ttr13[] = "sudo apt install pavucontrol";
char ttr14[] = "sudo apt-get install samba samba-common-bin";
char ttr15[] = "sudo nano /etc/samba/smb.conf";                            
char ttr16[] = "sudo smbpasswd -a pi";
char ttr17[] = "sudo mount -o username=,password= //PC11400/vmshare /mnt/share";  
char ttr18[] = "sudo mount /dev/sdc1 /mnt/Toshiba2TB"; 
char ttr19[] = "sudo mke2fs -t ext4 -L rootfs /dev/sda1";                             
char ttr20[] = "sudo mount /dev/sda1 /mnt/data1";  
char ttr21[] = "sudo rsync -axv / /mnt/data1";
char ttr22[] = "sudo nano /mnt/data1/etc/fstab"; 
char ttr23[] = "sudo umount /dev/sda1";                             
char ttr24[] = "uname -a"; 


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ctl + Alt + Backspace Logs you out of X back to the login screen - works with all desktop environments and window managers.
// Ctrl + Alt + Delete   Start the reboot process - works in all desktop environments and all window managers
// Ctrl + Alt + T is the shortcut to open terminal in Ubuntu.
// Alt + F2: Run console quick command, instead of opening a terminal and running the command there, you can use Alt+F2 to run the console.
// Alt + F1 (= Super key) Switch between the Activities overview and desktop
// Super + Tab Switch between windows. Hold down Shift for reverse order.
// Super +`~ Switch between windows from the same application
// Alt + Esc Switch between windows in the current workspace. Hold down Shift for reverse order.
// Ctrl + Alt + Tab Give keyboard focus to the top bar. 
// Super + A Show the list of applications.
// Super + PageUp/PageDown Switch between workspaces.
// Shift + Super + PageUp/PageDown Move the current window to a different workspace.
// Shift + Super + ? Move the current window one monitor to the left.
// Super + L Lock the screen.
// Super + V Show the notification list - Esc to close.
// Ironically Linux choose to name a key with an apple/road-sign or a window on it, "Super" - https://en.wikipedia.org/wiki/Command_key
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
