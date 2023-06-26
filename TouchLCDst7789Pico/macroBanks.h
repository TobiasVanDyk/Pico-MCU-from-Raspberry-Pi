////////////////
// macroBanks.h
////////////////
#define cSt const static  // line space needed
// Save ActionM ActionS ActionT in here [0]*am*n [1*as*n [2]*at*n with n=1-5 1-3 sets of 24 macro actions or 4 Run or 5 Cmd
byte Bank123[3] = {1,2,5}; // Here M = 1 Windows shortcuts, S = 2 Pshop, T = 5 Cmd windows + command
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                       0    1    2    3    4    5     6    7    8    9    10   11    12   13   14   15   16   17    18   19   29   21   22   23
cSt byte PShop1[24]  = { CtrL,CtrL,CtrL,CtrL,CtrL,CtrL, CtrL,CtrL,CtrL,CtrL,CtrL,CtrL, CtrL,CtrL,CtrL,CtrL,CtrL,CtrL, CtrL,CtrL,CtrL,CtrL,CtrL,CtrL };  
cSt byte PShop2[24]  = { 0x2E,0x2D,0x27,ShfL,ShfL,0x1A, AltL,ShfL,ShfL,ShfL,ShfL,AltL, 0x11,0x1A,0x17,0x4A,0x07,ShfL, AltL,0x56,AltL,AltL,AltL,AltL }; 
cSt byte PShop3[24]  = { 0x00,0x00,0x00,0x11,0x16,0x00, ShfL,0x11,0x08,0x07,0x0C,0x04, 0x00,0x00,0x00,0x00,0x00,0x16, 0x56,0x00,0x15,0x17,0x37,0x06 }; 
//                       Notepad Shortcuts              Photoshop Shortcuts            Firefox Shortcuts              MSWord Special Characters
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                        0    1    2    3    4    5     6    7    8    9    10   11    12   13   14   15   16   17    18   19   29   21   22   23
cSt byte Windows1[24] = { CtrL,GuiL,GuiL,GuiL,GuiL,GuiL, GuiL,GuiL,GuiL,AltL,ShfL,CtrL, CtrL,AltL,GuiL,GuiL, GuiL,GuiL,GuiL,GuiL,GuiL,GuiL,GuiL,GuiL };  
cSt byte Windows2[24] = { ShfL,KeyX,KeyR,KPse,Key1,Key2, Key3,Tab, CtrL,PScr,F10, AltL, ShfL,KEnt,CtrL,Key9, Key6,Key7,Key8,Key0,KEqu,F01, Key4,Key5 }; 
cSt byte Windows3[24] = { Esc, 0x00,0x00,0x00,0x00,0x00, 0x16,0x00,ShfL,0x00,0x00,DelK, KeyN,0x00,KeyO,0x00, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 }; 
cSt byte Windows4[24] = { 0x00,0x00,0x00,0x00,0x00,0x00, 0x00,0x00,KeyB,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 }; 
//  Windows Shortcuts     TskMan StrtRC Run PCInfo Calc PwerSh FMan TskView WakePC APrtScr MseRClk CtrAltDel NewFldr Prop OSK GUI9 6 7 8 0 = F1 4 5      
//                        GUI 4 Restart 5 Shutdown 6 Edge 7 Outlook 8 Word 9 Firefox 0 Minimize = Magnifier F1 Help 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Control panel tool Command: Accessibility Options control access.cpl Add New Hardware control sysdm.cpl add new hardware Add/Remove Programs control appwiz.cpl 
// Date/Time Properties control timedate.cpl Display Properties control desk.cpl FindFast control findfast.cpl Fonts Folder control fonts Internet control inetcpl.cpl
// Joystick Properties control joy.cpl Keyboard Properties control main.cpl keyboard Messaging control mlcfg32.cpl Post Office control wgpocpl.cpl 
// Modem Properties control modem.cpl Mouse Properties control main.cpl Multimedia Properties control mmsys.cpl Network Properties control netcpl.cpl 
// NOTE: In Windows NT 4.0, Network properties is Ncpa.cpl, not Netcpl.cpl Password Properties control password.cpl PC Card control main.cpl
// Power control powercfg.cpl Printers Folder control printers Regional Settings control intl.cpl Scanners and Cameras control sticpl.cpl 
// Sound Properties control mmsys.cpl sounds System Properties control sysdm.cpl
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
cSt char RunCode[24][60] = 
{"netplwiz","cmd","winver","msconfig","sndvol","msinfo32",
 "lusrmgr.msc","dxdiag","powershell","sndvol","SystemPropertiesPerformance.exe","rundll32.exe keymgr.dll KRShowKeyMgr",
 "msinfo32","regedit","control userpasswords2","mmc","control printers","printui.exe /s",
 "control panel","rundll32.exe shell32.dll,Control_RunDLL appwiz.cpl","control Ncpa.cpl","control inetcpl.cpl users","control main.cpl","control main.cpl keyboard" };
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 // Other CMD (not Powershell): assoc netstat -an powercfg /a  tasklist schtasks systeminfo  nslookup getmac gpresult /r  whoami  set  route print  
 // diskpart  net user  arp  hostname  rsop   w32tm /query /computer: /configuration    ver  -  Use & or && to chain commands
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
cSt char CmdCode[24][50] = 
{"Get-Process","Get-Service","Get-History","Get-ExecutionPolicy","Get-Command","Get-Help",
 "ipconfig /all","ipconfig /Displaydns","ipconfig /flushdns","ipconfig /registerdns","ipconfig /release","ipconfig /renew",
 "start-process PowerShell -verb runas","Get-Location","Get-PSDrive","Get-EventLog -List","Get-EventLog -LogName System -EntryType Error","Clear-History",
 "Clear-History -Command *help*, *command","Get-WinEvent -ListLog *","Get-WinEvent -LogName System","Get-Clipboard","Get-Date","Get-Random" };
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                           
static const int BSDMax = 23;     // Number of choices for actions
// #define BSDMax 24;             // Number of choices for actions
int BsDNum = 0 ;                  // Index into BsDLabel[0-3]
char BsDNumFile[2] = "0";         // char [0] = 0-8 on file = BsDNum ASCII - 48
//                         0     1     2     3     4     5     6     7     8     9     10    11    12    13    14    15    16    17    18    19    20    21    22    23
cSt char BsDLabel[24][4]={"Del","Bks","Tab","aTb","Ins","Esc","PrS","aPr","Ret","Snp", "Osk","Num","Cap","Scr","Cut","Cpy","Pst","Tsk","Run","wX","Pws","FFx","Edg","Mnm"};   
cSt byte BsDCode1[24]   ={ DelK, BckS, Tab,  AltL, Ins,  Esc,  PScr, AltL, KEnt, GuiL, GuiL, 0x53, 0x39, 0x47, CtrL, CtrL, CtrL, CtrL, GuiL, GuiL, GuiL, GuiL, GuiL, GuiL};  
cSt byte BsDCode2[24]   ={ 0x00, 0x00, 0x00, Tab,  0x00, 0x00, 0x00, PScr, 0x00, ShfL, CtrL, 0x00, 0x00, 0x00, KeyX, KeyC, KeyV, ShfL, KeyR, KeyX, Key2, Key9, Key6, Key0}; 
cSt byte BsDCode3[24]   ={ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, KeyS, KeyO, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, Esc,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
