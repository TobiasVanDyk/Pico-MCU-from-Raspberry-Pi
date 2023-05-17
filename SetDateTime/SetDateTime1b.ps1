$a = "<T"
$b = ">"
$t = Get-Date
$c = $a+$t+$b
$port= new-Object System.IO.Ports.SerialPort COM6,9600,None,8,one
$port.open()
$port.Write($c)
$port.Close()


Get-Date > c:\Temp\DATE.TXT
$g = Get-Content c:\Temp\DATE.TXT
$c = $a+$g+$b
$port.open()
$port.Write($c)
$port.Close()
