////////////////
// mathKeys.h
////////////////
// NB: When using the Symbols-SpecialChar-Math-Greek-Algebra Keyboard (press Pad [s]), note that MSWord insists on
// capitalising the first letter of a sentence - which will change the math symbol to something else.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Unicodes from Mathboard at https://github.com/nup002/Mathboard (MIT license)
// Also see: https://en.wikipedia.org/wiki/Mathematical_operators_and_symbols_in_Unicode
//           https://devblogs.microsoft.com/oldnewthing/20240702-00/?p=109951
// Use this to make Math1 file on SDCard:
// Source: https://altcodeunicode.com/alt-codes-math-symbols/
//         Math Operator Signs https://altcodeunicode.com/alt-codes-math-symbols-operators/
//         Miscellaneous Math Symbols https://altcodeunicode.com/alt-codes-for-math-symbols-miscellaneous-math-symbols/
// Greek Letters lower-case alpha to omega
// alpha beta gamma delta epsilon zeta eta theta iota kappa lambda
// mu nu xi omikron pi rho sigma(1,2) tau upsilon phi chi psi omega  
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Greek Letters Upper-case and Set theory and Various
// Gamma Delta Theta Lambda Xi Pi Sigma Phi Psi Omega 
// Set theory: not-subset-or-equal disjoint-union union intersection element-of not-element-of empty-set subset-of 
//             not-subset-of set-difference subset-or-equal plus-sign-above-equals-sign double-square-intersection
//             double-square-union intersection-with-dot amalgamation join
// Various:  dotproduct crossproduct plusminus parallel degree infinity 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Algebra and calculus: sum  n-ary-product  integral  double-integral  triple-integral  line-integral  surface-integral  
//                       volume-integral  square-root  cube-root  fourth-root  arrow  partial-derivative  nabla
// Comparison and equivalence: notequal almostequal proportional identicalto lessorequal greaterorequal 
// Modifiers: check combining-tilde combining-bar combining-arrow combining-dot combining-2dot prime double-prime 
//            sup-0 sup-1 sup-2 sup-n sup-i sup-j sub-0 sub-1 sub-2 sub-n sub-i sub-j circumflex 
// Logic: arrow-implies arrow-if-and-only-if there-exists there-does-not-exists uncoditionally-true 
//        unconditionally-false and or xor not proves models  
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int MathSet = 0;                                                                    // 0=Default 1-9 other Math Symbols Set
const static int statusSz = 32;                                                     // Character space in statusbar
const static char statusBlank[statusSz + 1]  = "                                ";  // To clear statusbar

// Each Math0 - Math9 has 144 elements (test strings) in 3 sections. Each file is 5616 bytes.
// One file needed on SCard with defaults and one in Flash with current values read from file on SDCard
char MathLabel[4][12][6] =  // MathLabel 4 Pages x 12 Keys x 6 chars (end 0) = 288 bytes                                             
{"abg",   "dez",   "ehi",   "Load", "klm",  "nko",  "prs",  "Page1", "stu",  "fcp",  "oO ",  "Send",
 "GDH",   "LKP",   "SFP",   "Exit", "Os1",  "Set1", "Set2", "Page2", "Set3", "Var1", "Var2", "Send",
 "Alg1",  "Cal1",  "Cal2",  "Load", "Alg2", "Cal3", "Com1", "Page3", "Com2", "Mod1", "Mod2", "Send", 
 "Log1",  "Log2",  "Log3",  "Exit", "Log4", "Mod2", "Mod3", "Page4", "Mod5", "Set4", "Set5", "Send" };
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Type in Alt + Numpad key then after symbol show press Alt+X to get hex codes used below
// MathNum[4][3][12][5] -> 4 pages x 3 options per key x 12 keys x 5 chars per uni(hex)code
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char MathNum[4][3][12][5] = // MathNum = 4 pages x 3 options per key x 12 keys x 5 chars per description = 720 bytes
{"03B1",  "03B4",   "03B7",  "0   ",  "03BA",   "03BD",   "03C0",   "0  ",  "03C3",  "03C6",  "03C9", "0  ",
 "03B2",  "03B5",   "03B8",  "0   ",  "03BB",   "03BE",   "03C1",   "0  ",  "03C4",  "03C7",  "03A9", "0  ",
 "03B3",  "03B6",   "03B9",  "0   ",  "03BC",   "03BF",   "03C2",   "0  ",  "03C5",  "03C8",  "0020", "0  ",
 "0393",  "039B",   "03A3",  "0   ",  "03A9",   "222A",   "2209",   "0  ",  "2284",  "22C5",  "2225", "0  ",
 "0394",  "039E",   "03A6",  "0   ",  "2288",   "2229",   "2205",   "0  ",  "2216",  "2A2F",  "00B0", "0  ",
 "0398",  "03A0",   "03A8",  "0   ",  "2A06",   "2208",   "2282",   "0  ",  "2286",  "00B1",  "221E", "0  ",
 "2211",  "2230",   "222D",  "0   ",  "221C",   "220F",   "2260",   "0  ",  "2261",  "0303",  "0307", "0  ",
 "2192",  "222B",   "222E",  "0   ",  "221A",   "2202",   "2248",   "0  ",  "2264",  "0304",  "0308", "0  ",
 "2207",  "222C",   "222F",  "0   ",  "221B",   "2032",   "221D",   "0  ",  "2265",  "20D7",  "2033", "0  ", 
 "21D2",  "2204",   "22BB",  "0   ",  "00AC",   "2070",   "2080",   "0  ",  "2099",  "2A1D",  "2A4F", "0  ",
 "21D4",  "22A4",   "2227",  "0   ",  "22A2",   "2071",   "2081",   "0  ",  "1D62",  "2A3F",  "2A4E", "0  ",
 "2203",  "22A5",   "2228",  "0   ",  "22A8",   "02B2",   "2082",   "0  ",  "2C7C",  "2A40",  "2A72", "0  " }; 

char MathName[4][3][12][statusSz] = // MathName -> 4 pages x 3 options per key x 12 keys x 32 chars per description = 4608 bytes
{"alpha", "delta",   "eta",   "0", "kappa",              "nu",          "pi",            "0 ","sigma2",         "phi",          "omega",          "0",
 "beta",  "epsilon", "theta", "0", "lambda",             "xi",          "rho",           "0 ","tau",            "chi",          "Omega",          "0",
 "gamma", "zeta",    "iota",  "0", "mu",                 "omicron",     "sigma1",        "0 ","upsilon",        "psi",          "space",          "0",
 "Gamma", "Lambda", "Sigma",  "0", "Omega",              "union",       "not-element-of","0", "not-subsetof",   "dotproduct",   "parallel",       "0",
 "Delta", "Xi",     "Phi",    "0", "not-subset-or-equal","intersection","empty-set",     "0", "setdifference",  "crossproduct", "degree",         "0",
 "Theta", "Pi",     "Psi",    "0", "disjoint-union",     "element-of",  "subset-of",     "0", "subset-or-equal","plusminus",    "infinity",       "0",
 "sum",  "volume-integral","triple-integral", "0", "fourth-root", "n-ary-prod",    "notequal",     "0", "identicalto",   "combining-tilde", "combining-dot",  "0",
 "arrow","integral",       "line-integral",   "0", "square-root", "partial-deriv", "almostequal",  "0", "lessorequal",   "combining-bar",   "combining-2dot", "0",
 "nabla","double-integral","surface-integral","0", "cube-root",   "prime",         "proportional", "0", "greaterorequal","combining-arrow", "double-prime",   "0",
 "arrow-implies",          "there-does-not-exists", "xor",  "0", "not",    "sup-0", "sub-0",  "0", "sup-n", "join",                 "double-square-union",  "0", 
 "arrow-if-and-only-if",   "unconditionally-true",  "and",  "0", "proves", "sup-i", "sub-1",  "0", "sub-i", "amalgamation",         "double-square-intersection",  "0",   
 "there-exists",           "unconditionally-false", "or",   "0", "models", "sup-j", "sub-2",  "0", "sub-j", "intersection-with-dot","plus-sign-above-equals-sign", "0" }; 


/*
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// If file Math0 does not exist on SDCard it is created from the 3 arrays MathLabel and MathNum and MathName on TouchLCD power-on.
// Rename existing Math0 file to for example Math2 by using Math0=Math2 and the [Ren] key in the Macro Editor - make sure both source and destination is brown not white
// Then reboot TouchLCD and Math0 will be created then rename it to Math1
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// Each Math0 - Math9 has 144 elements (test strings) in 3 sections. Each file is 5616 bytes.
// One file needed on SCard with defaults and one in Flash with current values read from file on SDCard
char MathLabel[4][12][6] =  // MathLabel 4 Pages x 12 Keys x 6 chars (end 0) = 288 bytes                                             
{"FCP",  "TTI",  "NEM",  "Load", "MDD", "SAR", "BSC", "Page1", "FPI", "TBD", "GSW", "Send",
 "MMM",  "SSC",  "CCC",  "Exit", "CCC", "CCS", "SSS", "Page2", "RLI", "XNN", "RDD", "Send",
 "SDL",  "RLR",  "DDS",  "Load", "SAE", "LUI", "TIB", "Page3", "RJL", "ZZZ", "PMM", "Send", 
 "IRZ",  "AZZ",  "TTD",  "Exit", "DDS", "ABG", "DTT", "Page4", "RTT", "IHM", "MHH", "Send"  };
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Type in Alt + Numpad key then after symbol show press Alt+X to get hex codes used below
// MathNum[4][3][12][5] -> 4 pages x 3 options per key x 12 keys x 5 chars per uni(hex)code
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char MathNum[4][3][12][5] = // MathNum = 4 pages x 3 options per key x 12 keys x 5 chars per description = 720 bytes
{"2200",  "2203",   "2207",  "0   ",  "2213",   "2216",   "2219",   "0  ",  "221C",  "2234",  "223A", "0  ",
 "2201",  "2204",   "220E",  "0   ",  "2214",   "2217",   "221A",   "0  ",  "221D",  "2235",  "223F", "0  ",
 "2202",  "2206",   "2212",  "0   ",  "2215",   "2218",   "221B",   "0  ",  "221E",  "2238",  "2240", "0  ",
 "228C",  "2293",   "2296",  "0   ",  "2299",   "229C",   "229F",   "0  ",  "22A2",  "22BB",  "22BE", "0  ",
 "228D",  "2294",   "2297",  "0   ",  "229A",   "229D",   "22A0",   "0  ",  "22A3",  "22BC",  "22C4", "0  ",
 "228E",  "2295",   "2298",  "0   ",  "229B",   "229E",   "22A1",   "0  ",  "22BA",  "22BD",  "22C5", "0  ",
 "22C6",  "22CA",   "22D2",  "0   ",  "27CF",   "27D3",   "29DD",   "0  ",  "29F9",  "2A1F",  "2A39", "0  ",
 "22C7",  "22CB",   "22D3",  "0   ",  "27D1",   "27D4",   "29DE",   "0  ",  "2A1D",  "2A20",  "2A3A", "0  ",
 "22C9",  "22CC",   "27CE",  "0   ",  "27D2",   "29DC",   "29F8",   "0  ",  "2A1E",  "2A21",  "2A3B", "0  ", 
 "2A3C",  "2A3F",   "2AF6",  "0   ",  "2AFE",   "2135",   "2138",   "0  ",  "2143",  "23AE",  "2715", "0  ",
 "2A3D",  "2A64",   "2AFC",  "0   ",  "2AFF",   "2136",   "2141",   "0  ",  "2144",  "23AF",  "2716", "0  ",
 "2A3E",  "2A65",   "2AFD",  "0   ",  "2118",   "2137",   "2142",   "0  ",  "214B",  "266F",  "2795", "0  " }; 

char MathName[4][3][12][statusSz] = // MathName -> 4 pages x 3 options per key x 12 keys x 32 chars per description = 4608 bytes
{"For all",              "There exists",         "Nabla",        "0", "Minus-or-plus",   "Set minus",  "Bullet",      "0", "Fourth root",     "Therefore", "Geometric proportion", "0",
 "Complement",           "There does not exist", "End of proof", "0", "Dot plus",        "Asterisk",   "Square root", "0", "Proportional to", "Because",   "Sine wave",            "0",
 "Partial differential", "Increment",            "Minus",        "0", "Division slash",  "Ring",       "Cube root",   "0", "Infinity",        "Dot minus", "Wreath product",       "0",
 "Multiset",           "Square cap",   "Circled minus",    "0", "Circled dot",      "Circled equals", "Squared minus", "0", "Right tack",  "Xor",  "Right triangle",   "0",
 "Multiset multiply",  "Square cup",   "Circled times",    "0", "Circled ring",     "Circled dash",   "Squared times", "0", "Left tack",   "Nand", "Diamond operator", "0",
 "Multiset union",     "Circled plus", "Circled division", "0", "Circled asterisk", "Squared plus",   "Squared dot",   "0", "Intercalate", "Nor",  "Dot operator",     "0",
 "Star operator",             "Right n semidirect product", "Double intersection", "0", "Squared logic or",          "Lower right corner with dot", "Tie over infinity",             "0", "Reverse solidus",  "Z schema composition", "Plus triangle",    "0",
 "Division times",            "Left semidirect product",    "Double union",        "0", "And with dot",              "Upper left corner with dot",  "Infinity negated vertical bar", "0", "Join",             "Z schema piping",      "Minus triangle",   "0",
 "Left n semidirect product", "Right semidirect product",   "Squared logic and",   "0", "Element of opening upward", "Incomplete infinity",         "Big solidus",                   "0", "Left triangle",    "Z schema projection",  "Multiply triangle","0",
 "Interior product",         "Amalgamation - coproduct",  "Triple colon operator", "0", "Dijkstra choice",       "Alef symbol",  "Dalet symbol", "0", "Reversed l",       "Integral extension",        "Multiplication x",      "0", 
 "Right interior product",   "Z domain antirestriction",  "Triple vertical bar",   "0", "Dijkstra n-ary choice", "Bet symbol",   "Turned g",     "0", "Turned y",         "Horizontal line extension", "Heavy multiplication",  "0",   
 "Z relational composition", "Z range antirestriction",   "Double solidus",        "0", "Script capital P",      "Gimel symbol", "Turned l",     "0", "Turned ampersand", "Music sharp",               "Heavy plus sign",       "0" };  

*/


/**************************************************************************************************************************
Symbols-SpecialChar-Math-Greek-Algebra Keyboard: 

Press Pad [s]. This is a triple-key macro keyboard with 4 pages and 4 x 9 x 3 = 108 Special characters, Math/Algebra, and
Greek symbols. It is sent to the PC as (hex) Unicode + [ALT] + [x] and can be directly entered into MSWord and in 
LibreOffice (also on Linux). The three control keys are [EXE] - send the symbol to the PC, [NXT] - next page of symbols, 
and [ESC] go back to the main Config layout - or press the small blue pad again to leave the Symbols keyboard. Watch out 
for MSWord insisting on capitalising the first letter of a sentence - which will change the first math symbol in a sentence
or line after a second symbol or letter or enter is typed. 

Using Alt + Number Keypad for Maths and special symbols or characters:
There are 2 ways to do it - the first is to type the character's Unicode (hex without the 0x or decimal) into MSWord and 
then press [ALT] + [x]. The second method is to hold the Alt key down, then type in a special code number using the number
keypad and then release the Alt key. There are examples of entering the open infinity symbol and the small pi symbol below:

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
 
 
