#ifndef __DEFINE_H_
#define __DEFINE_H_

// Nicht verändern!!!
//#######################################
#define YES                         1
#define NO                          0
#define ON                          true
#define OFF                         false
#define HM_TIMEOUT                  100
#define PRINT                       true
#define NO_PRINT                    false
#define READY                       true
#define NEW                         false
#define BOLD                        true
#define SMALL                       false
#define FontSans9pt7b               1
#define FontSansBold9pt7b           2
#define FontMonospaced_bold_10      3
#define FontMonospaced_bold_13      4
#define FontMonospaced_bold_16      5
#define FontSansSerif_plain_11      6
//#######################################
#define SCREEN_MAIN                 0
#define SCREEN_SECOND               1
#define SCREEN_AKTUEL               2
#define SCREEN_PV                   3
#define SCREEN_EINST                4
#define SCREEN_REFRESH              5
#define SCREEN_UPDATE               6
#define SCREEN_SAVE                 7
#define SCREEN_WETTER               8
#define SCREEN_MAX                  9
//#######################################
// Positionen
typedef enum {
  Main_R1_S1      =  0,
  Main_R1_S2      =  1,
  Main_R2_S1      =  2,
  Main_R2_S2      =  3,
  Main_R3_S1      =  4,
  Main_R3_S2      =  5,
  Small_R1_S1     =  6,
  Small_Menue     =  6,
  Small_R2_S1     =  7,
  Small_Back      =  7,
  Small_R3_S1     =  8,
  Small_R4_S1     =  9,
  Small_R5_S1     =  10,
  Small_R6_S1     =  11,
  Small_R1_S2     =  12,
  Small_R2_S2     =  13,
  Small_R3_S2     =  14,
  Small_R4_S2     =  15,
  Small_R5_S2     =  16,
  Small_R6_S2     =  17,
  Pos_Sym_PV      =  18,
  Pos_Sym_Grid    =  19,
  Pos_Sym_Bat     =  20,
  Pos_Sym_Ext     =  21,
  Pos_Sym_Home    =  22,
  Pos_Sym_E3DC    =  23,
  Pos_Sym_Back    =  24,
  Pos_PV_Mitte    =  25,
  Pos_Modul_1     =  26,
  Pos_Modul_2     =  27,
  Pos_newCall     =  28,
  Pos_Wetter      =  29
 } TOUCH_INDEX_T;
static unsigned short touchXmin[] =
   //  0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29
    { 20, 140,  20, 140,  20, 140,  12,  12,  12,  12,  12,  12,  60,  60,  60,  60,  60,  60,  20, 160,  20,  20, 160,  80,  14,  90,  20, 160, 196,  80};
static unsigned short touchYmin[] =
    { 40,  40, 130, 130, 220, 220,  28,  76, 124, 172, 220, 268,  28,  76, 124, 172, 220, 268,  16,  16, 212, 136, 212, 100, 290,  40, 144, 144, 286,  12};
static unsigned short touchXmax[] =
    {100, 220, 100, 220, 100, 220,  52,  52,  52,  52,  52,  52,  98,  98,  98,  98,  98,  98,  80, 220,  80,  72, 222, 160, 120, 170, 100, 220, 240, 160};
static unsigned short touchYmax[] =
    {120, 120, 210, 210, 300, 300,  68, 116, 164, 212, 260, 308,  68, 116, 162, 212, 260, 308,  96,  76, 272, 188, 272, 180, 350, 120, 196, 196, 320,  96};

#define POS_SD        214
#define POS_WWW       190
#define POS_MB        166

//#######################################
// Umlaute
const unsigned char AE = static_cast<unsigned char>(142);
const unsigned char ae = static_cast<unsigned char>(132);
const unsigned char OE = static_cast<unsigned char>(153);
const unsigned char oe = static_cast<unsigned char>(148);
const unsigned char UE = static_cast<unsigned char>(154);
const unsigned char ue = static_cast<unsigned char>(129);
const unsigned char ss = static_cast<unsigned char>(225);


#endif // __DEFINE_H_
