#ifndef _OPTIMIZATIONHEADER
#define _OPTIMIZATIONHEADER

// Look at Settings.h to configure!

const int16_t TestMaxRecipeListItem PROGMEM = (sizeof(RecipeList) / sizeof(RecipeList[0])) - 1;
#if TestMaxRecipeListItem > 250
#define RINT int16_t
#define RINT_IS_16bit true
#else
#define RINT int8_t
#define RINT_IS_16bit false
#endif
const RINT MaxRecipeListItem PROGMEM = TestMaxRecipeListItem;

const uint16_t TestTankCount PROGMEM = sizeof(BottleContents) / sizeof(BottleContents[0]);
#if TestTankCount > 250
#define TINT uint16_t
#define TINT_IS_16bit true
#else
#define TINT uint8_t
#define TINT_IS_16bit false
#endif
const TINT TankCount PROGMEM = TestTankCount;

const uint16_t TestMaxCategorieItem PROGMEM = (sizeof(Categories) / sizeof(Categories[0])) - 1;
#if MaxCategorieItem > 250
#define CINT uint16_t
#define CINT_IS_16bit true
#else
#define CINT uint8_t
#define CINT_IS_16bit false
#endif
const CINT MaxCategorieItem PROGMEM = TestMaxCategorieItem;

const uint16_t TestMaxGlassSizesItem PROGMEM = (sizeof(GlassSizes) / sizeof(GlassSizes[0])) - 1;
#if MaxCategorieItem > 250
#define GINT uint16_t
#define GINT_IS_16bit true
#else
#define GINT uint8_t
#define GINT_IS_16bit false
#endif
const GINT MaxGlassSizesItem PROGMEM = TestMaxGlassSizesItem;

#endif