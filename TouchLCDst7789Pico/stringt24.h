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
char ttr10[] = "sudo fttrim / --verbose"; 
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

