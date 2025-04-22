//////////////////////////////////////////////////////////////////////////////////////////
// sdcard.h
//////////////////////////////////////////////////////////////////////////////////////////
// Size = 24x10 * 2 + 24x7 + 24x3 = 720 bytes
//////////////////////////////////////////////////////////////////////////////////////////
// SD Filenames not case sensitive but are case preserving
// LittleFS Filenames are case sensitive
//
// Could therefore use STRname, MTRname, TTRname instead of SDNameS SDNameM SDNameT 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SDCard File Names:
// Note that 3char text filenames are required for use in X01Link strings => cannot add .txt to all the files here
// 3-Char filenames produced by *sd*4-9 (UVWXYZ U01-U24) *sd*mst (MST M01-M24) *sd*abcdef (A01-A24)
//
// SDNum 1-3    S0001.txt-S0024.txt A1.txt-A6,txt - C1.txt-C6.txt 01-24           *sd*n=1-3 Can be edited here 
// SDNum 4-9    U-Z + 01-24 = U01-U24 - Z01-Z24                                   *sd*n=4-9 This + below formed in SDCardSelectFiles()
// SDnum 10-12  MST mst 01-24 = M01-M24 S01-S24 T01-T24                           *sd*m,s,t *sd*M,S,T
// SDNum 13-19  A-G a-g 01-24 = A01-A24 - G01-D24                                 *sd*A-G   *sd*a-g     check Dnn for delay in Link()
// SDNum 20-21  K,k  K=K01Link-K24Link k = K01-K24                                *sd*K,k
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char SD1[23]   = { " 123UVWXYZMSTABCDEFGKK" }; // Not a set 0
char SD2[23]   = { " 123uvwxyzmstabcdefgKk" }; // 1-3, 4-9 = U V W X Y Z, 10 = M m 11 = S s 12 = T t 13-19 A-G 20 = K 21 = k
bool sdShow = false;                           // SDFleList sent to Serial Port
bool SDNumChange = false;          // If 1 save SCard Set with [Cfg][Sav]
File SDFile;                       // File object instance
byte SDByte;                       // Byte from File read from SDCard
byte SDNum = 1;                    // 1 - 9 10 - 12 13-19 20,21 1-3 U-Z A-G M,S,T k,K Select 22 groups of 24 files SDCard size limited to its capacity 
                                   // SDnum=0 => SDCard file read disabled
byte SDCardArr[4] = {1,0,0,0};     // [0] Store/Retrieve SDnum  [1] = char 0 = " ", 1-3, 4-9 = U V W X Y Z, k K m s t  [2] 0=Uppercase 1=Lowercase Filename 
char SDName [24][10];              // Current Filename choice - see comments below other ways to do this
                                   // 24 * (10 + 10 + 7 + 3) = 720 bytes - other names are constructed
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Set of SDNum = 1-3 has filenames easily customized by editing below - SDCard 8.3 restriction o filenames
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const char SDName1 [24][10] = { "S0001.txt", "S0002.txt", "S0003.txt", "S0004.txt", "S0005.txt", "S0006.txt", 
                                       "S0007.txt", "S0008.txt", "S0009.txt", "S0010.txt", "S0011.txt", "S0012.txt", 
                                       "S0013.txt", "S0014.txt", "S0015.txt", "S0016.txt", "S0017.txt", "S0018.txt", 
                                       "S0019.txt", "S0020.txt", "S0021.txt", "S0022.txt", "S0023.txt", "S0024.txt" }; 
static const char SDName2 [24][7]  = { "A1.txt", "A2.txt", "A3.txt", "A4.txt", "A5.txt", "A6.txt", 
                                       "B1.txt", "B2.txt", "B3.txt", "B4.txt", "B5.txt", "B6.txt", 
                                       "C1.txt", "C2.txt", "C3.txt", "C4.txt", "C5.txt", "C6.txt", 
                                       "D1.txt", "D2.txt", "D3.txt", "D4.txt", "D5.txt", "D6.txt" };   
static const char SDName3 [24][3]  = { "01", "02", "03", "04", "05", "06", 
                                       "07", "08", "09", "10", "11", "12", 
                                       "13", "14", "15", "16", "17", "18", 
                                       "19", "20", "21", "22", "23", "24" };
                                       
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                                   

                                      
                                      
                                      
