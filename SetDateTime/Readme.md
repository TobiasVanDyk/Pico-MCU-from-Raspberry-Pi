# Display Date and Time on the Touch Macropad

Note that this is an alternative Date Time - it is only displayed and not set as the sytem time-date.
This uses <T  > and the system time date uses <t  >.

The powershell script can be configured to run via the Windows Task Scheduler on a regular basis as [**explained here**](https://www.partitionwizard.com/partitionmanager/automate-powershell-scripts.html). 

The script used to [**display**] date-time only is the first picture below - two date time formats are shown.
The script used [**set**] the system date-time is the second picture below. The two scripts are also listed below.

<p align="left">
<img src="images/SetDateTime1.png" height="180" /> 
<img src="images/SetDateTime2.png" height="180" /> 
</p>

``` 
PS C:\Users\Tobias> $a = "<T"
PS C:\Users\Tobias> $b = ">"
PS C:\Users\Tobias> $t = Get-Date
PS C:\Users\Tobias> $t

Wednesday, May 17, 2023 10:57:31 AM

PS C:\Users\Name> $c = $a+$t+$b
PS C:\Users\Name> $c
<T05/17/2023 10:57:31>
PS C:\Users\Name> $port= new-Object System.IO.Ports.SerialPort COM6,9600,None,8,one
PS C:\Users\Name> $port.open()
PS C:\Users\Name> $port.Write($c)
PS C:\Users\Name> $port.Close()
PS C:\Users\Name> $t

Wednesday, May 17, 2023 10:57:31 AM

PS C:\Users\Name> Get-Date > c:\Temp\DATE.TXT
PS C:\Users\Name> $g = Get-Content c:\Temp\DATE.TXT
PS C:\Users\Name> $g

Wednesday, May 17, 2023 11:03:19 AM

PS C:\Users\Name> $c = $a+$g+$b
PS C:\Users\Name> $c
<T Wednesday, May 17, 2023 11:03:19 AM  >
PS C:\Users\Name> $port.open()
PS C:\Users\Name> $port.Write($c)
PS C:\Users\Name> $port.Close()
PS C:\Users\Name>
``` 

``` 
PS C:\Users\Tobias> $a = "<t"
PS C:\Users\Tobias> $b = ">"
PS C:\Users\Tobias> $t = Get-Date -UFormat "%y%m%d%w%H%M"
PS C:\Users\Tobias> $t
23051731020
PS C:\Users\Name> $c = $a+$t+$b
PS C:\Users\Name> $c
<t23051731020>
PS C:\Users\Name> $port= new-Object System.IO.Ports.SerialPort COM6,9600,None,8,one
PS C:\Users\Name> $port.open()
PS C:\Users\Name> $port.Write($c)
PS C:\Users\Name> $port.Close()
PS C:\Users\Name>
``` 

Only three sets of changes were made:

Add the following global variables:
``` 
bool tTimeDate = false;                    // Show Date and Time
static const int tTimeDateSize = 48;       // Must check this Tuesday, May 16, 2023 11:27:51 AM
char tTimeDateArr[mPlaySize]  = { " " };      
``` 

Add the following function:
``` 
//////////////////////////////////////////////////////////////////////////////////////////////////////
void WriteDateTime()              // Alternative Date Time - only displayed not set a sytem time-date
//////////////////////////////////////////////////////////////////////////////////////////////////////
// Date Time <TTuesday, May 16, 2023 11:27:51 AM>   max = 48 chararacters 
///////////////////////////////////////////////////////////////////////////// 
{ int i; 
  if (NumBytes>tTimeDateSize) { status("Date Time Data too long..."); return; }               
  for (i=0;  i<NumBytes;  i++) tTimeDateArr[i] = RecBytes[i+1];    // Skip "T"               
  status(tTimeDateArr); 
  tTimeDate = false; 
}
``` 

Add the following two lines in function DoNewData():
``` 
  tTimeDate = (a==36);       // 0x54 = 'T' Time Date Display (not system time-date)
  if (tTimeDate) { WriteDateTime();         tTimeDate = false; return; }
``` 

[**RTC-1.ino**](RTC-1.ino) is the simplest example (based on the Pico SDK), of how to set and access the HW-RTC on the RP2040 using the Pico Arduino library.
It can be tested on a bare RPi Pico i.e. connected only to a PC via USB, and the output monitored via a serial monitor.
