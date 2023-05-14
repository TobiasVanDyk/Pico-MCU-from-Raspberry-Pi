$file_data = Get-Content c:\Temp\FOOBAR.TXT
$a = "<m"
$b = ">"
$d = $a+$file_data+$b
$port= new-Object System.IO.Ports.SerialPort COM6,9600,None,8,one
$port.open()
$port.Write($d)
$port.Close()