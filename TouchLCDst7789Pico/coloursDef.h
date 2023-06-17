// RGB565 Color Picker
// http://www.barth-dev.de/online/rgb565-color-picker/
//////////////////////////////////////////////////////
#define Black    0x0000      //   0,   0,   0 
#define LGrey2   0x39E7      //
#define LGrey    0x5AEB      //
#define Black1   0x2945      //   
#define Navy     0x000F      //   0,   0, 128 
#define DGreen   0x03E0      //   0, 128,   0 
#define DCyan    0x03EF      //   0, 128, 128 
#define Maroon   0x7800      // 128,   0,   0 
#define Purple   0x780F      // 128,   0, 128 
#define Olive    0x7BE0      // 128, 128,   0 
#define LGrey1   0xD69A      // 211, 211, 211
#define LGrey3   0xA534 
#define DGrey    0x7BEF      // 128, 128, 128 
#define Blue     0x001F      //   0,   0, 255 
#define Green    0x07E0      //   0, 255,   0 
#define Cyan     0x07FF      //   0, 255, 255 
#define Red      0xF800      // 255,   0,   0 
#define Magenta  0xF81F      // 255,   0, 255 
#define Yellow   0xFFE0      // 255, 255,   0 
#define White    0xFFFF      // 255, 255, 255 
#define White1   0xCE79      
#define Orange   0xFDA0      // 255, 180,   0 
#define Green1   0xB7E0      // 180, 255,   0 
#define Green2   0x26C8      
#define Green3   0x1750
#define Green4   0x65C8
#define Green5   0x5527
#define Pink1    0xFE19      // 255, 192, 203 
#define Pink     0xFBEE 
#define Brown    0x9A60      // 150,  75,   0 
#define Gold     0xFEA0      // 255, 215,   0 
#define Silver   0xC618      // 192, 192, 192 
#define SkyBlue1 0x867D      // 135, 206, 235 
#define SkyBlue2 0x35B9
#define SkyBlue3 0x2D57
#define SkyBlue  0x2CD5
#define Violet   0x915C      // 180,  46, 226 

///////////////////////////////////////////////////////////////////////////////////
// Four layout sets of 12 button colours - use the colours defined in TFT_eSPI.h
// Modified by Caps-Scroll-Num Lock Keys cannot use const - 16x12 = 192 16bit int
// 16 groups of 4 columns by 3 rows i.e. Layout 1 - 4 each has 4 Groups 0 - 3
///////////////////////////////////////////////////////////////////////////////////
#define BrG    Blue,    Red,     Green5
#define LG3    LGrey ,  LGrey ,  LGrey 
#define BLR    Blue,    LGrey ,  Red
#define LbL    LGrey ,  Black1, LGrey 
#define SB3    SkyBlue, SkyBlue, SkyBlue
#define Pk3    Pink,    Pink,    Pink

uint16_t Colours[16][12] = {
BrG, Orange,   // Row 1 Normal L1 ///////////////////// 12 Keys group 0 12x4=48
LG3, Orange,   // Row 2 12 keys [0][0] to [0][11]
LG3, Orange,   // Row 3 [0][8] Layer1 Key8
BLR, Orange,   // Navigation L2
LbL, Orange, 
BLR, Orange,   // [1][8] 
BrG, Orange,   // Text Macros S1-S6 L3
SB3, Orange, 
SB3, Orange,   // [2][8]
BrG, Orange,   // Text Macros T1-T6 l4
Pk3, Orange, 
Pk3, Orange,   // [3][8]
BrG, Orange,   // Normal + VolMute OFF L1 ///////////// 12 Keys group 1 12x8=96
LG3, Violet, 
LG3, Orange,   // [4][8]
BLR, Orange,   // Navigation + VolMute OFF L2
LbL, Violet, 
BLR, Orange,   // [5][8]
BrG, Orange,   // Text Macros S1-S6 + VolMute OFF L3
SB3, Violet, 
SB3, Orange,   // [6][8]
BrG, Orange,   // Text Macros T1-T6 + VolMute OFF L4
Pk3, Violet, 
Pk3, Orange,   // [7][8]
BrG, Blue,     // Normal + Vol OFF L1 ///////////////// 12 Keys group 2 12x12=144
LG3, Orange, 
LG3, Red,      // [8][8]
BLR, Blue,     // Navigation
LbL, Orange, 
BLR, Red,      // [9][8]
BrG, Blue,     // Text Macros S1-S6
SB3, Orange, 
SB3, Red,      // [10][8]
BrG, Blue,     // Text Macros T1-T6
Pk3, Orange, 
Pk3, Red,      // [11][8]
BrG, Blue,     // Normal + Vol OFF VolMute OFF //////// 12 Keys group 3 12x16=192
LG3, Violet, 
LG3, Red,      // [12][8]
BLR, Blue,     // Navigation + Vol OFF VolMute OFF 
LbL, Violet, 
BLR, Red,      // [13][8]
BrG, Blue,     // Text Macros S1-S6 + Vol OFF VolMute OFF 
SB3, Violet, 
SB3, Red,      // [14][8]
BrG, Blue,     // Text Macros T1-T6 + Vol OFF VolMute OFF 
Pk3, Violet, 
Pk3, Red };    // [15][8]
                             
uint16_t keyColor[12] = {0x0000}; // = {};  // construct = {0}; should init all elements 0x00 required for local vars

uint16_t BackgroundColor[4] = {Black, Black, Black, Black};    // 4 Layouts 1-4

uint16_t ButtonOutlineColor[4] = {White, White, White, White}; // 4 Layouts 1-4

uint16_t CfgColour     = SkyBlue;                               // Layout 2 KeyBrd
uint16_t MediaColour   = Orange;                                // Layout 2 Media Keys 
uint16_t MouseColour   = SkyBlue2;                              // Layout 2 Mouse Keys      ***Mouse
uint16_t MathColour0   = 0xFF51;                                // Math KeyBrd                               
uint16_t MathColour10  = 0xF692;                                // Teal colours
uint16_t MathColour3   = 0xEE0E;
uint16_t MathColour4   = 0xEDCB;
uint16_t MathColour20  = 0xE547;
uint16_t MathColour1   = 0xDD42;
uint16_t MathColour5   = 0xAC41;
uint16_t MathColour2   = 0x8B81;

uint16_t MouseKeyColour[12] = {Blue, SkyBlue2, Blue, Green2, SkyBlue2, Blue, SkyBlue2, Blue, Red, SkyBlue2, Red, Green2 }; 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NumPad Resistor Colour Code // https://eepower.com/resistor-guide/resistor-standards-and-codes/resistor-color-code/#
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
byte     RXlat[12]   = {0,     7,     8,   9,      0,      4,      5,    6,      0,     1,      2,    3      };  // NumPad keys
uint16_t RColour[12] = {Black, Brown, Red, Orange, Yellow, Green2, Blue, Purple, DGrey, White1, Gold, Silver };  // 4 band - 3 band colour
float    RVal[12]    = {0,     1,     2,   3,      4,      5,      6,    7,      8,     9,      1/10,  1/100 }; 
bool Resistor = true;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////