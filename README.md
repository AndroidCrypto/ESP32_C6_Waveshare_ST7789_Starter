# ESP32-C6 Waveshare with ST7789 1.47 inch TFT display Starter
Getting started with an **ESP32-C6 Waveshare** device and an 1.47 inch TFT display with driver chip **ST7789**.

This is the accompanying repository for my article "**Getting Started with an ESP32-C6 Waveshare LCD device with 1.47 inch  ST7789 TFT display**":   .

The display is a 1.47 inch large TFT display with 172 x 320 pixels.

## Settings for the display specific setup file

I'm using a display specific setup file for the combination ESP32-C6 Waveshare with TFT display driver ST7789. This file contains e.g. the display driver, size and pins for the ESP32-device.  

### Copy a file to the User_Setups folder

Please copy the file

    Setup701_C6_WS_ST7789_172x320.h

to the **User_Setups** folder.

### Edit the User_Setup_Select.h file

You need to place the following line in the root folder's "**User_Setup_Select.h**" file

    #include <User_Setups/Setup701_C6_WS_ST7789_172x320.h> // ESP32-C6 Supermini, 80 MHz

Second: please comment all other "#include..." entries like this, especially the "//#include <User_Setup.h>" entry.

````
// Example User_Setup files are stored in the "User_Setups" folder. These can be used
// unmodified or adapted for a particular hardware configuration.
#ifndef USER_SETUP_LOADED //  Lets PlatformIO users define settings in
                          //  platformio.ini, see notes in "Tools" folder.
///////////////////////////////////////////////////////
//   User configuration selection lines are below    //
///////////////////////////////////////////////////////
// Only ONE line below should be uncommented to define your setup.  Add extra lines and files as needed.
//#include <User_Setup.h>                       // Default setup is root library folder
// Setup file in folder Arduino/libraries (updates will not overwrite your setups)
#include <User_Setups/Setup701_C6_WS_ST7789_172x320.h> // ESP32-C6 Waveshare, 80 MHz
````

## Troubleshooting for upload problems

I'm used to being able to start the upload directly on other boards, but this failed on my board. The reason is simple - the board is not automatically in upload mode. To resolve this problem,  I have to press the **Boot button** and hold it pressed, then press the **Reset button** and now release the **Boot button**. That's it, the board is in upload mode and receives the new code.

But that's not all: after uploading the sketch, you have to press the **Reset button** again to bring the board back to regular mode.

## Important note

You need to modify the display library TFT_eSPI to get the code to work. Please find instructions on how to do this in my forked TFT_eSPI repository here on GitHub: [https://github.com/AndroidCrypto/TFT_eSPI](https://github.com/AndroidCrypto/TFT_eSPI).
