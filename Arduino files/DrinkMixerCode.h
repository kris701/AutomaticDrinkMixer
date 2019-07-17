#ifndef _STRUCTHEADER
#define _STRUCTHEADER

#include "LCDWIKI_GUI.h"
#include "LCDWIKI_KBV.h"
#include "Settings.h"
#include "CategoryData.h"
#include "BottleData.h"
#include "StructsHeader.h"
#include "LanguageHeader.h"
#include "ColorsHeader.h"

// Look at Settings.h to configure!

void setup();
void Run(LCDWIKI_KBV _MainLCD);
void loop();
void StopAllPumps();
void ButtonFadeBlink(int Delay, bool TurnOffAfter);
void DrawBaseMenu(LCDWIKI_KBV _MainLCD, String TopText, bool ChangeTopLabel = true, bool ChangeBottomLabel = true, bool ClearCenterField = true);
void RefeshGlassSizeValues(LCDWIKI_KBV _MainLCD, MixRecipe MomentItem, int YVal, int _SelectedDrinkSizeIndex);
int CenterTextAtHorisontal(int At, int FontSize, String Text);
int CenterTextAtVertical(int At, int FontSize, String Text);
int GetTextWidth(int FontSize, String Text);
int GetTextHeight(int FontSize, String Text);
uint8_t GetFontSizeFitWithinBounds(RECT Bounds, int Margin, String Text, uint8_t MaxFontSize, uint8_t MinFontSize = 1);
int FindNextIndexRanges(int Index, int MaxIndex);
int FindPreviousIndexRanges(int Index, int MaxIndex);
int FindNextIndexRangesCategory(int Index, int MaxIndex, int CategoryIndex);
int FindPreviousIndexRangesCategory(int Index, int MaxIndex, int CategoryIndex);
uint16_t uint16_t_ReadPROGMEM(const uint16_t* ptr);
uint8_t uint8_t_ReadPROGMEM(const uint8_t* ptr);
char* char_ReadPROGMEM(char* const* ptr);
Liquid GetLiquidElement(const Liquid* ptr);
MixRecipe GetOneRecipeListElement(int Index);
CategoryItem GetCategoryElement(const CategoryItem* ptr);
bool AnyOfCategoryPossible(int CategoryIndex);
bool AnyOfCategoryPossibleSingle(int CategoryIndex, int Index);
int GetCountOfPossibleInCategory(int CategoryIndex, int Minimum = 3);
void LCD_ClearScreen(LCDWIKI_KBV _MainLCD, uint16_t WithColor = C_BLACK);
void LCD_PrintText(LCDWIKI_KBV _MainLCD, uint16_t FrontColor, uint16_t BackColor, uint8_t TextSize, String InputText, uint16_t X = 0, uint16_t Y = 0, RECT OverwriteBox = { 0,0,0,0 }, uint16_t OverwriteColor = C_BLACK);
void LCD_PrintTextWithinBounds(LCDWIKI_KBV _MainLCD, uint16_t FrontColor, uint16_t BackColor, String InputText, RECT Bounds, int Margin, uint16_t OverwriteColor = C_BLACK, bool DoOverwriteColor = true, bool FitTextSize = true, uint8_t TextSize = 1);
void LCD_PrintMultilineTextWithinBounds(LCDWIKI_KBV _MainLCD, String SplitText, RECT Bounds, uint8_t YValAdd, uint8_t FontSize, uint16_t PenColor, uint16_t FillColor);
void LCD_DrawRectangle(LCDWIKI_KBV _MainLCD, uint16_t PenColor, uint16_t FillColor, RECT Bounds, bool Fill = true);
void LCD_DrawRoundRectangle(LCDWIKI_KBV _MainLCD, uint16_t PenColor, uint16_t FillColor, RECT Bounds, uint16_t Radius, bool Fill = true);

#endif
