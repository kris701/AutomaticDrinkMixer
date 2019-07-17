#ifndef _CATEGORYDATA
#define _CATEGORYDATA

#include "StructsHeader.h"

// File for the categories you want to have
// Whatever category you add, should be added the the Categories[] array
// The program will figure you if it should display the category or not, depending on if it can mix it

//					Name						Display Name					Index
const CategoryItem C_Rom			PROGMEM = { "Rom"							, 0		 };
const CategoryItem C_Vodka			PROGMEM = { "Vodka"							, 1		 };
const CategoryItem C_Gin			PROGMEM = { "Gin"							, 2		 };
const CategoryItem C_Settings		PROGMEM = { "Indstillinger"					, 3		 };

const CategoryItem Categories[] PROGMEM = { C_Rom, C_Vodka, C_Gin, C_Settings };

#endif