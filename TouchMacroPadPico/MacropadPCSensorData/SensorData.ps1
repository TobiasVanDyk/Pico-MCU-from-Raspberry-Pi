reg query HKEY_CURRENT_USER\SOFTWARE\HWiNFO64\VSB > c:\Temp\HWINFO.TXT
$file_data = Get-Content c:\Temp\HWINFO.TXT
$a = "<"
$b = ">"
$d = $a+$file_data[5]+$b
$e = $a+$file_data[20]+$b
$d = ($d).Replace("    ValueRaw","s")
$e = ($e).Replace("    ValueRaw","s")
$port= new-Object System.IO.Ports.SerialPort COM6,9600,None,8,one
$port.open()
$port.Write($d)
$port.Write($e)
$port.Close()