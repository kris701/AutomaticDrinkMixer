#include "DrinkMixerCode.h"

// By Kristian Skov Johansen
// 2019
// Look at Settings.h to configure!

void setup() {
	pinMode(LeftButton, INPUT);
	pinMode(LeftButtonLight, OUTPUT);
	pinMode(MiddleButton, INPUT);
	pinMode(MiddleButtonLight, OUTPUT);
	pinMode(RightButton, INPUT);
	pinMode(RightButtonLight, OUTPUT);

	for (TINT i = 0; i < TINT_ReadPROGMEM(&TankCount); i++)
	{
		pinMode(uint8_t_ReadPROGMEM(&PumpPin[i]), OUTPUT);
	}

	for (uint8_t i = 0; i < 3; i++)
	{
		ButtonFadeBlink(2, true);
	}

	LCDWIKI_KBV MainLCD(ILI9486, 40, 38, 39, 44, 41);

	MainLCD.Init_LCD();
	MainLCD.Set_Rotation(1);
	MainLCD.Set_Text_Mode(1);
	
	LCD_ClearScreen(MainLCD);

	uint16_t TotalPossible = 0;
	for (RINT i = 0; i < CINT_ReadPROGMEM(&MaxCategorieItem) + 1; i++)
	{
		TotalPossible += GetCountOfPossibleInCategory(i, 0);
	}

	LCD_PrintTextWithinBounds(MainLCD, C_MAINBLUE, C_BLACK, ATotalOfString, RECT(0, 0, LCDWidth, 100), 5);
	LCD_PrintTextWithinBounds(MainLCD, C_MAINBLUE, C_BLACK, INT_STRING_INT_ToString(TotalPossible, F("/") , (RINT_ReadPROGMEM(&MaxRecipeListItem) + 1)), RECT(0, 100, LCDWidth, 200), 5);
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
	RINT SelectedDrinkIndex = 0;
	uint8_t SelectedDrinkSizeIndex = 0;
	uint8_t MenuState = 0;
	bool MenuState1Overwrite = false;
	bool MenuState2Overwrite = false;
	bool MenuState3Overwrite = false;
	bool MenuState4Overwrite = false;
	TINT SelectedTankFill = 0;
	unsigned long FillTimeStart = 0;
	unsigned long FillTime = 0;
	unsigned long FillTimeProgressbarTimer = 0;
	bool StartFill = false;
	TINT TankUsedCount = 0;
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
				SelectedCategoryIndex = NextIndex(SelectedCategoryIndex, CINT_ReadPROGMEM(&MaxCategorieItem));
				while (!IsAnyInCategory(SelectedCategoryIndex))
				{
					SelectedCategoryIndex = NextIndex(SelectedCategoryIndex, CINT_ReadPROGMEM(&MaxCategorieItem));
				}
			}

			if (MiddleButtonDown)
			{
				MenuState = 2;
				MenuState2Overwrite = true;

				DrawBaseMenu(_MainLCD, GetCategoryElement(&Categories[SelectedCategoryIndex]).CategoryName, true, false, true);

				MixRecipe MomentItem = GetRecipeElement(SelectedDrinkIndex);;
				while (SelectedCategoryIndex != MomentItem.Category.Index)
				{
					SelectedDrinkIndex = NextIndex(SelectedDrinkIndex, RINT_ReadPROGMEM(&MaxRecipeListItem));
					MomentItem = GetRecipeElement(SelectedDrinkIndex);;
				}
			}
			else
			{
				if (LeftButtonDown)
				{
					SelectedCategoryIndex = NextIndex(SelectedCategoryIndex, CINT_ReadPROGMEM(&MaxCategorieItem));
					while (!IsAnyInCategory(SelectedCategoryIndex))
					{
						SelectedCategoryIndex = NextIndex(SelectedCategoryIndex, CINT_ReadPROGMEM(&MaxCategorieItem));
					}
				}
				else
				{
					if (RightButtonDown)
					{
						SelectedCategoryIndex = PreviousIndex(SelectedCategoryIndex, CINT_ReadPROGMEM(&MaxCategorieItem));
						while (!IsAnyInCategory(SelectedCategoryIndex))
						{
							SelectedCategoryIndex = PreviousIndex(SelectedCategoryIndex, CINT_ReadPROGMEM(&MaxCategorieItem));
						}
					}
				}

				int MomentSelectedCategoryIndex = SelectedCategoryIndex;
				for (CINT i = 0; i < CINT_ReadPROGMEM(&MaxCategorieItem) / 2; i++)
				{
					MomentSelectedCategoryIndex = PreviousIndex(MomentSelectedCategoryIndex, CINT_ReadPROGMEM(&MaxCategorieItem));
					while (!IsAnyInCategory(MomentSelectedCategoryIndex))
					{
						MomentSelectedCategoryIndex = PreviousIndex(MomentSelectedCategoryIndex, CINT_ReadPROGMEM(&MaxCategorieItem));
					}
					LCD_PrintTextWithinBounds(_MainLCD, C_SECOBLUE, C_BLACK, GetCategoryElement(&Categories[MomentSelectedCategoryIndex]).CategoryName, RECT(0, 150 + (20 * i), LCDWidth / 2, 150 + (20 * i) + 20), 2);
				}

				LCD_PrintTextWithinBounds(_MainLCD, C_MAINBLUE, C_BLACK, GetCategoryElement(&Categories[SelectedCategoryIndex]).CategoryName, RECT(0, 100, LCDWidth, 150), 5);

				MomentSelectedCategoryIndex = SelectedCategoryIndex;
				for (CINT i = 0; i < CINT_ReadPROGMEM(&MaxCategorieItem) / 2; i++)
				{
					MomentSelectedCategoryIndex = NextIndex(MomentSelectedCategoryIndex, CINT_ReadPROGMEM(&MaxCategorieItem));
					while (!IsAnyInCategory(MomentSelectedCategoryIndex))
					{
						MomentSelectedCategoryIndex = NextIndex(MomentSelectedCategoryIndex, CINT_ReadPROGMEM(&MaxCategorieItem));
					}
					LCD_PrintTextWithinBounds(_MainLCD, C_SECOBLUE, C_BLACK, GetCategoryElement(&Categories[MomentSelectedCategoryIndex]).CategoryName, RECT(LCDWidth / 2, 150 + (20 * i), LCDWidth, 150 + (20 * i) + 20), 2);
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
				SelectedDrinkIndex = NextIndexCategory(SelectedDrinkIndex, RINT_ReadPROGMEM(&MaxRecipeListItem), SelectedCategoryIndex);
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
						SelectedDrinkIndex = NextIndexCategory(SelectedDrinkIndex, RINT_ReadPROGMEM(&MaxRecipeListItem), SelectedCategoryIndex);
					}
					else
					{
						if (RightButtonDown)
						{
							SelectedDrinkIndex = PreviousIndexCategory(SelectedDrinkIndex, RINT_ReadPROGMEM(&MaxRecipeListItem), SelectedCategoryIndex);
						}
					}

					RINT MomentSelectedDrinkIndex = SelectedDrinkIndex;
					for (CINT i = 0; i < GetCountOfPossibleInCategory(SelectedCategoryIndex) / 2; i++)
					{
						MomentSelectedDrinkIndex = PreviousIndexCategory(MomentSelectedDrinkIndex, RINT_ReadPROGMEM(&MaxRecipeListItem), SelectedCategoryIndex);
						LCD_PrintTextWithinBounds(_MainLCD, C_SECOBLUE, C_BLACK, GetRecipeElement(MomentSelectedDrinkIndex).Name, RECT(0, 150 + (20 * i), LCDWidth / 2, 150 + (20 * i) + 20), 2);
					}

					LCD_PrintTextWithinBounds(_MainLCD, C_MAINBLUE, C_BLACK, GetRecipeElement(SelectedDrinkIndex).Name, RECT(0, 100, LCDWidth, 150), 5);

					MomentSelectedDrinkIndex = SelectedDrinkIndex;
					for (CINT i = 0; i < GetCountOfPossibleInCategory(SelectedCategoryIndex) / 2; i++)
					{
						MomentSelectedDrinkIndex = NextIndexCategory(MomentSelectedDrinkIndex, RINT_ReadPROGMEM(&MaxRecipeListItem), SelectedCategoryIndex);
						LCD_PrintTextWithinBounds(_MainLCD, C_SECOBLUE, C_BLACK, GetRecipeElement(MomentSelectedDrinkIndex).Name, RECT(LCDWidth / 2, 150 + (20 * i), LCDWidth, 150 + (20 * i) + 20), 2);
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

				MixRecipe MomentItem = GetRecipeElement(SelectedDrinkIndex);
				LCD_PrintTextWithinBounds(_MainLCD, C_SECOBLUE, C_BLACK, MomentItem.Name, RECT(0, 0, LCDWidth, 60),5);

				LCD_DrawRoundRectangle(_MainLCD, C_MAINGRAY, C_SECOGRAY, RECT(10, 60, 310, 310), 5);

				LCD_PrintMultilineTextWithinBounds(_MainLCD, MomentItem.Description, RECT(20,70,300,130), 15, 2, C_WHITE, C_SECOGRAY);

				int YVal = 150;

				for (RINT j = 0; j < MomentItem.TankUsageCount; j++)
				{
					String NameOfLiquid;
					for (TINT i = 0; i < TINT_ReadPROGMEM(&TankCount); i++)
					{
						if (MomentItem.Liquids[j].SelectedLiquid.ID == GetLiquidElement(&BottleContents[i]).ID)
						{
							NameOfLiquid = GetLiquidElement(&BottleContents[i]).TankContenName;
							break;
						}
					}
					LCD_PrintText(_MainLCD, C_WHITE, C_SECOGRAY, 2, NameOfLiquid, 20, YVal);
					LCD_PrintText(_MainLCD, C_WHITE, C_SECOGRAY, 2, INT_STRING_ToString(MomentItem.Liquids[j].SelectedLiquidPart, F("%")), 140, YVal, RECT(140, YVal, 245, YVal + 20), C_SECOGRAY);

					YVal += 20;
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
						SelectedDrinkSizeIndex = NextIndex(SelectedDrinkSizeIndex, GINT_ReadPROGMEM(&MaxGlassSizesItem));

						RefeshGlassSizeValues(_MainLCD, GetRecipeElement(SelectedDrinkIndex), 150, SelectedDrinkSizeIndex);
					}

					if (RightButtonDown)
					{
						SelectedDrinkSizeIndex = PreviousIndex(SelectedDrinkSizeIndex, GINT_ReadPROGMEM(&MaxGlassSizesItem));

						RefeshGlassSizeValues(_MainLCD, GetRecipeElement(SelectedDrinkIndex), 150, SelectedDrinkSizeIndex);
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
				MixRecipe MomentItem = GetRecipeElement(SelectedDrinkIndex);

				if (!StartFill)
				{
					if (MomentItem.RunAtSameTime)
					{
						for (TINT j = 0; j < MomentItem.TankUsageCount; j++)
						{
							for (TINT i = 0; i < TINT_ReadPROGMEM(&TankCount); i++)
							{
								if (MomentItem.Liquids[j].SelectedLiquid.ID == GetLiquidElement(&BottleContents[i]).ID)
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
						for (TINT i = 0; i < TINT_ReadPROGMEM(&TankCount); i++)
						{
							if (MomentItem.Liquids[SelectedTankFill].SelectedLiquid.ID == GetLiquidElement(&BottleContents[i]).ID)
							{
								FillTimeStart = millis();
								StartFill = true;
								digitalWrite(uint8_t_ReadPROGMEM(&PumpPin[i]), 1);
							}
						}

						if (!StartFill)
						{
							SelectedTankFill++;
							if (SelectedTankFill > TINT_ReadPROGMEM(&TankCount))
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
							for (TINT j = 0; j < MomentItem.TankUsageCount; j++)
							{
								for (TINT i = 0; i < TINT_ReadPROGMEM(&TankCount); i++)
								{
									if (MomentItem.Liquids[j].SelectedLiquid.ID == GetLiquidElement(&BottleContents[i]).ID)
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
						for (TINT j = 0; j < MomentItem.TankUsageCount; j++)
						{
							for (TINT i = 0; i < TINT_ReadPROGMEM(&TankCount); i++)
							{
								if (MomentItem.Liquids[j].SelectedLiquid.ID == GetLiquidElement(&BottleContents[i]).ID)
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
							if (SelectedTankFill > TINT_ReadPROGMEM(&TankCount))
							{
								StopFillLoop = true;
							}
						}
					}
				}

				if (StopFillLoop)
				{
					LCD_PrintTextWithinBounds(_MainLCD, C_WHITE, C_BLACK, MixingFinishedString, RECT(0, 0, LCDWidth, 200), 10);

					for (uint8_t i = 0; i < 3; i++)
					{
						for (TINT i = 0; i < TINT_ReadPROGMEM(&TankCount); i++)
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
	for (TINT i = 0; i < TINT_ReadPROGMEM(&TankCount); i++)
	{
		digitalWrite(uint8_t_ReadPROGMEM(&PumpPin[i]), 0);
	}
}

void ButtonFadeBlink(int Delay, bool TurnOffAfter)
{
	for (uint8_t j = 0; j < 255; j++)
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
		LCD_PrintTextWithinBounds(_MainLCD, C_WHITE, C_WHITE, LeftEnterRightString, RECT(0, LCDHeight - 50, LCDWidth, LCDHeight), 10, C_SECOGRAY);
	}
}

void RefeshGlassSizeValues(LCDWIKI_KBV _MainLCD, MixRecipe MomentItem, int YVal, GINT _SelectedDrinkSizeIndex)
{
	LCD_DrawRoundRectangle(_MainLCD, C_MAINGRAY, C_SECOGRAY, RECT(320, 60, LCDWidth - 10, 225), 5);

	int MomentSelectedDrinkSizeIndex = _SelectedDrinkSizeIndex;
	for (uint8_t i = 0; i < 3; i++)
	{
		MomentSelectedDrinkSizeIndex = PreviousIndex(MomentSelectedDrinkSizeIndex, GINT_ReadPROGMEM(&MaxGlassSizesItem));
		LCD_PrintTextWithinBounds(_MainLCD, C_WHITE, C_SECOGRAY, char_ReadPROGMEM(&GlassSizesName[MomentSelectedDrinkSizeIndex]), RECT(330, 95 - (15 * i), LCDWidth - 20, 95 - (15 * i) + 15), 2, C_SECOGRAY);
	}

	LCD_PrintTextWithinBounds(_MainLCD, C_WHITE, C_SECOGRAY, char_ReadPROGMEM(&GlassSizesName[_SelectedDrinkSizeIndex]), RECT(330, 110, LCDWidth - 20, 165), 5, C_SECOGRAY);

	MomentSelectedDrinkSizeIndex = _SelectedDrinkSizeIndex;
	for (uint8_t i = 0; i < 3; i++)
	{
		MomentSelectedDrinkSizeIndex = NextIndex(MomentSelectedDrinkSizeIndex, GINT_ReadPROGMEM(&MaxGlassSizesItem));
		LCD_PrintTextWithinBounds(_MainLCD, C_WHITE, C_SECOGRAY, char_ReadPROGMEM(&GlassSizesName[MomentSelectedDrinkSizeIndex]), RECT(330, 165 + (15 * i), LCDWidth - 20, 165 + (15 * i) + 15), 2, C_SECOGRAY);
	}

	LCD_PrintTextWithinBounds(_MainLCD, C_WHITE, C_BLACK, INT_STRING_ToString(uint16_t_ReadPROGMEM(&GlassSizes[_SelectedDrinkSizeIndex]), F("ml")), RECT(320, 225, LCDWidth, LCDHeight), 5);
}

#pragma endregion

#pragma region General Call Region

uint16_t CenterTextAtHorisontal(uint16_t At, uint8_t FontSize, uint8_t TextLength)
{
	return At - (GetTextWidth(FontSize, TextLength) / 2);
}

uint16_t CenterTextAtVertical(uint16_t At, uint8_t FontSize, uint8_t TextLength)
{
	return At - (GetTextHeight(FontSize) / 2);
}

uint16_t GetTextWidth(uint8_t FontSize, uint8_t TextLength)
{
	return (TextLength * FontWidth * FontSize);
}

uint8_t GetTextHeight(uint8_t FontSize)
{
	return FontHeight * FontSize;
}

uint8_t FitFontWithingBounds(RECT Bounds, uint8_t Margin, uint8_t TextLength, uint8_t MaxFontSize, uint8_t MinFontSize = 1)
{
	uint8_t MomentFontSize = (((Bounds.Width - Bounds.X) - Margin * 2) / FontWidth) / TextLength;
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

RINT NextIndex(RINT Index, RINT MaxIndex)
{
	Index++;
	if (Index > MaxIndex)
		Index = 0;
	return Index;
}

RINT PreviousIndex(RINT Index, RINT MaxIndex)
{
	Index--;
	if (Index < 0)
		Index = MaxIndex;
	return Index;
}

RINT NextIndexCategory(RINT Index, RINT MaxIndex, CINT CategoryIndex)
{
	Index = NextIndex(Index, MaxIndex);

	MixRecipe MomentItem = GetRecipeElement(Index);
	while (CategoryIndex != MomentItem.Category.Index || !IsAnyInCategoryInner(CategoryIndex, Index))
	{
		Index = NextIndex(Index, MaxIndex);
		MomentItem = GetRecipeElement(Index);
	}

	return Index;
}

RINT PreviousIndexCategory(RINT Index, RINT MaxIndex, CINT CategoryIndex)
{
	Index = PreviousIndex(Index, MaxIndex);

	MixRecipe MomentItem = GetRecipeElement(Index);
	while (CategoryIndex != MomentItem.Category.Index || !IsAnyInCategoryInner(CategoryIndex, Index))
	{
		Index = PreviousIndex(Index, MaxIndex);
		MomentItem = GetRecipeElement(Index);
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

RINT RINT_ReadPROGMEM(const RINT* ptr)
{
	#if RINT_IS_16bit
		return (RINT)pgm_read_word(ptr);
	#else
		return (RINT)pgm_read_byte(ptr);
	#endif
}

TINT TINT_ReadPROGMEM(const TINT* ptr)
{
	#if TINT_IS_16bit
		return (RINT)pgm_read_word(ptr);
	#else
		return (RINT)pgm_read_byte(ptr);
	#endif
}

CINT CINT_ReadPROGMEM(const CINT* ptr)
{
	#if CINT_IS_16bit
		return (CINT)pgm_read_word(ptr);
	#else
		return (CINT)pgm_read_byte(ptr);
	#endif
}

GINT GINT_ReadPROGMEM(const GINT* ptr)
{
	#if GINT_IS_16bit
		return (GINT)pgm_read_word(ptr);
	#else
		return (GINT)pgm_read_byte(ptr);
	#endif
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

MixRecipe GetRecipeElement(RINT Index)
{
	MixRecipe oneItem;
	memcpy_P(&oneItem, &RecipeList[Index], sizeof oneItem);
	return oneItem;
}

bool IsAnyInCategory(CINT CategoryIndex)
{
	for (RINT i = 0; i < RINT_ReadPROGMEM(&MaxRecipeListItem) + 1; i++)
	{
		if (IsAnyInCategoryInner(CategoryIndex, i))
			return true;
	}
	return false;
}

bool IsAnyInCategoryInner(CINT CategoryIndex, RINT Index)
{
	MixRecipe MomentItem = GetRecipeElement(Index);
	if (MomentItem.Category.Index == CategoryIndex)
	{
		int IsThereCount = 0;
		for (TINT l = 0; l < MomentItem.TankUsageCount; l++)
		{
			for (TINT j = 0; j < TINT_ReadPROGMEM(&TankCount); j++)
			{
				if (MomentItem.Liquids[l].SelectedLiquid.ID == GetLiquidElement(&BottleContents[j]).ID)
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

RINT GetCountOfPossibleInCategory(CINT CategoryIndex, uint8_t Minimum = 3)
{
	int Count = 0;
	for (RINT i = 0; i < RINT_ReadPROGMEM(&MaxRecipeListItem) + 1; i++)
	{
		if (IsAnyInCategoryInner(CategoryIndex, i))
			Count++;
	}
	if (Count < Minimum)
		Count = Minimum;
	return Count;
}

String INT_STRING_INT_ToString(int Interget1, String MiddleString, int Interger2)
{
	String OutString;
	OutString += Interget1;
	OutString += MiddleString;
	OutString += Interger2;
	return OutString;
}

String INT_STRING_ToString(int Interget1, String MiddleString)
{
	String OutString;
	OutString += Interget1;
	OutString += MiddleString;
	return OutString;
}

#pragma endregion

#pragma region LCD Functions Region

void LCD_ClearScreen(LCDWIKI_KBV _MainLCD, uint16_t WithColor = C_BLACK)
{
	_MainLCD.Fill_Screen(WithColor);
}

void LCD_PrintText(LCDWIKI_KBV _MainLCD, uint16_t FrontColor, uint16_t BackColor, uint8_t TextSize, String InputText, uint16_t X = 0, uint16_t Y = 0, RECT OverwriteBox = {0,0,0,0}, uint16_t OverwriteColor = C_BLACK)
{
	if ((OverwriteBox.X == 0 && OverwriteBox.Y == 0 && OverwriteBox.Width == 0 && OverwriteBox.Height == 0))
		LCD_DrawRectangle(_MainLCD, OverwriteColor, OverwriteColor, OverwriteBox);

	_MainLCD.Set_Text_colour(FrontColor);
	_MainLCD.Set_Text_Back_colour(BackColor);
	_MainLCD.Set_Text_Size(TextSize);
	_MainLCD.Print_String(InputText, X, Y);
}

void LCD_PrintTextWithinBounds(LCDWIKI_KBV _MainLCD, uint16_t FrontColor, uint16_t BackColor, String InputText, RECT Bounds, uint8_t Margin, uint16_t OverwriteColor = C_BLACK, bool DoOverwriteColor = true, bool FitTextSize = true, uint8_t TextSize = 1 )
{
	if (DoOverwriteColor)
		LCD_DrawRectangle(_MainLCD, OverwriteColor, OverwriteColor, Bounds);

	_MainLCD.Set_Text_colour(FrontColor);
	_MainLCD.Set_Text_Back_colour(BackColor);
	uint8_t NewFontSize = TextSize;
	if (FitTextSize)
	{
		NewFontSize = FitFontWithingBounds(Bounds, Margin,InputText.length(),20,1);
	}
	_MainLCD.Set_Text_Size(NewFontSize);
	_MainLCD.Print_String(InputText, Bounds.X + CenterTextAtHorisontal((Bounds.Width - Bounds.X) / 2, NewFontSize, InputText.length()), Bounds.Y + CenterTextAtVertical((Bounds.Height - Bounds.Y) / 2, NewFontSize, InputText.length()));
}

void LCD_PrintMultilineTextWithinBounds(LCDWIKI_KBV _MainLCD, String SplitText, RECT Bounds, uint8_t YValAdd, uint8_t FontSize, uint16_t PenColor, uint16_t FillColor)
{
	String PrintString;
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
