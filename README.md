# Automatic Drink Mixer

I made an automatic drink mixer, because i thought it would be fun, it not only proved to be a fair amount construction, but also a lot of programming. So all in all its a machine that can take the contents of 6 different bottles and mix them together to make drinks. It has a little screen and and three buttons to control it. Im not saying you should build it just the way i did, but this is a good guide line to follow.

*IMAGE*

List of items used:
* 1x Arduino Mega 2560
* 1x TFT LCD Screen module (i used a ILI9486/ILI9488 controller but any can do)
* 6x pumps (i used 550ml/m pumps, can be upgraded if wanted)
* 1x prototype board (for the motor controller)
* 6x bc337 transistors
* 6x 1k resistors
* 6x 10k resistors
* a handfull of pin connecters, male and female
* 1-2x LM7805 voltage regulators (you can probably use the MEGAs own regulator for this, but i fried mine)
* 3x buttons (i choose some with light)
* 1x plexiglass panel
* ~8m of 4mm(inner)*6mm(outer) tubing

## The electronics

The electonics are pretty simple if you know how to solder. The biggest part is the controller board for the pumps, you could probably buy them as module, but you will learn much more from this by making it yourself. As you can see in the schematic the design it simple, just a transistor bridge times 6, one for each pump. If you follow the schematic you should end up with something like the second picture. That was the biggest part of the electronics section, the rest if just hooking up the board to the MEGA, wiring the buttons, adding the LM7805 power regulator, and placing the screen. 

![Error Loading Image!](https://lmvnvg.db.files.1drv.com/y4mIIVTgkiJM4zY1uSfsQuK9PG0vSGBu9jyzLFHhQSi3jqLqnOUUMOmhuhsMb2lG8Vd94Oxkgix8gNQhe_4O-dfHzyF9zOvJQDpnPdA1NH0ePwB3jDz83eJxhMdAi2piUbv5vzdrBFko6jKMa66brjNDqi0ZImhuirVvHZOd-eTFR1Hkgwk5E4VEh5G_A9IASSDB8olgUY52Q4OW2CMoKQe0Q?width=1547&height=574&cropmode=none)
![Error Loading Image!](https://yk6vpw.db.files.1drv.com/y4mynN3DNnRBQjebDBR7DuQxdi4z7Jt-AUB39jXKgVfKf3_IsUqfIyX2EexmC8Zuu1fWycg44wy1V1UJCKJz5m4G7QZ6xlPrtynLd1hjxLdVu-FAKW55_ef7agpYZeZGlpuqzau8SodpfX5pVBrxGF6bqbKzG5uMXIh5PsNOmdUwrWBH7iF2KOS3h3Agb5EOtKLAivMaRtQZPAn--CmFbshYg?width=1376&height=774&cropmode=none)

## Construction

The construction is made mainly from reused items, consisting of:
* An old box shaped bucket for the main body
* An old wooden plate as a base
* A metal plate that i bended
* An old water faucet for the tubes
* Some other pieces of wood for internal construction

All have been put together with nuts and bolts to make deconstruction easier if something breaks. And as a finish to the construction, the bucket and the wooden plate got a nice black color, and the metal plate got vibrant red one, while the faucet stays metalic. As it is visable in the pictures, the 6 tubes goes down the bottles and into the bucket, and into the pumps, that then pumps the liquid out through the faucet and down your cup!

![Error Loading Image!](https://lgvgtg.db.files.1drv.com/y4mspVOnhzHvkpo_OnWoUykMgt1yeQBEpFvrYU8IgiLfl_xQ_dZyhMbAwXWbCT3KglYYNkWJNEVtXHCnfJjS9Qk_99j8wvgrXQt0DdaPq20AkSfCHt2AIhNDfndpdZkjiXvuW5G8RGXqiklzESu6BXVbGFO_7reCw7cOhUvOWCGnR6qIHWjPPG8iTwmmqqhAzTVIEpdXlbG7BC7CP5GDgKgwg?width=1376&height=774&cropmode=none)
![Error Loading Image!](https://pfc4ew.db.files.1drv.com/y4m-pJn6c86Aup0eXH07ylUGRwfCf7fIPrrBMILB49ImSryAvpj4XHei6ak2I6kLmLYYqI6RUQKdtDo-3GMScCGahsVM6UhZGIV8NsdlUU5NcpQRJk5eP_IgS7dFJi8UIVkbrj2IxW59BlucdkjrML6LDfmStJ_RETl23hriU2SXuk8N3ZaXUCsNFaOI_CMyHY32YTGDoAeEI4Ev9WB5EiueA?width=725&height=864&cropmode=none)

## Programming

Then it came to the code to run it all, i made it into 4 files that can be edited, to make it simple to add new mixing recipes. These files are:
* BottleData.h, containing data about diffrent liquid types.
* CategoryData.h, containing data about categories.
* LanguageHeader.h, a header file containing different GUI languages.
* Settings.h, a header file containing most of the actural machine settings.

These files can be changed as pleased, and the languages included are Danish and English.
