#ifndef _STRUCTHEADER
#define _STRUCTHEADER

#include "LCDWIKI_GUI.h"
#include "LCDWIKI_KBV.h"
#include "Settings.h"
#include "StructsHeader.h"
#include "CategoryData.h"
#include "BottleData.h"
#include "LanguageHeader.h"
#include "ColorsHeader.h"

// Look at Settings.h to configure!

void setup();
void Run(LCDWIKI_KBV _MainLCD);
void loop();
void StopAllPumps();
void ButtonFadeBlink(int Delay, bool TurnOffAfter);
void DrawBaseMenu(LCDWIKI_KBV _MainLCD, String TopText, bool ChangeTopLabel = true, bool ChangeBottomLabel = true, bool ClearCenterField = true);
void RefeshGlassSizeValues(LCDWIKI_KBV _MainLCD, MixRecipe MomentItem, int YVal, GINT _SelectedDrinkSizeIndex);
uint16_t CenterTextAtHorisontal(uint16_t At, uint8_t FontSize, uint8_t TextLength);
uint16_t CenterTextAtVertical(uint16_t At, uint8_t FontSize, uint8_t TextLength);
uint16_t GetTextWidth(uint8_t FontSize, uint8_t TextLength);
uint8_t GetTextHeight(uint8_t FontSize);
uint8_t FitFontWithingBounds(RECT Bounds, uint8_t Margin, uint8_t TextLength, uint8_t MaxFontSize, uint8_t MinFontSize = 1);
RINT NextIndex(RINT Index, RINT MaxIndex);
RINT PreviousIndex(RINT Index, RINT MaxIndex);
RINT NextIndexCategory(RINT Index, RINT MaxIndex, CINT CategoryIndex);
RINT PreviousIndexCategory(RINT Index, RINT MaxIndex, CINT CategoryIndex);
uint16_t uint16_t_ReadPROGMEM(const uint16_t* ptr);
uint8_t uint8_t_ReadPROGMEM(const uint8_t* ptr);
RINT RINT_ReadPROGMEM(const RINT* ptr);
TINT TINT_ReadPROGMEM(const TINT* ptr);
char* char_ReadPROGMEM(char* const* ptr);
Liquid GetLiquidElement(const Liquid* ptr);
MixRecipe GetRecipeElement(RINT Index);
CategoryItem GetCategoryElement(const CategoryItem* ptr);
bool IsAnyInCategory(CINT CategoryIndex);
bool IsAnyInCategoryInner(CINT CategoryIndex, RINT Index);
RINT GetCountOfPossibleInCategory(CINT CategoryIndex, uint8_t Minimum = 3);
String INT_STRING_INT_ToString(int Interget1, String MiddleString, int Interger2);
String INT_STRING_ToString(int Interget1, String MiddleString);
void LCD_ClearScreen(LCDWIKI_KBV _MainLCD, uint16_t WithColor = C_BLACK);
void LCD_PrintText(LCDWIKI_KBV _MainLCD, uint16_t FrontColor, uint16_t BackColor, uint8_t TextSize, String InputText, uint16_t X = 0, uint16_t Y = 0, RECT OverwriteBox = { 0,0,0,0 }, uint16_t OverwriteColor = C_BLACK);
void LCD_PrintTextWithinBounds(LCDWIKI_KBV _MainLCD, uint16_t FrontColor, uint16_t BackColor, String InputText, RECT Bounds, uint8_t Margin, uint16_t OverwriteColor = C_BLACK, bool DoOverwriteColor = true, bool FitTextSize = true, uint8_t TextSize = 1);
void LCD_PrintMultilineTextWithinBounds(LCDWIKI_KBV _MainLCD, String SplitText, RECT Bounds, uint8_t YValAdd, uint8_t FontSize, uint16_t PenColor, uint16_t FillColor);
void LCD_DrawRectangle(LCDWIKI_KBV _MainLCD, uint16_t PenColor, uint16_t FillColor, RECT Bounds, bool Fill = true);
void LCD_DrawRoundRectangle(LCDWIKI_KBV _MainLCD, uint16_t PenColor, uint16_t FillColor, RECT Bounds, uint16_t Radius, bool Fill = true);

#endif
