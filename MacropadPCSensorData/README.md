# PC Sensor Data for the Touch Pad

It is relatively easy to display sensor data from [**HWInfo**](https://www.hwinfo.com/) on the statusbar of the Touch Macropad using [**HWInfo's gadget registry data**](https://github.com/Timthreetwelve/HWiNFO-VSB-Viewer) sent to the macropad via a [**powershell script**](https://github.com/TobiasVanDyk/Pico-MCU-from-Raspberry-Pi/blob/main/MacropadPCSensorData/PowerShellSession.txt) - note that the sensor data is accessed directly and not using the VSB Viewer. To configure HWInfo refer to the 4 pictures below

The powershell script can be configured to run via the Windows Task Scheduler on a regular basis as [**explained here**](https://www.partitionwizard.com/partitionmanager/automate-powershell-scripts.html). 

The script used is listed below - undoubtedly it can be much improved. Currently only two sensors are included in the script - but it can be easily expanded.

<p align="left">
<img src="images/PowerShellSession.jpg" height="180" /> 
</p>

``` 
reg query HKEY_CURRENT_USER\SOFTWARE\HWiNFO64\VSB > c:\Temp\HWINFO.TXT
$file_data = Get-Content c:\Temp\HWINFO.TXT
$a = "<"
$b = ">"
$d = $a+$file_data[5]+$b
$e = $a+$file_data[20]+$b
$d = ($d).Replace("    ValueRaw","s")
$e = ($e).Replace("    ValueRaw","s")
$d
<s0    REG_SZ    35>
$e
<s3    REG_SZ    975>
$port= new-Object System.IO.Ports.SerialPort COM6,9600,None,8,one
$port.open()
$port.Write($d)
$port.Write($e)
$port.Close()
``` 

<p align="left">
<img src="images/HWInfo0.jpg" height="120" /> 
<img src="images/HWInfo1.jpg" height="120" /> 
<img src="images/HWInfo2.jpg" height="120" /> 
<img src="images/HWInfo3.jpg" height="120" /> 
</p>

Only the code for the Raspberry Pi display adapted for the Pico have been updated for displaying the sensor data.

Only three sets of changes need to be made:

Add the following global variables:
``` 
bool sSens = false;                    // PC sensor value sent from HWInfo
char sSArr[2][21]  = { "CPU    C Fan     rpm", "Sys    C Fan     rpm" }; 
``` 

Add the following function:
``` 
/////////////////////////////////////////////////////////////////////////////
void WriteSensorData()                   // PC sensor value sent from HWInfo
/////////////////////////////////////////////////////////////////////////////
// CPU Data <s0    REG_SZ    36>
//           012345678901234567
// CPU  Fan <s3    REG_SZ    978 >
//           01234567890123456789
///////////////////////////////////////////////////////////////////////////// 
{ // char sSArr[2][21]  = { "CPU    C Fan     rpm", "Sys    C Fan     rpm" };
  //                         01234567890123456789    01234567890123456789
  //                   Option    0        3
  int i;                 
  byte Option = RecBytes[1] - 48;
  
  if (Option==0) { for (i=4;  i<6;  i++) sSArr[0][i] = RecBytes[i+12]; }
  if (Option==3) { for (i=13; i<16; i++) sSArr[0][i] = RecBytes[i+3];  
                   if (NumBytes>20) sSArr[0][16] = RecBytes[19];       }
                   
  status(sSArr[0]);  
  sSens = false;
}
``` 

Add the following two lines in function DoNewData():
``` 
  sSens = (a==67);       // 0x73 = 's' PC sensor value from HWInfo
  if (sSens) { WriteSensorData(); return; }
``` 

