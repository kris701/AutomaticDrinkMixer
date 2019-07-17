#ifndef _SETTINGS
#define _SETTINGS

#include "CategoryData.h"
#include "BottleData.h"
#include "StructsHeader.h"
#include "LanguageHeader.h"

// Physical Settings
//		Pin definitions on the arduino as well as some misc data

#define LeftButton 13
#define LeftButtonLight 12
#define MiddleButton 11
#define MiddleButtonLight 10
#define RightButton 9
#define RightButtonLight 8

#define FontWidth 6
#define FontHeight 8

#define LCDWidth 480
#define LCDHeight 320

// Language Settings
//		Languages Available:
//		Lang_English Lang_Danish
//			Warning: You will still need to change the language of the recipelist youself!

#define SelectedLanguage Lang_English

// Pump Settings
//		PumpPin			=	Pin number on the arduino
//		PumpCalibration =	flow rate of the pump (ml/minute)

const uint8_t PumpPin[] PROGMEM = { 2,3,4,5,6,7 };
const uint16_t PumpCalibration[] PROGMEM = { 550,550,550,550,550,550 };

// Current Bottle Configuration (See BottleData.h for more info)
//		Currently set to use 6 bottles, you can add as many more as you want.

const Liquid BottleContents[] PROGMEM = { Rom, Cola, Vodka, Juice, Gin, Tonic };

// Mix recipe data
//		List of all the recipes, the program will automatically check if a recipe can be made with the current bottle configuration, 
//		so no need to switch out the list all the time.

const MixRecipe RecipeList[] PROGMEM = {
	//								Contents {Liquid, %}				Content		Fill at			Category
	//	Display Name				(See BottleData.h)					Count 		same time		(See CategoryData.h)	Description

	{	"Let Rom og Cola",			{ {Rom, 20}, {Cola, 80} },			2,			true,			C_Rom,					"Courtesy of dunners drink imperium" },
	{	"Normal Rom og Cola",		{ {Rom, 30}, {Cola, 70} },			2,			true,			C_Rom,					"Courtesy of dunners drink imperium" },
	{	"Staerk Rom og Cola",		{ {Rom, 40}, {Cola, 60} },			2,			true,			C_Rom,					"Courtesy of dunners drink imperium" },
	{	"ROM og Cola",				{ {Rom, 70}, {Cola, 30} },			2,			true,			C_Rom,					"Courtesy of dunners drink imperium, suicide edition" },
	{	"Let Vodka-Juice",			{ {Vodka, 20}, {Juice, 80} },		2,			true,			C_Vodka,				"Description" },
	{	"Normal Vodka-Juice",		{ {Vodka, 40}, {Juice, 60} },		2,			true,			C_Vodka,				"Description" },
	{	"Staerk Vodka-Juice",		{ {Vodka, 60}, {Juice, 40} },		2,			true,			C_Vodka,				"Description" },
	{	"Let Gin og Tonic",			{ {Gin, 10}, {Tonic, 90} },			2,			false,			C_Gin,					"Description" },
	{	"Normal Gin og Tonic",		{ {Gin, 20}, {Tonic, 80} },			2,			false,			C_Gin,					"Description" },
	{	"Staerk Gin og Tonic",		{ {Gin, 30}, {Tonic, 70} },			2,			false,			C_Gin,					"Description" },

	// Runtime Settings

	{ SettingStartupOfPumpsString, { {BottleContents[0], 20}, {BottleContents[1], 20}, {BottleContents[2], 20}, {BottleContents[3], 20}, {BottleContents[4], 20}, {BottleContents[5], 20} }, 6, true, C_Settings, SettingStartupOfPumpsDescString },
	{ SettingsPurgeSystem, { {BottleContents[0], 100}, {BottleContents[1], 100}, {BottleContents[2], 100}, {BottleContents[3], 100}, {BottleContents[4], 100}, {BottleContents[5], 100}}, 6, true, C_Settings, SettingsPurgeDescSystem },
	{ SettingsTestPumps1by1String, { {BottleContents[0], 100}, {BottleContents[1], 100}, {BottleContents[2], 100}, {BottleContents[3], 100}, {BottleContents[4], 100}, {BottleContents[5], 100}}, 6, false, C_Settings, SettingsTestPumps1by1DescString }
};

// Glass size settings

const uint16_t GlassSizes[] PROGMEM = { 250, 150, 50 };
char* const GlassSizesName[] PROGMEM = { "Stor", "Medium", "Lille" };

#endif