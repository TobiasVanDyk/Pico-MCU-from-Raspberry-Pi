////////////////
// mathKeys.h
////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Unicodes from Mathboard at https://github.com/nup002/Mathboard (MIT license)
// Also see: https://en.wikipedia.org/wiki/Mathematical_operators_and_symbols_in_Unicode
// https://devblogs.microsoft.com/oldnewthing/20240702-00/?p=109951
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
const static int statusSz = 32;                                                     // Character space in statusbar
const static char statusBlank[statusSz + 1]  = "                                ";  // To clear statusbar

const static char MathLabel[4][12][4] =                                                    
{"abg",  "dez",  "ehi",  "ESC", "klm", "nko", "prs", "NXT", "stu", "fcp", "oO ", "EXE",
 "GDH",  "LKP",  "SFP",  "ESC", "Os1", "Set", "Set", "NXT", "Set", "Var", "Var", "EXE",
 "Alg",  "Cal",  "Cal",  "ESC", "Alg", "Cal", "Com", "NXT", "Com", "Mod", "Mod", "EXE", 
 "Log",  "Log",  "Log",  "ESC", "Log", "Mod", "Mod", "NXT", "Mod", "Set", "Set", "EXE" };
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Type in Alt + Numpad key then after symbol show press Alt+X to get hex codes used below
// MathNum[4][3][12][5] -> 4 pages x 3 options per key x 12 keys x 5 chars per uni(hex)code
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const static char MathNum[4][3][12][5] = 
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

//MathName[4][3][12][32] -> 4 pages x 3 options per key x 12 keys x 32 chars per description
const static char MathName[4][3][12][statusSz] = 
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
 
