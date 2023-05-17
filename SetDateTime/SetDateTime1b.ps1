Get-Date > c:\Temp\DATE.TXT
$g = Get-Content c:\Temp\DATE.TXT
$c = $a+$g+$b
$port.open()
$port.Write($c)
$port.Close()
