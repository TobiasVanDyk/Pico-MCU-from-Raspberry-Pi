typedef union { struct {
          
bool BackLightOn = true;               	// LCD Backlight Saver
bool CapsLock = false;                 	// Green "C"
bool NumLock = false;                  	// Green "N"
bool ScrollLock = false;               	// Green "S" 
bool Change = false;                   	// Indicators changed
bool MuteDisable = true;               	// Disable VolMute with Layout 1 2 3 4 select Default is L1-L4
bool VolDisable = false;               	// Disable Vol+ Vol- then keys are Del/BSpc Enter
bool LockMedia = false;                	// Lock-in layout 2 if it is all media keys + escape to Layer 2 if Layer 1 + [Vo]
bool Media = false;                    	// Activate 4 of possible 11 Media Keys in Layout 2
bool MouseK = false;                   	// 12 Mouse keys active via Pad option           
bool MediaPrev = false;                	// Restore 4 Media Keys in Layout 2
bool ToneControls = false;             	// Bass Treble on Media Keyboard
bool Brightness = false;               	// *br* = brightness up/dwn in M S T repace volume
bool MediaCfg = false;                 	// Set later = to MediaConfig[0]
bool DoCal = false;                    	// Set true if you want the calibration to run on each boot 
bool XFiles = false;                   	// static const bool XFiles = true;
bool MacroTimerOK = false;             	//
bool Rotate180 = false;                 // Rotate screen 180 degrees so that USB cable is on other side
bool Math = false;               	// Layer for Math KeyBrd
bool Fx = false;                 	// F1-F12 current
bool Kbrd = false;               	// Layer for KeyBrd
bool KBType = false;             	// If 1st byte>0x7F use keycode[0-5] else use keyPress
bool KeyRepeat = false;          	// If button same a sprevious button pressed
bool KBrdActive = false;         	// Let [ADD] know any char key has been pressed at least once before
bool KeyBrdDirect = false;       	// Echo Keyboard keys to PC as they are typed
bool DoUpKey = false;            	// Set true by SendBytes to let Upkey know valid macro can be saved
bool KeySkip = true;             	// Skip first key pressed in LCD in dimmed state - used to wake LCD - toggle with *ks*
bool SaveReadLinks = true;              // set via *lr* - later default true
bool NumKeys = false;            	// Numeric Keyboard
bool Resistor = true;                   //
bool PowerKeys = false;          	// Bottom row on Cfg Layout 3 Logoff Off Restart
bool PowerKeysMenu = false;      	// If true use GUI+X+u,i,r instead of GUI+R+shutdown - change with *po*
bool MacroTimerDisp = false;     	// Display Timer Keys if ON 
bool MacroTimer18 = false;       	// One of the Timers1-4 has been set Once-of or Repeat Macro by time or by clock (need PicoW?)
bool MacroTimer1  = false;       	// Repeat Time
bool Macrotimer2  = false;       	// Repeat time
bool MacroTimer3  = false;       	// Onceof Time
bool Macrotimer4  = false;       	// Onceof time
bool MacroTimer5  = false;       	// Repeat Clock Time
bool MacroTimer6  = false;       	// Onceof Clock time
bool MacroTimer7  = false;       	// Repeat Countdown Clock Time
bool MacroTimer8  = false;       	// Onceof Countdown Clock time
bool PadKeys = false;            	// 5 Small Config Buttons on RH side Red Blue Green
bool KeyFontBold = false;        	// Button Font Bold/Normal labels
bool KeyFontColour = false;      	// FontColour is White/Black
bool StrOK = false;          		// String is complete - ended with 0x0A
bool ByteOK = false;         		// Bytes received is complete - ended with 0x00
bool NewData = false;			//
bool CheckSerial = false;    		// Switch off serial check unless *se* toggles CheckSerial but it is not saved default is off
bool pressed = false;                  	// Detect touch
bool sSens = false;                    	// PC sensor value sent from HWInfo
bool mPlay = false;                    	// PC Playing Music
bool tTimeDate = false;                 // Show Date and Time
bool TimeSet = false;         		// true if Clock Time has been set for example <tyymmddwhhmm> through serial port

  } ;
 byte BoolByte[54]; 
} BoolData;

BoolData U;
