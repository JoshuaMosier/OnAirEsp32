run, C:\Users\*Your User Directory*\AppData\Roaming\Zoom\bin\Zoom.exe, C:\Users\*Your User Directory*\AppData\Roaming\Zoom\bin
Run cmd.exe /c curl Enter Your ESP32 IP**/H,, hide
SendMode Input

loop {
sleep 10000
Process, Exist, Zoom.exe;
If (ErrorLevel = 0)
  {
  Run cmd.exe /c curl *Enter Your ESP32 IP*/L,, hide
  ExitApp
  }
Else
  {
  sleep 5
  }
}
