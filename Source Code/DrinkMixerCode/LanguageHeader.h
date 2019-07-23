#ifndef _LANGUAGEHEADER
#define _LANGUAGEHEADER

// Language macro string file, look at Settings.h to edit

#define Lang_English 0
#define Lang_Danish 1

#if SelectedLanguage == Lang_English
#define ATotalOfString F("A Total Of")
#define RecipiesPossibleString F("Mixing Recipes Possible")
#define PressAnyButtonToContinueString F("Press any button to continue")
#define CategoryString F("Categories")
#define MixingString F("Mixing...")
#define MixingFinishedString F("Finished!")
#define LeftEnterRightString F("<--     ENTER     -->")
#define SettingStartupOfPumpsString "Initialize Pumps"
#define SettingStartupOfPumpsDescString "Initially pump a little bit of liquid into the pipes"
#define SettingsPurgeSystem "Purge System"
#define SettingsPurgeDescSystem "Empty all pumps and pipes"
#define SettingsTestPumps1by1String "Test pumps 1 by 1"
#define SettingsTestPumps1by1DescString "Run the pumps 1 by 1 to test them"
#endif

#if SelectedLanguage == Lang_Danish
#define ATotalOfString F("Et total af")
#define RecipiesPossibleString F("Opskrifter mulige")
#define PressAnyButtonToContinueString F("Tryk paa en knap for at fortsaette")
#define CategoryString F("Kategorier")
#define MixingString F("Mixer...")
#define MixingFinishedString F("Feardig!")
#define LeftEnterRightString F("<--     ENTER     -->")
#define SettingStartupOfPumpsString "Opstart af pumper"
#define SettingStartupOfPumpsDescString "Opstart alle pumper, saa der er lidt vaeske i systemet"
#define SettingsPurgeSystem "Toem System"
#define SettingsPurgeDescSystem "Toem systemet for vaeske"
#define SettingsTestPumps1by1String "Test pumperne 1 efter 1"
#define SettingsTestPumps1by1DescString "Test pumperne 1 efter 1"
#endif

#endif
