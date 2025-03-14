////////////////
// macroBanks.h
////////////////
#define cSt const static  // line space needed
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Instruction List Simple: (Only execute 0-9,a-d exit if 0, can be in any order or length (set to 12 currently) such as 6 1 3 0 )
// 1=SDCardTextFiles 2=Flash+M-L 3=Flash+L-M 4=SDCard+M-L 5=SDCard+L-M 6=Do1 7=Do2 8=Do3 9=Bank123 0=Exit 
// a=Flash+M b=Flash+L c=SDCard+M d=SDCard+L Note: a=0x62=97->after -48 a b c d = 49 50 51 52
// For example 1 2 5 9 0   is the same as Layout=3 below
//             2 5 9 0     is the same as Layout=4 below 
//             2 5 6 7 9 0 is the same as Layout=1 below  
cSt int iListMax = 12;                               // Number of entries
byte MacroInstructionList[3][iListMax] = 
{ 2, 5, 6, 7, 9, 0, 0, 0, 0, 0, 0, 0,                // Layout 1 = M
  1, 2, 5, 9, 0, 0, 0, 0, 0, 0, 0, 0,                // Layout 3 = S
  2, 5, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0 } ;             // Layout 4 = T
cSt byte MacroInstructionListDefault[3][iListMax] =  // Defaults to reset MacroInstructionList
{ 2, 5, 6, 7, 9, 0, 0, 0, 0, 0, 0, 0,    1, 2, 5, 9, 0, 0, 0, 0, 0, 0, 0, 0,     2, 5, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0 } ;  
bool iList = false;     // Select between new instructionlist or old coded section - use *ix* to switch new list-based on
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Save ActionM ActionS ActionT in here [0]*am*n [1]*as*n [2]*at*n with n=1-5 1-3 sets of 24 macro actions or 4 Run or 5 Cmd
byte Bank123[3] = {1,2,5}; // DoMacroButtons() set this to M=0 S=1 T=2 Here M = 1 Windows shortcuts, S = 2 Pshop, T = 5 Cmd windows + commandA
int Bank123Color[5] = {Orange,Green,Blue,Yellow,White };  // Indicator for MacroBank Selected in Layouts 1 3 4
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                            0    1    2    3    4    5     6    7    8    9    10   11    12   13   14   15   16   17    18   19   29   21   22   23
cSt byte WinMacro[4][24] = {{ CtrL,GuiL,GuiL,GuiL,GuiL,GuiL, GuiL,GuiL,GuiL,AltL,ShfL,CtrL, CtrL,AltL,GuiL,GuiL, GuiL,GuiL,GuiL,GuiL,GuiL,GuiL,GuiL,GuiL },  
                            { ShfL,KeyX,KeyR,KPse,Key1,Key2, Key3,Tab, CtrL,PScr,F10, AltL, ShfL,KEnt,CtrL,Key9, Key6,Key7,Key8,Key0,KEqu,F01, Key4,Key5 }, 
                            { Esc, 0x00,0x00,0x00,0x00,0x00, 0x16,0x00,ShfL,0x00,0x00,DelK, KeyN,0x00,KeyO,0x00, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 }, 
                            { 0x00,0x00,0x00,0x00,0x00,0x00, 0x00,0x00,KeyB,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 }}; 
//  Windows Shortcuts     TskMan StrtRC Run PCInfo Calc PwerSh FMan TskView WakePC APrtScr MseRClk CtrAltDel NewFldr Prop OSK GUI9 6 7 8 0 = F1 4 5      
//                        GUI 4 Restart 5 Shutdown 6 Edge 7 Outlook 8 Word 9 Firefox 0 Minimize = Magnifier F1 Help 
//                        GUI+Number: Open the Item Number on the Taskbar - if 1st item is the Calculator it will open this
//                        GUI+V: Open Windows Clipboard (if enabled)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                              0    1    2    3    4    5     6    7    8    9    10   11    12   13   14   15   16   17    18   19   29   21   22   23
cSt byte PShopMacro[3][24] = {{ CtrL,CtrL,CtrL,CtrL,CtrL,CtrL, CtrL,CtrL,CtrL,CtrL,CtrL,CtrL, CtrL,CtrL,CtrL,CtrL,CtrL,CtrL, CtrL,CtrL,CtrL,CtrL,CtrL,CtrL },  
                              { 0x2E,0x2D,0x27,ShfL,ShfL,0x1A, AltL,ShfL,ShfL,ShfL,ShfL,AltL, 0x11,0x1A,0x17,0x4A,0x07,ShfL, AltL,0x56,AltL,AltL,AltL,AltL }, 
                              { 0x00,0x00,0x00,0x11,0x16,0x00, ShfL,0x11,0x08,0x07,0x0C,0x04, 0x00,0x00,0x00,0x00,0x00,0x16, 0x56,0x00,0x15,0x17,0x37,0x06 }}; 
//                              Notepad Shortcuts              Photoshop Shortcuts            Firefox Shortcuts              MSWord Special Characters
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
// Use same BSDCode1 - BSDCode3 XNum[0,1,2] x1,x2,x3 for Layout1, XNum[3,4,5] x,4,x5,x6 for Layouts 3, and 4 -> use *x1*1-BSDMax to *x6*1-BSDMax
// XKeys active replacing Cut Copy Paste in Layouts M S T 
static const char XLabel [6][4] = { "x 1", "x 2", "x 3", "x 4", "x 5", "x 6" };  // Maps 3x to a72-a90 - not yet programmed to use replace x1-x6 options
byte XNum[6] = { 14,15,16,14,15,16 };                                            // Index into BSDCode1 - BSDCode3 XNum[6] = { 14,15,16,14,15,16 }; 
char XFileNum[2] = "0";                                                          // char [0] = 0-5 on file = XNumMST ASCII - 48  
bool XFiles = false;                                                             // static const bool XFiles = true;   
bool SaveX1X6 = false;                                                           // Save changes top 3 keys only once  
bool Key1Key6 = false;                                                           // Top 3 keys + Del + Ret when = K1-K24 programmable 
byte BSDKeyKArr[72];                                                             // Store Keys K1-K24 BSD1-BSD3 defines                       
static const int BSDMax = 54;     // Number of choices for actions
int BsDNum = 0 ;                  // Index into BsDLabel[0-3] used for 4th key top row default [Del]ete
int RetNum = 8 ;                  // Index into BsDLabel[0-3] used for 4th key bottom row default [Ret]urn
// Keys K1-K6 Filenames on SDCard, make with *sd*K then send <1...> from Layout 3 Layer orange A see SDCardSelectFiles
// Check if Key1 to Key24 have been hard-coded i.e not 0x00 as here, else do files named BsDLabel[Num] = "K01Link" to "K24Link"
//                             0     1     2     3     4     5     6     7     8     9     10    11    12    13    14    15    16    17    18    19    20    
//                             21    22    23    24    25    26    27    28    29    30    31    32    33    34    35    36    37    38    39    40    41
//                             42    43    44    45    46    47    48    49    50
cSt char BsDLabel[BSDMax][4]={"Del","Bks","Tab","aTb","Ins","Esc","PrS","aPr","Ret","Snp","Osk","UnD","ReD","caD","Cut","Cpy","Pst","Tsk","Run","wX", "CPi",
                              "Ts1","Ts2","Ts3","Ts4","Ts5","Ts6","K1", "K2", "K3", "K4", "K5", "K6", "K7", "K8", "K9", "K10","K11","K12","K13","K14","K15",  
                              "K16","K17","K18","K19","K20","K21","K22","K23","K24","Num","Cap","Scr" };  
byte BsDCode1[BSDMax] =     { DelK, BckS, Tab,  AltL, Ins,  Esc,  PScr, AltL, KEnt, GuiL, GuiL, CtrL, CtrL, CtrL, CtrL, CtrL, CtrL, CtrL, GuiL, GuiL, GuiL, 
                              GuiL, GuiL, GuiL, GuiL, GuiL, GuiL, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x53, 0x39, 0x47  };  
byte BsDCode2[BSDMax] =     { 0x00, 0x00, 0x00, Tab,  0x00, 0x00, 0x00, PScr, 0x00, ShfL, CtrL, KeyZ, KeyY, AltL, KeyX, KeyC, KeyV, ShfL, KeyR, KeyX, ShfL,
                              Key1, Key2, Key3, Key4, Key5, Key6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  }; 
byte BsDCode3[BSDMax] =     { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, KeyS, KeyO, 0x00, 0x00, DelK, 0x00, 0x00, 0x00, Esc,  0x00, 0x00, F23,  
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  }; 
//                            GUI + Number: Open the Item Number on the Taskbar - if 1st item is the Calculator it will open this
cSt char BsDName[BSDMax][17]=
   {"Delete",   "Backspace", "Tab",        "AltTab",     "Insert","Escape","PrintScreen","AltPrintScreen","Return",   "Snipping", "OnScreenKeyBoard","UnDo",     "ReDo", "Ctr-Alt-Del",
    "Cut Ctr-X","Copy Ctr-C","Paste Ctr-V","TaskManager","Run",   "MenuX", "CoPilot",   "Taskbar 1",      "Taskbar 2","Taskbar 3","Taskbar 4",       "Taskbar 5","Taskbar 6",
    "Key 1",    "Key 2",    "Key 3",       "Key 4",      "Key 5", "Key 6", "Key 7",     "Key 8",          "Key 9",    "Key 10",   "Key 11",          "Key 12",
    "Key 13",   "Key 14",   "Key 15",      "Key 16",     "Key 17","Key 18","Key 19",    "Key 20",         "Key 21",   "Key 22",   "Key 23",          "Key 24",
    "NumLock",  "CapsLock", "ScrollLock"  };
                          


                          
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
