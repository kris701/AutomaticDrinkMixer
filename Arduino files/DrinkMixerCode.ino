#include "DrinkMixerCode.h"

// By Kristian Skov Johansen
// 2019
// Look at Settings.h to configure!

const uint8_t MaxRecipeListItem PROGMEM = (sizeof(RecipeList) / sizeof(RecipeList[0])) - 1;
const uint8_t MaxCategorieItem PROGMEM = (sizeof(Categories) / sizeof(Categories[0])) - 1;
const uint8_t MaxGlassSizesItem PROGMEM = (sizeof(GlassSizes) / sizeof(GlassSizes[0])) - 1;
const uint8_t TankCount PROGMEM = (sizeof(BottleContents) / sizeof(BottleContents[0])) - 1;

void setup() {
	pinMode(LeftButton, INPUT);
	pinMode(LeftButtonLight, OUTPUT);
	pinMode(MiddleButton, INPUT);
	pinMode(MiddleButtonLight, OUTPUT);
	pinMode(RightButton, INPUT);
	pinMode(RightButtonLight, OUTPUT);

	for (int i = 0; i < TankCount; i++)
	{
		pinMode(uint8_t_ReadPROGMEM(&PumpPin[i]), OUTPUT);
	}

	for (int i = 0; i < 3; i++)
	{
		ButtonFadeBlink(2, true);
	}

	LCDWIKI_KBV MainLCD(ILI9486, 40, 38, 39, 44, 41);

	MainLCD.Init_LCD();
	MainLCD.Set_Rotation(1);
	MainLCD.Set_Text_Mode(1);
	
	LCD_ClearScreen(MainLCD);

	uint16_t TotalPossible = 0;
	for (int i = 0; i < uint8_t_ReadPROGMEM(&MaxCategorieItem) + 1; i++)
	{
		TotalPossible += GetCountOfPossibleInCategory(i, 0);
	}

	LCD_PrintTextWithinBounds(MainLCD, C_MAINBLUE, C_BLACK, ATotalOfString, RECT(0, 0, LCDWidth, 100), 5);
	LCD_PrintTextWithinBounds(MainLCD, C_MAINBLUE, C_BLACK, (String)TotalPossible + F(" / ") + (String)(uint8_t_ReadPROGMEM(&MaxRecipeListItem) + 1), RECT(0, 100, LCDWidth, 200), 5);
	LCD_PrintTextWithinBounds(MainLCD, C_MAINBLUE, C_BLACK, RecipiesPossibleString, RECT(0, 200, LCDWidth, 250), 5);
	LCD_PrintTextWithinBounds(MainLCD, C_MAINBLUE, C_BLACK, PressAnyButtonToContinueString, RECT(0, 250, LCDWidth, LCDHeight), 5);

	while (digitalRead(LeftButton) && digitalRead(MiddleButton) && digitalRead(RightButton)) { delay(100); }

	Run(MainLCD);
}

void Run(LCDWIKI_KBV _MainLCD)
{
	bool LeftButtonDown = false;
	bool MiddleButtonDown = false;
	bool RightButtonDown = false;
	bool LeftButtonDownInner = true;
	bool MiddleButtonDownInner = true;
	bool RightButtonDownInner = true;

	uint8_t SelectedCategoryIndex = 0;
	uint8_t SelectedDrinkIndex = 0;
	uint8_t SelectedDrinkSizeIndex = 0;
	uint8_t MenuState = 0;
	bool MenuState1Overwrite = false;
	bool MenuState2Overwrite = false;
	bool MenuState3Overwrite = false;
	bool MenuState4Overwrite = false;
	uint8_t SelectedTankFill = 0;
	long FillTimeStart = 0;
	long FillTime = 0;
	long FillTimeProgressbarTimer = 0;
	bool StartFill = false;
	uint8_t TankUsedCount = 0;
	bool StopFillLoop = false;

	while (true)
	{
		#pragma region ButtonInput Region

		if (!LeftButtonDownInner)
		{
			if (!digitalRead(LeftButton))
			{
				digitalWrite(LeftButtonLight, 1);
				LeftButtonDown = true;
				LeftButtonDownInner = true;
			}
		}
		else
		{
			if (digitalRead(LeftButton))
			{
				digitalWrite(LeftButtonLight, 0);
				LeftButtonDown = false;
				LeftButtonDownInner = false;
			}
		}

		if (!MiddleButtonDownInner)
		{
			if (!digitalRead(MiddleButton))
			{
				digitalWrite(MiddleButtonLight, 1);
				MiddleButtonDown = true;
				MiddleButtonDownInner = true;
			}
		}
		else
		{
			if (digitalRead(MiddleButton))
			{
				digitalWrite(MiddleButtonLight, 0);
				MiddleButtonDown = false;
				MiddleButtonDownInner = false;
			}
		}

		if (!RightButtonDownInner)
		{
			if (!digitalRead(RightButton))
			{
				digitalWrite(RightButtonLight, 1);
				RightButtonDown = true;
				RightButtonDownInner = true;
			}
		}
		else
		{
			if (digitalRead(RightButton))
			{
				digitalWrite(RightButtonLight, 0);
				RightButtonDown = false;
				RightButtonDownInner = false;
			}
		}

		#pragma endregion

		#pragma region MenuState 0 Region

		if (MenuState == 0)
		{
			DrawBaseMenu(_MainLCD, CategoryString);

			MenuState = 1;
			MenuState1Overwrite = true;
		}

		#pragma endregion

		#pragma region MenuState 1 Region

		if (MenuState1Overwrite && MenuState == 1 || MenuState == 1 && (LeftButtonDown || RightButtonDown || MiddleButtonDown))
		{
			if (MenuState1Overwrite)
			{
				MenuState1Overwrite = false;
				SelectedCategoryIndex = FindNextIndexRanges(SelectedCategoryIndex, uint8_t_ReadPROGMEM(&MaxCategorieItem));
				while (!AnyOfCategoryPossible(SelectedCategoryIndex))
				{
					SelectedCategoryIndex = FindNextIndexRanges(SelectedCategoryIndex, uint8_t_ReadPROGMEM(&MaxCategorieItem));
				}
			}

			if (MiddleButtonDown)
			{
				MenuState = 2;
				MenuState2Overwrite = true;

				DrawBaseMenu(_MainLCD, GetCategoryElement(&Categories[SelectedCategoryIndex]).CategoryName, true, false, true);

				MixRecipe MomentItem = GetOneRecipeListElement(SelectedDrinkIndex);;
				while (SelectedCategoryIndex != MomentItem.Category.Index)
				{
					SelectedDrinkIndex = FindNextIndexRanges(SelectedDrinkIndex, uint8_t_ReadPROGMEM(&MaxRecipeListItem));
					MomentItem = GetOneRecipeListElement(SelectedDrinkIndex);;
				}
			}
			else
			{
				if (LeftButtonDown)
				{
					SelectedCategoryIndex = FindNextIndexRanges(SelectedCategoryIndex, uint8_t_ReadPROGMEM(&MaxCategorieItem));
					while (!AnyOfCategoryPossible(SelectedCategoryIndex))
					{
						SelectedCategoryIndex = FindNextIndexRanges(SelectedCategoryIndex, uint8_t_ReadPROGMEM(&MaxCategorieItem));
					}
				}

				if (RightButtonDown)
				{
					SelectedCategoryIndex = FindPreviousIndexRanges(SelectedCategoryIndex, uint8_t_ReadPROGMEM(&MaxCategorieItem));
					while (!AnyOfCategoryPossible(SelectedCategoryIndex))
					{
						SelectedCategoryIndex = FindPreviousIndexRanges(SelectedCategoryIndex, uint8_t_ReadPROGMEM(&MaxCategorieItem));
					}
				}

				int MomentSelectedCategoryIndex = SelectedCategoryIndex;
				for (int i = 0; i < uint8_t_ReadPROGMEM(&MaxCategorieItem) / 2; i++)
				{
					MomentSelectedCategoryIndex = FindPreviousIndexRanges(MomentSelectedCategoryIndex, uint8_t_ReadPROGMEM(&MaxCategorieItem));
					while (!AnyOfCategoryPossible(MomentSelectedCategoryIndex))
					{
						MomentSelectedCategoryIndex = FindPreviousIndexRanges(MomentSelectedCategoryIndex, uint8_t_ReadPROGMEM(&MaxCategorieItem));
					}
					String MomentCategoryName = GetCategoryElement(&Categories[MomentSelectedCategoryIndex]).CategoryName;
					LCD_PrintTextWithinBounds(_MainLCD, C_SECOBLUE, C_BLACK, MomentCategoryName, RECT(0, 150 + (20 * i), LCDWidth / 2, 150 + (20 * i) + 20), 2);
				}

				LCD_PrintTextWithinBounds(_MainLCD, C_MAINBLUE, C_BLACK, GetCategoryElement(&Categories[SelectedCategoryIndex]).CategoryName, RECT(0, 100, LCDWidth, 150), 5);

				MomentSelectedCategoryIndex = SelectedCategoryIndex;
				for (int i = 0; i < uint8_t_ReadPROGMEM(&MaxCategorieItem) / 2; i++)
				{
					MomentSelectedCategoryIndex = FindNextIndexRanges(MomentSelectedCategoryIndex, uint8_t_ReadPROGMEM(&MaxCategorieItem));
					while (!AnyOfCategoryPossible(MomentSelectedCategoryIndex))
					{
						MomentSelectedCategoryIndex = FindNextIndexRanges(MomentSelectedCategoryIndex, uint8_t_ReadPROGMEM(&MaxCategorieItem));
					}
					String MomentCategoryName = GetCategoryElement(&Categories[MomentSelectedCategoryIndex]).CategoryName;
					LCD_PrintTextWithinBounds(_MainLCD, C_SECOBLUE, C_BLACK, MomentCategoryName, RECT(LCDWidth / 2, 150 + (20 * i), LCDWidth, 150 + (20 * i) + 20), 2);
				}
			}

			LeftButtonDown = false;
			RightButtonDown = false;
			MiddleButtonDown = false;
		}

		#pragma endregion

		#pragma region MenuState 2 Region

		if (MenuState2Overwrite && MenuState == 2 || MenuState == 2 && (LeftButtonDown || RightButtonDown || MiddleButtonDown))
		{
			if (MenuState2Overwrite)
			{
				SelectedDrinkIndex = FindNextIndexRangesCategory(SelectedDrinkIndex, uint8_t_ReadPROGMEM(&MaxRecipeListItem), SelectedCategoryIndex);
				MenuState2Overwrite = false;
			}

			if (MiddleButtonDown)
			{
				MenuState = 3;
				MenuState3Overwrite = true;
			}
			else
			{
				if (LeftButtonDown && RightButtonDown)
				{
					MenuState = 1;

					DrawBaseMenu(_MainLCD, CategoryString, true, false, true);

					MenuState1Overwrite = true;
				}
				else
				{

					if (LeftButtonDown)
					{
						SelectedDrinkIndex = FindNextIndexRangesCategory(SelectedDrinkIndex, uint8_t_ReadPROGMEM(&MaxRecipeListItem), SelectedCategoryIndex);
					}

					if (RightButtonDown)
					{
						SelectedDrinkIndex = FindPreviousIndexRangesCategory(SelectedDrinkIndex, uint8_t_ReadPROGMEM(&MaxRecipeListItem), SelectedCategoryIndex);
					}

					int MomentSelectedDrinkIndex = SelectedDrinkIndex;
					for (int i = 0; i < GetCountOfPossibleInCategory(SelectedCategoryIndex) / 2; i++)
					{
						MomentSelectedDrinkIndex = FindPreviousIndexRangesCategory(MomentSelectedDrinkIndex, uint8_t_ReadPROGMEM(&MaxRecipeListItem), SelectedCategoryIndex);
						MixRecipe MomentSideItemLeft = GetOneRecipeListElement(MomentSelectedDrinkIndex);
						LCD_PrintTextWithinBounds(_MainLCD, C_SECOBLUE, C_BLACK, MomentSideItemLeft.Name, RECT(0, 150 + (20 * i), LCDWidth / 2, 150 + (20 * i) + 20), 2);
					}

					MixRecipe MomentItem = GetOneRecipeListElement(SelectedDrinkIndex);
					LCD_PrintTextWithinBounds(_MainLCD, C_MAINBLUE, C_BLACK, MomentItem.Name, RECT(0, 100, LCDWidth, 150), 5);

					MomentSelectedDrinkIndex = SelectedDrinkIndex;
					for (int i = 0; i < GetCountOfPossibleInCategory(SelectedCategoryIndex) / 2; i++)
					{
						MomentSelectedDrinkIndex = FindNextIndexRangesCategory(MomentSelectedDrinkIndex, uint8_t_ReadPROGMEM(&MaxRecipeListItem), SelectedCategoryIndex);
						MixRecipe MomentSideItemRight = GetOneRecipeListElement(MomentSelectedDrinkIndex);
						LCD_PrintTextWithinBounds(_MainLCD, C_SECOBLUE, C_BLACK, MomentSideItemRight.Name, RECT(LCDWidth / 2, 150 + (20 * i), LCDWidth, 150 + (20 * i) + 20), 2);
					}
				}
			}

			LeftButtonDown = false;
			RightButtonDown = false;
			MiddleButtonDown = false;
		}

		#pragma endregion

		#pragma region MenuState 3 Region

		if (MenuState3Overwrite && MenuState == 3 || MenuState == 3 && (LeftButtonDown || RightButtonDown || MiddleButtonDown))
		{
			if (MenuState3Overwrite)
			{
				LCD_ClearScreen(_MainLCD);

				MixRecipe MomentItem = GetOneRecipeListElement(SelectedDrinkIndex);
				LCD_PrintTextWithinBounds(_MainLCD, C_SECOBLUE, C_BLACK, MomentItem.Name, RECT(0, 0, LCDWidth, 60),5);

				LCD_DrawRoundRectangle(_MainLCD, C_MAINGRAY, C_SECOGRAY, RECT(10, 60, 310, 310), 5);

				LCD_PrintMultilineTextWithinBounds(_MainLCD, MomentItem.Description, RECT(20,70,300,130), 15, 2, C_WHITE, C_SECOGRAY);

				int YVal = 150;

				for (int j = 0; j < TankCount; j++)
				{
					if (MomentItem.Liquids[j].SelectedLiquidPart > 0)
					{
						String NameOfLiquid;
						for (int i = 0; i < 6; i++)
						{
							if ((String)MomentItem.Liquids[j].SelectedLiquid.TankContenName == GetLiquidElement(&BottleContents[i]).TankContenName)
							{
								NameOfLiquid = GetLiquidElement(&BottleContents[i]).TankContenName;
							}
						}
						LCD_PrintText(_MainLCD, C_WHITE, C_SECOGRAY, 2, NameOfLiquid, 20, YVal);
						LCD_PrintText(_MainLCD, C_WHITE, C_SECOGRAY, 2, (String)MomentItem.Liquids[j].SelectedLiquidPart + F("%"), 140, YVal, RECT(140, YVal, 245, YVal + 20), C_SECOGRAY);

						YVal += 20;
					}
				}

				RefeshGlassSizeValues(_MainLCD, MomentItem, 150, SelectedDrinkSizeIndex);

				MenuState3Overwrite = false;
			}

			if (MiddleButtonDown)
			{
				MenuState = 4;
				MenuState4Overwrite = true;
			}
			else
			{
				if (LeftButtonDown && RightButtonDown)
				{
					MenuState = 2;
					MenuState2Overwrite = true;

					DrawBaseMenu(_MainLCD, GetCategoryElement(&Categories[SelectedCategoryIndex]).CategoryName);
				}
				else
				{
					if (LeftButtonDown)
					{
						SelectedDrinkSizeIndex = FindNextIndexRanges(SelectedDrinkSizeIndex, uint8_t_ReadPROGMEM(&MaxGlassSizesItem));

						MixRecipe MomentItem = GetOneRecipeListElement(SelectedDrinkIndex);
						RefeshGlassSizeValues(_MainLCD, MomentItem, 150, SelectedDrinkSizeIndex);
					}

					if (RightButtonDown)
					{
						SelectedDrinkSizeIndex = FindPreviousIndexRanges(SelectedDrinkSizeIndex, uint8_t_ReadPROGMEM(&MaxGlassSizesItem));

						MixRecipe MomentItem = GetOneRecipeListElement(SelectedDrinkIndex);
						RefeshGlassSizeValues(_MainLCD, MomentItem, 150, SelectedDrinkSizeIndex);
					}
				}
			}

			LeftButtonDown = false;
			RightButtonDown = false;
			MiddleButtonDown = false;
		}

		#pragma endregion

		#pragma region MenuState 4 Region

		if (MenuState == 4 || MenuState4Overwrite && MenuState == 4 || MenuState == 4 && (LeftButtonDown || RightButtonDown || MiddleButtonDown))
		{
			if (MenuState4Overwrite)
			{
				LCD_ClearScreen(_MainLCD);

				LCD_PrintTextWithinBounds(_MainLCD, C_WHITE, C_BLACK, MixingString, RECT(0,0,LCDWidth, 200),10);
				LCD_PrintTextWithinBounds(_MainLCD, C_WHITE, C_BLACK, PressAnyButtonToContinueString, RECT(0, 275, LCDWidth, LCDHeight), 10);

				LCD_DrawRoundRectangle(_MainLCD, C_RED, C_RED, RECT(25, 225, 35, 250), 5);
				LCD_DrawRoundRectangle(_MainLCD, C_WHITE, C_RED, RECT(25, 225, LCDWidth - 25, 250), 5, false);

				MenuState4Overwrite = false;
			}

			if ((LeftButtonDown || RightButtonDown || MiddleButtonDown))
			{
				StopAllPumps();
				TankUsedCount = 0;
				SelectedTankFill = 0;
				StartFill = false;
				MenuState = 3;
				MenuState3Overwrite = true;
				StopFillLoop = false;

				digitalWrite(LeftButtonLight, 0);
				digitalWrite(MiddleButtonLight, 0);
				digitalWrite(RightButtonLight, 0);
			}
			else
			{
				MixRecipe MomentItem = GetOneRecipeListElement(SelectedDrinkIndex);

				if (!StartFill)
				{
					if (MomentItem.RunAtSameTime)
					{
						for (int j = 0; j < MomentItem.TankUsageCount; j++)
						{
							for (int i = 0; i < TankCount; i++)
							{
								if ((String)MomentItem.Liquids[j].SelectedLiquid.TankContenName == GetLiquidElement(&BottleContents[i]).TankContenName)
								{
									FillTimeStart = millis();
									StartFill = true;
									digitalWrite(uint8_t_ReadPROGMEM(&PumpPin[i]), 1);
								}
							}
						}

						if (!StartFill)
						{
							StopFillLoop = true;
						}
					}
					else
					{
						if (MomentItem.Liquids[SelectedTankFill].SelectedLiquidPart > 0)
						{
							for (int i = 0; i < TankCount; i++)
							{
								if ((String)MomentItem.Liquids[SelectedTankFill].SelectedLiquid.TankContenName == GetLiquidElement(&BottleContents[i]).TankContenName)
								{
									FillTimeStart = millis();
									StartFill = true;
									digitalWrite(uint8_t_ReadPROGMEM(&PumpPin[i]), 1);
								}
							}
						}

						if (!StartFill)
						{
							SelectedTankFill++;
							if (SelectedTankFill > TankCount)
							{
								StopFillLoop = true;
							}
						}
					}
				}
				else
				{
					FillTime = millis();

					if (FillTime - FillTimeProgressbarTimer >= 100)
					{
						FillTimeProgressbarTimer = millis();

						int TotalProgress = 0;

						if (MomentItem.RunAtSameTime)
						{
							for (int j = 0; j < MomentItem.TankUsageCount; j++)
							{
								for (int i = 0; i < TankCount; i++)
								{
									if ((String)MomentItem.Liquids[j].SelectedLiquid.TankContenName == GetLiquidElement(&BottleContents[i]).TankContenName)
									{
										if (!(((FillTime - FillTimeStart) / 100) * (((double)uint16_t_ReadPROGMEM(&PumpCalibration[i]) / 600) * MomentItem.Liquids[j].SelectedLiquid.FizzleFactor) >= ((double)uint16_t_ReadPROGMEM(&GlassSizes[SelectedDrinkSizeIndex]) * ((double)MomentItem.Liquids[j].SelectedLiquidPart / 100))))
										{
											int Progress = ((((LCDWidth - 25) - 35) / MomentItem.TankUsageCount) * (((FillTime - FillTimeStart) / 100) * (((double)uint16_t_ReadPROGMEM(&PumpCalibration[i]) / 600) * MomentItem.Liquids[j].SelectedLiquid.FizzleFactor) / ((double)uint16_t_ReadPROGMEM(&GlassSizes[SelectedDrinkSizeIndex]) * ((double)MomentItem.Liquids[j].SelectedLiquidPart / 100))));
											if (Progress < 0)
												Progress = 0;
											if (Progress > (((LCDWidth - 25) - 35) / MomentItem.TankUsageCount))
												Progress = (((LCDWidth - 25) - 35) / MomentItem.TankUsageCount);

											TotalProgress += Progress;
										}
										else
										{
											TotalProgress += (((LCDWidth - 25) - 35) / MomentItem.TankUsageCount);
										}
									}
								}
							}
						}
						else
						{

							TotalProgress = ((((LCDWidth - 25) - 35) / MomentItem.TankUsageCount) * TankUsedCount) + ((((LCDWidth - 25) - 35) / MomentItem.TankUsageCount) * (((FillTime - FillTimeStart) / 100) * (((double)uint16_t_ReadPROGMEM(&PumpCalibration[SelectedTankFill]) / 600) * MomentItem.Liquids[SelectedTankFill].SelectedLiquid.FizzleFactor) / ((double)uint16_t_ReadPROGMEM(&GlassSizes[SelectedDrinkSizeIndex]) * ((double)MomentItem.Liquids[SelectedTankFill].SelectedLiquidPart / 100))));
							if (TotalProgress < 0)
								TotalProgress = 0;
						}

						LCD_DrawRoundRectangle(_MainLCD, C_RED, C_RED, RECT(25, 225, 35 + TotalProgress, 250), 5);
						LCD_DrawRoundRectangle(_MainLCD, C_WHITE, C_RED, RECT(25, 225, LCDWidth - 25, 250), 5, false);

						int TotalProgressLight = ((double)TotalProgress / ((LCDWidth - 25) - 35)) * 765;

						int RightValue = TotalProgressLight;
						if (RightValue > 256)
							RightValue = 255;
						if (RightValue < 0)
							RightValue = 0;
						analogWrite(RightButtonLight, RightValue);

						int MiddleValue = TotalProgressLight - 255;
						if (MiddleValue > 256)
							MiddleValue = 255;
						if (MiddleValue < 0)
							MiddleValue = 0;
						analogWrite(MiddleButtonLight, MiddleValue);

						int LeftValue = TotalProgressLight - 510;
						if (LeftValue > 256)
							LeftValue = 255;
						if (LeftValue < 0)
							LeftValue = 0;
						analogWrite(LeftButtonLight, LeftValue);
					}

					if (MomentItem.RunAtSameTime)
					{
						bool AllDone = true;
						for (int j = 0; j < MomentItem.TankUsageCount; j++)
						{
							if (MomentItem.Liquids[j].SelectedLiquidPart > 0)
							{
								for (int i = 0; i < TankCount; i++)
								{
									if ((String)MomentItem.Liquids[j].SelectedLiquid.TankContenName == GetLiquidElement(&BottleContents[i]).TankContenName)
									{
										if (((FillTime - FillTimeStart) / 100) * (((double)uint16_t_ReadPROGMEM(&PumpCalibration[i]) / 600) * MomentItem.Liquids[j].SelectedLiquid.FizzleFactor) >= ((double)uint16_t_ReadPROGMEM(&GlassSizes[SelectedDrinkSizeIndex]) * ((double)MomentItem.Liquids[j].SelectedLiquidPart / 100)))
										{
											digitalWrite(uint8_t_ReadPROGMEM(&PumpPin[i]), 0);
										}
										else
											AllDone = false;
									}
								}
							}
						}

						if (AllDone)
						{
							StopFillLoop = true;
						}
					}
					else
					{
						if (((FillTime - FillTimeStart) / 100) * (((double)uint16_t_ReadPROGMEM(&PumpCalibration[SelectedTankFill]) / 600) * MomentItem.Liquids[SelectedTankFill].SelectedLiquid.FizzleFactor) >= ((double)uint16_t_ReadPROGMEM(&GlassSizes[SelectedDrinkSizeIndex]) * ((double)MomentItem.Liquids[SelectedTankFill].SelectedLiquidPart / 100)))
						{
							TankUsedCount++;
							StartFill = false;
							StopAllPumps();

							SelectedTankFill++;
							if (SelectedTankFill > TankCount)
							{
								StopFillLoop = true;
							}
						}
					}
				}

				if (StopFillLoop)
				{
					String MixingString = MixingFinishedString;
					LCD_PrintTextWithinBounds(_MainLCD, C_WHITE, C_BLACK, MixingString, RECT(0, 0, LCDWidth, 200), 10);

					for (int i = 0; i < 3; i++)
					{
						for (int i = 0; i < TankCount; i++)
						{
							analogWrite(uint8_t_ReadPROGMEM(&PumpPin[i]), 30);
						}
						ButtonFadeBlink(1, true);
						StopAllPumps();
						ButtonFadeBlink(1, true);
					}
					StopAllPumps();

					TankUsedCount = 0;
					SelectedTankFill = 0;
					StartFill = false;
					MenuState = 3;
					MenuState3Overwrite = true;
					StopFillLoop = false;
				}
			}

			LeftButtonDown = false;
			RightButtonDown = false;
			MiddleButtonDown = false;
		}

		#pragma endregion

		delay(100);
	}
}

void StopAllPumps()
{
	for (int i = 0; i < TankCount; i++)
	{
		digitalWrite(uint8_t_ReadPROGMEM(&PumpPin[i]), 0);
	}
}

void ButtonFadeBlink(int Delay, bool TurnOffAfter)
{
	for (int j = 0; j < 256; j++)
	{
		analogWrite(LeftButtonLight, j);
		analogWrite(MiddleButtonLight, j);
		analogWrite(RightButtonLight, j);
		delay(Delay);
	}
	if (TurnOffAfter)
	{
		digitalWrite(LeftButtonLight, 0);
		digitalWrite(MiddleButtonLight, 0);
		digitalWrite(RightButtonLight, 0);
	}
}

#pragma region External Menu Refresh Calls Region

void DrawBaseMenu(LCDWIKI_KBV _MainLCD, String TopText, bool ChangeTopLabel = true, bool ChangeBottomLabel = true, bool ClearCenterField = true)
{
	if (ChangeTopLabel)
	{
		LCD_DrawRectangle(_MainLCD, C_SECOGRAY, C_SECOGRAY, RECT(0, 0, LCDWidth, 75));
		LCD_PrintTextWithinBounds(_MainLCD, C_WHITE, C_WHITE, TopText, RECT(0, 0, LCDWidth, 75), 10, C_SECOGRAY);
	}

	if (ClearCenterField)
		LCD_DrawRectangle(_MainLCD, C_BLACK, C_BLACK, RECT(0, 75, LCDWidth, LCDHeight - 50));

	if (ChangeBottomLabel)
	{
		LCD_DrawRectangle(_MainLCD, C_SECOGRAY, C_SECOGRAY, RECT(0, LCDHeight - 50, LCDWidth, LCDHeight));
		String BottomString = LeftEnterRightString;
		LCD_PrintTextWithinBounds(_MainLCD, C_WHITE, C_WHITE, BottomString, RECT(0, LCDHeight - 50, LCDWidth, LCDHeight), 10, C_SECOGRAY);
	}
}

void RefeshGlassSizeValues(LCDWIKI_KBV _MainLCD, MixRecipe MomentItem, int YVal, int _SelectedDrinkSizeIndex)
{
	LCD_DrawRoundRectangle(_MainLCD, C_MAINGRAY, C_SECOGRAY, RECT(320, 60, LCDWidth - 10, 225), 5);

	int MomentSelectedDrinkSizeIndex = _SelectedDrinkSizeIndex;
	for (int i = 0; i < 3; i++)
	{
		MomentSelectedDrinkSizeIndex = FindPreviousIndexRanges(MomentSelectedDrinkSizeIndex, uint8_t_ReadPROGMEM(&MaxGlassSizesItem));
		String MomentString = char_ReadPROGMEM(&GlassSizesName[MomentSelectedDrinkSizeIndex]);
		LCD_PrintTextWithinBounds(_MainLCD, C_WHITE, C_SECOGRAY, MomentString, RECT(330, 95 - (15 * i), LCDWidth - 20, 95 - (15 * i) + 15), 2, C_SECOGRAY);
	}

	String MomentCenterString = char_ReadPROGMEM(&GlassSizesName[_SelectedDrinkSizeIndex]);
	LCD_PrintTextWithinBounds(_MainLCD, C_WHITE, C_SECOGRAY, MomentCenterString, RECT(330, 110, LCDWidth - 20, 165), 5, C_SECOGRAY);

	MomentSelectedDrinkSizeIndex = _SelectedDrinkSizeIndex;
	for (int i = 0; i < 3; i++)
	{
		MomentSelectedDrinkSizeIndex = FindNextIndexRanges(MomentSelectedDrinkSizeIndex, uint8_t_ReadPROGMEM(&MaxGlassSizesItem));
		String MomentString = char_ReadPROGMEM(&GlassSizesName[MomentSelectedDrinkSizeIndex]);
		LCD_PrintTextWithinBounds(_MainLCD, C_WHITE, C_SECOGRAY, MomentString, RECT(330, 165 + (15 * i), LCDWidth - 20, 165 + (15 * i) + 15), 2, C_SECOGRAY);
	}

	String SizeText = (String)uint16_t_ReadPROGMEM(&GlassSizes[_SelectedDrinkSizeIndex]) + " ml";
	LCD_PrintTextWithinBounds(_MainLCD, C_WHITE, C_BLACK, SizeText, RECT(320, 225, LCDWidth, LCDHeight), 5);
}

#pragma endregion

#pragma region General Call Region

int CenterTextAtHorisontal(int At, int FontSize, String Text)
{
	return At - (GetTextWidth(FontSize, Text) / 2);
}

int CenterTextAtVertical(int At, int FontSize, String Text)
{
	return At - (GetTextHeight(FontSize, Text) / 2);
}

int GetTextWidth(int FontSize, String Text)
{
	return (Text.length() * FontWidth * FontSize);
}

int GetTextHeight(int FontSize, String Text)
{
	return FontHeight * FontSize;
}

uint8_t GetFontSizeFitWithinBounds(RECT Bounds, int Margin, String Text, uint8_t MaxFontSize, uint8_t MinFontSize = 1)
{
	uint8_t MomentFontSize = (((Bounds.Width - Bounds.X) - Margin * 2) / FontWidth) / Text.length();
	if (MomentFontSize > MaxFontSize)
		MomentFontSize = MaxFontSize;
	if (MomentFontSize < MinFontSize)
		MomentFontSize = MinFontSize;

	while (MomentFontSize * FontHeight > Bounds.Height - Bounds.Y - Margin * 2)
	{
		MomentFontSize--;
		if (MomentFontSize <= MinFontSize)
			break;
	}
	return MomentFontSize;
}

int FindNextIndexRanges(int Index, int MaxIndex)
{
	Index++;
	if (Index > MaxIndex)
		Index = 0;
	return Index;
}

int FindPreviousIndexRanges(int Index, int MaxIndex)
{
	Index--;
	if (Index < 0)
		Index = MaxIndex;
	return Index;
}

int FindNextIndexRangesCategory(int Index, int MaxIndex, int CategoryIndex)
{
	Index = FindNextIndexRanges(Index, MaxIndex);

	MixRecipe MomentItem = GetOneRecipeListElement(Index);
	while (CategoryIndex != MomentItem.Category.Index || !AnyOfCategoryPossibleSingle(CategoryIndex, Index))
	{
		Index = FindNextIndexRanges(Index, MaxIndex);
		MomentItem = GetOneRecipeListElement(Index);
	}

	return Index;
}

int FindPreviousIndexRangesCategory(int Index, int MaxIndex, int CategoryIndex)
{
	Index = FindPreviousIndexRanges(Index, MaxIndex);

	MixRecipe MomentItem = GetOneRecipeListElement(Index);
	while (CategoryIndex != MomentItem.Category.Index || !AnyOfCategoryPossibleSingle(CategoryIndex, Index))
	{
		Index = FindPreviousIndexRanges(Index, MaxIndex);
		MomentItem = GetOneRecipeListElement(Index);
	}

	return Index;
}

uint16_t uint16_t_ReadPROGMEM(const uint16_t* ptr)
{
	return (uint16_t)pgm_read_word(ptr);
}

uint8_t uint8_t_ReadPROGMEM(const uint8_t* ptr)
{
	return (uint8_t)pgm_read_byte(ptr);
}

char* char_ReadPROGMEM(char* const* ptr)
{
	return (char*)pgm_read_word(ptr);
}

Liquid GetLiquidElement(const Liquid* ptr)
{
	Liquid oneItem;
	memcpy_P(&oneItem, ptr, sizeof oneItem);
	return oneItem;
}

CategoryItem GetCategoryElement(const CategoryItem* ptr)
{
	CategoryItem oneItem;
	memcpy_P(&oneItem, ptr, sizeof oneItem);
	return oneItem;
}

MixRecipe GetOneRecipeListElement(int Index)
{
	MixRecipe oneItem;
	memcpy_P(&oneItem, &RecipeList[Index], sizeof oneItem);
	return oneItem;
}

bool AnyOfCategoryPossible(int CategoryIndex)
{
	for (int i = 0; i < uint8_t_ReadPROGMEM(&MaxRecipeListItem) + 1; i++)
	{
		if (AnyOfCategoryPossibleSingle(CategoryIndex, i))
			return true;
	}
	return false;
}

bool AnyOfCategoryPossibleSingle(int CategoryIndex, int Index)
{
	MixRecipe MomentItem = GetOneRecipeListElement(Index);
	if (MomentItem.Category.Index == CategoryIndex)
	{
		int IsThereCount = 0;
		for (int l = 0; l < MomentItem.TankUsageCount; l++)
		{
			for (int j = 0; j < TankCount; j++)
			{
				if ((String)MomentItem.Liquids[l].SelectedLiquid.TankContenName == GetLiquidElement(&BottleContents[j]).TankContenName)
				{
					IsThereCount++;
					break;
				}
			}
		}
		if (IsThereCount == MomentItem.TankUsageCount)
			return true;
	}
	return false;
}

int GetCountOfPossibleInCategory(int CategoryIndex, int Minimum = 3)
{
	int Count = 0;
	for (int i = 0; i < uint8_t_ReadPROGMEM(&MaxRecipeListItem) + 1; i++)
	{
		if (AnyOfCategoryPossibleSingle(CategoryIndex, i))
			Count++;
	}
	if (Count < Minimum)
		Count = Minimum;
	return Count;
}

#pragma endregion

#pragma region LCD Functions Region

void LCD_ClearScreen(LCDWIKI_KBV _MainLCD, uint16_t WithColor = C_BLACK)
{
	_MainLCD.Fill_Screen(WithColor);
}

void LCD_PrintText(LCDWIKI_KBV _MainLCD, uint16_t FrontColor, uint16_t BackColor, uint8_t TextSize, String InputText, uint16_t X = 0, uint16_t Y = 0, RECT OverwriteBox = {0,0,0,0}, uint16_t OverwriteColor = C_BLACK)
{
	LCD_DrawRectangle(_MainLCD, OverwriteColor, OverwriteColor, OverwriteBox);

	_MainLCD.Set_Text_colour(FrontColor);
	_MainLCD.Set_Text_Back_colour(BackColor);
	_MainLCD.Set_Text_Size(TextSize);
	_MainLCD.Print_String(InputText, X, Y);
}

void LCD_PrintTextWithinBounds(LCDWIKI_KBV _MainLCD, uint16_t FrontColor, uint16_t BackColor, String InputText, RECT Bounds, int Margin, uint16_t OverwriteColor = C_BLACK, bool DoOverwriteColor = true, bool FitTextSize = true, uint8_t TextSize = 1 )
{
	if (DoOverwriteColor)
		LCD_DrawRectangle(_MainLCD, OverwriteColor, OverwriteColor, Bounds);

	_MainLCD.Set_Text_colour(FrontColor);
	_MainLCD.Set_Text_Back_colour(BackColor);
	uint8_t NewFontSize = TextSize;
	if (FitTextSize)
	{
		NewFontSize = GetFontSizeFitWithinBounds(Bounds, Margin,InputText,20,1);
	}
	_MainLCD.Set_Text_Size(NewFontSize);
	_MainLCD.Print_String(InputText, Bounds.X + CenterTextAtHorisontal((Bounds.Width - Bounds.X) / 2, NewFontSize, InputText), Bounds.Y + CenterTextAtVertical((Bounds.Height - Bounds.Y) / 2, NewFontSize, InputText));
}

void LCD_PrintMultilineTextWithinBounds(LCDWIKI_KBV _MainLCD, String SplitText, RECT Bounds, uint8_t YValAdd, uint8_t FontSize, uint16_t PenColor, uint16_t FillColor)
{
	String PrintString = "";
	int YVal = 0;
	bool NoMoreSpace = false;
	for (int i = 0; i < SplitText.length(); i++)
	{
		if ((PrintString.length() + 2) * FontWidth * FontSize > Bounds.Width - Bounds.X)
		{
			if (Bounds.Y + YVal + YValAdd > Bounds.Height)
			{
				NoMoreSpace = true;
				break;
			}
			if (SplitText[i - 1] != ' ')
				PrintString += F("-");
			LCD_PrintText(_MainLCD, PenColor, FillColor, FontSize, PrintString, Bounds.X, Bounds.Y + YVal);
			PrintString = "";
			YVal += YValAdd;
		}
		PrintString += SplitText[i];
	}
	if (!NoMoreSpace)
		LCD_PrintText(_MainLCD, PenColor, FillColor, FontSize, PrintString, Bounds.X, Bounds.Y + YVal);
}

void LCD_DrawRectangle(LCDWIKI_KBV _MainLCD, uint16_t PenColor, uint16_t FillColor, RECT Bounds, bool Fill = true)
{
	if (Fill)
	{
		_MainLCD.Set_Draw_color(FillColor);
		_MainLCD.Fill_Rectangle(Bounds.X, Bounds.Y, Bounds.Width, Bounds.Height);
	}
	_MainLCD.Set_Draw_color(PenColor);
	_MainLCD.Draw_Rectangle(Bounds.X, Bounds.Y, Bounds.Width, Bounds.Height);
}

void LCD_DrawRoundRectangle(LCDWIKI_KBV _MainLCD, uint16_t PenColor, uint16_t FillColor, RECT Bounds, uint16_t Radius, bool Fill = true)
{
	if (Fill)
	{
		_MainLCD.Set_Draw_color(FillColor);
		_MainLCD.Fill_Round_Rectangle(Bounds.X, Bounds.Y, Bounds.Width, Bounds.Height, Radius);
	}
	_MainLCD.Set_Draw_color(PenColor);
	_MainLCD.Draw_Round_Rectangle(Bounds.X, Bounds.Y, Bounds.Width, Bounds.Height, Radius);
}

#pragma endregion

void loop() {
	delay(1000);
}
