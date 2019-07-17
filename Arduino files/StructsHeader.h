#ifndef _STRUCTSHEADER
#define _STRUCTSHEADER

// Look at Settings.h to configure!

struct Liquid
{
	char TankContenName[20];
	double FizzleFactor;
};

struct LiquidType
{
	Liquid SelectedLiquid;
	uint16_t SelectedLiquidPart;
};

struct CategoryItem
{
	char CategoryName[20];
	uint8_t Index;
};

struct MixRecipe
{
	char Name[32];
	LiquidType Liquids[6];
	uint8_t TankUsageCount;
	bool RunAtSameTime;
	CategoryItem Category;
	char Description[256];
};

struct RECT
{
	uint16_t X;
	uint16_t Y;
	uint16_t Width;
	uint16_t Height;

	RECT(uint16_t _X = 0, uint16_t _Y = 0, uint16_t _Width = 0, uint16_t _Height = 0)
	{
		X = _X;
		Y = _Y;
		Width = _Width;
		Height = _Height;
	}
};

#endif