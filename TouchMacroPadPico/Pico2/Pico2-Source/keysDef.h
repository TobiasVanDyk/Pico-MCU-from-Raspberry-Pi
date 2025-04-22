///////////////
// keysDef.h
///////////////

//////////////////////////////////////////////////////////////////////////////////////////
// Consumer keys are 16 bits - names are long to use will use hex values or short alias
//////////////////////////////////////////////////////////////////////////////////////////
#define VolUp   0x00E9
#define VolDown 0x00EA
#define VolMute 0x00E2
#define BassUp  0x0152
#define BassDwn 0x0153
#define TrebUp  0x0154
#define TrebDwn 0x0155
#define PlayMed 0x00CD  // PlayPause
#define NxtMed  0x00B5  // Next
#define PrevMed 0x00B6  // Previous
#define StopMed 0x00B7  // Stop
////////////////////////////////////////////////////////////////////////////////////////
#define KeyCut   0x7B   // Ctrl + X
#define KeyCopy  0x7C   // Ctrl + C
#define KeyPaste 0x7D   // Ctrl + V
#define KeyX     HID_KEY_X
#define KeyY     HID_KEY_Y
#define KeyZ     HID_KEY_Z
#define KeyC     HID_KEY_C
#define KeyV     HID_KEY_V
#define KeyR     HID_KEY_R
#define KeyS     HID_KEY_S
#define KeyO     HID_KEY_O
#define KeyB     HID_KEY_B 
#define KeyN     HID_KEY_N
#define Key0     HID_KEY_0
#define Key1     HID_KEY_1
#define Key2     HID_KEY_2
#define Key3     HID_KEY_3
#define Key4     HID_KEY_4
#define Key5     HID_KEY_5
#define Key6     HID_KEY_6
#define Key7     HID_KEY_7
#define Key8     HID_KEY_8
#define Key9     HID_KEY_9
#define KPse     HID_KEY_PAUSE
#define PScr     0x46   // PrintScreen
#define KeyTab   0x2B
#define ArrUp    0x52
#define ArrDown  0x51
#define ArrLeft  0x50
#define ArrRight 0x4F
#define KeyHome  0x4A
#define KeyEnd   0x4D
#define PageUp   0x4B
#define PageDwn  0x4E
#define KEqu     HID_KEY_EQUAL
#define KEnt     0x28 // KEYPAD_ENTER = 0x58 KEY_RETURN = 0x9E KEY_EXECUTE = 0x74
#define KeyYes   0x1C // "Y" or "y" note ACII Y = 0x59 y = 0x79 (difference of 61 decimal)
///////////////////////////////////////////////////////////////////////////////////////////
#define F01  0x3A  
#define F02  0x3B
#define F03  0x3C
#define F04  0x3D
#define F05  0x3E
#define F06  0x3F
#define F07  0x40
#define F08  0x41
#define F09  0x42
#define F10  0x43
#define F11  0x44 
#define F12  0x45
#define F13  0x68
#define F14  0x69
#define F15  0x6A
#define F16  0x6B
#define F17  0x6C
#define F18  0x6D
#define F19  0x6E
#define F20  0x6F
#define F21  0x70
#define F22  0x71
#define F23  0x72
#define F24  0x73
#define BShl 0x31   // Backslash
#define FShl 0x38   // Slash
#define Ins  0x49
#define Tab  0x2B
#define Spc  0x20
#define Esc  0x29
#define CtrL 0xE0  // Modifier values defined in button()
#define CtrR 0xE4
#define ShfL 0xE1
#define ShfR 0xE5
#define AltL 0xE2
#define AltR 0xE6
#define WinL 0xE3 
#define WinR 0xE7
#define GuiL 0xE3 // Super Key Left
#define GuiR 0xE7 // Super Key Right
#define BckS 0x2A
#define DelK 0x4C
#define NmL  0x53 //NumlockKey
////////////////////////////////////////////////////////////////////////////////////////////////
#define KpDiv             0x54  
#define KpMul             0x55
#define KpMin             0x56
#define KpAdd             0x57
#define KpRet             0x58
#define Kp1               0x59  // Keypad Number used in Alt + Number for special chars and Math Symbols
#define Kp2               0x5A  // Alternative is Number + Alt+X
#define Kp3               0x5B
#define Kp4               0x5C
#define Kp5               0x5D
#define Kp6               0x5E
#define Kp7               0x5F
#define Kp8               0x60
#define Kp9               0x61
#define Kp0               0x62
#define KpDot             0x63
#define KpEqu             0x67
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Four Layers A B C D of four layers by four layouts of 12 button-labels <layer><layout><name>[<number-of-lables>][<number-of-chars-per-label]
// The number of chars per label include the termination char \0 => 3 character labels here:
// ~ https://wiki.sei.cmu.edu/confluence/display/c/STR11-C.+Do+not+specify+the+bound+of+a+character+array+initialized+with+a+string+literal
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  b = Layout - 1                          Layout 1 - 4  and b = 0 - 3 
//  b = 4,5,6,7      !VolOn                 b = b + 8 
//  b = 8,9,10,11    !MuteOn                b = b + 4 !VolOn AND !MuteOn  b = b + 12 
//  Labels[LayerAD][b][n][m]; 
//  Colours[b][n];
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Same 16 groups of 4 columns by 3 rows i.e. Layout 1 - 4 has 4 Groups 0 - 3 used in Colours[16][12]
// < 2 Layers A B > < 4 Groups of 4 Layouts 1 - 4 > < 12 Keys > < 4 chars > chars include \0
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CcP    "Cut", "Cpy", "Pst"
#define XCV    "cX",  "cC",  "cV"
#define HPU    "Hme", "Up",  "PgU"
#define LcR    "<",   "Cfg", ">"
#define EPD    "End", "Dwn", "PgD"
#define S13    "S1",  "S2",  "S3"
#define S46    "S4",  "S5",  "S6"
#define T13    "T1",  "T2",  "T3"
#define T46    "T4",  "T5",  "T6"
#define M13    "M1",  "M2",  "M3"
#define M46    "M4",  "M5",  "M6"
#define S79    "S7",  "S8",  "S9"
#define S12    "S10", "S11", "S12"
#define T79    "T7",  "T8",  "T9"
#define T12    "T10", "T11", "T12"
#define M79    "M7",  "M8",  "M9"
#define M12    "M10", "M11", "M12"
#define S35    "S13", "S14", "S15"
#define S68    "S16", "S17", "S18"
#define T35    "T13", "T14", "T15"
#define T68    "T16", "T17", "T18"
#define M35    "M13", "M14", "M15"
#define M68    "M16", "M17", "M18"
#define S91    "S19", "S20", "S21"
#define S24    "S22", "S23", "S24"
#define T91    "T19", "T20", "T21"
#define T24    "T22", "T23", "T24"
#define M91    "M19", "M20", "M21"
#define M24    "M22", "M23", "M24"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const static char Labels[4][16][12][4] =   // Size = 3072 bytes                                          
{ CcP,   "V+",  M13,  "Vo",  M46,   "V-",  // L1 Mute ON Vol=ON   0     Group 0 Layer A
  HPU,   "V+",  LcR,  "Vo",  EPD,   "V-",  // L2                  1
  XCV,   "V+",  S13,  "Vo",  S46,   "V-",  // L3                  2
  XCV,   "V+",  T13,  "Vo",  T46,   "V-",  // L4                  3
  CcP,   "V+",  M13,  "L1",  M46,   "V-",  // L1 Mute OFF Vol=ON    4    Group 1 Layer A 
  HPU,   "V+",  LcR,  "L2",  EPD,   "V-",  //                       5
  XCV,   "V+",  S13,  "L3",  S46,   "V-",  //                       6
  XCV,   "V+",  T13,  "L4",  T46,   "V-",  //                       7
  CcP,   "BsD", M13,  "Vo",  M46,   "Ret", // L1 Mute ON Vol=OFF  8      Group 2 Layer A
  HPU,   "BsD", LcR,  "Vo",  EPD,   "Ret", //                     9
  XCV,   "BsD", S13,  "Vo",  S46,   "Ret", //                     10
  XCV,   "BsD", T13,  "Vo",  T46,   "Ret", //                     11
  CcP,   "BsD", M13,  "L1",  M46,   "Ret", // L1 Mute OFF Vol=OFF    12  Group 3 Layer A
  HPU,   "BsD", LcR,  "L2",  EPD,   "Ret", //                        13
  XCV,   "BsD", S13,  "L3",  S46,   "Ret", //                        14
  XCV,   "BsD", T13,  "L4",  T46,   "Ret", //                        15
  CcP,   "V+",  M79,  "Vo",  M12,   "V-",  // L1 Mute ON Vol=ON          Group 0 Layer B 
  HPU,   "V+",  LcR,  "Vo",  EPD,   "V-",  // L2
  XCV,   "V+",  S79,  "Vo",  S12,   "V-",  // L3
  XCV,   "V+",  T79,  "Vo",  T12,   "V-",  // L4
  CcP,   "V+",  M79,  "L1",  M12,   "V-",  // L1 Mute OFF Vol=ON         Group 1 Layer B
  HPU,   "V+",  LcR,  "L2",  EPD,   "V-",
  XCV,   "V+",  S79,  "L3",  S12,   "V-",
  XCV,   "V+",  T79,  "L4",  T12,   "V-",
  CcP,   "BsD", M79,  "Vo",  M12,   "Ret", // L1 Mute ON Vol=OFF         Group 2 Layer B
  HPU,   "BsD", LcR,  "Vo",  EPD,   "Ret",
  XCV,   "BsD", S79,  "Vo",  S12,   "Ret",
  XCV,   "BsD", T79,  "Vo",  T12,   "Ret",
  CcP,   "BsD", M79,  "L1",  M12,   "Ret", // L1 Mute OFF Vol=OFF        Group 3 Layer B 
  HPU,   "BsD", LcR,  "L2",  EPD,   "Ret",
  XCV,   "BsD", S79,  "L3",  S12,   "Ret",
  XCV,   "BsD", T79,  "L4",  T12,   "Ret",
  CcP,   "V+",  M35,  "Vo",  M68,   "V-",  // L1 Mute ON Vol=ON   0      Group 0 Layer C
  HPU,   "V+",  LcR,  "Vo",  EPD,   "V-",  // L2                  1
  XCV,   "V+",  S35,  "Vo",  S68,   "V-",  // L3                  2
  XCV,   "V+",  T35,  "Vo",  T68,   "V-",  // L4                  3
  CcP,   "V+",  M35,  "L1",  M68,   "V-",  // L1 Mute OFF Vol=ON    4    Group 1 Layer C 
  HPU,   "V+",  LcR,  "L2",  EPD,   "V-",  //                       5
  XCV,   "V+",  S35,  "L3",  S68,   "V-",  //                       6
  XCV,   "V+",  T35,  "L4",  T68,   "V-",  //                       7
  CcP,   "BsD", M35,  "Vo",  M68,   "Ret", // L1 Mute ON Vol=OFF  8      Group 2 Layer C
  HPU,   "BsD", LcR,  "Vo",  EPD,   "Ret", //                     9
  XCV,   "BsD", S35,  "Vo",  S68,   "Ret", //                     10
  XCV,   "BsD", T35,  "Vo",  T68,   "Ret", //                     11
  CcP,   "BsD", M35,  "L1",  M68,   "Ret", // L1 Mute OFF Vol=OFF    12  Group 3 Layer C
  HPU,   "BsD", LcR,  "L2",  EPD,   "Ret", //                        13
  XCV,   "BsD", S35,  "L3",  S68,   "Ret", //                        14
  XCV,   "BsD", T35,  "L4",  T68,   "Ret", //                        15
  CcP,   "V+",  M91,  "Vo",  M24,   "V-",  // L1 Mute ON Vol=ON          Group 0 Layer D 
  HPU,   "V+",  LcR,  "Vo",  EPD,   "V-",  // L2
  XCV,   "V+",  S91,  "Vo",  S24,   "V-",  // L3
  XCV,   "V+",  T91,  "Vo",  T24,   "V-",  // L4
  CcP,   "V+",  M91,  "L1",  M24,   "V-",  // L1 Mute OFF Vol=ON         Group 1 Layer D
  HPU,   "V+",  LcR,  "L2",  EPD,   "V-",
  XCV,   "V+",  S91,  "L3",  S24,   "V-",
  XCV,   "V+",  T91,  "L4",  T24,   "V-",
  CcP,   "BsD", M91,  "Vo",  M24,   "Ret", // L1 Mute ON Vol=OFF         Group 2 Layer D
  HPU,   "BsD", LcR,  "Vo",  EPD,   "Ret",
  XCV,   "BsD", S91,  "Vo",  S24,   "Ret",
  XCV,   "BsD", T91,  "Vo",  T24,   "Ret",
  CcP,   "BsD", M91,  "L1",  M24,   "Ret", // L1 Mute OFF Vol=OFF        Group 3 Layer D 
  HPU,   "BsD", LcR,  "L2",  EPD,   "Ret",
  XCV,   "BsD", S91,  "L3",  S24,   "Ret",
  XCV,   "BsD", T91,  "L4",  T24,   "Ret" };


#define ModCtrL 0x01 // 0xEn -> 0x0n for modifiers
#define ModCtrR 0x10 // Can just add (instead of or)
#define ModShfL 0x02
#define ModShfR 0x20
#define ModAltL 0x04
#define ModAltR 0x40
#define ModWinL 0x08 
#define ModWinR 0x80
#define ModGuiL 0x08 // Super Key Left
#define ModGuiR 0x80 // Super Key Right

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Do Button Actions Layouts 1 to 4
// Three HID Keys action types: 
// (1) Keyboard Keys pressed in sequence - usb_hid.keyboardPress(RID_KEYBOARD, keyvar) - keyvar single variable - only ASCII type keys
//     see: https://github.com/adafruit/Adafruit_TinyUSB_Arduino/issues/50
// (2) Keyboard Keys pressed together    - usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode) - keycode 0 - 5 i.e. 6 keys pressed together
//     If modifier used i.e (0xEn keys become 0x0n) use tud_hid_keyboard_report(HIDKbrd, modifier, keycode); to press and send keycode[0]=0x00
//     tud_task(); tud_hid_keyboard_report(HIDKbrd, 0x00, keycode); to release - see Cut Copy Paste in M1 and Control Shift Escape in M6 below - 
//     also see https://forums.raspberrypi.com/viewtopic.php?t=311221
// (3) Keyboard Key 1 or more held + others typed in sequence - Alt + Numpad for Math sym,bols + special characters eg. Alt-Hold + 2, 3 - use
//     tud_hid_keyboard_report(HIDKbrd, modifier, keycode); do not usb_hid.keyboardReport
// (4) Media Keys pressed in sequence    - usb_hid.sendReport16(RID_CONSUMER_CONTROL, mediakeyvar) - mediakeyvar single variable
// The challenge with macro instructions is then to know how to deal with each type (1) to (4) or any combination of (1) to (4)
// Generally if non-test macro has Shift, Alt, GUI, Ctrl as the first key then it will be of type (2) or (3) (at least initially)
// If any other first key then is a normal keyboard key it will be of type (1) initially
// If any other first media key then is type (4) initially 
//
// typedef struct TU_ATTR_PACKED {
//  uint8_t modifier;   /**< Keyboard modifier (KEYBOARD_MODIFIER_* masks). */
//  uint8_t reserved;   /**< Reserved for OEM use, always set to 0. */
//  uint8_t keycode[6]; /**< Key codes of the currently pressed keys. */
//  } hid_keyboard_report_t;
//
//
//   Key modifiers as defined by the IEEE HID Spec:
//    LEFT  CONTROL          1    # 00000001 as binary
//    LEFT  SHIFT            2    # 00000010
//    LEFT  ALT              4    # 00000100
//    LEFT  CMD|WIN          8    # 00001000
//    RIGHT CONTROL          16   # 00010000
//    RIGHT SHIFT            32   # 00100000
//    RIGHT ALT              64   # 01000000
//    RIGHT CMD|WIN          128  # 10000000
//
//  Keyboard modifier codes bitmap
//  typedef enum {
//  KEYBOARD_MODIFIER_LEFTCTRL   = TU_BIT(0), ///< Left Control
//  KEYBOARD_MODIFIER_LEFTSHIFT  = TU_BIT(1), ///< Left Shift
//  KEYBOARD_MODIFIER_LEFTALT    = TU_BIT(2), ///< Left Alt
//  KEYBOARD_MODIFIER_LEFTGUI    = TU_BIT(3), ///< Left Window
//  KEYBOARD_MODIFIER_RIGHTCTRL  = TU_BIT(4), ///< Right Control
//  KEYBOARD_MODIFIER_RIGHTSHIFT = TU_BIT(5), ///< Right Shift
//  KEYBOARD_MODIFIER_RIGHTALT   = TU_BIT(6), ///< Right Alt
//  KEYBOARD_MODIFIER_RIGHTGUI   = TU_BIT(7)  ///< Right Window
//  }hid_keyboard_modifier_bm_t;
//                                         modifier reserved=0 keycode[6]
//  usb_hid.keyboardReport(RID_KEYBOARD=1, 0,                  keycode)
//
//  usb_hid.keyboardReport(KEYBOARD_MODIFIER_LEFTSHIFT | KEYBOARD_MODIFIER_LEFTALT, 0, 0); // press shift + alt
//  USB keyboards HID Report Descriptor:
// Bit:        7   6   5   4   3   2   1   0
//           +---+---+---+---+---+---+---+---+
// Byte 0    | RG| RA| RS| RC| LG| LA| LS| LC|  Modifier bits (LC=Left Control, LS= Left Shift, etc)
//           +---+---+---+---+---+---+---+---+
// Byte 1    |        Reserved byte          |
//           +---+---+---+---+---+---+---+---+
// Byte 2    |        Key 1                  |
//           +---+---+---+---+---+---+---+---+
// Byte 3    |        Key 2                  |
//           +---+---+---+---+---+---+---+---+
// Byte 4    |        Key 3                  |
//           +---+---+---+---+---+---+---+---+
// Byte 5    |        Key 4                  |
//           +---+---+---+---+---+---+---+---+
// Byte 6    |        Key 5                  |
//           +---+---+---+---+---+---+---+---+
// Byte 7    |        Key 6                  |
//           +---+---+---+---+---+---+---+---+
// https://stackoverflow.com/questions/66671427/multiple-modifiers-2-in-keyboard-input-report-for-custom-hid-keyboard
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

      // Send Alt (Hold) + Numpad numbers for Math symbols and special characters
      //////////////////////////////////////////////////////////////////////////////////
      // Working Alt + NumPad Special Key left arrow Symbol - NB Numlock must be ON
      //         Alt + 27 as keycode [0]=Alt-L [1]=KeyPad2 [2]=KeyPad4 [3]=0x00 works
      //         Alt + 8733 (infinity symbol) only does up to Alt + 873 (small d thingy)
      //////////////////////////////////////////////////////////////////////////////////
      // Can toggle Numlock on/off with HID_KEY_NUM_LOCK as keycode[0] (not keyPress)
      //////////////////////////////////////////////////////////////////////////////////
      //keycode[0] = HID_KEY_ALT_LEFT;                    // Working
      //keycode[1] = HID_KEY_KEYPAD_2;
      //keycode[2] = HID_KEY_KEYPAD_7;                    // Alt+Keypad 27 = <- symbol               
      //keycode[3] = 0x00;                                // Needed else adds a L/F
      //usb_hid.keyboardReport(HIDKbrd, 0, keycode);      delay(keydelay2);
      //usb_hid.keyboardRelease(HIDKbrd);                 break;
      /////////////////////////////////////////////////////////////////////////////////////////
      // 1D6D1 + Alt + X = small pi works in MS Word (1D6D1 = 120529)
      /////////////////////////////////////////////////////////////////////////////////////////
      //for (n=0; n<6; n++) {usb_hid.keyboardPress(HIDKbrd, AltNum[n]); delay(keydelay2);
      //                     usb_hid.keyboardRelease(HIDKbrd);          delay(keydelay2);}
      //keycode[0] = AltL;
      //keycode[1] = 0x1B;  // char X from hid.h
      //keycode[2] = 0x00; 
      //usb_hid.keyboardReport(HIDKbrd, 0, keycode);      delay(keydelay2);
      //usb_hid.keyboardRelease(HIDKbrd);                 
      //break;          
      ////////////////////////////////////////////////////////////////////////////////////////
      // This turns sticky codes on and off
      // If used for Alt + keypad numbers make sure the option turn off sticky when 
      // 2 keys are pressed at one is off and lock modifier keys when prssed twice in a row
      // is ON. Then turn on sticky keys, press Alt twice then in word press numlock on, then 
      // type 8733 on the keypad, press Alt twice and the open infinity symbol will show
      /////////////////////////////////////////////////////////////////////////////////////////
      /*
      for (n=0; n<5; n++) { keycode[0] = HID_KEY_SHIFT_LEFT;             delay(keydelay2);
                            usb_hid.keyboardReport(HIDKbrd, 0, keycode); delay(keydelay2);
                            usb_hid.keyboardRelease(HIDKbrd);            delay(keydelay2); }
      usb_hid.keyboardPress(HIDKbrd, '\r'); delay(keydelay);
      usb_hid.keyboardRelease(HIDKbrd);     break;
      */
      ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      //                         <    0   CtrL 0xE0 + ShfL 0xE1 + Esc 0x29 >
      //                                     224         225        41
      // Macro 0x3C 0x30 0xE0 0xE1 0x29 0x3E <0 Control Shift Escape > with Layer 4 visible press [M4] will open the Task Manager
      ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      /*
      if (ByteOK)       { // <0777> shows 4 number HEX values 30 37 37 37 0     0777    RecBytes[1] 55 which is correct
                          // Must do this while pink layer is active
                          keycode[0] = RecBytes[1];     // 0xE0 224 RecBytes[0] = 7,8,9 ASCII
                          keycode[1] = RecBytes[2];     // 0xE1 225
                          keycode[2] = RecBytes[3];     // 0x29 41
                          keycode[3] = KEnt;
                          usb_hid.keyboardReport(HIDKbrd, 0, keycode);      delay(keydelay2);
                          usb_hid.keyboardRelease(HIDKbrd);                 delay(keydelay2); break;} 
      */
