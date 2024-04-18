$a = "<T"
$b = ">"
$t = Get-Date
$c = $a+$t+$b
$port= new-Object System.IO.Ports.SerialPort COM6,9600,None,8,one
$port.open()
$port.Write($c)
$port.Close()


